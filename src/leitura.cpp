#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>

// A função deve se chamar 'executar_leitura', e não 'main'.
void executar_leitura() {
    std::ifstream file("Data/adult.data");
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir Data/adult.data" << std::endl;
        return;
    }

    std::cout << "\n--- Executando Leitura Simples ---\n";
    std::string linha;
    int contador = 0;

    while (getline(file, linha) && contador < 5) {
        std::stringstream ss(linha);
        std::string campo;
        std::vector<std::string> campos;

        while (getline(ss, campo, ',')) {
            campos.push_back(campo);
        }

        std::cout << "Linha " << contador + 1 << ": ";
        for (auto& c : campos) {
            c.erase(std::remove(c.begin(), c.end(), ' '), c.end());
            std::cout << "[" << c << "] ";
        }
        std::cout << std::endl;
        contador++;
    }

    file.close();
    std::cout << "--- Fim da Leitura Simples ---\n";
}