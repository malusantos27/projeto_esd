#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace std::chrono;

const int MAX_LEVEL = 6;

struct Node {
    string key;
    vector<Node*> forward;
    Node(string k, int level) : key(k), forward(level + 1, nullptr) {}
};

class SkipList {
    Node* header;
    int level;
    int total_nodes;

public:
    SkipList() {
        level = 0;
        header = new Node("", MAX_LEVEL);
        total_nodes = 0;
    }

    int randomLevel() {
        int lvl = 0;
        while ((rand() & 1) && lvl < MAX_LEVEL) lvl++;
        return lvl;
    }

    void insert(string key) {
        vector<Node*> update(MAX_LEVEL + 1);
        Node* current = header;
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->key < key)
                current = current->forward[i];
            update[i] = current;
        }
        current = current->forward[0];
        if (!current || current->key != key) {
            int rlevel = randomLevel();
            if (rlevel > level) {
                for (int i = level + 1; i <= rlevel; i++)
                    update[i] = header;
                level = rlevel;
            }
            Node* n = new Node(key, rlevel);
            for (int i = 0; i <= rlevel; i++) {
                n->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = n;
            }
            total_nodes++;
        }
    }

    bool search(string key) {
        Node* current = header;
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->key < key)
                current = current->forward[i];
        }
        current = current->forward[0];
        return current && current->key == key;
    }

    void remove(string key) {
        vector<Node*> update(MAX_LEVEL + 1);
        Node* current = header;
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->key < key)
                current = current->forward[i];
            update[i] = current;
        }
        current = current->forward[0];
        if (current && current->key == key) {
            for (int i = 0; i <= level; i++) {
                if (update[i]->forward[i] != current) break;
                update[i]->forward[i] = current->forward[i];
            }
            delete current;
            while (level > 0 && header->forward[level] == nullptr) level--;
            total_nodes--;
        }
    }

    int getMemoryEstimate() const {
        return total_nodes * sizeof(Node) + (MAX_LEVEL + 1) * sizeof(Node*);
    }

    void clear() {
        delete header;
        header = new Node("", MAX_LEVEL);
        level = 0;
        total_nodes = 0;
    }
};

int main() {
    srand(time(0));
    ifstream file("Data/adult.data");
    if (!file.is_open()) {
        cerr << "Erro ao abrir Data/adult.data" << endl;
        return 1;
    }

    vector<string> dados;
    string linha;
    while (getline(file, linha)) {
        string campo;
        int cont = 0;
        string chave;
        for (char c : linha) {
            if (c == ',') cont++;
            else if (cont == 3) chave += c;
            if (cont > 3) break;
        }
        chave.erase(remove(chave.begin(), chave.end(), ' '), chave.end());
        if (!chave.empty()) dados.push_back(chave);
    }
    file.close();

    vector<int> tamanhos = {int(dados.size() * 0.1), int(dados.size() * 0.5), int(dados.size())};
    const int repeticoes = 100;

    ofstream bench("benchmark/escalabilidade_skiplist.csv");
    bench << "Tamanho,TempoInsercao(ms),TempoBusca(ms),TempoRemocao(ms),LatenciaMedia(ms),Memoria(B)\n";

    for (int tam : tamanhos) {
        vector<string> chaves(dados.begin(), dados.begin() + tam);
        SkipList sl;

        auto ini_ins = steady_clock::now();
        for (int r = 0; r < repeticoes; ++r) {
            sl.clear();
            for (const auto& chave : chaves)
                sl.insert(chave);
        }
        auto fim_ins = steady_clock::now();

        auto ini_busca = steady_clock::now();
        for (int r = 0; r < repeticoes; ++r) {
            for (int i = 0; i < tam / 2; ++i)
                sl.search(chaves[i]);
        }
        auto fim_busca = steady_clock::now();

        auto ini_rem = steady_clock::now();
        for (int r = 0; r < repeticoes; ++r) {
            SkipList temp;
            for (const auto& chave : chaves)
                temp.insert(chave);
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

        bench << tam << "," << t_ins_ms << "," << t_bus_ms << "," << t_rem_ms << "," << latencia << "," << sl.getMemoryEstimate() << "\n";
    }

    bench.close();
    cout << "✅ Benchmark de escalabilidade gerado em benchmark/escalabilidade_skiplist.csv\n";
    return 0;
}
