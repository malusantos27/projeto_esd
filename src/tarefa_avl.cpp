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

int total_nos_avl = 0;

struct NodoAVL {
    string education;
    map<string, int> workclass_map;
    int altura;
    NodoAVL* esq;
    NodoAVL* dir;

    NodoAVL(string edu, string work) {
        education = edu;
        workclass_map[work] = 1;
        altura = 1;
        esq = dir = nullptr;
        total_nos_avl++;
    }
};

int altura(NodoAVL* n) {
    return n ? n->altura : 0;
}

NodoAVL* rotacaoDireita(NodoAVL* y) {
    NodoAVL* x = y->esq;
    NodoAVL* T2 = x->dir;
    x->dir = y;
    y->esq = T2;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    return x;
}

NodoAVL* rotacaoEsquerda(NodoAVL* x) {
    NodoAVL* y = x->dir;
    NodoAVL* T2 = y->esq;
    y->esq = x;
    x->dir = T2;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    return y;
}

int balanceamento(NodoAVL* n) {
    return n ? altura(n->esq) - altura(n->dir) : 0;
}

NodoAVL* inserir(NodoAVL* raiz, string edu, string work) {
    if (!raiz) return new NodoAVL(edu, work);
    if (edu < raiz->education)
        raiz->esq = inserir(raiz->esq, edu, work);
    else if (edu > raiz->education)
        raiz->dir = inserir(raiz->dir, edu, work);
    else {
        raiz->workclass_map[work]++;
        return raiz;
    }

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));
    int fb = balanceamento(raiz);

    if (fb > 1 && edu < raiz->esq->education)
        return rotacaoDireita(raiz);
    if (fb < -1 && edu > raiz->dir->education)
        return rotacaoEsquerda(raiz);
    if (fb > 1 && edu > raiz->esq->education) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }
    if (fb < -1 && edu < raiz->dir->education) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

NodoAVL* buscar(NodoAVL* raiz, string edu) {
    if (!raiz || raiz->education == edu) return raiz;
    if (edu < raiz->education)
        return buscar(raiz->esq, edu);
    else
        return buscar(raiz->dir, edu);
}

NodoAVL* remover(NodoAVL* raiz, string edu) {
    if (!raiz) return nullptr;

    if (edu < raiz->education)
        raiz->esq = remover(raiz->esq, edu);
    else if (edu > raiz->education)
        raiz->dir = remover(raiz->dir, edu);
    else {
        if (!raiz->esq || !raiz->dir) {
            NodoAVL* temp = raiz->esq ? raiz->esq : raiz->dir;
            delete raiz;
            return temp;
        }
        NodoAVL* min = raiz->dir;
        while (min->esq) min = min->esq;
        raiz->education = min->education;
        raiz->workclass_map = min->workclass_map;
        raiz->dir = remover(raiz->dir, min->education);
    }

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));
    int fb = balanceamento(raiz);

    if (fb > 1 && balanceamento(raiz->esq) >= 0)
        return rotacaoDireita(raiz);
    if (fb > 1 && balanceamento(raiz->esq) < 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }
    if (fb < -1 && balanceamento(raiz->dir) <= 0)
        return rotacaoEsquerda(raiz);
    if (fb < -1 && balanceamento(raiz->dir) > 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

int main() {
    ifstream file("Data/adult.data");
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return 1;
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

    ofstream bench("benchmark/escalabilidade_avl.csv");
    bench << "Tamanho,TempoInsercao(ms),TempoBusca(ms),TempoRemocao(ms),LatenciaMedia(ms),Memoria(B)\n";

    for (int tam : tamanhos) {
        vector<pair<string, string>> entrada(dados.begin(), dados.begin() + tam);

        auto ini_ins = steady_clock::now();
        for (int r = 0; r < repeticoes; ++r) {
            total_nos_avl = 0;
            NodoAVL* raiz = nullptr;
            for (const auto& par : entrada)
                raiz = inserir(raiz, par.first, par.second);
        }
        auto fim_ins = steady_clock::now();

        NodoAVL* raiz_busca = nullptr;
        for (const auto& par : entrada)
            raiz_busca = inserir(raiz_busca, par.first, par.second);

        auto ini_busca = steady_clock::now();
        for (int r = 0; r < repeticoes; ++r) {
            for (int i = 0; i < tam / 2; ++i)
                buscar(raiz_busca, entrada[i].first);
        }
        auto fim_busca = steady_clock::now();

        auto ini_rem = steady_clock::now();
        for (int r = 0; r < repeticoes; ++r) {
            NodoAVL* temp = nullptr;
            for (const auto& par : entrada)
                temp = inserir(temp, par.first, par.second);
            for (int i = 0; i < tam / 2; ++i)
                temp = remover(temp, entrada[i].first);
        }
        auto fim_rem = steady_clock::now();

        duration<double, micro> t_ins = (fim_ins - ini_ins) / repeticoes;
        duration<double, micro> t_bus = (fim_busca - ini_busca) / repeticoes;
        duration<double, micro> t_rem = (fim_rem - ini_rem) / repeticoes;

        double t_ins_ms = t_ins.count() / 1000.0;
        double t_bus_ms = t_bus.count() / 1000.0;
        double t_rem_ms = t_rem.count() / 1000.0;
        double latencia = (t_ins_ms + t_bus_ms + t_rem_ms) / 3.0;

        long memoria = total_nos_avl * (sizeof(NodoAVL) + sizeof(string) + sizeof(int));
        bench << tam << "," << t_ins_ms << "," << t_bus_ms << "," << t_rem_ms << "," << latencia << "," << memoria << "\n";
    }

    bench.close();
    cout << "✅ Benchmark de escalabilidade gerado em benchmark/escalabilidade_avl.csv\n";
    return 0;
}
