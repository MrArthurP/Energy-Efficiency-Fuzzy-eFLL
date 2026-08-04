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

---

## ⚙️ Passo a Passo de Configuração

### 1. Configuração do Projeto no STM32CubeMX

Ao gerar o projeto pelo STM32CubeMX, vá até a aba **Project Manager** e configure o **Toolchain/IDE** para **CMake**.

### 2. Instalação das Extensões no VS Code

* Instale a extensão **STM32Cube for Visual Studio Code**.
* Instale a extensão **PlatformIO IDE**.
* Na aba de extensões do STM32Cube, vincule o caminho do executável do `STM32CubeMX.exe`.
* Instale os drivers de identificação da placa na seção **Install ST-Link USB drivers** da extensão.

### 3. Configuração do Compilador no `CMakePresets.json`

Adicione a variável indicando o caminho do seu compilador ARM dentro do arquivo `CMakePresets.json`:

```json
{
    "version": 3,
    "configurePresets": [
        {
            "name": "default",
            "hidden": true,
            "generator": "Ninja",
            "binaryDir": "${sourceDir}/build/${presetName}",
            "toolchainFile": "${sourceDir}/cmake/gcc-arm-none-eabi.cmake",
            "cacheVariables": {
              "STM32_TOOLCHAIN_PATH": "C:/arm-compiler/xpack-arm-none-eabi-gcc-15.2.1-1.1/bin"
            }
        }
    ]
}

```

### 4. Atualização do Cache do CMake

1. Pressione `Ctrl + Shift + P`, digite e selecione: **CMake: Delete Cache and Reconfigure**.
2. Repita o atalho `Ctrl + Shift + P` e selecione: **Developer: Reload Window**.
> *Nota:* Se o painel `OUTPUT` do VS Code retornar `-- Generating done`, o ambiente foi configurado com sucesso.

### 5. Configuração dos Arquivos de Depuração (`.vscode/`)

Crie ou edite os seguintes arquivos dentro da pasta `.vscode` do seu projeto:

#### `settings.json`

```json
{
    "cmake.sourceDirectory": "${workspaceFolder}/Stm32-Nucleo",
    "cmake.configureEnvironment": {
        "PATH": "C:/arm-compiler/xpack-arm-none-eabi-gcc-15.2.1-1.1/bin;${env:PATH}"
    },
    "cmake.debugConfig": {
        "type": "cppdbg",
        "MIMode": "gdb",
        "miDebuggerPath": "C:/arm-compiler/xpack-arm-none-eabi-gcc-15.2.1-1.1/bin/arm-none-eabi-gdb.exe",
        "miDebuggerServerAddress": "localhost:50000",
        "program": "${workspaceFolder}/Stm32-Nucleo/build/Debug/Stm32-Nucleo.elf",
        "cwd": "${workspaceFolder}/Stm32-Nucleo",
        "externalConsole": false,
        "preLaunchTask": "Iniciar OpenOCD"
    }
}

```

#### `launch.json`

```json
{
    "version": "0.2.0",
    "configurations": []
}

```

#### `tasks.json`

Este arquivo inicia o OpenOCD em segundo plano via porta `50000`, evitando erros de caminhos com caracteres especiais do Windows:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Iniciar OpenOCD",
            "type": "shell",
            "command": "C:/arm-compiler/xpack-openocd-0.12.0-7/bin/openocd.exe",
            "args": [
                "-s", "C:/arm-compiler/xpack-openocd-0.12.0-7/openocd/scripts",
                "-f", "interface/stlink.cfg",
                "-f", "target/stm32f3x.cfg",
                "-c", "gdb_port 50000"
            ],
            "isBackground": true,
            "problemMatcher": [
                {
                    "pattern": [
                        {
                            "regexp": ".",
                            "file": 1,
                            "location": 2,
                            "message": 3
                        }
                    ],
                    "background": {
                        "activeOnStart": true,
                        "beginsPattern": "^xPack Open On-Chip Debugger.*",
                        "endsPattern": "^Info : Listening on port 50000 for gdb connections.*"
                    }
                }
            ]
        }
    ]
}

```

---

## 🚀 Como Executar e Depurar

### Stm32-Nucleo
1. Conecte sua placa STM32 Nucleo à porta USB do computador.
2. Pressione `Ctrl + Shift + P` e digite **CMake: Debug** para compilar o código, gravá-lo na memória flash e iniciar a sessão de depuração.
3. **Para encerrar a depuração:** Clique no botão de parada (quadrado vermelho `[]`), retorne ao terminal e pressione `Ctrl + C` para fechar o processo do compilador/OpenOCD.

### Arduino-Uno
1. Conecte sua placa Arduino-Uno à porta USB do computador.
2. Aperte o botão ✔️ localizado na extremidade inferior mais à direita do painel do VsCode para depurar e gravar o código.

# A fazer
[] Escrever o que foi realizado no tópico de "Machine Learning (otimização do modelo), e explicar o que cada coisa faz"
[] Comparar os modelos existentes (fuzzy, Regressão Logística e Árvore de Decisão) com um condicional básico no funcionamento do Stm32Nucleo e realizar a análise
[] Atualizar README.md de acordo com os Processos registrados


