#include "Algoritmos.hpp"
#include <iostream>
#include <limits>
#include <random>
#include <vector>
#include <cstdlib>

// Se alpha == 0.0, se comporta como Algoritmo Guloso clássico (100% determinístico e sem consumir rand())
Solucao construirSolucaoGRASP(const Grafo &grafo, double alpha)
{
    Solucao solucao;
    solucao.custoTotal = 0.0;

    int numNos = grafo.getNumNos();
    int numGrupos = grafo.getNumGrupos();

    if (numNos == 0 || numGrupos == 0)
    {
        solucao.custoTotal = std::numeric_limits<double>::max();
        return solucao;
    }

    std::vector<bool> gruposVisitados(numGrupos, false);
    // NOVA REGRA: Rastrear quais nós estão na árvore
    std::vector<bool> nosNaArvore(numNos, false);

    // Início aleatório, controlado pela semente atual
    int noInicial = rand() % numNos;
    int grupoInicial = grafo.getGrupo(noInicial);

    gruposVisitados[grupoInicial] = true;
    nosNaArvore[noInicial] = true;

    int gruposConectados = 1;

    while (gruposConectados < numGrupos)
    {
        std::vector<Aresta> candidatasNovas;
        std::vector<Aresta> candidatasPontes;
        
        double cMinNovas = std::numeric_limits<double>::max();
        double cMaxNovas = std::numeric_limits<double>::lowest();
        
        double cMinPontes = std::numeric_limits<double>::max();
        double cMaxPontes = std::numeric_limits<double>::lowest();

        // 1. Levantar todas as arestas que SAEM da árvore
        for (int u = 0; u < numNos; ++u)
        {
            if (nosNaArvore[u])
            {
                const std::vector<Aresta> &vizinhos = grafo.getAdjacentes(u);
                for (const Aresta &aresta : vizinhos)
                {
                    int v = aresta.destino;
                    
                    if (!nosNaArvore[v]) // Aresta expande a árvore
                    {
                        int grupoDoDestino = grafo.getGrupo(v);
                        
                        if (!gruposVisitados[grupoDoDestino]) 
                        {
                            // Leva para um grupo inédito (Prioridade Máxima)
                            candidatasNovas.push_back(aresta);
                            if (aresta.peso < cMinNovas) cMinNovas = aresta.peso;
                            if (aresta.peso > cMaxNovas) cMaxNovas = aresta.peso;
                        } 
                        else 
                        {
                            // Leva para um grupo repetido (Serve como Ponte)
                            candidatasPontes.push_back(aresta);
                            if (aresta.peso < cMinPontes) cMinPontes = aresta.peso;
                            if (aresta.peso > cMaxPontes) cMaxPontes = aresta.peso;
                        }
                    }
                }
            }
        }

        // 2. Escolher qual lista usar (Priorizamos grupos novos!)
        std::vector<Aresta>* listaBase = nullptr;
        double cMin, cMax;

        if (!candidatasNovas.empty())
        {
            listaBase = &candidatasNovas;
            cMin = cMinNovas;
            cMax = cMaxNovas;
        }
        else if (!candidatasPontes.empty())
        {
            // Beco sem saída! Somos obrigados a usar uma ponte para expandir a árvore.
            listaBase = &candidatasPontes;
            cMin = cMinPontes;
            cMax = cMaxPontes;
        }
        else
        {
            // Grafo fisicamente desconexo
            solucao.custoTotal = std::numeric_limits<double>::max();
            return solucao;
        }

        // 3. Calcular o limite de custo para a LRC
        double limite = cMin + alpha * (cMax - cMin);

        // 4. Preencher a Lista Restrita de Candidatos (LRC)
        std::vector<Aresta> lrc;
        for (const Aresta &c : *listaBase)
        {
            if (c.peso <= limite + 1e-9)
            {
                lrc.push_back(c);
            }
        }

        // 5. Selecionar da LRC
        int idxSorteado = 0; 
        if (alpha > 0.0)
        {
            idxSorteado = rand() % lrc.size();
        }

        Aresta escolhida = lrc[idxSorteado];
        int v = escolhida.destino;
        int grupoDoDestino = grafo.getGrupo(v);

        // 6. Atualizar a solução e conectar o nó fisicamente à árvore
        solucao.arestas.push_back(escolhida);
        solucao.custoTotal += escolhida.peso;
        nosNaArvore[v] = true;

        // Se a ponte nos levou a um grupo novo, contabilizamos a conquista!
        if (!gruposVisitados[grupoDoDestino])
        {
            gruposVisitados[grupoDoDestino] = true;
            gruposConectados++;
        }
    }

    return solucao;
}

