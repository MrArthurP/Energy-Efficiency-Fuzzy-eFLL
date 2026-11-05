# Eficiencia Energética

# Configuração e Depuração STM32 no VS Code (Windows)

Guia completo para configurar o ambiente de desenvolvimento, compilação via CMake/Ninja e depuração via OpenOCD e GDB para microcontroladores STM32 utilizando o **Visual Studio Code**, contornando problemas comuns de caminhos e caracteres especiais no Windows.

---

## 📋 Pré-requisitos

Antes de iniciar a configuração do projeto, certifique-se de ter as seguintes ferramentas instaladas no seu sistema:

1. **Visual Studio Code** instalado.
2. **STM32CubeMX** (com o executável `STM32CubeMX.exe` integrado à extensão do VS Code).
3. **CMake** (Instalado em `C:\Program Files\CMake`).
4. **Ninja Build** (O arquivo `ninja.exe` deve ser colocado dentro da pasta `C:\Program Files\CMake\bin`).
5. **Arm GNU Toolchain** (`arm-none-eabi-gcc`) para sistemas x64. (Recomenda-se criar uma pasta dedicada direto em `C:`, como `C:/arm-compiler/`).
6. **OpenOCD (xPack)** (Baixado e descompactado na mesma pasta do compilador, ex: `C:/arm-compiler/xpack-openocd-...`).
7. **Python 3.8+** (necessário para gerar o relatório de memória por modelo ao compilar).

---

## ⚙️ Passo a Passo de Configuração
...
# A fazer
- [] Escrever o que foi realizado no tópico de "Machine Learning (otimização do modelo), e explicar o que cada coisa faz"
- [] Comparar os modelos existentes (fuzzy, Regressão Logística e Árvore de Decisão) com um condicional básico no funcionamento do Stm32Nucleo e realizar a análise
- [] Atualizar README.md de acordo com os Processos registrados

## Medicao de memoria dos modelos STM32

Ao compilar `Stm32-Nucleo` com CMake, o build gera `build/Debug/model_memory_report.txt`
(ou `build/Release/model_memory_report.txt`). O relatorio separa, para cada modelo,
Flash e RAM estatica com base nas secoes vivas dos objetos no mapa do linker.

Durante a inicializacao, o log serial tambem informa o crescimento observado do heap
para cada modelo e o total reservado ate aquele ponto. Esse crescimento mede quanto
o heap aumentou via `_sbrk`; nao necessariamente equivale ao total de memoria
solicitado por um modelo ao alocador, pois o alocador pode reutilizar blocos ja
reservados.

Cada chamada de inferencia tambem informa ciclos, pico de stack, variacao do heap e
heap reservado. O pico de stack e medido por watermark na regiao reservada pelo
linker; `(limite atingido)` indica que toda a regiao disponivel foi usada e a medida
nao deve ser tratada como um pico completo. A reserva minima de stack foi elevada
para 8 KiB porque a funcao RFR requer aproximadamente 5,3 KiB em seu proprio frame,
sem contar as funcoes chamadoras.

Os valores por objeto nao incluem codigo de bibliotecas/runtime compartilhados,
nem heap ou stack dinamicos. Consulte tambem o resumo de uso total de memoria do
linker para verificar os limites globais do firmware.
