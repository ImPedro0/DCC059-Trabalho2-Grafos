#include <iostream>
#include <chrono>
#include <random>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include "Grafo.hpp"
#include "Algoritmos.hpp"

struct ResultadoCompleto {
    std::string nomeAlgoritmo;
    double custoTotal;
    double tempoMs;
    unsigned int seed;
    double alphaUsado;      
    int numIteracoes;       
    int tamanhoBloco;       
    double gap;
};

void exportarCSV(const std::string& nomeInstancia, 
                 const std::vector<ResultadoCompleto>& resultados,
                 double otimo) {
    
    std::string nomeArquivo = "resultados.csv";
    bool arquivoExiste = std::ifstream(nomeArquivo).good();
    
    std::ofstream arquivo(nomeArquivo, std::ios::app);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo CSV para escrita!" << std::endl;
        return;
    }
    
    if (!arquivoExiste) {
        // Cabeçalho corrigido e padronizado
        arquivo << "DataHora,Instancia,Algoritmo,Alpha,Iteracoes,TamanhoBloco,Seed,Custo,Tempo(ms),Gap(%)\n";
    }
    
    auto agora = std::chrono::system_clock::now();
    auto tempo = std::chrono::system_clock::to_time_t(agora);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&tempo), "%Y-%m-%d %H:%M:%S");
    std::string dataHora = oss.str();
    
    for (const auto& res : resultados) {
        arquivo << dataHora << ","
                << nomeInstancia << ","
                << res.nomeAlgoritmo << ","
                << std::fixed << std::setprecision(2) << res.alphaUsado << ","
                << res.numIteracoes << ","
                << res.tamanhoBloco << ","
                << res.seed << ","
                << std::fixed << std::setprecision(2) << res.custoTotal << ","
                << res.tempoMs << ","
                << res.gap << "\n";
    }
    
    arquivo.close();
    std::cout << "\n Resultados exportados para " << nomeArquivo << "\n";
}

double calcularGap(double custo, double otimo) {
    if (otimo <= 0) return 0.0;
    return ((custo - otimo) / otimo) * 100.0;
}

std::string extrairNomeArquivo(const std::string& caminho) {
    size_t pos = caminho.find_last_of("/\\");
    return (pos == std::string::npos) ? caminho : caminho.substr(pos + 1);
}

