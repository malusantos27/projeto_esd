#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cmath>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Registro {
    string education;
    string workclass;
    int age;
    int hours;
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
            string edu = campos[3], work = campos[1];
            int age = stoi(campos[0]);
            int hrs = stoi(campos[12]);
            edu.erase(remove(edu.begin(), edu.end(), ' '), edu.end());
            work.erase(remove(work.begin(), work.end(), ' '), work.end());
            registros.push_back({edu, work, age, hrs});
        }
    }

    ofstream saida("analise/estatisticas.txt");
    if (!saida.is_open()) {
        cerr << "Erro ao salvar arquivo de saída em analise/estatisticas.txt" << endl;
        return 1;
    }

    map<string, vector<int>> idade_por_edu;
    for (const auto& r : registros)
        idade_por_edu[r.education].push_back(r.age);

    cout << fixed << setprecision(2);
    saida << fixed << setprecision(2);

    cout << "Média de idade por nível de educação:\n";
    saida << "Média de idade por nível de educação:\n";
    for (const auto& par : idade_por_edu) {
        double soma = 0;
        for (int v : par.second) soma += v;
        double media = soma / par.second.size();
        cout << par.first << ": " << media << " anos\n";
        saida << par.first << ": " << media << " anos\n";
    }

    map<string, int> freq_workclass;
    for (const auto& r : registros)
        freq_workclass[r.workclass]++;

    cout << "\nFrequência de cada workclass:\n";
    saida << "\nFrequência de cada workclass:\n";
    for (const auto& par : freq_workclass) {
        cout << par.first << ": " << par.second << " registros\n";
        saida << par.first << ": " << par.second << " registros\n";
    }

    cout << "\nDesvio padrão de horas semanais por education:\n";
    saida << "\nDesvio padrão de horas semanais por education:\n";
    for (const auto& par : idade_por_edu) {
        vector<int> horas;
        for (const auto& r : registros)
            if (r.education == par.first)
                horas.push_back(r.hours);
        double media = 0;
        for (int h : horas) media += h;
        media /= horas.size();
        double soma_quadrados = 0;
        for (int h : horas)
            soma_quadrados += pow(h - media, 2);
        double desvio = sqrt(soma_quadrados / horas.size());
        cout << par.first << ": " << desvio << " h/semana\n";
        saida << par.first << ": " << desvio << " h/semana\n";
    }

    saida.close();
    return 0;
}
