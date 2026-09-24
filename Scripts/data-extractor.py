import serial
import csv
import time
import sys

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

    executando = True
    contador = 0
    linhas_salvas = 0

    print("Iniciando comunicação serial...")

    porta_serial = serial.Serial(com_port, baud_rate, timeout=1)

    print(f"Abrindo arquivo '{nome_arquivo}' para gravação...")

    with open(nome_arquivo, mode='w', newline='') as arquivo_csv:
        escritor_csv = csv.writer(arquivo_csv)
        print("Arquivo aberto. Iniciando captura de dados...\n")

        while executando:
            linha = porta_serial.readline().decode(errors='ignore').strip()
            contador += 1

            if linha:
                valores = linha.split(',')

                if len(valores) == col_count:
                    escritor_csv.writerow(valores)
                    linhas_salvas += 1
                    print(f"[{contador}] DADO SALVO ({len(valores)} colunas)")
                else:
                    print(f"[{contador}] LINHA DESCARTADA ({len(valores)} colunas)")

            else:
                print(f"[{contador}] Nenhum dado recebido...")

            if contador > 300:
                executando = False
                print("\nEncerrando captura de dados...")
                print(f"Total de linhas salvas: {linhas_salvas}")

            time.sleep(0.2)

    print("Programa finalizado.")

if __name__ == '__main__':
    main()