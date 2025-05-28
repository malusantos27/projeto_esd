import matplotlib.pyplot as plt

# Gráfico: Frequência de workclass
freq_workclass = {}
with open("analise/estatisticas.txt", encoding="utf-8") as f:
    ler = False
    for linha in f:
        if "Frequência de cada workclass" in linha:
            ler = True
            continue
        if ler and ":" in linha:
            try:
                cls, qtd = linha.split(":")
                freq_workclass[cls.strip()] = int(qtd.strip().split()[0])
            except:
                break

plt.figure(figsize=(10, 6))
plt.barh(list(freq_workclass.keys()), list(freq_workclass.values()), color="orchid")
plt.title("Frequência de Workclass no Dataset")
plt.xlabel("Número de Registros")
plt.tight_layout()
plt.savefig("gráficos/frequencia_workclass.png")
plt.close()

# Gráfico: Educação por Income
edu_por_income = {"<=50K": [], ">50K": []}
with open("analise/agrupamento.txt", encoding="utf-8") as f:
    captura = False
    for linha in f:
        if "Classificação de education por income" in linha:
            captura = True
        elif captura and ":" in linha:
            inc, edus = linha.split(":")
            lista = edus.strip().split(",")
            edu_por_income[inc.strip()] = [e.strip() for e in lista if e.strip()]

plt.figure(figsize=(8, 5))
valores = [len(edu_por_income["<=50K"]), len(edu_por_income[">50K"])]
plt.bar(["<=50K", ">50K"], valores, color=["steelblue", "crimson"])
plt.title("Diversidade de Níveis de Educação por Faixa de Renda")
plt.ylabel("Níveis de Educação")
plt.tight_layout()
plt.savefig("gráficos/educacao_por_income.png")
plt.close()

# Gráfico: Workclass dos que trabalham >40h
workclass_40h = {}
with open("analise/filtragem_ordenacao.txt", encoding="utf-8") as f:
    for linha in f:
        if "|" in linha:
            classe = linha.split("|")[0].strip()
            workclass_40h[classe] = workclass_40h.get(classe, 0) + 1

plt.figure(figsize=(10, 6))
plt.barh(list(workclass_40h.keys()), list(workclass_40h.values()), color="goldenrod")
plt.title("Distribuição de Workclass (> 40h semanais)")
plt.xlabel("Registros")
plt.tight_layout()
plt.savefig("gráficos/workclass_40h+.png")
plt.close()

print("✅ Gráficos adicionais gerados na pasta 'gráficos'")
