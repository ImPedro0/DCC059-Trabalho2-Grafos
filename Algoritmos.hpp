#ifndef ALGORITMOS_HPP
#define ALGORITMOS_HPP

#include "Grafo.hpp"
#include <vector>
#include <string>
#include <chrono>

struct Solucao {
    std::vector<Aresta> arestas;
    double custoTotal;
};

struct ResultadoExecucao {
    std::string nomeAlgoritmo;
    double custoTotal;
    double tempoExecucaoMs; // Tempo em milissegundos
    unsigned int seed;
    std::string parametros; // ex: "alpha=0.3, iter=1000"
};

// Algoritmo Guloso 
Solucao algoritmoGuloso(const Grafo& grafo);

// Algoritmo Guloso Randomizado (Parâmetros: alfa, num_iterações)
Solucao algoritmoGulosoRandomizado(const Grafo& grafo, double alpha, int numIteracoes);

// Algoritmo Guloso Randomizado Reativo (Parâmetros: num_iterações, tamanho do bloco)
Solucao algoritmoGulosoRandomizadoReativo(const Grafo& grafo, int numIteracoes, int tamanhoBloco);

#endif