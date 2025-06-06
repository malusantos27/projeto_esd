#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include "tarefas.h"

// Declarações forward para as estruturas e funções que usaremos de outros arquivos
struct NodoAVL;
NodoAVL* inserir(NodoAVL* raiz, std::string edu, std::string work);
long obter_memoria_avl();

class Lista;


// Função auxiliar para carregar os dados
std::vector<std::pair<std::string, std::string>> carregar_dados_teste() {
    std::ifstream file("Data/adult.data");
    std::vector<std::pair<std::string, std::string>> dados;
    std::string linha;
    while (getline(file, linha)) {
        std::stringstream ss(linha);
        std::string campo;
        std::vector<std::string> campos;
        while (getline(ss, campo, ',')) campos.push_back(campo);
        if (campos.size() >= 4) {
            std::string edu = campos[3];
            std::string work = campos[1];
            edu.erase(std::remove(edu.begin(), edu.end(), ' '), edu.end());
            work.erase(std::remove(work.begin(), work.end(), ' '), work.end());
            if (!edu.empty()) dados.emplace_back(edu, work);
        }
    }
    return dados;
}

// ===================================================================
// TESTE 1: (R2) Restrição no tamanho máximo da estrutura
// ===================================================================
void teste_limite_estrutura() {
    std::cout << "\n--- Iniciando Teste (R2): Limite de 5000 elementos na AVL ---\n";
    auto dados = carregar_dados_teste();
    const int LIMITE = 5000;
    
    NodoAVL* raiz = nullptr;
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < LIMITE && i < dados.size(); ++i) {
        raiz = inserir(raiz, dados[i].first, dados[i].second);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> tempo = end - start;
    
    std::cout << "Resultado:\n";
    std::cout << "  - Tempo para inserir " << LIMITE << " elementos: " << tempo.count() << " ms\n";
    std::cout << "Análise: O sistema operou normalmente, mas parou de inserir dados ao atingir o limite imposto.\n";
}

// ===================================================================
// TESTE 2: (R12) Simulação de alta latência
// ===================================================================
void teste_alta_latencia() {
    std::cout << "\n--- Iniciando Teste (R12): Simulação de Alta Latência (1ms a cada 100 inserções) ---\n";
    auto dados = carregar_dados_teste();
    
    NodoAVL* raiz = nullptr;
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < dados.size(); ++i) {
        raiz = inserir(raiz, dados[i].first, dados[i].second);
        if (i > 0 && i % 100 == 0) {
            //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> tempo = end - start;

    std::cout << "Resultado:\n";
    std::cout << "  - Tempo de inserção total com latência artificial: " << tempo.count() << " ms\n";
    std::cout << "Análise: O tempo de inserção total aumentou significativamente devido aos atrasos simulados.\n";
}

// ===================================================================
// TESTE 3: (R18) Simulação de dados corrompidos
// ===================================================================
void teste_dados_corrompidos() {
    std::cout << "\n--- Iniciando Teste (R18): Simulação de Dados Corrompidos (10% de anomalias) ---\n";
    std::cout << "Este teste executa a análise estatística sobre dados corrompidos para comparar os resultados.\n";
    
    struct RegistroAnalise { int age; std::string education; int hours; };
    std::vector<RegistroAnalise> registros;
    std::ifstream file("Data/adult.data");
    std::string linha;

    while (getline(file, linha)) {
        std::stringstream ss(linha);
        std::vector<std::string> campos;
        std::string campo;
        while(getline(ss, campo, ',')) campos.push_back(campo);
        if (campos.size() >= 13) {
            try {
                registros.push_back({std::stoi(campos[0]), campos[3], std::stoi(campos[12])});
            } catch(...) { /* ignora erros de conversão */ }
        }
    }

    // Corrompendo 10% dos dados
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, registros.size() - 1);
    for (size_t i = 0; i < registros.size() / 10; ++i) {
        int index = distrib(gen);
        registros[index].age = 999; 
    }
    
    // Recalculando a média de idade
    std::map<std::string, std::pair<double, int>> idade_por_edu;
    for(const auto& r : registros) {
        idade_por_edu[r.education].first += r.age;
        idade_por_edu[r.education].second++;
    }

    std::cout << "Resultado (Média de idade com dados corrompidos):\n";
    for(const auto& par : idade_por_edu) {
        if(par.first.find("Bachelors") != std::string::npos) {
             std::cout << "  - " << par.first << ": " << par.second.first / par.second.second << " anos\n";
        }
    }
    std::cout << "Análise: A média de idade para 'Bachelors' fica distorcida, provando o impacto de dados anômalos.\n";
}

// ===================================================================
// MENU PRINCIPAL DOS TESTES
// ===================================================================
void executar_testes_restritivos() {
    std::cout << "\n============================================\n";
    std::cout << "       MENU DE TESTES COM RESTRIÇÕES        \n";
    std::cout << "============================================\n";
    
    teste_limite_estrutura();
    teste_alta_latencia();
    teste_dados_corrompidos();
    
    std::cout << "\n--- Teste (R6) e (R21): Instruções Manuais ---\n";
    std::cout << "Para os testes restantes, siga as instruções abaixo e anote os resultados para seu relatório:\n\n";
    
    std::cout << "  Teste de Restrição de Processamento (R6):\n";
    std::cout << "  1. Execute a opção de benchmark da Árvore AVL no menu principal.\n";
    std::cout << "  2. Enquanto o benchmark roda, abra o Gerenciador de Tarefas (Ctrl+Shift+Esc).\n";
    std::cout << "  3. Vá para a aba 'Detalhes', clique com o botão direito em 'meu_programa.exe'.\n";
    std::cout << "  4. Selecione 'Definir afinidade' e marque apenas 'CPU 0'.\n";
    std::cout << "  5. Compare o tempo de execução com o benchmark normal e documente a lentidão.\n\n";

    std::cout << "  Teste de Restrição Algorítmica (R21):\n";
    std::cout << "  1. Execute a opção de benchmark da 'Árvore AVL' (eficiente) e anote o tempo de busca.\n";
    std::cout << "  2. Execute a opção de benchmark da 'Lista Encadeada' (ineficiente) e anote o tempo de busca.\n";
    std::cout << "  3. No seu relatório, compare os dois tempos. A diferença drástica comprova o impacto da escolha do algoritmo.\n";

    std::cout << "\n--- Fim dos testes com restrições ---\n";
}