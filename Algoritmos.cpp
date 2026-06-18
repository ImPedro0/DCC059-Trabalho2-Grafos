#include "Algoritmos.hpp"
#include <iostream>
#include <limits>
#include <random>
#include <vector>
#include <cstdlib>

// -------------------------------------------------------------------------
// FUNÇÃO AUXILIAR: Constrói uma solução usando a lógica GRASP (LRC)
// Se alpha == 0.0, comporta-se exatamente como o Algoritmo Guloso clássico.
// -------------------------------------------------------------------------
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

    // Início aleatório (já dependente da semente do srand)
    int noInicial;

    if(alpha == 0.0)
        noInicial = 0;
    else
        noInicial = rand() % numNos;
        
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

        // Se não houver candidatas, o grafo é desconexo entre os grupos restantes
        if (candidatas.empty()) {
            solucao.custoTotal = std::numeric_limits<double>::max(); // Solução inválida
            return solucao;
        }

        // 2. Calcular o limite de custo para a LRC
        double limite = cMin + alpha * (cMax - cMin);

        // 3. Preencher a Lista Restrita de Candidatos (LRC)
        std::vector<Aresta> lrc;
        for (const Aresta& c : candidatas) {
            // Usa-se uma pequena tolerância (+1e-9) por conta da imprecisão do double
            if (c.peso <= limite + 1e-9) { 
                lrc.push_back(c);
            }
        }

        // 4. Sortear uniformemente um elemento da LRC
        int idxSorteado = rand() % lrc.size();
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

// -------------------------------------------------------------------------
// 1. Algoritmo Guloso (Construtivo Clássico)
// -------------------------------------------------------------------------
Solucao algoritmoGuloso(const Grafo& grafo) {
    // Para o guloso puro, alpha é 0 (sempre pega o cMin)
    Solucao sol = construirSolucaoGRASP(grafo, 0.0);
    std::cout << "[Guloso] Concluido. Custo Encontrado: " << sol.custoTotal << "\n";
    return sol;
}

// -------------------------------------------------------------------------
// 2. Algoritmo Guloso Randomizado
// -------------------------------------------------------------------------
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

// -------------------------------------------------------------------------
// 3. Algoritmo Guloso Randomizado Reativo
// -------------------------------------------------------------------------
Solucao algoritmoGulosoRandomizadoReativo(const Grafo& grafo, int numIteracoes, int tamanhoBloco) {
    Solucao melhorSolucaoGlob;
    melhorSolucaoGlob.custoTotal = std::numeric_limits<double>::max();
    
    // Conjunto de alphas sugeridos frequentemente na literatura
    std::vector<double> alphas = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    int numAlphas = alphas.size();
    
    std::vector<double> probabilidades(numAlphas, 1.0 / numAlphas);
    std::vector<double> somaCustos(numAlphas, 0.0);
    std::vector<int> qtdUsos(numAlphas, 0);

    for (int iter = 0; iter < numIteracoes; ++iter) {
        // A cada final de bloco, reajustar as probabilidades (Reativo)
        if (iter > 0 && iter % tamanhoBloco == 0) {
            double somaQ = 0.0;
            std::vector<double> q(numAlphas, 0.0);

            for (int i = 0; i < numAlphas; ++i) {
                if (qtdUsos[i] == 0) {
                    q[i] = 1.0; // Valor neutro para garantir que alphas não usados tenham chance
                } else {
                    double mediaAlpha = somaCustos[i] / qtdUsos[i];
                    // q_i = Z* / A_i (Melhor global dividido pela média do alpha)
                    q[i] = melhorSolucaoGlob.custoTotal / mediaAlpha; 
                }
                somaQ += q[i];
            }

            // Atualiza a roleta de probabilidades
            if (somaQ > 0) {
                for (int i = 0; i < numAlphas; ++i) {
                    probabilidades[i] = q[i] / somaQ;
                }
            }
        }
        
        // Sorteio do Alpha usando a roleta (Roulette Wheel Selection)
        double r = (double)rand() / RAND_MAX; // num aleatório entre 0 e 1
        double acumulado = 0.0;
        int alphaSorteado = numAlphas - 1; // fallback
        
        for (int i = 0; i < numAlphas; ++i) {
            acumulado += probabilidades[i];
            if (r <= acumulado) {
                alphaSorteado = i;
                break;
            }
        }
        
        // Executar iteração com o alpha selecionado
        Solucao solAtual = construirSolucaoGRASP(grafo, alphas[alphaSorteado]);
        
        // Se encontrou uma solução válida, atualiza estatísticas do reativo
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