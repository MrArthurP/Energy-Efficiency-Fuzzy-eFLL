#!/usr/bin/env python3
"""Gera knn_model.h / knn_model.c a partir de um KNeighborsClassifier salvo em .joblib.

Suporta:
  - KNeighborsClassifier puro ou Pipeline cujo(s) passo(s) anterior(es) sejam
    StandardScaler / MinMaxScaler (aplicados à entrada no C).
  - metric euclidiana (minkowski p=2) ou manhattan (p=1)
  - weights = 'uniform' ou 'distance'
  - dados em float32, sem malloc, sem dependências além de math.h

Uso:
  python knn_to_c.py model.joblib --out-dir saida
  python knn_to_c.py model.joblib --out-dir saida --test-x X_teste.npy --n-tests 200
"""
import argparse
import json
import os

import joblib
import numpy as np
from sklearn.neighbors import KNeighborsClassifier
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import MinMaxScaler, StandardScaler


# ----------------------------------------------------------------- utilidades
def fmt_f(v):
    v = np.float32(v)
    if not np.isfinite(v):
        raise ValueError("valor não finito encontrado nos dados")
    return f"{float(v):.9g}f"


def c_array_1d(values):
    return ", ".join(fmt_f(v) for v in values)


def c_matrix(name, mat, per_line=1):
    rows = []
    for r in mat:
        rows.append("    {" + c_array_1d(r) + "}")
    return f"static const float {name}[{mat.shape[0]}][{mat.shape[1]}] = {{\n" + ",\n".join(rows) + "\n};\n"


def c_int_array(name, values, ctype="int"):
    body = ", ".join(str(int(v)) for v in values)
    return f"static const {ctype} {name}[{len(values)}] = {{ {body} }};\n"


# ------------------------------------------------------- extração do modelo
def extract(model):
    """Retorna (knn, a, b) onde a entrada é transformada por x*a + b (ou None)."""
    pre = []
    if isinstance(model, Pipeline):
        steps = [s for _, s in model.steps]
        knn, pre = steps[-1], steps[:-1]
    else:
        knn = model

    if not isinstance(knn, KNeighborsClassifier):
        raise TypeError(f"Esperava KNeighborsClassifier, recebi {type(knn).__name__}")

    a = b = None
    for step in pre:
        if isinstance(step, StandardScaler):
            n = step.n_features_in_
            mean = step.mean_ if step.with_mean else np.zeros(n)
            scale = step.scale_ if step.with_std else np.ones(n)
            sa, sb = 1.0 / scale, -mean / scale
        elif isinstance(step, MinMaxScaler):
            sa, sb = step.scale_, step.min_
        else:
            raise TypeError(
                f"Passo de pré-processamento não suportado: {type(step).__name__}. "
                "Aplique-o manualmente ou estenda o script."
            )
        if a is None:
            a, b = np.asarray(sa, float), np.asarray(sb, float)
        else:  # compõe: (x*a + b)*sa + sb
            a, b = a * sa, b * sa + sb
    return knn, a, b


def check_knn(knn):
    if hasattr(knn, "_fit_X") and not isinstance(knn._fit_X, np.ndarray):
        raise ValueError("Dados de treino esparsos não são suportados.")
    if knn.weights not in ("uniform", "distance"):
        raise ValueError(f"weights={knn.weights!r} não suportado (use 'uniform' ou 'distance').")
    metric, p = knn.effective_metric_, (knn.effective_metric_params_ or {}).get("p", 2)
    if metric in ("euclidean", "l2") or (metric == "minkowski" and p == 2):
        return 2
    if metric in ("manhattan", "cityblock", "l1") or (metric == "minkowski" and p == 1):
        return 1
    raise ValueError(f"Métrica {metric!r} (p={p}) não suportada. Use euclidiana ou manhattan.")


