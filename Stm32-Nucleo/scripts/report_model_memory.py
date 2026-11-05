#!/usr/bin/env python3
"""Report model-specific live section sizes from a GNU ld map file."""

import argparse
import re
from collections import defaultdict
from pathlib import Path


MODEL_OBJECTS = {
    "Fuzzy": {
        "fuzzy-controller.c.obj",
        "Fuzzy.cpp.obj",
        "FuzzyComposition.cpp.obj",
        "FuzzyInput.cpp.obj",
        "FuzzyIO.cpp.obj",
        "FuzzyOutput.cpp.obj",
        "FuzzyRule.cpp.obj",
        "FuzzyRuleAntecedent.cpp.obj",
        "FuzzyRuleConsequent.cpp.obj",
        "FuzzySet.cpp.obj",
        "eFLL_wrapper.cpp.obj",
    },
    "Logistica": {"reg_model.c.obj"},
    "ArvoreDecisao": {"dec_model.c.obj"},
    "SVM": {"svm_model.c.obj"},
    "LGB": {"lgb_model.c.obj"},
    "RFR": {"rfr_model.c.obj"},
    "KNN": {"knn_model.c.obj"},
    "QDA": {
        "qda_model.c.obj",
        "qda_model_data.c.obj",
        "qda_model_data_params.c.obj",
        "qda_model_wrapper.c.obj",
    },
    "NNW": {
        "nnw_model.c.obj",
        "nnw_model_data.c.obj",
        "nnw_model_data_params.c.obj",
        "nnw_model_wrapper.c.obj",
    },
    "NBA": {
        "nba_model.c.obj",
        "nba_model_data.c.obj",
        "nba_model_data_params.c.obj",
        "nba_model_wrapper.c.obj",
    },
}

DIRECT_ENTRY = re.compile(
    r"^\s*(\.[^\s]+)\s+(0x[0-9a-fA-F]+)\s+(0x[0-9a-fA-F]+)\s+(\S+)$"
)
SECTION_ONLY = re.compile(r"^\s*(\.[^\s]+)\s*$")
ADDRESS_ENTRY = re.compile(r"^\s*(0x[0-9a-fA-F]+)\s+(0x[0-9a-fA-F]+)\s+(\S+)$")


def iter_map_entries(map_text):
    pending_section = None

    for line in map_text.splitlines():
        direct = DIRECT_ENTRY.match(line)
        if direct:
            section, address, size, obj = direct.groups()
            yield section, int(address, 16), int(size, 16), obj
            pending_section = None
            continue

        section_match = SECTION_ONLY.match(line)
        if section_match:
            pending_section = section_match.group(1)
            continue

        if pending_section is not None:
            address_match = ADDRESS_ENTRY.match(line)
            if address_match:
                address, size, obj = address_match.groups()
                yield pending_section, int(address, 16), int(size, 16), obj
            pending_section = None


def get_memory_kinds(section):
    if section.startswith((".data", ".tdata")):
        return ("flash", "ram")
    if section.startswith((".text", ".rodata", ".init", ".fini", ".ARM")):
        return ("flash",)
    if section.startswith((".bss", ".tbss", ".noinit", ".ccmram")):
        return ("ram",)
    return ()


def build_report(map_text):
    sizes = {
        name: {"flash": 0, "ram": 0}
        for name in MODEL_OBJECTS
    }
    seen_objects = set()

    for section, address, size, obj in iter_map_entries(map_text):
        object_name = obj.replace("\\", "/").rsplit("/", 1)[-1]
        for model, object_names in MODEL_OBJECTS.items():
            if object_name in object_names:
                seen_objects.add((model, object_name))
                if address != 0 and size > 0:
                    for kind in get_memory_kinds(section):
                        sizes[model][kind] += size
                break

    missing = []
    for model, object_names in MODEL_OBJECTS.items():
        model_objects_seen = {
            name for owner, name in seen_objects if owner == model
        }
        missing_objects = object_names - model_objects_seen
        if missing_objects:
            missing.append(f"{model}: {', '.join(sorted(missing_objects))}")
    if missing:
        raise ValueError("Model object(s) absent from linker map: " + "; ".join(missing))

    lines = [
        "Memoria por modelo (secoes vivas no mapa do linker)",
        "Modelo         Flash propria (B)   RAM estatica (B)",
        "----------------------------------------------------",
    ]
    for model, model_sizes in sizes.items():
        lines.append(
            f"{model:<14} {model_sizes['flash']:>17}   {model_sizes['ram']:>15}"
        )
    lines.extend(
        [
            "",
            "Flash/RAM contabilizadas a partir das secoes dos objetos de cada modelo.",
            "O relatorio nao atribui bibliotecas/runtime compartilhados a um modelo.",
            "RAM estatica nao inclui heap dinamico nem stack de execucao.",
            "Compare o crescimento do heap por inicializacao no log serial.",
        ]
    )
    return "\n".join(lines) + "\n"


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--map", required=True, type=Path, help="GNU ld map file")
    parser.add_argument("--output", required=True, type=Path, help="Report output path")
    args = parser.parse_args()

    report = build_report(args.map.read_text(encoding="utf-8", errors="replace"))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(report, encoding="utf-8")
    print(report, end="")


if __name__ == "__main__":
    main()
