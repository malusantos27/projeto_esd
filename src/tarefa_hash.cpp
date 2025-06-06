#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace chrono;

int contarColisoes(const unordered_map<string, int>& tabela, const vector<string>& dados) {
    unordered_map<size_t, int> hash_count;
    hash<string> hasher;
    int colisoes = 0;
    for (const auto& key : dados) {
        size_t h = hasher(key);
        if (++hash_count[h] > 1) colisoes++;
    }
    return colisoes;
}

void executar_benchmark_hash() {
    cout << "\n--- Executando Benchmark: Hash (unordered_map) ---\n";
    ifstream file("Data/adult.data");
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return;
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

    ofstream bench("benchmark/escalabilidade_hash.csv");
    bench << "Tamanho,TempoInsercao(ms),TempoBusca(ms),TempoRemocao(ms),LatenciaMedia(ms),Memoria(B),Colisoes\n";

    for (int tam : tamanhos) {
        unordered_map<string, int> tabela;
        vector<string> chaves(dados.begin(), dados.begin() + tam);

        auto ini_ins = steady_clock::now();
        for (int r = 0; r < repeticoes; ++r) {
            tabela.clear();
            for (const auto& key : chaves) tabela[key]++;
        }
        auto fim_ins = steady_clock::now();

        auto ini_busca = steady_clock::now();
        for (int r = 0; r < repeticoes; ++r) {
            for (int i = 0; i < tam / 2; ++i) tabela.find(chaves[i]);
        }
        auto fim_busca = steady_clock::now();

        auto ini_rem = steady_clock::now();
        for (int r = 0; r < repeticoes; ++r) {
            unordered_map<string, int> temp = tabela;
            for (int i = 0; i < tam / 2; ++i) temp.erase(chaves[i]);
        }
        auto fim_rem = steady_clock::now();

        duration<double, micro> t_ins = (fim_ins - ini_ins) / repeticoes;
        duration<double, micro> t_bus = (fim_busca - ini_busca) / repeticoes;
        duration<double, micro> t_rem = (fim_rem - ini_rem) / repeticoes;

        double t_ins_ms = t_ins.count() / 1000.0;
        double t_bus_ms = t_bus.count() / 1000.0;
        double t_rem_ms = t_rem.count() / 1000.0;
        double latencia = (t_ins_ms + t_bus_ms + t_rem_ms) / 3.0;

        long memoria = tabela.size() * (sizeof(string) + sizeof(int) + sizeof(void*));
        int colisoes = contarColisoes(tabela, chaves);

        bench << tam << "," << t_ins_ms << "," << t_bus_ms << "," << t_rem_ms << "," << latencia << "," << memoria << "," << colisoes << "\n";
    }

    bench.close();
    cout << "✅ Benchmark de escalabilidade gerado em benchmark/escalabilidade_hash.csv\n";
}