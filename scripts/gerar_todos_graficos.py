import pandas as pd
import matplotlib.pyplot as plt
import os

# ===================================================================
# FUNÇÃO 1: LÓGICA DE 'analises_complementares.py'
# ===================================================================
def gerar_plots_complementares_pandas():
    """Gera gráficos diretamente do CSV original usando Pandas."""
    print("Gerando gráficos complementares (análise Pandas)...")
    try:
        df = pd.read_csv("Data/adult.data", header=None, names=[
            "age", "workclass", "fnlwgt", "education", "education_num", "marital_status",
            "occupation", "relationship", "race", "sex", "capital_gain", "capital_loss",
            "hours_per_week", "native_country", "income"
        ])
        df = df.replace(' ?', pd.NA).dropna()

        # Gráfico 1
        cross_income_edu = pd.crosstab(df["education"], df["income"])
        cross_income_edu.plot(kind='bar', stacked=True, figsize=(10, 6))
        plt.title("Nível educacional por faixa de renda")
        plt.ylabel("Número de Registros")
        plt.xlabel("Educação")
        plt.tight_layout()
        plt.savefig("gráficos/educacao_salario.png")
        plt.close()

        # Gráfico 2
        plt.figure(figsize=(12, 8))
        df.groupby("education")["hours_per_week"].mean().sort_values().plot(kind='barh', color="skyblue")
        plt.title("Média de horas semanais por nível educacional")
        plt.xlabel("Média de Horas por Semana")
        plt.tight_layout()
        plt.savefig("gráficos/media_horas_por_educacao.png")
        plt.close()

        # Gráfico 3
        df['age_group'] = pd.cut(df['age'], bins=[15,25,35,45,55,65,100], labels=['16-25','26-35','36-45','46-55','56-65','66+'])
        age_trend = df.groupby(['age_group', 'income']).size().unstack()
        age_trend.plot(kind='line', marker='o', figsize=(10, 6))
        plt.title("Tendência de renda por faixa etária (proxy temporal)")
        plt.ylabel("Número de Registros")
        plt.xlabel("Faixa Etária")
        plt.grid(True)
        plt.tight_layout()
        plt.savefig("gráficos/tendencia_renda_idade.png")
        plt.close()

        print("✅ Gráficos complementares gerados.")
    except FileNotFoundError:
        print("⚠️  Arquivo 'Data/adult.data' não encontrado. Pulando gráficos complementares.")


# ===================================================================
# FUNÇÃO 2: LÓGICA DE 'gerar_graficos.py'
# ===================================================================
def gerar_plots_benchmark():
    """Gera gráficos a partir dos CSVs de benchmark das estruturas de dados."""
    print("Gerando gráficos de benchmark das estruturas...")
    arquivos = {
        "AVL": "benchmark/escalabilidade_avl.csv", "Lista": "benchmark/escalabilidade_lista.csv",
        "Hash": "benchmark/escalabilidade_hash.csv", "SkipList": "benchmark/escalabilidade_skiplist.csv",
        "Cuckoo": "benchmark/escalabilidade_cuckoo.csv", "Trie": "benchmark/escalabilidade_trie.csv"
    }
    dfs = []
    for nome, path in arquivos.items():
        if os.path.exists(path):
            df = pd.read_csv(path)
            df["Estrutura"] = nome
            dfs.append(df)
        else:
            print(f"⚠️  Arquivo de benchmark '{path}' não encontrado. Pulando.")
    
    if not dfs:
        print("⚠️ Nenhum dado de benchmark encontrado. Pulando todos os gráficos de benchmark.")
        return

    df_all = pd.concat(dfs)
    df_final = df_all[df_all["Tamanho"] == df_all["Tamanho"].max()]
    def adicionar_rotulos(ax, valores):
        for i, v in enumerate(valores):
            ax.text(i, v, f"{v:.1f}", ha='center', va='bottom', fontsize=8)

    fig, ax = plt.subplots(figsize=(10, 6)); ax.bar(df_final["Estrutura"], df_final["TempoInsercao(ms)"], color="cornflowerblue"); ax.set_title("Tempo de Inserção (100%)"); ax.set_ylabel("ms"); ax.grid(axis="y"); adicionar_rotulos(ax, df_final["TempoInsercao(ms)"]); plt.tight_layout(); plt.savefig("gráficos/tempo_insercao.png"); plt.close()
    fig, ax = plt.subplots(figsize=(10, 6)); ax.bar(df_final["Estrutura"], df_final["LatenciaMedia(ms)"], color="darkorange"); ax.set_title("Latência Média (100%)"); ax.set_ylabel("ms"); ax.grid(axis="y"); adicionar_rotulos(ax, df_final["LatenciaMedia(ms)"]); plt.tight_layout(); plt.savefig("gráficos/latencia_media.png"); plt.close()
    fig, ax = plt.subplots(figsize=(10, 6)); ax.bar(df_final["Estrutura"], df_final["Memoria(B)"], color="mediumseagreen"); ax.set_title("Uso de Memória Estimada"); ax.set_ylabel("Bytes (escala log)"); ax.set_yscale("log"); ax.grid(axis="y", which="both"); plt.tight_layout(); plt.savefig("gráficos/memoria_estimada.png"); plt.close()
    
    print("✅ Gráficos de benchmark gerados.")

