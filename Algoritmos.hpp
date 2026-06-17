#ifndef ALGORITMOS_HPP
#define ALGORITMOS_HPP

#include "Grafo.hpp"
#include <vector>

struct Solucao {
    std::vector<Aresta> arestas;
    double custoTotal;
};

// Algoritmo Guloso 
Solucao algoritmoGuloso(const Grafo& grafo);

// Algoritmo Guloso Randomizado (Parâmetros: alfa, num_iterações)
Solucao algoritmoGulosoRandomizado(const Grafo& grafo, double alpha, int numIteracoes);

// Algoritmo Guloso Randomizado Reativo (Parâmetros: num_iterações, tamanho do bloco)
Solucao algoritmoGulosoRandomizadoReativo(const Grafo& grafo, int numIteracoes, int tamanhoBloco);

#endif