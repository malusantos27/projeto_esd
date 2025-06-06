#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace chrono;

struct RegistroLista {
    string education;
    string workclass;
    RegistroLista* prox;
    RegistroLista(string e, string w) : education(e), workclass(w), prox(nullptr) {}
};

class Lista {
    RegistroLista* head;
    int tamanho;

public:
    Lista() : head(nullptr), tamanho(0) {}

    void inserir(string edu, string work) {
        RegistroLista* novo = new RegistroLista(edu, work);
        novo->prox = head;
        head = novo;
        tamanho++;
    }

    bool buscar(string edu) {
        RegistroLista* atual = head;
        while (atual) {
            if (atual->education == edu) return true;
            atual = atual->prox;
        }
        return false;
    }

    void remover(string edu) {
        RegistroLista* atual = head;
        RegistroLista* ant = nullptr;
        while (atual) {
            if (atual->education == edu) {
                if (ant) ant->prox = atual->prox;
                else head = atual->prox;
                delete atual;
                return;
            }
            ant = atual;
            atual = atual->prox;
        }
    }

    int getMemoria() const {
        return tamanho * (sizeof(RegistroLista) + sizeof(string) * 2);
    }
};

void executar_benchmark_lista() {
    cout << "\n--- Executando Benchmark: Lista Encadeada ---\n";
    ifstream file("Data/adult.data");
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    vector<pair<string, string>> dados;
    string linha;
    while (getline(file, linha)) {
        stringstream ss(linha);
        string campo;
        vector<string> campos;
        while (getline(ss, campo, ',')) campos.push_back(campo);
        if (campos.size() >= 15) {
            string edu = campos[3];
            string work = campos[1];
            edu.erase(remove(edu.begin(), edu.end(), ' '), edu.end());
            work.erase(remove(work.begin(), work.end(), ' '), work.end());
            dados.emplace_back(edu, work);
        }
    }
    file.close();

    vector<int> tamanhos = {int(dados.size() * 0.1), int(dados.size() * 0.5), int(dados.size())};
    const int repeticoes = 100;

    ofstream bench("benchmark/escalabilidade_lista.csv");
    bench << "Tamanho,TempoInsercao(ms),TempoBusca(ms),TempoRemocao(ms),LatenciaMedia(ms),Memoria(B)\n";

    for (int tam : tamanhos) {
        Lista lista;
        vector<pair<string, string>> entrada(dados.begin(), dados.begin() + tam);

        auto ini_ins = steady_clock::now();
        for (int r = 0; r < repeticoes; ++r) {
            for (const auto& par : entrada) lista.inserir(par.first, par.second);
        }
        auto fim_ins = steady_clock::now();

        auto ini_busca = steady_clock::now();
        for (int r = 0; r < repeticoes; ++r) {
            for (int i = 0; i < tam / 2; ++i)
                lista.buscar(entrada[i].first);
        }
        auto fim_busca = steady_clock::now();

        auto ini_rem = steady_clock::now();
        for (int r = 0; r < repeticoes; ++r) {
            Lista temp;
            for (const auto& par : entrada) temp.inserir(par.first, par.second);
            for (int i = 0; i < tam / 2; ++i)
                temp.remover(entrada[i].first);
        }
        auto fim_rem = steady_clock::now();

        duration<double, micro> t_ins = (fim_ins - ini_ins) / repeticoes;
        duration<double, micro> t_bus = (fim_busca - ini_busca) / repeticoes;
        duration<double, micro> t_rem = (fim_rem - ini_rem) / repeticoes;

        double t_ins_ms = t_ins.count() / 1000.0;
        double t_bus_ms = t_bus.count() / 1000.0;
        double t_rem_ms = t_rem.count() / 1000.0;
        double latencia = (t_ins_ms + t_bus_ms + t_rem_ms) / 3.0;

        bench << tam << "," << t_ins_ms << "," << t_bus_ms << "," << t_rem_ms << "," << latencia << "," << lista.getMemoria() << "\n";
    }

    bench.close();
    cout << "✅ Benchmark de escalabilidade gerado em benchmark/escalabilidade_lista.csv\n";
}