int main(int argc, char* argv[]) {
    std::string caminhoArquivo = "instancia.txt";
    if(argc > 1) caminhoArquivo = argv[1];
    
    double otimoCOnhecido = 1e9; // Inicializa com um valor muito alto (infinito)
    if(argc > 2) otimoCOnhecido = std::stod(argv[2]);

    std::string arquivoSolucao = caminhoArquivo.substr(0, caminhoArquivo.find_last_of('.')) + "_solucao.txt";
    
    // 1. LER OTIMO SALVO ANTERIORMENTE NO ARQUIVO, SE EXISTIR
    std::ifstream inSolInicial(arquivoSolucao);
    if(inSolInicial.is_open()) {
        std::string linha;
        if(std::getline(inSolInicial, linha)) {
            if(linha.find("Custo:") != std::string::npos) {
                try {
                    double custoAntigo = std::stod(linha.substr(linha.find(":") + 1));
                    if(custoAntigo < otimoCOnhecido) {
                        otimoCOnhecido = custoAntigo;
                    }
                } catch(...) {}
            }
        }
        inSolInicial.close();
    }

    unsigned int sementeBase;
    if (argc > 3) {
        sementeBase = std::stoul(argv[3]);
    } else {
        sementeBase = std::chrono::system_clock::now().time_since_epoch().count();
    }

    std::mt19937 geradorSementes(sementeBase);
    
    Grafo* grafo = Grafo::carregarDeArquivo(caminhoArquivo);
    if (!grafo) {
        return 1;
    }

    std::string nomeInstancia = extrairNomeArquivo(caminhoArquivo);
    std::cout << "=== TESTE DO PROGRAMA AGMG ===\n";
    std::cout << "Instancia: " << nomeInstancia << "\n";
    std::cout << "Grafo processado! Nos: " << grafo->getNumNos() 
              << ", Grupos: " << grafo->getNumGrupos() << "\n";
    std::cout << "Otimo Conhecido Inicial (Parametro ou TXT): " << otimoCOnhecido << "\n";
    std::cout << "Semente Base: " << sementeBase << "\n\n";
    
    // Tres valores de alpha para GRASP
    std::vector<double> alphasGRASP = {0.1, 0.3, 0.5};
    int iteracoesGRASP = 100;  
    int iteracoesReativo = 300;
    int tamanhoBloco = 50;

    std::vector<ResultadoCompleto> todosOsResultados;
    Solucao melhorSolucaoGlobal;
    melhorSolucaoGlobal.custoTotal = 1e9; // Inicializa com infinito
    
    
    int NUM_REPETICOES = 10;
    
    for (int rep = 1; rep <= NUM_REPETICOES; ++rep) {
        std::cout << "\n=== REPETICAO " << rep << "/" << NUM_REPETICOES << " ===\n";
        
        unsigned int sementeDaIteracao = geradorSementes();
        std::cout << "Semente da Iteracao: " << sementeDaIteracao << "\n";
        
        // --- Algoritmo Guloso ---
        srand(sementeDaIteracao);
        auto iniciGuloso = std::chrono::high_resolution_clock::now();
        Solucao solGuloso = algoritmoGuloso(*grafo);
        auto fimGuloso = std::chrono::high_resolution_clock::now();
        double tempoGuloso = std::chrono::duration<double, std::milli>(fimGuloso - iniciGuloso).count();
        
        if (solGuloso.custoTotal < melhorSolucaoGlobal.custoTotal) melhorSolucaoGlobal = solGuloso;
        if (solGuloso.custoTotal < otimoCOnhecido) otimoCOnhecido = solGuloso.custoTotal;
        
        ResultadoCompleto resGuloso;
        resGuloso.nomeAlgoritmo = "Guloso";
        resGuloso.custoTotal = solGuloso.custoTotal;
        resGuloso.tempoMs = tempoGuloso;
        resGuloso.seed = sementeDaIteracao;
        resGuloso.alphaUsado = 0.0;
        resGuloso.numIteracoes = 1;
        resGuloso.tamanhoBloco = 0;
        resGuloso.gap = 0.0; // Sera recalculado no final
        todosOsResultados.push_back(resGuloso);
        
        std::cout << "  [Guloso] Custo: " << solGuloso.custoTotal 
                  << ", Tempo: " << tempoGuloso << "ms, GAP Momentaneo: " << calcularGap(solGuloso.custoTotal, otimoCOnhecido) << "%\n";
        
        // --- Algoritmo GRASP ---
        srand(sementeDaIteracao);
        int indiceSorteado = rand() % alphasGRASP.size();
        double alpha = alphasGRASP[indiceSorteado];
        
        auto iniciGRASP = std::chrono::high_resolution_clock::now();
        Solucao solGRASP = algoritmoGulosoRandomizado(*grafo, alpha, iteracoesGRASP);
        auto fimGRASP = std::chrono::high_resolution_clock::now();
        double tempoGRASP = std::chrono::duration<double, std::milli>(fimGRASP - iniciGRASP).count();
        
        if (solGRASP.custoTotal < melhorSolucaoGlobal.custoTotal) melhorSolucaoGlobal = solGRASP;
        if (solGRASP.custoTotal < otimoCOnhecido) otimoCOnhecido = solGRASP.custoTotal;
        
        ResultadoCompleto resGRASP;
        resGRASP.nomeAlgoritmo = "GRASP";
        resGRASP.custoTotal = solGRASP.custoTotal;
        resGRASP.tempoMs = tempoGRASP;
        resGRASP.seed = sementeDaIteracao;
        resGRASP.alphaUsado = alpha;
        resGRASP.numIteracoes = iteracoesGRASP;
        resGRASP.tamanhoBloco = 0;
        resGRASP.gap = 0.0; // Sera recalculado no final
        todosOsResultados.push_back(resGRASP);
        
        std::cout << "  [GRASP alpha=" << alpha << "] Custo: " << solGRASP.custoTotal 
                  << ", Tempo: " << tempoGRASP << "ms, GAP Momentaneo: " << calcularGap(solGRASP.custoTotal, otimoCOnhecido) << "%\n";
        
        // --- ALGORITMO REATIVO ---
        srand(sementeDaIteracao);
        
        auto iniciReativo = std::chrono::high_resolution_clock::now();
        Solucao solReativo = algoritmoGulosoRandomizadoReativo(*grafo, iteracoesReativo, tamanhoBloco);
        auto fimReativo = std::chrono::high_resolution_clock::now();
        double tempoReativo = std::chrono::duration<double, std::milli>(fimReativo - iniciReativo).count();
        
        if (solReativo.custoTotal < melhorSolucaoGlobal.custoTotal) melhorSolucaoGlobal = solReativo;
        if (solReativo.custoTotal < otimoCOnhecido) otimoCOnhecido = solReativo.custoTotal;
        
        ResultadoCompleto resReativo;
        resReativo.nomeAlgoritmo = "Reativo";
        resReativo.custoTotal = solReativo.custoTotal;
        resReativo.tempoMs = tempoReativo;
        resReativo.seed = sementeDaIteracao;
        resReativo.alphaUsado = 0.0; // Reativo usa pool adaptativo (veja tamanhoBloco)
        resReativo.numIteracoes = iteracoesReativo;
        resReativo.tamanhoBloco = tamanhoBloco;
        resReativo.gap = 0.0; // Sera recalculado no final
        todosOsResultados.push_back(resReativo);
        
        std::cout << "  [Reativo] Custo: " << solReativo.custoTotal 
                  << ", Tempo: " << tempoReativo << "ms, GAP Momentaneo: " << calcularGap(solReativo.custoTotal, otimoCOnhecido) << "%\n";
    }
    
    // 2. RECALCULAR O GAP DE TODOS OS RESULTADOS USANDO O MELHOR OTIMO GLOBAL DEFINITIVO
    for (auto& res : todosOsResultados) {
        res.gap = calcularGap(res.custoTotal, otimoCOnhecido);
    }
    
    std::cout << "\n\n=== ESTATISTICAS FINAIS (Usando Otimo = " << otimoCOnhecido << ") ===\n";
    
    struct EstatisticasAlgoritmo {
        std::string nome;
        double custoMedio = 0.0;
        double custoMelhor = 1e9;
        double tempoMedio = 0.0;
        double gapMedio = 0.0;
        double gapMelhor = 0.0;
        int count = 0;
    };
    
    std::vector<EstatisticasAlgoritmo> stats;
    std::vector<std::string> nomes = {"Guloso", "GRASP", "Reativo"};
    
    for (const auto& nome : nomes) {
        EstatisticasAlgoritmo stat;
        stat.nome = nome;
        
        for (const auto& res : todosOsResultados) {
            if (res.nomeAlgoritmo == nome) {
                stat.custoMedio += res.custoTotal;
                stat.custoMelhor = std::min(stat.custoMelhor, res.custoTotal);
                stat.tempoMedio += res.tempoMs;
                stat.gapMedio += res.gap;
                stat.count++;
            }
        }
        
        if (stat.count > 0) {
            stat.custoMedio /= stat.count;
            stat.tempoMedio /= stat.count;
            stat.gapMedio /= stat.count;
            stat.gapMelhor = calcularGap(stat.custoMelhor, otimoCOnhecido);
            stats.push_back(stat);
        }
    }
    
    std::cout << "\n"
              << std::left << std::setw(10) << "Algoritmo" << "| "
              << std::right << std::setw(12) << "Melhor Custo" << " | "
              << std::setw(11) << "Custo Medio" << " | "
              << std::setw(16) << "Tempo Medio (ms)" << " | "
              << std::setw(14) << "GAP Melhor (%)" << " | "
              << std::setw(13) << "GAP Medio (%)" << "\n";
    std::cout << "----------+--------------+-------------+------------------+----------------+--------------\n";
    
    for (const auto& stat : stats) {
        std::cout << std::left << std::setw(10) << stat.nome << "| "
                  << std::right << std::fixed << std::setprecision(2) 
                  << std::setw(12) << stat.custoMelhor << " | "
                  << std::setw(11) << stat.custoMedio << " | "
                  << std::setw(16) << stat.tempoMedio << " | "
                  << std::setw(14) << stat.gapMelhor << " | "
                  << std::setw(13) << stat.gapMedio << "\n";
    }
    
    exportarCSV(nomeInstancia, todosOsResultados, otimoCOnhecido);
    
    // EXPORTAR A MELHOR SOLUÇÃO ENCONTRADA
    bool deveAtualizar = true;
    std::ifstream inSol(arquivoSolucao);
    if(inSol.is_open()) {
        std::string linha;
        if(std::getline(inSol, linha)) {
            if(linha.find("Custo:") != std::string::npos) {
                try {
                    double custoAntigo = std::stod(linha.substr(linha.find(":") + 1));
                    if(melhorSolucaoGlobal.custoTotal >= custoAntigo) {
                        deveAtualizar = false;
                        std::cout << " A solucao atual (" << std::fixed << std::setprecision(2) << melhorSolucaoGlobal.custoTotal 
                                  << ") nao superou a salva anteriormente (" << custoAntigo << "). O arquivo .txt e a imagem foram mantidos.\n";
                    }
                } catch(...) {}
            }
        }
        inSol.close();
    }

    if(deveAtualizar) {
        std::ofstream outSol(arquivoSolucao);
        if(outSol.is_open()) {
            outSol << "Custo: " << std::fixed << std::setprecision(2) << melhorSolucaoGlobal.custoTotal << "\n";
            for(const auto& ar : melhorSolucaoGlobal.arestas) {
                outSol << ar.origem << " " << ar.destino << "\n";
            }
            outSol.close();
            std::cout << " Melhor solucao (Custo: " << melhorSolucaoGlobal.custoTotal << ") exportada para " << arquivoSolucao << "\n";
            
            std::cout << " Gerando grafico da solucao automaticamente...\n";
            std::string comando = "python plotar_solucao.py " + caminhoArquivo;
            system(comando.c_str());
        }
    }
    
    delete grafo;
    
    std::cout << "\n Teste concluido com sucesso!\n";
    return 0;
}