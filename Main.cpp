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
    std::string parametros;
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
        arquivo << "DataHora,Instancia,Algoritmo,Parametros,Seed,Custo,Tempo(ms),Gap(%)\n";
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
                << res.parametros << ","
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
    
    double otimoCOnhecido = 5.0;
    if(argc > 2) otimoCOnhecido = std::stod(argv[2]);

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
    std::cout << "Otimo Conhecido: " << otimoCOnhecido << "\n";
    std::cout << "Semente Base: " << sementeBase << "\n\n";
    
    double alpha = 0.3;
    int iteracoesGRASP = 100;  
    int iteracoesReativo = 300;
    int tamanhoBloco = 50;

    std::vector<ResultadoCompleto> todosOsResultados;
    
    int NUM_REPETICOES = 10;
    
    for (int rep = 1; rep <= NUM_REPETICOES; ++rep) {
        std::cout << "\n=== REPETICAO " << rep << "/" << NUM_REPETICOES << " ===\n";
        
        unsigned int sementeDaIteracao = geradorSementes();
        std::cout << "Semente da Iteracao: " << sementeDaIteracao << "\n";
        srand(sementeDaIteracao);
        
        // Algoritmo Guloso
        auto iniciGuloso = std::chrono::high_resolution_clock::now();
        Solucao solGuloso = algoritmoGuloso(*grafo);
        auto fimGuloso = std::chrono::high_resolution_clock::now();
        double tempoGuloso = std::chrono::duration<double, std::milli>(fimGuloso - iniciGuloso).count();
        
        ResultadoCompleto resGuloso;
        resGuloso.nomeAlgoritmo = "Guloso";
        resGuloso.custoTotal = solGuloso.custoTotal;
        resGuloso.tempoMs = tempoGuloso;
        resGuloso.seed = sementeDaIteracao;
        resGuloso.parametros = "N/A";
        resGuloso.gap = calcularGap(solGuloso.custoTotal, otimoCOnhecido);
        todosOsResultados.push_back(resGuloso);
        
        std::cout << "  [Guloso] Custo: " << solGuloso.custoTotal 
                  << ", Tempo: " << tempoGuloso << "ms, GAP: " << resGuloso.gap << "%\n";
        
        srand(sementeDaIteracao);
        
        // Algoritmo Guloso Randomizado
        auto iniciGRASP = std::chrono::high_resolution_clock::now();
        Solucao solGRASP = algoritmoGulosoRandomizado(*grafo, alpha, iteracoesGRASP);
        auto fimGRASP = std::chrono::high_resolution_clock::now();
        double tempoGRASP = std::chrono::duration<double, std::milli>(fimGRASP - iniciGRASP).count();
        
        ResultadoCompleto resGRASP;
        resGRASP.nomeAlgoritmo = "GRASP";
        resGRASP.custoTotal = solGRASP.custoTotal;
        resGRASP.tempoMs = tempoGRASP;
        resGRASP.seed = sementeDaIteracao;
        resGRASP.parametros = "alpha=" + std::to_string(alpha) + ",iter=" + std::to_string(iteracoesGRASP);
        resGRASP.gap = calcularGap(solGRASP.custoTotal, otimoCOnhecido);
        todosOsResultados.push_back(resGRASP);
        
        std::cout << "  [GRASP] Custo: " << solGRASP.custoTotal 
                  << ", Tempo: " << tempoGRASP << "ms, GAP: " << resGRASP.gap << "%\n";
        
        srand(sementeDaIteracao);
        
        // Algoritmo Guloso Randomizado Reativo
        auto iniciReativo = std::chrono::high_resolution_clock::now();
        Solucao solReativo = algoritmoGulosoRandomizadoReativo(*grafo, iteracoesReativo, tamanhoBloco);
        auto fimReativo = std::chrono::high_resolution_clock::now();
        double tempoReativo = std::chrono::duration<double, std::milli>(fimReativo - iniciReativo).count();
        
        ResultadoCompleto resReativo;
        resReativo.nomeAlgoritmo = "Reativo";
        resReativo.custoTotal = solReativo.custoTotal;
        resReativo.tempoMs = tempoReativo;
        resReativo.seed = sementeDaIteracao;
        resReativo.parametros = "iter=" + std::to_string(iteracoesReativo) + ",bloco=" + std::to_string(tamanhoBloco);
        resReativo.gap = calcularGap(solReativo.custoTotal, otimoCOnhecido);
        todosOsResultados.push_back(resReativo);
        
        std::cout << "  [Reativo] Custo: " << solReativo.custoTotal 
                  << ", Tempo: " << tempoReativo << "ms, GAP: " << resReativo.gap << "%\n";
    }
    
    std::cout << "\n\n=== ESTATISTICAS FINAIS ===\n";
    
    struct EstatisticasAlgoritmo {
        std::string nome;
        double custoMedio = 0.0;
        double custoMelhor = 1e9;
        double tempoMedio = 0.0;
        double gapMedio = 0.0;
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
            stats.push_back(stat);
        }
    }
    
    std::cout << "\nAlgoritmo | Melhor Custo | Custo Medio | Tempo Medio (ms) | GAP Medio (%)\n";
    std::cout << "----------|--------------|------------|-----------------|-------------\n";
    
    for (const auto& stat : stats) {
        std::cout << std::left << std::setw(9) << stat.nome << "| "
                  << std::fixed << std::setprecision(2) << std::setw(12) << stat.custoMelhor << "| "
                  << std::setw(10) << stat.custoMedio << "| "
                  << std::setw(15) << stat.tempoMedio << "| "
                  << std::setw(11) << stat.gapMedio << "\n";
    }
    
    exportarCSV(nomeInstancia, todosOsResultados, otimoCOnhecido);
    
    delete grafo;
    
    std::cout << "\n Teste concluido com sucesso!\n";
    return 0;
}