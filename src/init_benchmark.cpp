#include <fstream>
using namespace std;

int main() {
    ofstream out("benchmark/resultados.csv");
    out << "Estrutura,Tempo(ms),ColisoesOuKicks,Memoria,Observacoes\n";
    return 0;
}
