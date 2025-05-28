#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ifstream file("Data/adult.data");
    if (!file.is_open()) {
        cerr << "Erro ao abrir Data/adult.data" << endl;
        return 1;
    }

    string linha;
    int contador = 0;

    while (getline(file, linha) && contador < 5) {
        stringstream ss(linha);
        string campo;
        vector<string> campos;

        while (getline(ss, campo, ',')) {
            campos.push_back(campo);
        }

        cout << "Linha " << contador + 1 << ": ";
        for (auto& c : campos) {
            c.erase(remove(c.begin(), c.end(), ' '), c.end());
            cout << "[" << c << "] ";
        }
        cout << endl;
        contador++;
    }

    file.close();
    return 0;
}