// Algoritmo Guloso
Solucao algoritmoGuloso(const Grafo &grafo)
{
    Solucao melhorSol;
    melhorSol.custoTotal = std::numeric_limits<double>::max();
    
    // Tenta até metade dos nós (ou 20 tentativas) como pontos iniciais
    // para encontrar uma solução válida que conecte todos os grupos
    int maxTentativas = std::min(20, (grafo.getNumNos() + 1) / 2);
    
    for (int tentativa = 0; tentativa < maxTentativas; ++tentativa)
    {
        // Cada tentativa usa um nó inicial diferente (determinístico via semente)
        Solucao sol = construirSolucaoGRASP(grafo, 0.0);
        
        if (sol.custoTotal < melhorSol.custoTotal)
        {
            melhorSol = sol;
        }
        
        // Se encontrou uma solução válida (não infinito), pode parar
        if (melhorSol.custoTotal < std::numeric_limits<double>::max())
        {
            break;
        }
    }
    
    std::cout << "[Guloso] Concluido. Custo Encontrado: " << melhorSol.custoTotal << "\n";
    return melhorSol;
}

// Algoritmo Guloso Randomizado
Solucao algoritmoGulosoRandomizado(const Grafo &grafo, double alpha, int numIteracoes)
{
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
Solucao algoritmoGulosoRandomizadoReativo(const Grafo &grafo, int numIteracoes, int tamanhoBloco)
{
    Solucao melhorSolucaoGlob;
    melhorSolucaoGlob.custoTotal = std::numeric_limits<double>::max();

    std::vector<double> alphas = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    int numAlphas = alphas.size();

    std::vector<double> probabilidades(numAlphas, 1.0 / numAlphas);
    std::vector<double> somaCustos(numAlphas, 0.0);
    std::vector<int> qtdUsos(numAlphas, 0);

    for (int iter = 0; iter < numIteracoes; ++iter)
    {
        if (iter > 0 && iter % tamanhoBloco == 0)
        {
            double somaQ = 0.0;
            std::vector<double> q(numAlphas, 0.0);

            for (int i = 0; i < numAlphas; ++i)
            {
                if (qtdUsos[i] == 0 || melhorSolucaoGlob.custoTotal >= std::numeric_limits<double>::max())
                {
                    q[i] = 1.0;
                }
                else
                {
                    double mediaAlpha = somaCustos[i] / qtdUsos[i];
                    if (mediaAlpha > 0) {
                        q[i] = melhorSolucaoGlob.custoTotal / mediaAlpha;
                    } else {
                        q[i] = 1.0;
                    }
                }
                somaQ += q[i];
            }

            if (somaQ > 0)
            {
                for (int i = 0; i < numAlphas; ++i)
                {
                    probabilidades[i] = q[i] / somaQ;
                }
            }
        }

        double r = (double)rand() / RAND_MAX;
        double acumulado = 0.0;
        int alphaSorteado = numAlphas - 1;

        for (int i = 0; i < numAlphas; ++i)
        {
            acumulado += probabilidades[i];
            if (r <= acumulado)
            {
                alphaSorteado = i;
                break;
            }
        }

        Solucao solAtual = construirSolucaoGRASP(grafo, alphas[alphaSorteado]);

        if (solAtual.custoTotal < std::numeric_limits<double>::max())
        {
            somaCustos[alphaSorteado] += solAtual.custoTotal;
            qtdUsos[alphaSorteado]++;

            if (solAtual.custoTotal < melhorSolucaoGlob.custoTotal)
            {
                melhorSolucaoGlob = solAtual;
            }
        }
    }

    std::cout << "[Guloso Randomizado Reativo] Concluido. Custo: " << melhorSolucaoGlob.custoTotal << "\n";
    return melhorSolucaoGlob;
}