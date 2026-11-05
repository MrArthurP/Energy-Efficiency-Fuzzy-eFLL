#!/usr/bin/env python3
"""Captura os registros do controlador fuzzy do Arduino em um arquivo CSV.

Uso:
    python Scripts/data-extractor-arduino.py
    python Scripts/data-extractor-arduino.py --port COM4 --output Data/fuzzydata/teste.csv
"""

import argparse
import csv
import time
from pathlib import Path

import serial


AMOSTRAS_ESPERADAS = 300
COLUNAS = (
    "index",
    *(f"Regra{grupo}_{regra}" for grupo in range(1, 6) for regra in range(1, 5)),
    "Entrada1",
    "Entrada2",
    "PertinenciaLiga",
    "PertinenciaNaoLiga",
    "Saida",
    "EstadoSaida",
)
TEMPO_LIMITE_INATIVIDADE = 15


def parse_linha(linha: str) -> tuple[int, list[str]] | None:
    """Retorna o índice e os valores de um registro válido do firmware."""
    valores = [valor.strip() for valor in linha.split(",")]
    if len(valores) != len(COLUNAS):
        return None

    try:
        indice = int(valores[0])
        for valor in valores[1:]:
            float(valor)
    except ValueError:
        return None

    if not 0 <= indice < AMOSTRAS_ESPERADAS:
        return None

    try:
        estado_saida = int(valores[-1])
    except ValueError:
        return None

    if estado_saida not in (0, 1):
        return None

    return indice, valores


def criar_argumentos() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Captura os dados do modelo fuzzy enviados pelo Arduino."
    )
    parser.add_argument("--port", default="COM7", help="Porta serial (padrão: COM7)")
    parser.add_argument(
        "--baud-rate", type=int, default=9600, help="Baud rate (padrão: 9600)"
    )
    parser.add_argument(
        "--output",
        default="Data/fuzzydata/arduino-fuzzy-report.csv",
        help="Arquivo CSV de saída",
    )
    parser.add_argument(
        "--timeout",
        type=int,
        default=TEMPO_LIMITE_INATIVIDADE,
        help="Segundos sem registro válido antes de interromper a captura",
    )
    return parser.parse_args()


def main() -> None:
    args = criar_argumentos()
    caminho_saida = Path(args.output)
    caminho_saida.parent.mkdir(parents=True, exist_ok=True)

    indices_recebidos: set[int] = set()
    linhas_descartadas = 0
    ultima_linha_valida = time.monotonic()
    print(
        f"Aguardando {AMOSTRAS_ESPERADAS} registros fuzzy na porta "
        f"{args.port} ({args.baud_rate} baud)..."
    )

    with serial.Serial(args.port, args.baud_rate, timeout=1) as porta_serial:
        with caminho_saida.open("w", newline="", encoding="utf-8") as arquivo_csv:
            escritor_csv = csv.writer(arquivo_csv)
            escritor_csv.writerow(COLUNAS)

            while len(indices_recebidos) < AMOSTRAS_ESPERADAS:
                linha = porta_serial.readline().decode(errors="ignore").strip()
                if not linha:
                    if time.monotonic() - ultima_linha_valida >= args.timeout:
                        raise TimeoutError(
                            f"Sem registros válidos por {args.timeout} segundos. "
                            f"Capturados {len(indices_recebidos)}/"
                            f"{AMOSTRAS_ESPERADAS}; verifique a placa e a conexão serial."
                        )
                    continue

                registro = parse_linha(linha)
                if registro is None:
                    linhas_descartadas += 1
                    continue

                indice, valores = registro
                if indice in indices_recebidos:
                    linhas_descartadas += 1
                    continue

                escritor_csv.writerow(valores)
                indices_recebidos.add(indice)
                ultima_linha_valida = time.monotonic()

                if len(indices_recebidos) % 25 == 0:
                    print(
                        f"\r{len(indices_recebidos)}/{AMOSTRAS_ESPERADAS} "
                        "registros salvos",
                        end="",
                        flush=True,
                    )

    print(
        f"\nCaptura concluída: {len(indices_recebidos)} registros salvos em "
        f"'{caminho_saida}'; {linhas_descartadas} linhas descartadas."
    )


if __name__ == "__main__":
    main()
