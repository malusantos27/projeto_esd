import os
import matplotlib.pyplot as plt

arquivos = {
    "estatisticas": "analise/estatisticas.txt",
    "tendencias": "analise/tendencias.txt",
}

# Gráfico: Média de idade por educação
idades_edu = {}
with open(arquivos["estatisticas"], encoding="utf-8") as f:
    for linha in f:
        if "anos" in linha and ":" in linha:
            try:
                edu, valor = linha.split(":")
                valor = valor.strip().split(" ")[0]
                idades_edu[edu.strip()] = float(valor)
            except:
                pass

plt.figure(figsize=(12, 6))
plt.barh(list(idades_edu.keys()), list(idades_edu.values()), color="skyblue")
plt.title("Média de Idade por Educação")
plt.xlabel("Idade Média")
plt.tight_layout()
plt.savefig("gráficos/media_idade_educacao.png")
plt.close()

# Gráfico: Tendência de horas semanais
tendencias = {}
with open(arquivos["tendencias"], encoding="utf-8") as f:
    for linha in f:
        if "h/semana" in linha and ":" in linha:
            try:
                edu, valor = linha.split(":")
                valor = valor.strip().split(" ")[0]
                tendencias[edu.strip()] = float(valor)
            except:
                pass

plt.figure(figsize=(12, 6))
plt.barh(list(tendencias.keys()), list(tendencias.values()), color="mediumseagreen")
plt.title("Tendência de Horas Semanais por Educação (Simulação)")
plt.xlabel("Horas Semanais")
plt.tight_layout()
plt.savefig("gráficos/tendencias_horas_educacao.png")
plt.close()

print("✅ Gráficos de análise gerados na pasta 'gráficos'")
