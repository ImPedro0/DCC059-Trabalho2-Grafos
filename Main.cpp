#include <iostream>
#include <chrono>
#include <random>
#include <string>
#include "Grafo.hpp"
#include "Algoritmos.hpp"

int main(int argc, char* argv[]) {
    unsigned int semente;
    
    // Configura semente baseada na data/hora ou por parâmetro
    if (argc > 1) {
        semente = std::stoul(argv[1]);
    } else {
        semente = std::chrono::system_clock::now().time_since_epoch().count();
    }
    
    std::cout << "Semente de randomizacao (Seed): " << semente << std::endl;
    // Opcional para C-style rand(), mas usar o std::mt19937 nos algoritmos é superior
    srand(semente); 
    
    // Caminho fixo ou via argumento para a instância
    std::string caminhoArquivo = "instancia.txt";
    if(argc > 2) caminhoArquivo = argv[2];

    Grafo* grafo = Grafo::carregarDeArquivo(caminhoArquivo);
    if (!grafo) {
        return 1;
    }

    std::cout << "Grafo processado! Nós: " << grafo->getNumNos() 
              << ", Grupos: " << grafo->getNumGrupos() << "\n\n";

    // --- 1. Algoritmo Guloso ---
    Solucao solGuloso = algoritmoGuloso(*grafo);

    // --- 2. Algoritmo Guloso Randomizado (ex: alfa=0.3, iteracoes=100) ---
    double alpha = 0.3;
    int iteracoesGRASP = 1000;
    Solucao solGRASP = algoritmoGulosoRandomizado(*grafo, alpha, iteracoesGRASP);

    // --- 3. Algoritmo Guloso Randomizado Reativo ---
    int iteracoesReativo = 1000;
    int tamanhoBloco = 100;
    Solucao solReativo = algoritmoGulosoRandomizadoReativo(*grafo, iteracoesReativo, tamanhoBloco);

    // Libera a memória
    delete grafo;
    return 0;
}