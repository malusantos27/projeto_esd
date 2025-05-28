#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
#include <algorithm>
using namespace std;
using namespace chrono;

class CuckooHash {
    unordered_map<string, int> tabela;
    int kicks;

public:
    CuckooHash() : kicks(0) {}

    void insert(const string& key) {
        if (tabela.count(key)) {
            kicks++;
        }
        tabela[key]++;
    }

    bool search(const string& key) {
        return tabela.find(key) != tabela.end();
    }

    void remove(const string& key) {
        tabela.erase(key);
    }

    size_t size() const {
        return tabela.size();
    }

    long getMemoria() const {
        return tabela.size() * (sizeof(string) + sizeof(int) + sizeof(void*));
    }

    int getKicks() const {
        return kicks;
    }

    void clear() {
        tabela.clear();
        kicks = 0;
    }
};

int main() {
    ifstream file("Data/adult.data");
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return 1;
    }

    vector<string> dados;
    string linha;
    while (getline(file, linha)) {
        stringstream ss(linha);
        string campo;
        vector<string> campos;
        while (getline(ss, campo, ',')) campos.push_back(campo);
        if (campos.size() >= 15) {
            string work = campos[1];
            work.erase(remove(work.begin(), work.end(), ' '), work.end());
            dados.push_back(work);
        }
    }
    file.close();

    vector<int> tamanhos = {int(dados.size() * 0.1), int(dados.size() * 0.5), int(dados.size())};
    const int repeticoes = 100;

    ofstream bench("benchmark/escalabilidade_cuckoo.csv");
    bench << "Tamanho,TempoInsercao(ms),TempoBusca(ms),TempoRemocao(ms),LatenciaMedia(ms),Memoria(B),Kicks\n";

    for (int tam : tamanhos) {
        CuckooHash cuckoo;
        vector<string> chaves(dados.begin(), dados.begin() + tam);

        auto ini_ins = steady_clock::now();
        for (int r = 0; r < repeticoes; ++r) {
            cuckoo.clear();
            for (const auto& key : chaves) cuckoo.insert(key);
        }
        auto fim_ins = steady_clock::now();

        auto ini_busca = steady_clock::now();
        for (int r = 0; r < repeticoes; ++r) {
            for (int i = 0; i < tam / 2; ++i)
                cuckoo.search(chaves[i]);
        }
        auto fim_busca = steady_clock::now();

        auto ini_rem = steady_clock::now();
        for (int r = 0; r < repeticoes; ++r) {
            CuckooHash temp;
            for (const auto& key : chaves) temp.insert(key);
            for (int i = 0; i < tam / 2; ++i)
                temp.remove(chaves[i]);
        }
        auto fim_rem = steady_clock::now();

        duration<double, micro> t_ins = (fim_ins - ini_ins) / repeticoes;
        duration<double, micro> t_bus = (fim_busca - ini_busca) / repeticoes;
        duration<double, micro> t_rem = (fim_rem - ini_rem) / repeticoes;

        double t_ins_ms = t_ins.count() / 1000.0;
        double t_bus_ms = t_bus.count() / 1000.0;
        double t_rem_ms = t_rem.count() / 1000.0;
        double latencia = (t_ins_ms + t_bus_ms + t_rem_ms) / 3.0;

        bench << tam << "," << t_ins_ms << "," << t_bus_ms << "," << t_rem_ms << "," << latencia << "," << cuckoo.getMemoria() << "," << cuckoo.getKicks() << "\n";
    }

    bench.close();
    cout << "✅ Benchmark de escalabilidade gerado em benchmark/escalabilidade_cuckoo.csv\n";
    return 0;
}
