# DCC059 - Teoria dos Grafos: Trabalho 2

Este projeto implementa soluções heurísticas baseadas em **GRASP** (Guloso, Guloso Randomizado e Guloso Randomizado Reativo) para resolver um problema de Otimização Combinatória em Grafos (conectar diferentes grupos minimizando o custo total).

## 🚀 Estrutura do Projeto
- `Main.cpp`: Arquivo principal, gerencia os testes, exportação da solução e logs (CSV).
- `Grafo.hpp` / `Grafo.cpp`: Estrutura de dados do grafo e leitura de instâncias.
- `Algoritmos.hpp` / `Algoritmos.cpp`: Implementação dos métodos construtivos (Guloso, GRASP, Reativo).
- `instancias/`: Diretório onde cada instância ganha sua própria pasta, organizando arquivos de texto, coordenadas e imagens geradas.
- `gerador.py`: Script gerador de instâncias planares e organizador de pastas.
- `plotar_solucao.py`: Script chamado automaticamente pelo programa C++ para desenhar a resposta (o melhor caminho em vermelho) após o teste.
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
1. `Caminho da Instancia` (Obrigatório caso queira testar uma instância específica gerada)
2. `Custo Ótimo Conhecido` (Padrão: *1e9*)
3. `Seed / Semente` (Opcional. Se não informada, usa a data/hora do sistema).

### Executando no Linux
```bash
./trabalho2.out instancias/grafo1/grafo1.txt 12.5 12345
```

### Executando no Windows
```cmd
.\trabalho2.exe instancias\grafo1\grafo1.txt 12.5 12345
```

> **Notas sobre a Saída Final:** 
> 1. O arquivo `resultados.csv` será atualizado com os tempos, custos e GAPs testados.
> 2. O programa chamará o Python sozinho para criar uma imagem `.png` mostrando o melhor caminho encontrado. Essa imagem ficará salva na mesma pasta da instância testada!

---

## 🐍 Utilitários em Python

O repositório conta com scripts extras escritos em Python para automatizar tarefas secundárias. É necessário ter o Python instalado com algumas bibliotecas:

### Instalação de Dependências
```bash
pip install matplotlib
```

### 1. Gerador de Instâncias (`gerador.py`)
Gera instâncias planas e conexas prontas para testes. Ele cria a pasta `instancias/` e gera subdiretórios individuais contendo os vértices, arestas e coordenadas físicas.
```bash
python gerador.py
```

### 2. Plotador de Solução (`plotar_solucao.py`)
**Você não precisa executar este script manualmente!** Ele é acionado por baixo dos panos na execução final do `Main.cpp`. Ele cruza o melhor resultado do C++ com as coordenadas 2D e renderiza o caminho traçado pelo GRASP.