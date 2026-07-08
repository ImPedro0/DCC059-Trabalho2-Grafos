# DCC059 - Teoria dos Grafos: Trabalho 2

Este projeto implementa soluções heurísticas baseadas em **GRASP** (Guloso, Guloso Randomizado e Guloso Randomizado Reativo) para resolver um problema de Otimização Combinatória em Grafos (conectar diferentes grupos minimizando o custo total).

## 🚀 Estrutura do Projeto
- `Main.cpp`: Arquivo principal, gerencia os testes, controle de seeds e logs (CSV).
- `Grafo.hpp` / `Grafo.cpp`: Estrutura de dados do grafo e leitura de instâncias.
- `Algoritmos.hpp` / `Algoritmos.cpp`: Implementação dos métodos construtivos (Guloso, GRASP, Reativo).
- `gerador.py`: Script gerador de instâncias planares (com representação gráfica).
- `gerador_pptx.py`: Script automatizado para geração dos slides da apresentação.

---

## 🛠️ Como Compilar o Código (C++)

Você precisará de um compilador C++ (como o `g++`) instalado na sua máquina.

### No Linux (Ubuntu / Mint / etc)
Abra o terminal na pasta raiz do projeto e execute:
```bash
g++ Main.cpp Grafo.cpp Algoritmos.cpp -o trabalho2.out
```
*(Isso criará um arquivo executável chamado `trabalho2.out`)*

### No Windows (MinGW / Prompt de Comando / PowerShell)
Abra o terminal na pasta raiz do projeto e execute:
```cmd
g++ Main.cpp Grafo.cpp Algoritmos.cpp -o trabalho2.exe
```
*(Isso criará um arquivo executável chamado `trabalho2.exe`)*

---

## ▶️ Como Executar o Programa

O programa pode receber até 3 parâmetros via linha de comando:
1. `Caminho da Instancia` (Padrão: *instancia.txt*)
2. `Custo Ótimo Conhecido` (Padrão: *5.0*)
3. `Seed / Semente` (Opcional. Se não informada, usa a data/hora do sistema).

### Executando no Linux
**1. Execução Padrão (usa instância padrão e gera seed aleatória):**
```bash
./trabalho2.out
```
**2. Execução passando os 3 parâmetros:**
```bash
./trabalho2.out instancia_teste.txt 12.5 12345
```

### Executando no Windows
**1. Execução Padrão (usa instância padrão e gera seed aleatória):**
```cmd
.\trabalho2.exe
```
**2. Execução passando os 3 parâmetros:**
```cmd
.\trabalho2.exe instancia_teste.txt 12.5 12345
```

> **Nota sobre a Saída:** 
> Após a execução, o programa exibirá as estatísticas no console e salvará (ou atualizará) o arquivo `resultados.csv` contendo os custos, tempos, GAP e seeds usadas, facilitando a elaboração do relatório final.

---

## 🐍 Utilitários em Python

O repositório conta um scripts extras escritos em Python. Para executá-los, é necessário ter o Python instalado e as bibliotecas listadas abaixo.

**Gerador de Instâncias e Gráficos 2D**
Cria instâncias planares garantindo que o grafo seja conexo.
```bash
pip install matplotlib
python gerador.py
```