# ===================================================================
# FUNÇÃO 3: LÓGICA DE 'gerar_graficos_analise.py' e 'gerar_graficos_analise_extra.py'
# ===================================================================
def gerar_plots_analise_txt():
    """Gera gráficos a partir dos arquivos TXT criados pelo C++."""
    print("Gerando gráficos a partir dos arquivos de análise (.txt)...")
    
    arquivos_analise = {
        "estatisticas": "analise/estatisticas.txt", "tendencias": "analise/tendencias.txt",
        "agrupamento": "analise/agrupamento.txt", "filtragem": "analise/filtragem_ordenacao.txt"
    }
    for nome, path in arquivos_analise.items():
        if not os.path.exists(path):
            print(f"⚠️  Arquivo de análise '{path}' não encontrado. Alguns gráficos podem não ser gerados.")
            return

    # Gráfico: Média de idade por educação
    idades_edu = {}
    with open(arquivos_analise["estatisticas"], encoding="utf-8") as f:
        for linha in f:
            if "anos" in linha and ":" in linha:
                try: edu, valor = linha.split(":"); idades_edu[edu.strip()] = float(valor.strip().split(" ")[0])
                except: pass
    plt.figure(figsize=(12, 6)); plt.barh(list(idades_edu.keys()), list(idades_edu.values()), color="skyblue"); plt.title("Média de Idade por Educação"); plt.xlabel("Idade Média"); plt.tight_layout(); plt.savefig("gráficos/media_idade_educacao.png"); plt.close()

    # Gráfico: Tendência de horas semanais
    tendencias = {}
    with open(arquivos_analise["tendencias"], encoding="utf-8") as f:
        for linha in f:
            if "h/semana" in linha and ":" in linha:
                try: edu, valor = linha.split(":"); tendencias[edu.strip()] = float(valor.strip().split(" ")[0])
                except: pass
    plt.figure(figsize=(12, 6)); plt.barh(list(tendencias.keys()), list(tendencias.values()), color="mediumseagreen"); plt.title("Tendência de Horas Semanais por Educação (Simulação)"); plt.xlabel("Horas Semanais"); plt.tight_layout(); plt.savefig("gráficos/tendencias_horas_educacao.png"); plt.close()

    # Gráfico: Frequência de workclass
    freq_workclass = {}
    with open(arquivos_analise["estatisticas"], encoding="utf-8") as f:
        ler = False
        for linha in f:
            if "Frequência de cada workclass" in linha: ler = True; continue
            if ler and ":" in linha:
                try: cls, qtd = linha.split(":"); freq_workclass[cls.strip()] = int(qtd.strip().split()[0])
                except: break
    plt.figure(figsize=(10, 6)); plt.barh(list(freq_workclass.keys()), list(freq_workclass.values()), color="orchid"); plt.title("Frequência de Workclass no Dataset"); plt.xlabel("Número de Registros"); plt.tight_layout(); plt.savefig("gráficos/frequencia_workclass.png"); plt.close()
    
    # Gráfico: Diversidade de Educação por Renda
    edu_por_income = {"<=50K": [], ">50K": []}
    with open(arquivos_analise["agrupamento"], encoding="utf-8") as f:
        captura = False
        for linha in f:
            if "Classificação de education por income" in linha: captura = True
            elif captura and ":" in linha:
                inc, edus = linha.split(":"); lista = edus.strip().split(","); edu_por_income[inc.strip()] = [e.strip() for e in lista if e.strip()]
    plt.figure(figsize=(8, 5)); valores = [len(edu_por_income["<=50K"]), len(edu_por_income[">50K"])]; plt.bar(["<=50K", ">50K"], valores, color=["steelblue", "crimson"]); plt.title("Diversidade de Níveis de Educação por Faixa de Renda"); plt.ylabel("Níveis de Educação"); plt.tight_layout(); plt.savefig("gráficos/educacao_por_income.png"); plt.close()
    
    # Gráfico: Workclass dos que trabalham >40h
    workclass_40h = {}
    with open(arquivos_analise["filtragem"], encoding="utf-8") as f:
        for linha in f:
            if "|" in linha:
                classe = linha.split("|")[0].strip(); workclass_40h[classe] = workclass_40h.get(classe, 0) + 1
    plt.figure(figsize=(10, 6)); plt.barh(list(workclass_40h.keys()), list(workclass_40h.values()), color="goldenrod"); plt.title("Distribuição de Workclass (> 40h semanais)"); plt.xlabel("Registros"); plt.tight_layout(); plt.savefig("gráficos/workclass_40h+.png"); plt.close()

    print("✅ Gráficos de análise (.txt) gerados.")

# ===================================================================
# FUNÇÃO PRINCIPAL
# ===================================================================
if __name__ == "__main__":
    # Garante que as pastas de saída existem
    os.makedirs("gráficos", exist_ok=True)
    os.makedirs("analise", exist_ok=True)
    
    print("--- Iniciando Geração de Gráficos ---\n")
    
    # 1. Gráficos que dependem dos arquivos .txt da análise C++
    gerar_plots_analise_txt()
    
    # 2. Gráficos que dependem do .csv original
    gerar_plots_complementares_pandas()
    
    # 3. Gráficos que dependem dos .csv de benchmark (de uma etapa anterior)
    gerar_plots_benchmark()

    print("\n--- Todos os scripts de geração de gráficos foram executados! ---")