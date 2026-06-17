#include "Algoritmos.hpp"
#include <iostream>
#include <limits>
#include <random>

Solucao algoritmoGuloso(const Grafo& grafo) {
    Solucao solucao;
    solucao.custoTotal = 0.0;
    
    // TODO: Implementar lógica do Guloso Construtivo para AGMG.
    // Dica: Inicie em um nó qualquer, marque o grupo desse nó como visitado.
    // Enquanto houver grupos não visitados, busque a aresta de menor peso 
    // que ligue um nó já pertencente à árvore a um nó de um GRUPO ainda não visitado.
    
    std::cout << "[Guloso] Concluido.\n";
    return solucao;
}

Solucao algoritmoGulosoRandomizado(const Grafo& grafo, double alpha, int numIteracoes) {
    Solucao melhorSolucao;
    melhorSolucao.custoTotal = std::numeric_limits<double>::max();

    for (int iter = 0; iter < numIteracoes; ++iter) {
        Solucao solAtual;
        solAtual.custoTotal = 0.0;
        
        // TODO: Lógica de construção da LRC (Lista Restrita de Candidatos).
        // Custo limite = CustoMin + alpha * (CustoMax - CustoMin)
        // Ao invés de pegar a melhor aresta rigidamente, liste as que estão abaixo
        // do custo limite e escolha aleatoriamente uma delas para adicionar à solução.
        
        if (solAtual.custoTotal < melhorSolucao.custoTotal) {
            melhorSolucao = solAtual; // Atualiza a melhor solução encontrada
        }
    }
    
    std::cout << "[Guloso Randomizado] Concluido (" << numIteracoes << " iteracoes).\n";
    return melhorSolucao;
}

Solucao algoritmoGulosoRandomizadoReativo(const Grafo& grafo, int numIteracoes, int tamanhoBloco) {
    Solucao melhorSolucao;
    melhorSolucao.custoTotal = std::numeric_limits<double>::max();
    
    // Conjunto de alphas a serem testados
    std::vector<double> alphas = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    std::vector<double> probabilidades(alphas.size(), 1.0 / alphas.size());
    
    for (int iter = 0; iter < numIteracoes; ++iter) {
        if (iter > 0 && iter % tamanhoBloco == 0) {
            // TODO: A cada bloco, recalcular as probabilidades de acordo com o
            // desempenho (qualidade da solução) das iterações geradas por cada alpha.
        }
        
        // TODO: Sortear um alpha através do vetor de probabilidades (roleta).
        // Executar iteração do Guloso Randomizado utilizando o alpha sorteado
    }
    
    std::cout << "[Guloso Randomizado Reativo] Concluido.\n";
    return melhorSolucao;
}