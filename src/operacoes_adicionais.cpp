#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>

struct Registro {
    int age;
    std::string education;
    std::string workclass;
    std::string income;
};

void executar_operacoes_adicionais() {
    std::cout << "\n--- Executando Análises Adicionais ---\n";
    std::ifstream file("Data/adult.data");
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
        return;
    }

    std::vector<Registro> registros;
    std::string linha;

    while (getline(file, linha)) {
        std::stringstream ss(linha);
        std::string campo;
        std::vector<std::string> campos;
        while (getline(ss, campo, ',')) {
            campos.push_back(campo);
        }

        if (campos.size() < 15) continue;

        Registro r;
        r.age = stoi(campos[0]);
        r.workclass = campos[1];
        r.education = campos[3];
        r.income = campos[14];

        for (std::string* campo_ptr : {&r.education, &r.workclass, &r.income}) {
            campo_ptr->erase(std::remove(campo_ptr->begin(), campo_ptr->end(), ' '), campo_ptr->end());
        }

        registros.push_back(r);
    }

    file.close();

    std::map<std::string, std::pair<int, int>> idade_por_edu;
    for (const auto& r : registros) {
        idade_por_edu[r.education].first += r.age;
        idade_por_edu[r.education].second++;
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "1️⃣  Média de idade por education:\n";
    for (const auto& par : idade_por_edu) {
        double media = (double)par.second.first / par.second.second;
        std::cout << "  " << par.first << ": " << media << " anos\n";
    }

    std::cout << "\n2️⃣  Pessoas com income >50K e education == Bachelors:\n";
    for (const auto& r : registros) {
        if (r.education == "Bachelors" && r.income == ">50K") {
            std::cout << "  Idade: " << r.age << ", Workclass: " << r.workclass << std::endl;
        }
    }

    std::cout << "\n3️⃣  Top 3 workclass por education:\n";
    std::map<std::string, std::map<std::string, int>> contador;
    for (const auto& r : registros) {
        contador[r.education][r.workclass]++;
    }

    for (const auto& par : contador) {
        std::string edu = par.first;
        const auto& workmap = par.second;

        std::vector<std::pair<std::string, int>> vec(workmap.begin(), workmap.end());
        sort(vec.begin(), vec.end(), [](const auto& a, const auto& b) {
            return b.second < a.second;
        });

        std::cout << "  " << edu << ":\n";
        for (int i = 0; i < std::min(3, (int)vec.size()); ++i) {
            std::cout << "    " << vec[i].first << ": " << vec[i].second << std::endl;
        }
    }
    std::cout << "--- Fim das Análises Adicionais ---\n";
}