# ---------------------------------------------------------------- geração .h
def gen_header(prefix, n, d, k, n_classes, weights, p, has_pre):
    guard = f"{prefix.upper()}_MODEL_H"
    P = prefix.upper()
    return f"""/* Gerado automaticamente por knn_to_c.py - nao edite. */
#ifndef {guard}
#define {guard}

#define {P}_N_SAMPLES  {n}
#define {P}_N_FEATURES {d}
#define {P}_K          {k}
#define {P}_N_CLASSES  {n_classes}
#define {P}_WEIGHTS_DISTANCE {1 if weights == 'distance' else 0}
#define {P}_P_NORM     {p}   /* 2 = euclidiana, 1 = manhattan */
#define {P}_HAS_PREPROCESS {1 if has_pre else 0}

/* Nomes originais das classes, indexados pelo valor retornado por {prefix}_predict. */
extern const char *const {prefix}_class_names[{P}_N_CLASSES];

/* input: vetor com {P}_N_FEATURES floats (valores brutos, antes de qualquer escala).
 * retorno: índice da classe (0..{P}_N_CLASSES-1), igual a posição em model.classes_. */
int {prefix}_predict(const float *input);

#endif /* {guard} */
"""


# ---------------------------------------------------------------- geração .c
def gen_source(prefix, X, y_idx, classes, a, b, weights, p, k):
    P = prefix.upper()
    parts = [f'/* Gerado automaticamente por knn_to_c.py - nao edite. */\n#include "{prefix}_model.h"\n#include <math.h>\n\n']

    parts.append(c_matrix(f"{prefix}_train_X", X))
    parts.append("\n")
    parts.append(c_int_array(f"{prefix}_train_y", y_idx, "unsigned short" if len(classes) < 65536 else "int"))
    parts.append("\n")

    names = ", ".join(json.dumps(str(c), ensure_ascii=True) for c in classes)
    parts.append(f"const char *const {prefix}_class_names[{P}_N_CLASSES] = {{ {names} }};\n\n")

    if a is not None:
        parts.append(f"static const float {prefix}_pre_a[{P}_N_FEATURES] = {{ {c_array_1d(a)} }};\n")
        parts.append(f"static const float {prefix}_pre_b[{P}_N_FEATURES] = {{ {c_array_1d(b)} }};\n\n")

    dist_term = "t * t" if p == 2 else "fabsf(t)"
    load_x = (
        f"        x[f] = input[f] * {prefix}_pre_a[f] + {prefix}_pre_b[f];\n"
        if a is not None
        else "        x[f] = input[f];\n"
    )

    if weights == "uniform":
        vote = f"""    int votes[{P}_N_CLASSES];
    for (int c = 0; c < {P}_N_CLASSES; c++) votes[c] = 0;
    for (int j = 0; j < {P}_K; j++) votes[best_c[j]]++;

    int best = 0;
    for (int c = 1; c < {P}_N_CLASSES; c++)
        if (votes[c] > votes[best]) best = c;   /* empate: menor índice de classe */
    return best;
"""
    else:
        root = "sqrtf(best_d[j])" if p == 2 else "best_d[j]"
        vote = f"""    /* Pesos = 1/distância. Se algum vizinho tem distância 0, só eles votam (como no sklearn). */
    int has_zero = 0;
    for (int j = 0; j < {P}_K; j++)
        if (best_d[j] == 0.0f) has_zero = 1;

    float votes[{P}_N_CLASSES];
    for (int c = 0; c < {P}_N_CLASSES; c++) votes[c] = 0.0f;
    for (int j = 0; j < {P}_K; j++) {{
        float w;
        if (has_zero) w = (best_d[j] == 0.0f) ? 1.0f : 0.0f;
        else          w = 1.0f / {root};
        votes[best_c[j]] += w;
    }}

    int best = 0;
    for (int c = 1; c < {P}_N_CLASSES; c++)
        if (votes[c] > votes[best]) best = c;
    return best;
"""

    parts.append(f"""int {prefix}_predict(const float *input)
{{
    float x[{P}_N_FEATURES];
    for (int f = 0; f < {P}_N_FEATURES; f++) {{
{load_x}    }}

    /* Os K vizinhos mais próximos, ordenados por distância crescente. */
    float best_d[{P}_K];
    int   best_c[{P}_K];
    for (int j = 0; j < {P}_K; j++) {{
        best_d[j] = INFINITY;
        best_c[j] = 0;
    }}

    for (int i = 0; i < {P}_N_SAMPLES; i++) {{
        float d = 0.0f;
        for (int f = 0; f < {P}_N_FEATURES; f++) {{
            float t = x[f] - {prefix}_train_X[i][f];
            d += {dist_term};
        }}
        if (d < best_d[{P}_K - 1]) {{
            int j = {P}_K - 1;
            while (j > 0 && best_d[j - 1] > d) {{   /* '>' estrito: empate mantém o índice menor */
                best_d[j] = best_d[j - 1];
                best_c[j] = best_c[j - 1];
                j--;
            }}
            best_d[j] = d;
            best_c[j] = (int){prefix}_train_y[i];
        }}
    }}

{vote}}}
""")
    return "".join(parts)


