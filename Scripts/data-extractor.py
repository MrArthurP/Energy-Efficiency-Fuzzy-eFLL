import serial
import csv
import time
import sys

# Quantidade de registros gerados pelo firmware durante as execuções.
execucoes_esperadas: int = 300
modelos_esperados: tuple[str, ...] = (
    'Fuzzy',
    'Logistica',
    'SVM',
    'ArvoreDecisao',
    'LGB',
    'RFR',
    'QDA',
    'NNW',
    'KNN',
)
modelos_por_execucao: int = len(modelos_esperados)
registros_esperados: int = execucoes_esperadas * modelos_por_execucao
timeout_inatividade: int = 15

# Valor padrão caso nenhum argumento seja passado pelo terminal
nome_arquivo_padrao: str = 'Data/stm32report/unidentified_model-1.csv'

# Se um argumento for fornecido na linha de comando, usa ele; caso contrário, usa o valor padrão
if len(sys.argv) > 1:
    nome_arquivo: str = sys.argv[1]
else:
    nome_arquivo: str = nome_arquivo_padrao

baud_rate: int = 115200         # taxa baud do monitor serial que deve ser definida: Arduino-9600, Stm32-115200
com_port: str = 'COM7'          # Definição de qual porta COM o controlador está conectado
col_count: int = 5              # Definição de quantas colunas de dados será impressa no monitor serial

def main():

    linhas_salvas = 0
    linhas_descartadas = 0
    registros_recebidos: set[tuple[int, str]] = set()
    ultima_linha_valida = time.monotonic()

    print(f"Iniciando comunicação serial; aguardando {registros_esperados} registros...")

    with serial.Serial(com_port, baud_rate, timeout=1) as porta_serial:
        print(f"Abrindo arquivo '{nome_arquivo}' para gravação...")

        with open(nome_arquivo, mode='w', newline='', encoding='utf-8') as arquivo_csv:
            escritor_csv = csv.writer(arquivo_csv)
            print("Arquivo aberto. Iniciando captura de dados...\n")

            while linhas_salvas < registros_esperados:
                linha = porta_serial.readline().decode(errors='ignore').strip()

                if not linha:
                    if time.monotonic() - ultima_linha_valida >= timeout_inatividade:
                        raise TimeoutError(
                            f"Sem registros válidos por {timeout_inatividade} segundos. "
                            f"Capturados {linhas_salvas}/{registros_esperados}; "
                            f"verifique a placa, a porta {com_port} e o baud rate."
                        )
                    continue

                valores = linha.split(',')
                if len(valores) != col_count:
                    linhas_descartadas += 1
                    continue

                try:
                    indice_execucao = int(valores[0].strip().strip('[]'))
                except ValueError:
                    linhas_descartadas += 1
                    continue

                # O firmware também reporta oito chamadas de inicialização no índice 0.
                if not 1 <= indice_execucao <= execucoes_esperadas:
                    linhas_descartadas += 1
                    continue

                nome_modelo = valores[1].strip()
                identificador = (indice_execucao, nome_modelo)
                if nome_modelo not in modelos_esperados or identificador in registros_recebidos:
                    linhas_descartadas += 1
                    continue

                escritor_csv.writerow(valores)
                registros_recebidos.add(identificador)
                linhas_salvas += 1
                ultima_linha_valida = time.monotonic()

                if linhas_salvas % modelos_por_execucao == 0:
                    print(
                        f"\rExecução {linhas_salvas // modelos_por_execucao}/"
                        f"{execucoes_esperadas} — {linhas_salvas}/{registros_esperados} "
                        "registros salvos",
                        end='',
                        flush=True
                    )

    print(
        f"\nCaptura concluída. {linhas_salvas} registros salvos; "
        f"{linhas_descartadas} linhas descartadas."
    )
    print("Programa finalizado.")

if __name__ == '__main__':
    main()