#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Registro {
    int age;
    string education;
    string workclass;
    int hours;
};

int main() {
    ifstream file("Data/adult.data");
    if (!file.is_open()) {
        cerr << "Erro ao abrir Data/adult.data" << endl;
        return 1;
    }

    vector<Registro> dados_originais;
    string linha;
    while (getline(file, linha)) {
        stringstream ss(linha);
        string campo;
        vector<string> campos;
        while (getline(ss, campo, ',')) campos.push_back(campo);

        if (campos.size() >= 15) {
            int age = stoi(campos[0]);
            string work = campos[1], edu = campos[3];
            int hrs = stoi(campos[12]);
            work.erase(remove(work.begin(), work.end(), ' '), work.end());
            edu.erase(remove(edu.begin(), edu.end(), ' '), edu.end());
            dados_originais.push_back({age, edu, work, hrs});
        }
    }

    vector<Registro> novos_dados;
    for (auto& r : dados_originais) {
        if (r.age < 60) {
            novos_dados.push_back({r.age + 1, r.education, r.workclass, r.hours + (r.hours > 30 ? 1 : 0)});
        }
    }

    map<string, vector<int>> horas_por_edu;
    for (const auto& r : novos_dados)
        horas_por_edu[r.education].push_back(r.hours);

    ofstream saida("analise/tendencias.txt");
    if (!saida.is_open()) {
        cerr << "Erro ao criar arquivo de saída em analise/tendencias.txt" << endl;
        return 1;
    }

    cout << fixed << setprecision(2);
    saida << fixed << setprecision(2);
    cout << "Previsão de média de horas por educação após 1 ano (simulado):\n";
    saida << "Previsão de média de horas por educação após 1 ano (simulado):\n";

    for (const auto& par : horas_por_edu) {
        double soma = 0;
        for (int h : par.second) soma += h;
        double media = soma / par.second.size();
        cout << par.first << ": " << media << " h/semana\n";
        saida << par.first << ": " << media << " h/semana\n";
    }

    cout << "\nTotal de novos registros simulados: " << novos_dados.size() << endl;
    saida << "\nTotal de novos registros simulados: " << novos_dados.size() << endl;

    saida.close();
    return 0;
}