# ------------------------------------------------------------- teste em C
def gen_test(prefix, Xt, expected):
    P = prefix.upper()
    m, d = Xt.shape
    return f"""/* Gerado automaticamente. Compile: gcc -O2 {prefix}_test.c {prefix}_model.c -lm -o {prefix}_test */
#include <stdio.h>
#include "{prefix}_model.h"

#define N_TESTS {m}
{c_matrix("test_X", Xt)}
{c_int_array("test_expected", expected)}
int main(void)
{{
    int bad = 0;
    for (int i = 0; i < N_TESTS; i++) {{
        int got = {prefix}_predict(test_X[i]);
        if (got != (int)test_expected[i]) {{
            printf("DIVERGE amostra %d: C=%d sklearn=%d\\n", i, got, (int)test_expected[i]);
            bad++;
        }}
    }}
    printf("%d/%d iguais ao sklearn\\n", N_TESTS - bad, N_TESTS);
    return bad ? 1 : 0;
}}
"""


# ------------------------------------------------------------------- main
def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("model", help="arquivo .joblib")
    ap.add_argument("--out-dir", default=".", help="pasta de saída")
    ap.add_argument("--name", default="knn", help="prefixo dos símbolos C e dos arquivos (padrão: knn)")
    ap.add_argument("--test-x", help=".npy com amostras BRUTAS para gerar o teste de equivalência")
    ap.add_argument("--n-tests", type=int, default=200, help="máximo de amostras no teste")
    args = ap.parse_args()

    model = joblib.load(args.model)
    knn, a, b = extract(model)
    p = check_knn(knn)

    X = np.asarray(knn._fit_X, dtype=np.float32)
    n, d = X.shape
    classes = knn.classes_
    y_idx = np.asarray(knn._y).astype(int)  # índices em classes_
    k = int(knn.n_neighbors)
    if k > n:
        raise ValueError(f"n_neighbors={k} > n_samples={n}")

    prefix = args.name
    os.makedirs(args.out_dir, exist_ok=True)

    with open(os.path.join(args.out_dir, f"{prefix}_model.h"), "w", encoding="utf-8", newline="\n") as f:
        f.write(gen_header(prefix, n, d, k, len(classes), knn.weights, p, a is not None))
    with open(os.path.join(args.out_dir, f"{prefix}_model.c"), "w", encoding="utf-8", newline="\n") as f:
        f.write(gen_source(prefix, X, y_idx, classes, a, b, knn.weights, p, k))

    size_kb = X.nbytes / 1024 + y_idx.size * 2 / 1024
    print(f"OK: {n} amostras x {d} features, k={k}, {len(classes)} classes, "
          f"weights={knn.weights}, p={p}, preprocess={'sim' if a is not None else 'não'}")
    print(f"Dados em flash: ~{size_kb:.1f} KB")

    if args.test_x:
        Xt = np.load(args.test_x)[: args.n_tests]
        pred = model.predict(Xt)
        expected = np.searchsorted(classes, pred)
        with open(os.path.join(args.out_dir, f"{prefix}_test.c"), "w", encoding="utf-8", newline="\n") as f:
            f.write(gen_test(prefix, Xt.astype(np.float32), expected))
        print(f"Teste gerado: gcc -O2 {prefix}_test.c {prefix}_model.c -lm -o {prefix}_test && ./{prefix}_test")


if __name__ == "__main__":
    main()