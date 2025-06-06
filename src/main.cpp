#include <iostream>
#include <limits>
#include "tarefas.h"

void mostrar_menu() {
    std::cout << "\n============================================\n";
    std::cout << "    PROJETO ESTRUTURA DE DADOS - MENU    \n";
    std::cout << "============================================\n";
    std::cout << "Analise de Dados:\n";
    std::cout << "  1. Leitura Simples (5 primeiras linhas)\n";
    std::cout << "  2. Analises Adicionais (Média, Filtro, Top 3)\n";
    std::cout << "\nBenchmarks de Estruturas:\n";
    std::cout << "  3. Executar Benchmark: AVL Tree\n";
    std::cout << "  4. Executar Benchmark: Cuckoo Hash\n";
    std::cout << "  5. Executar Benchmark: Hash (unordered_map)\n";
    std::cout << "  6. Executar Benchmark: Lista Encadeada\n";
    std::cout << "  7. Executar Benchmark: Skip List\n";
    std::cout << "  8. Executar Benchmark: Trie\n";
    std::cout << "  9. ✨ EXECUTAR TODOS OS BENCHMARKS ✨\n";
    std::cout << "  10. 🚦 EXECUTAR TESTES COM RESTRICOES 🚦\n";
    std::cout << "\n  0. Sair\n";
    std::cout << "============================================\n";
    std::cout << "Escolha uma opcao: ";
}

int main() {
    int opcao;
    while (true) {
        mostrar_menu();
        std::cin >> opcao;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\n❌ Opcao invalida. Por favor, digite um numero.\n";
            continue;
        }

        switch (opcao) {
            case 1:
                executar_leitura();
                break;
            case 2:
                executar_operacoes_adicionais();
                break;
            case 3:
                executar_benchmark_avl();
                break;
            case 4:
                executar_benchmark_cuckoo();
                break;
            case 5:
                executar_benchmark_hash();
                break;
            case 6:
                executar_benchmark_lista();
                break;
            case 7:
                executar_benchmark_skiplist();
                break;
            case 8:
                executar_benchmark_trie();
                break;
            case 9:
                std::cout << "\n🚀 Executando todos os benchmarks em sequencia...\n";
                executar_benchmark_avl();
                executar_benchmark_cuckoo();
                executar_benchmark_hash();
                executar_benchmark_lista();
                executar_benchmark_skiplist();
                executar_benchmark_trie();
                std::cout << "\n🏁 Todos os benchmarks foram concluidos\n";
                break;
            case 10:
                executar_testes_restritivos();
                break;
            case 0:
                std::cout << "Encerrando o programa. Ata logo\n";
                return 0;
            default:
                std::cout << "\n❌ Opcao inválida. Tente novamente.\n";
                break;
        }
    }
    return 0;
}