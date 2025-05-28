#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>
using namespace std;

// Estrutura para armazenar os dados relevantes
struct Registro {
    int age;
    string education;
    string workclass;
    string income;
};

int main() {
    ifstream file("Data/adult.data");
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return 1;
    }

    vector<Registro> registros;
    string linha;

    while (getline(file, linha)) {
        stringstream ss(linha);
        string campo;
        vector<string> campos;
        while (getline(ss, campo, ',')) {
            campos.push_back(campo);
        }

        if (campos.size() < 15) continue;

        Registro r;
        r.age = stoi(campos[0]);
        r.workclass = campos[1];
        r.education = campos[3];
        r.income = campos[14];

        // Remover espaços
        for (string* campo : {&r.education, &r.workclass, &r.income}) {
            campo->erase(remove(campo->begin(), campo->end(), ' '), campo->end());
        }

        registros.push_back(r);
    }

    file.close();

    // ===== 1️⃣ MÉDIA DE IDADE POR EDUCATION =====
    map<string, pair<int, int>> idade_por_edu; // {soma_idade, quantidade}

    for (const auto& r : registros) {
        idade_por_edu[r.education].first += r.age;
        idade_por_edu[r.education].second++;
    }

    cout << fixed << setprecision(2);
    cout << "1️⃣ Média de idade por education:\n";
    for (const auto& par : idade_por_edu) {
        double media = (double)par.second.first / par.second.second;
        cout << "  " << par.first << ": " << media << " anos\n";
    }

    // ===== 2️⃣ FILTRO: income >50K com Bachelors =====
    cout << "\n2️⃣ Pessoas com income >50K e education == Bachelors:\n";
    for (const auto& r : registros) {
        if (r.education == "Bachelors" && r.income == ">50K") {
            cout << "  Idade: " << r.age << ", Workclass: " << r.workclass << endl;
        }
    }

    // ===== 3️⃣ TOP 3 WORKCLASS por EDUCATION =====
    cout << "\n3️⃣ Top 3 workclass por education:\n";
    map<string, map<string, int>> contador;

    for (const auto& r : registros) {
        contador[r.education][r.workclass]++;
    }

    for (const auto& par : contador) {
        string edu = par.first;
        const map<string, int>& workmap = par.second;

        // converter para vetor e ordenar
        vector<pair<string, int>> vec(workmap.begin(), workmap.end());
        sort(vec.begin(), vec.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
            return b.second < a.second; // decrescente
        });

        cout << "  " << edu << ":\n";
        for (int i = 0; i < min(3, (int)vec.size()); ++i) {
            cout << "    " << vec[i].first << ": " << vec[i].second << endl;
        }
    }

    return 0;
}
