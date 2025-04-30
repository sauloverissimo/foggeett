# 📦 foggeett C++ Library for High-Performance KPIs

A `foggeett` é uma biblioteca de **indicadores técnicos financeiros**, escrita em **C++**, voltada para uso em **projetos Python de alto desempenho**, como sistemas de trading em tempo real ou análise quantitativa.

---

## 📀 Arquitetura Geral

A estrutura da biblioteca segue o padrão:

```
foggeett/
├── __init__.py               # Interface Python
├── _core.pyd                 # Módulo C++ compilado com Pybind11
├── functions/                # Implementações C++ de indicadores
│   ├── slope.cpp/.hpp
│   ├── sma.cpp/.hpp
│   ├── ...
│   └── enrich.cpp/.hpp
├── bindings.cpp             # Mapeamento das funções com Pybind11
├── CMakeLists.txt           # Build com CMake
├── pyproject.toml           # Build com scikit-build-core
└── utils.hpp/.cpp           # Funções auxiliares para manipular os dados
```

---

## 📥 Instalação

```bash
pip install -e ./foggeett
```

Requer:
- Python ≥ 3.9
- `scikit-build-core`, `pybind11`
- Compilador C++ com suporte C++17 (MSVC, gcc ou clang)

---

## 🧠 Conceitos

### 🔹 Tick

Representa um ponto de dados de mercado:

```python
Tick = Dict[str, float]
```

Exemplo:

```python
tick = {"price": 101.25, "volume": 5000, "quoteVolume": 512340}
```

Uma sequência de `ticks` é usada para cálculos de indicadores.

---

## ⚙️ Indicadores Disponíveis

Todos os indicadores implementam a seguinte assinatura:

```python
indicador(ticks: List[Dict[str, float]], campo: str, n: int = None, direction: str = "desc") -> KPIResult
```

### 🔸 Saída: `KPIResult`

```python
{
  'acro': 'SMA_PRI_10',
  'value': 101.2,
  'description': 'Simple Moving Average of price over last 10 points',
  'debug': { ... }
}
```

---

## 📈 Indicadores Implementados

| Nome       | Sigla       | Descrição                                              |
|------------|-------------|--------------------------------------------------------|
| `SLOPE`    | `SLOPEP_*`  | Inclinação da regressão linear                        |
| `SMA`      | `SMA_*`     | Média móvel simples                                   |
| `WMA`      | `WMA_*`     | Média móvel ponderada                                 |
| `EMA`      | `EMA_*`     | Média móvel exponencial                               |
| `MOM`      | `MOM_*`     | Momento do valor (diferença com n períodos atrás)     |
| `ROC`      | `ROC_*`     | Variação percentual com n períodos atrás              |
| `MACD`     | `MACD_*`    | Convergência/Divergência de Médias Móveis             |
| `OBV`      | `OBV_*`     | Volume Acumulado On-Balance                           |

---

## 🔄 Direção da Série (`direction`)

| Valor     | Ordem dos dados (ticks)            |
|-----------|------------------------------------|
| `"desc"`  | do mais recente → mais antigo (padrão) |
| `"asc"`   | do mais antigo → mais recente      |

---

## 🔧 Função `enrich()`

Calcula vários KPIs de uma só vez:

```python
from foggeett import enrich, KPIConfig

ticks = [{"price": 100}, {"price": 101}, ...]  # lista de ticks

kpis = {
    "sma_price": KPIConfig("sma", "price", {"n": 10}),
    "macd_price": KPIConfig("macd", "price", {"short": 5, "long": 10, "signal": 3})
}

results = enrich(ticks, kpis)
```

### Saída:

```python
{
  'sma_price': 101.2,
  'macd_price': 3.04,
  ...
}
```

---

## 🧪 Exemplos de Uso

```python
from foggeett import sma, slope, macd, obv, enrich, KPIConfig

ticks = [{"price": 100, "volume": 10}, {"price": 101, "volume": 15}, ...]

# Individual
resultado = sma(ticks, "price", n=10)
print(resultado.acro, resultado.value)

# Enriquecido
kpis = {
    "slope": KPIConfig("slope", "price", {"n": 10}),
    "macd": KPIConfig("macd", "price", {"short": 5, "long": 10, "signal": 3}),
}
resultados = enrich(ticks, kpis)
```

---

## 📘 Notas Técnicas

- Todos os cálculos são feitos com precisão de ponto flutuante `double`.
- O campo `"value"` é o dado principal.
- Campos `"debug"` ajudam a entender os cálculos.
- A biblioteca permite uso intensivo em *loops de alta frequência* com segurança e velocidade.

---

## 🛠️ Adicionando Novos Indicadores

Ver `FUNCTIONS.md` para o passo a passo completo:

1. Criar `.cpp` e `.hpp` com assinatura padrão.
2. Adicionar a função no `bindings.cpp`.
3. Declarar no `functions.cpp`.
4. Adicionar a chamada no `enrich.cpp` (se necessário).
5. Testar no Python com `KPIConfig(...)`.

---

## 📲 Suporte

Dúvidas, bugs ou sugestões? Abra um *issue* ou converse diretamente com o desenvolvedor do projeto.

