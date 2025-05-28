import pandas as pd
import matplotlib.pyplot as plt
import os

# Garantir pasta de gráficos
os.makedirs("gráficos", exist_ok=True)

# Carregar dataset original
df = pd.read_csv("Data/adult.data", header=None, names=[
    "age", "workclass", "fnlwgt", "education", "education_num", "marital_status",
    "occupation", "relationship", "race", "sex", "capital_gain", "capital_loss",
    "hours_per_week", "native_country", "income"
])

# Limpeza básica
df = df.replace(' ?', pd.NA).dropna()

# 1. Cruzamento níveis educacionais x salários
cross_income_edu = pd.crosstab(df["education"], df["income"])
cross_income_edu.plot(kind='bar', stacked=True, figsize=(10, 6))
plt.title("Nível educacional por faixa de renda")
plt.ylabel("Número de Registros")
plt.xlabel("Educação")
plt.tight_layout()
plt.savefig("gráficos/educacao_salario.png")
plt.close()

# 2. Frequência relativa horas trabalhadas por nível educacional
plt.figure(figsize=(12, 8))
df.groupby("education")["hours_per_week"].mean().sort_values().plot(kind='barh', color="skyblue")
plt.title("Média de horas semanais por nível educacional")
plt.xlabel("Média de Horas por Semana")
plt.tight_layout()
plt.savefig("gráficos/media_horas_por_educacao.png")
plt.close()

# 3. Análise longitudinal simulada (considerando idade como proxy temporal)
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

print("✅ Gráficos complementares gerados com sucesso na pasta 'gráficos'.")
