#include "Grafo.hpp"
#include <iostream>
#include <fstream>

Grafo::Grafo(int n) : numNos(n), numArestas(0), numGrupos(0) {
    nos.resize(n);
    listaAdj.resize(n);
    for (int i = 0; i < n; ++i) {
        nos[i].id = i;
        nos[i].grupo = -1;
    }
}

Grafo::~Grafo() {}

void Grafo::adicionarAresta(int u, int v, double peso) {
    listaAdj[u].push_back({u, v, peso});
    listaAdj[v].push_back({v, u, peso});
    numArestas++;
}

void Grafo::definirGrupo(int idNo, int idGrupo) {
    nos[idNo].grupo = idGrupo;
    if (idGrupo >= numGrupos) {
        numGrupos = idGrupo + 1;
    }
}

int Grafo::getNumNos() const { return numNos; }
int Grafo::getNumGrupos() const { return numGrupos; }
const std::vector<Aresta>& Grafo::getAdjacentes(int no) const { return listaAdj[no]; }
int Grafo::getGrupo(int no) const { return nos[no].grupo; }

// [Origem] [Destino] [Peso]

Grafo* Grafo::carregarDeArquivo(const std::string& caminhoArquivo) {
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << caminhoArquivo << std::endl;
        return nullptr;
    }

    int n, m, k;
    arquivo >> n >> m >> k;
    Grafo* grafo = new Grafo(n);

    for (int i = 0; i < n; ++i) {
        int id, grupo;
        arquivo >> id >> grupo;
        grafo->definirGrupo(id, grupo);
    }

    for (int i = 0; i < m; ++i) {
        int u, v;
        double peso;
        arquivo >> u >> v >> peso;
        grafo->adicionarAresta(u, v, peso);
    }

    arquivo.close();
    return grafo;
}

void Grafo::imprimirFormatoVisualizacao() const {
    for (int u = 0; u < numNos; ++u) {
        for (const auto& aresta : listaAdj[u]) {
            if (u < aresta.destino) { 
                std::cout << u << " " << aresta.destino << " " << aresta.peso << "\n";
            }
        }
    }
}