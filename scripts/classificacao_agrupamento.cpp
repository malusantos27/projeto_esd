#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Registro {
    string education;
    string workclass;
    string income;
};

int main() {
    ifstream file("Data/adult.data");
    if (!file.is_open()) {
        cerr << "Erro ao abrir Data/adult.data" << endl;
        return 1;
    }

    vector<Registro> registros;
    string linha;
    while (getline(file, linha)) {
        stringstream ss(linha);
        string campo;
        vector<string> campos;
        while (getline(ss, campo, ',')) campos.push_back(campo);

        if (campos.size() >= 15) {
            string edu = campos[3], work = campos[1], inc = campos[14];
            edu.erase(remove(edu.begin(), edu.end(), ' '), edu.end());
            work.erase(remove(work.begin(), work.end(), ' '), work.end());
            inc.erase(remove(inc.begin(), inc.end(), ' '), inc.end());
            registros.push_back({edu, work, inc});
        }
    }

    // Agrupar por workclass e contar nível de educação
    map<string, map<string, int>> agrupamento;
    for (const auto& r : registros)
        agrupamento[r.workclass][r.education]++;

    ofstream saida("analise/agrupamento.txt");
    if (!saida.is_open()) {
        cerr << "Erro ao criar analise/agrupamento.txt" << endl;
        return 1;
    }

    cout << fixed << setprecision(2);
    saida << fixed << setprecision(2);

    cout << "Distribuição de education por workclass:\n";
    saida << "Distribuição de education por workclass:\n";

    for (const auto& par : agrupamento) {
        cout << "\n" << par.first << ":\n";
        saida << "\n" << par.first << ":\n";
        for (const auto& edu : par.second) {
            cout << "  " << edu.first << ": " << edu.second << " registros\n";
            saida << "  " << edu.first << ": " << edu.second << " registros\n";
        }
    }

    // Agrupar por income e education
    map<string, set<string>> income_edu;
    for (const auto& r : registros)
        income_edu[r.income].insert(r.education);

    cout << "\nClassificação de education por income:\n";
    saida << "\nClassificação de education por income:\n";
    for (const auto& inc : income_edu) {
        cout << inc.first << ": ";
        saida << inc.first << ": ";
        for (const auto& e : inc.second) {
            cout << e << ", ";
            saida << e << ", ";
        }
        cout << "\n";
        saida << "\n";
    }

    saida.close();
    return 0;
}
