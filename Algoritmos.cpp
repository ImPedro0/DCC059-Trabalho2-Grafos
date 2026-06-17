#include "Algoritmos.hpp"
#include <iostream>
#include <limits>
#include <random>

Solucao algoritmoGuloso(const Grafo& grafo) {
    Solucao solucao;
    solucao.custoTotal = 0.0;
    
    int numNos = grafo.getNumNos();
    int numGrupos = grafo.getNumGrupos();
    
    if (numNos == 0 || numGrupos == 0) {
        return solucao;
    }

    std::vector<bool> gruposVisitados(numGrupos, false);
    
    std::vector<int> nosNaArvore;

    int noInicial = rand() % numNos;
    int grupoInicial = grafo.getGrupo(noInicial);

    gruposVisitados[grupoInicial] = true;
    nosNaArvore.push_back(noInicial);
    
    int gruposConectados = 1;

    while (gruposConectados < numGrupos) {
        
        Aresta melhorAresta;
        melhorAresta.peso = std::numeric_limits<double>::max();
        bool encontrouAresta = false;
        int novoNo = -1;

        for (int u : nosNaArvore) {
            const std::vector<Aresta>& vizinhos = grafo.getAdjacentes(u);
            
            for (const Aresta& aresta : vizinhos) {
                int v = aresta.destino;
                int grupoDoDestino = grafo.getGrupo(v);

                if (!gruposVisitados[grupoDoDestino]) {
                    
                    if (aresta.peso < melhorAresta.peso) {
                        melhorAresta = aresta;
                        novoNo = v;
                        encontrouAresta = true;
                    }
                }
            }
        }

        if (!encontrouAresta) {
            std::cerr << "Erro: Nao foi possivel conectar todos os grupos. O grafo e desconexo!\n";
            break;
        }

        solucao.arestas.push_back(melhorAresta);
        solucao.custoTotal += melhorAresta.peso;

        nosNaArvore.push_back(novoNo);
        gruposVisitados[grafo.getGrupo(novoNo)] = true;
        
        gruposConectados++;
    }

    std::cout << "[Guloso] Concluido. Custo Encontrado: " << solucao.custoTotal << "\n";
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