#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iomanip>
#include <cmath>

// Estrutura unificada para conter todos os campos necessários
struct Registro {
    int age;
    std::string workclass;
    std::string education;
    int hours;
    std::string income;
};

// ===================================================================
// FUNÇÃO 1: LÓGICA DE 'analise_estatistica.cpp'
// ===================================================================
void executar_analise_estatistica(const std::vector<Registro>& registros) {
    std::ofstream saida("analise/estatisticas.txt");
    if (!saida.is_open()) {
        std::cerr << "Erro ao salvar em analise/estatisticas.txt" << std::endl;
        return;
    }

    std::map<std::string, std::vector<int>> idade_por_edu;
    for (const auto& r : registros)
        idade_por_edu[r.education].push_back(r.age);

    saida << std::fixed << std::setprecision(2);
    saida << "Média de idade por nível de educação:\n";
    for (const auto& par : idade_por_edu) {
        double soma = 0;
        for (int v : par.second) soma += v;
        saida << par.first << ": " << (soma / par.second.size()) << " anos\n";
    }

    std::map<std::string, int> freq_workclass;
    for (const auto& r : registros)
        freq_workclass[r.workclass]++;

    saida << "\nFrequência de cada workclass:\n";
    for (const auto& par : freq_workclass) {
        saida << par.first << ": " << par.second << " registros\n";
    }

    saida << "\nDesvio padrão de horas semanais por education:\n";
    for (const auto& par : idade_por_edu) {
        std::vector<int> horas;
        for (const auto& r : registros)
            if (r.education == par.first)
                horas.push_back(r.hours);
        double media = 0;
        for (int h : horas) media += h;
        media /= horas.size();
        double soma_quadrados = 0;
        for (int h : horas)
            soma_quadrados += pow(h - media, 2);
        saida << par.first << ": " << sqrt(soma_quadrados / horas.size()) << " h/semana\n";
    }

    saida.close();
    std::cout << "✅ Análise estatística salva em 'analise/estatisticas.txt'\n";
}


// ===================================================================
// FUNÇÃO 2: LÓGICA DE 'classificacao_agrupamento.cpp'
// ===================================================================
void executar_classificacao_agrupamento(const std::vector<Registro>& registros) {
    std::ofstream saida("analise/agrupamento.txt");
    if (!saida.is_open()) {
        std::cerr << "Erro ao criar analise/agrupamento.txt" << std::endl;
        return;
    }
    
    std::map<std::string, std::map<std::string, int>> agrupamento;
    for (const auto& r : registros)
        agrupamento[r.workclass][r.education]++;

    saida << "Distribuição de education por workclass:\n";
    for (const auto& par : agrupamento) {
        saida << "\n" << par.first << ":\n";
        for (const auto& edu : par.second) {
            saida << "  " << edu.first << ": " << edu.second << " registros\n";
        }
    }

    std::map<std::string, std::set<std::string>> income_edu;
    for (const auto& r : registros)
        income_edu[r.income].insert(r.education);

    saida << "\nClassificação de education por income:\n";
    for (const auto& inc : income_edu) {
        saida << inc.first << ": ";
        for (const auto& e : inc.second) {
            saida << e << ", ";
        }
        saida << "\n";
    }

    saida.close();
    std::cout << "✅ Agrupamento/Classificação salvos em 'analise/agrupamento.txt'\n";
}


// ===================================================================
// FUNÇÃO 3: LÓGICA DE 'filtragem_ordenacao.cpp'
// ===================================================================
void executar_filtragem_ordenacao(const std::vector<Registro>& registros) {
    std::vector<Registro> filtrado;
    for (const auto& r : registros) {
        if (r.hours > 40) filtrado.push_back(r);
    }

    std::sort(filtrado.begin(), filtrado.end(), [](const Registro& a, const Registro& b) {
        return a.workclass < b.workclass;
    });

    std::ofstream saida("analise/filtragem_ordenacao.txt");
    if (!saida.is_open()) {
        std::cerr << "Erro ao criar analise/filtragem_ordenacao.txt" << std::endl;
        return;
    }

    saida << "Registros com >40h semanais ordenados por workclass:\n\n";
    for (const auto& r : filtrado) {
        saida << std::left << std::setw(15) << r.workclass << " | "
              << std::setw(15) << r.education << " | "
              << r.hours << "h/semana\n";
    }

    saida.close();
    std::cout << "✅ Filtragem/Ordenação salva em 'analise/filtragem_ordenacao.txt'\n";
}


// ===================================================================
// FUNÇÃO 4: LÓGICA DE 'simulacao_tendencias.cpp'
// ===================================================================
void executar_simulacao_tendencias(const std::vector<Registro>& dados_originais) {
    std::vector<Registro> novos_dados;
    for (auto r : dados_originais) { // Pass by value to modify a copy
        if (r.age < 60) {
            r.age += 1;
            if (r.hours > 30) r.hours += 1;
            novos_dados.push_back(r);
        }
    }

    std::map<std::string, std::vector<int>> horas_por_edu;
    for (const auto& r : novos_dados)
        horas_por_edu[r.education].push_back(r.hours);

    std::ofstream saida("analise/tendencias.txt");
    if (!saida.is_open()) {
        std::cerr << "Erro ao criar analise/tendencias.txt" << std::endl;
        return;
    }

    saida << std::fixed << std::setprecision(2);
    saida << "Previsão de média de horas por educação após 1 ano (simulado):\n";
    for (const auto& par : horas_por_edu) {
        double soma = 0;
        for (int h : par.second) soma += h;
        saida << par.first << ": " << (soma / par.second.size()) << " h/semana\n";
    }

    saida << "\nTotal de novos registros simulados: " << novos_dados.size() << std::endl;
    saida.close();
    std::cout << "✅ Simulação de tendências salva em 'analise/tendencias.txt'\n";
}


// ===================================================================
// FUNÇÃO PRINCIPAL
// ===================================================================
int main() {
    std::ifstream file("Data/adult.data");
    if (!file.is_open()) {
        std::cerr << "Erro fatal: não foi possível abrir Data/adult.data" << std::endl;
        return 1;
    }

    std::vector<Registro> registros;
    std::string linha;
    // Leitura única do arquivo de dados
    while (getline(file, linha)) {
        std::stringstream ss(linha);
        std::string campo;
        std::vector<std::string> campos;
        while (getline(ss, campo, ',')) campos.push_back(campo);

        if (campos.size() >= 15) {
            try {
                int age = std::stoi(campos[0]);
                std::string work = campos[1];
                std::string edu = campos[3];
                int hrs = std::stoi(campos[12]);
                std::string inc = campos[14];
                
                work.erase(std::remove(work.begin(), work.end(), ' '), work.end());
                edu.erase(std::remove(edu.begin(), edu.end(), ' '), edu.end());
                inc.erase(std::remove(inc.begin(), inc.end(), ' '), inc.end());
                
                registros.push_back({age, work, edu, hrs, inc});
            } catch (const std::invalid_argument& e) {
                // Ignora linhas com dados numéricos inválidos
            }
        }
    }
    file.close();
    
    std::cout << "Dados lidos. Iniciando análises C++...\n";
    
    // Executa todas as análises em sequência
    executar_analise_estatistica(registros);
    executar_classificacao_agrupamento(registros);
    executar_filtragem_ordenacao(registros);
    executar_simulacao_tendencias(registros);

    std::cout << "\nTodas as análises C++ foram concluídas!\n";
    return 0;
}