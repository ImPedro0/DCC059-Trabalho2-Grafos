#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>
#include <string>

struct Aresta {
    int origem;
    int destino;
    double peso;
};

struct No {
    int id;
    int grupo;
};

class Grafo {
private:
    int numNos;
    int numArestas;
    int numGrupos;
    std::vector<No> nos;
    std::vector<std::vector<Aresta>> listaAdj;

public:
    Grafo(int n);
    ~Grafo();

    void adicionarAresta(int u, int v, double peso);
    void definirGrupo(int idNo, int idGrupo);
    
    int getNumNos() const;
    int getNumGrupos() const;
    const std::vector<Aresta>& getAdjacentes(int no) const;
    int getGrupo(int no) const;

    static Grafo* carregarDeArquivo(const std::string& caminhoArquivo);
    
    void imprimirFormatoVisualizacao() const;
};

#endif