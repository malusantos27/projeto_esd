import pandas as pd
import matplotlib.pyplot as plt
import os

# Criar pasta se necessário
os.makedirs("gráficos", exist_ok=True)

arquivos = {
    "AVL": "benchmark/escalabilidade_avl.csv",
    "Lista": "benchmark/escalabilidade_lista.csv",
    "Hash": "benchmark/escalabilidade_hash.csv",
    "SkipList": "benchmark/escalabilidade_skiplist.csv",
    "Cuckoo": "benchmark/escalabilidade_cuckoo.csv",
    "Trie": "benchmark/escalabilidade_trie.csv"
}

dfs = []
for nome, path in arquivos.items():
    df = pd.read_csv(path)
    df["Estrutura"] = nome
    dfs.append(df)

df_all = pd.concat(dfs)
df_final = df_all[df_all["Tamanho"] == df_all["Tamanho"].max()]

# Função auxiliar para adicionar rótulos
def adicionar_rotulos(ax, valores):
    for i, v in enumerate(valores):
        ax.text(i, v, f"{v:.1f}", ha='center', va='bottom', fontsize=8)

# Gráfico: Tempo de Inserção
fig, ax = plt.subplots(figsize=(10, 6))
barras = ax.bar(df_final["Estrutura"], df_final["TempoInsercao(ms)"], color="cornflowerblue")
ax.set_title("Tempo de Inserção (100%)")
ax.set_ylabel("ms")
ax.grid(axis="y")
adicionar_rotulos(ax, df_final["TempoInsercao(ms)"])
plt.tight_layout()
plt.savefig("gráficos/tempo_insercao.png")
plt.close()

# Gráfico: Latência
fig, ax = plt.subplots(figsize=(10, 6))
barras = ax.bar(df_final["Estrutura"], df_final["LatenciaMedia(ms)"], color="darkorange")
ax.set_title("Latência Média (100%)")
ax.set_ylabel("ms")
ax.grid(axis="y")
adicionar_rotulos(ax, df_final["LatenciaMedia(ms)"])
plt.tight_layout()
plt.savefig("gráficos/latencia_media.png")
plt.close()

# Gráfico: Memória com escala logarítmica
fig, ax = plt.subplots(figsize=(10, 6))
barras = ax.bar(df_final["Estrutura"], df_final["Memoria(B)"], color="mediumseagreen")
ax.set_title("Uso de Memória Estimada")
ax.set_ylabel("Bytes (escala log)")
ax.set_yscale("log")
ax.grid(axis="y", which="both")
plt.tight_layout()
plt.savefig("gráficos/memoria_estimada.png")
plt.close()

# Gráfico: Colisões/Kicks com escala log
col_kick = [c for c in df_final.columns if "Colis" in c or "Kick" in c]
if col_kick:
    coluna = col_kick[0]
    fig, ax = plt.subplots(figsize=(10, 6))
    barras = ax.bar(df_final["Estrutura"], df_final[coluna], color="tomato")
    ax.set_title(f"{coluna} por Estrutura")
    ax.set_ylabel("Quantidade (log)")
    ax.set_yscale("log")
    ax.grid(axis="y", which="both")
    plt.tight_layout()
    plt.savefig("gráficos/colisoes_kicks.png")
    plt.close()

print("✅ Gráficos atualizados com melhorias visuais salvos na pasta 'gráficos'")