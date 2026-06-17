#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>
#include <string>

// Estrutura para representar as conexões (arestas)
struct Aresta {
    int origem;
    int destino;
    double peso;
};

// Estrutura do vértice pertencente a um agrupamento
struct No {
    int id;
    int grupo; // ID do agrupamento (cluster) a que o nó pertence
};

class Grafo {
private:
    int numNos;
    int numArestas;
    int numGrupos;
    std::vector<No> nos;
    std::vector<std::vector<Aresta>> listaAdj; // Lista de adjacência

public:
    Grafo(int n);
    ~Grafo();

    void adicionarAresta(int u, int v, double peso);
    void definirGrupo(int idNo, int idGrupo);
    
    // Getters
    int getNumNos() const;
    int getNumGrupos() const;
    const std::vector<Aresta>& getAdjacentes(int no) const;
    int getGrupo(int no) const;

    // Leitura estática para inicializar o grafo a partir de um arquivo
    static Grafo* carregarDeArquivo(const std::string& caminhoArquivo);
    
    // Impressão recomendada para usar no csacademy.com/app/grapheditor/
    void imprimirFormatoVisualizacao() const;
};

#endif // GRAFO_H