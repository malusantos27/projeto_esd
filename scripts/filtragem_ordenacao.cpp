#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Registro {
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

    vector<Registro> registros;
    string linha;
    while (getline(file, linha)) {
        stringstream ss(linha);
        string campo;
        vector<string> campos;
        while (getline(ss, campo, ',')) campos.push_back(campo);

        if (campos.size() >= 15) {
            string edu = campos[3], work = campos[1];
            int hrs = stoi(campos[12]);
            edu.erase(remove(edu.begin(), edu.end(), ' '), edu.end());
            work.erase(remove(work.begin(), work.end(), ' '), work.end());
            registros.push_back({edu, work, hrs});
        }
    }

    // Filtrar registros com mais de 40h semanais
    vector<Registro> filtrado;
    for (const auto& r : registros) {
        if (r.hours > 40) filtrado.push_back(r);
    }

    // Ordenar alfabeticamente por workclass
    sort(filtrado.begin(), filtrado.end(), [](const Registro& a, const Registro& b) {
        return a.workclass < b.workclass;
    });

    ofstream saida("analise/filtragem_ordenacao.txt");
    if (!saida.is_open()) {
        cerr << "Erro ao criar analise/filtragem_ordenacao.txt" << endl;
        return 1;
    }

    saida << "Registros com >40h semanais ordenados por workclass:\n\n";
    for (const auto& r : filtrado) {
        saida << left << setw(15) << r.workclass << " | "
              << setw(15) << r.education << " | "
              << r.hours << "h/semana\n";
    }

    saida.close();
    cout << "✅ Registros filtrados e ordenados foram salvos em analise/filtragem_ordenacao.txt\n";
    return 0;
}