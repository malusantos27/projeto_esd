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

struct TrieNode {
    unordered_map<char, TrieNode*> filhos;
    bool fim;
    int contador;
    TrieNode() : fim(false), contador(0) {}
};

class Trie {
    TrieNode* raiz;
    int total_nos;

public:
    Trie() : raiz(new TrieNode()), total_nos(1) {}

    void inserir(const string& palavra) {
        TrieNode* atual = raiz;
        for (char c : palavra) {
            if (!atual->filhos[c]) {
                atual->filhos[c] = new TrieNode();
                total_nos++;
            }
            atual = atual->filhos[c];
        }
        atual->fim = true;
        atual->contador++;
    }

    bool buscar(const string& palavra) {
        TrieNode* atual = raiz;
        for (char c : palavra) {
            if (!atual->filhos.count(c)) return false;
            atual = atual->filhos[c];
        }
        return atual && atual->fim;
    }

    void remover(const string& palavra) {
        removerAux(raiz, palavra, 0);
    }

    bool removerAux(TrieNode* no, const string& palavra, int idx) {
        if (!no) return false;
        if (idx == palavra.size()) {
            if (!no->fim) return false;
            no->fim = false;
            no->contador = 0;
            return no->filhos.empty();
        }
        char c = palavra[idx];
        if (!no->filhos.count(c)) return false;
        bool deve_apagar = removerAux(no->filhos[c], palavra, idx + 1);
        if (deve_apagar) {
            delete no->filhos[c];
            no->filhos.erase(c);
            total_nos--;
            return !no->fim && no->filhos.empty();
        }
        return false;
    }

    int getMemoria() const {
        return total_nos * (sizeof(TrieNode) + sizeof(void*));
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
            string ocup = campos[6];
            ocup.erase(remove(ocup.begin(), ocup.end(), ' '), ocup.end());
            dados.push_back(ocup);
        }
    }
    file.close();

    vector<int> tamanhos = {int(dados.size() * 0.1), int(dados.size() * 0.5), int(dados.size())};
    const int repeticoes = 100;

    ofstream bench("benchmark/escalabilidade_trie.csv");
    bench << "Tamanho,TempoInsercao(ms),TempoBusca(ms),TempoRemocao(ms),LatenciaMedia(ms),Memoria(B)\n";

    for (int tam : tamanhos) {
        Trie trie;
        vector<string> chaves(dados.begin(), dados.begin() + tam);

        auto ini_ins = steady_clock::now();
        for (int r = 0; r < repeticoes; ++r) {
            for (const auto& palavra : chaves)
                trie.inserir(palavra);
        }
        auto fim_ins = steady_clock::now();

        auto ini_busca = steady_clock::now();
        for (int r = 0; r < repeticoes; ++r) {
            for (int i = 0; i < tam / 2; ++i)
                trie.buscar(chaves[i]);
        }
        auto fim_busca = steady_clock::now();

        auto ini_rem = steady_clock::now();
        for (int r = 0; r < repeticoes; ++r) {
            Trie temp;
            for (const auto& palavra : chaves)
                temp.inserir(palavra);
            for (int i = 0; i < tam / 2; ++i)
                temp.remover(chaves[i]);
        }
        auto fim_rem = steady_clock::now();

        duration<double, micro> t_ins = (fim_ins - ini_ins) / repeticoes;
        duration<double, micro> t_bus = (fim_busca - ini_busca) / repeticoes;
        duration<double, micro> t_rem = (fim_rem - ini_rem) / repeticoes;

        double t_ins_ms = t_ins.count() / 1000.0;
        double t_bus_ms = t_bus.count() / 1000.0;
        double t_rem_ms = t_rem.count() / 1000.0;
        double latencia = (t_ins_ms + t_bus_ms + t_rem_ms) / 3.0;

        bench << tam << "," << t_ins_ms << "," << t_bus_ms << "," << t_rem_ms << "," << latencia << "," << trie.getMemoria() << "\n";
    }

    bench.close();
    cout << "✅ Benchmark de escalabilidade gerado em benchmark/escalabilidade_trie.csv\n";
    return 0;
}
