#include "Algoritmos.hpp"
#include <iostream>
#include <limits>
#include <random>
#include <vector>
#include <cstdlib>

// Se alpha == 0.0, se comporta como Algoritmo Guloso clássico (100% determinístico e sem consumir rand())
Solucao construirSolucaoGRASP(const Grafo& grafo, double alpha) {
    Solucao solucao;
    solucao.custoTotal = 0.0;
    
    int numNos = grafo.getNumNos();
    int numGrupos = grafo.getNumGrupos();
    
    if (numNos == 0 || numGrupos == 0) {
        solucao.custoTotal = std::numeric_limits<double>::max();
        return solucao;
    }

    std::vector<bool> gruposVisitados(numGrupos, false);
    std::vector<int> nosNaArvore;

    // Início aleatório ou fixo, dependendo do alpha
    int noInicial;

    if(alpha == 0.0) {
        noInicial = 0; // Determinístico: não chama rand()
    } else {
        noInicial = rand() % numNos;
    }
        
    int grupoInicial = grafo.getGrupo(noInicial);

    gruposVisitados[grupoInicial] = true;
    nosNaArvore.push_back(noInicial);
    
    int gruposConectados = 1;

    while (gruposConectados < numGrupos) {
        std::vector<Aresta> candidatas;
        double cMin = std::numeric_limits<double>::max();
        double cMax = std::numeric_limits<double>::lowest();

        // 1. Levantar todas as arestas que conectam a árvore a um grupo novo
        for (int u : nosNaArvore) {
            const std::vector<Aresta>& vizinhos = grafo.getAdjacentes(u);
            for (const Aresta& aresta : vizinhos) {
                int v = aresta.destino;
                int grupoDoDestino = grafo.getGrupo(v);

                if (!gruposVisitados[grupoDoDestino]) {
                    candidatas.push_back(aresta);
                    if (aresta.peso < cMin) cMin = aresta.peso;
                    if (aresta.peso > cMax) cMax = aresta.peso;
                }
            }
        }

        if (candidatas.empty()) {
            solucao.custoTotal = std::numeric_limits<double>::max();
            return solucao;
        }

        // 2. Calcular o limite de custo para a LRC
        double limite = cMin + alpha * (cMax - cMin);

        // 3. Preencher a Lista Restrita de Candidatos (LRC)
        std::vector<Aresta> lrc;
        for (const Aresta& c : candidatas) {
            if (c.peso <= limite + 1e-9) { 
                lrc.push_back(c);
            }
        }

        // 4. Selecionar da LRC
        int idxSorteado = 0; // Por padrão, pega o primeiro (determinístico)
        
        // SÓ CHAMA O RAND() SE FOR GRASP/REATIVO (alpha > 0)
        if (alpha > 0.0) {
            idxSorteado = rand() % lrc.size();
        }
        
        Aresta escolhida = lrc[idxSorteado];

        // 5. Atualizar a solução
        solucao.arestas.push_back(escolhida);
        solucao.custoTotal += escolhida.peso;
        nosNaArvore.push_back(escolhida.destino);
        gruposVisitados[grafo.getGrupo(escolhida.destino)] = true;
        
        gruposConectados++;
    }

    return solucao;
}

// Algoritmo Guloso
Solucao algoritmoGuloso(const Grafo& grafo) {
    Solucao sol = construirSolucaoGRASP(grafo, 0.0);
    std::cout << "[Guloso] Concluido. Custo Encontrado: " << sol.custoTotal << "\n";
    return sol;
}

// Algoritmo Guloso Randomizado
Solucao algoritmoGulosoRandomizado(const Grafo& grafo, double alpha, int numIteracoes) {
    Solucao melhorSolucao;
    melhorSolucao.custoTotal = std::numeric_limits<double>::max();

    for (int iter = 0; iter < numIteracoes; ++iter) {
        Solucao solAtual = construirSolucaoGRASP(grafo, alpha);
        
        if (solAtual.custoTotal < melhorSolucao.custoTotal) {
            melhorSolucao = solAtual;
        }
    }
    
    std::cout << "[Guloso Randomizado] Concluido (" << numIteracoes << " iteracoes). Custo: " << melhorSolucao.custoTotal << "\n";
    return melhorSolucao;
}

// Algoritmo Guloso Randomizado Reativo
Solucao algoritmoGulosoRandomizadoReativo(const Grafo& grafo, int numIteracoes, int tamanhoBloco) {
    Solucao melhorSolucaoGlob;
    melhorSolucaoGlob.custoTotal = std::numeric_limits<double>::max();
    
    std::vector<double> alphas = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    int numAlphas = alphas.size();
    
    std::vector<double> probabilidades(numAlphas, 1.0 / numAlphas);
    std::vector<double> somaCustos(numAlphas, 0.0);
    std::vector<int> qtdUsos(numAlphas, 0);

    for (int iter = 0; iter < numIteracoes; ++iter) {
        if (iter > 0 && iter % tamanhoBloco == 0) {
            double somaQ = 0.0;
            std::vector<double> q(numAlphas, 0.0);

            for (int i = 0; i < numAlphas; ++i) {
                if (qtdUsos[i] == 0) {
                    q[i] = 1.0;
                } else {
                    double mediaAlpha = somaCustos[i] / qtdUsos[i];
                    q[i] = melhorSolucaoGlob.custoTotal / mediaAlpha; 
                }
                somaQ += q[i];
            }

            if (somaQ > 0) {
                for (int i = 0; i < numAlphas; ++i) {
                    probabilidades[i] = q[i] / somaQ;
                }
            }
        }
        
        double r = (double)rand() / RAND_MAX;
        double acumulado = 0.0;
        int alphaSorteado = numAlphas - 1;
        
        for (int i = 0; i < numAlphas; ++i) {
            acumulado += probabilidades[i];
            if (r <= acumulado) {
                alphaSorteado = i;
                break;
            }
        }
        
        Solucao solAtual = construirSolucaoGRASP(grafo, alphas[alphaSorteado]);
        
        if (solAtual.custoTotal < std::numeric_limits<double>::max()) {
            somaCustos[alphaSorteado] += solAtual.custoTotal;
            qtdUsos[alphaSorteado]++;
            
            if (solAtual.custoTotal < melhorSolucaoGlob.custoTotal) {
                melhorSolucaoGlob = solAtual;
            }
        }
    }
    
    std::cout << "[Guloso Randomizado Reativo] Concluido. Custo: " << melhorSolucaoGlob.custoTotal << "\n";
    return melhorSolucaoGlob;
}