# 🔴🔵 EN-US 📘 FUNCTIONS.md – Function Integration Guide for the foggeett Library

This document defines the official standard for adding new functions (technical indicators) to the C++ library `foggeett`, integrated with Python via pybind11 and scikit-build-core.

All contributors must strictly follow this guide to ensure consistency, easy maintenance, and seamless integration with the Python ecosystem.

---

## ✅ Mandatory steps to add a new function

### 1. Create the function files
- Create `NAME.cpp` and `NAME.hpp` inside `foggeett/functions/`
- Include the necessary headers:
  ```cpp
  #include "NAME.hpp"
  #include "utils.hpp"
  #include "functions.hpp"
  ```
- Follow the function signature pattern:
  ```cpp
  std::optional<KPIResult> name(const std::vector<Tick>& ticks, const std::string& field, std::optional<int> n, const std::string& direction);
  std::optional<KPIResult> name(const std::vector<double>& values, std::optional<int> n, const std::string& direction);
  ```

### 2. Add to `functions.cpp`
- If `functions.cpp` acts as a centralized router, include `#include "NAME.hpp"` and define the function dispatch if needed.

### 3. Declare in `bindings.cpp`
- Inside the `PYBIND11_MODULE(_core, m)` block, add:
  ```cpp
  m.def("name", 
      py::overload_cast<const std::vector<Tick>&, const std::string&, std::optional<int>, const std::string&>(&name),
      py::arg("ticks"), py::arg("field"), py::arg("n") = std::nullopt, py::arg("direction") = "desc");

  m.def("name", 
      py::overload_cast<const std::vector<double>&, std::optional<int>, const std::string&>(&name),
      py::arg("values"), py::arg("n") = std::nullopt, py::arg("direction") = "desc");
  ```

### 4. Update `CMakeLists.txt`
- Add the `.cpp` path to the `_core` target:
  ```cmake
  foggeett/functions/NAME.cpp
  ```

### 5. Update `__init__.py`
- To expose the function directly into the Python namespace:
  ```python
  from ._core import name
  ```

### 6. Import in the test file
- Include `from foggeett import name` in your `test_functions.py`
- Create unit tests using `pytest` validating:
  - `result.acro`
  - `result.value`
  - `result.description`
  - `result.debug`

---

## 📐 Naming convention and structure

| Field         | Format       | Example                     |
|---------------|---------------|-----------------------------|
| `acro`        | `TAG_FIELD`   | `SMA_PRI`, `WMA_VOL`, etc.  |
| `value`       | `double`      | `103.52`                    |
| `description` | `string`      | `"Weighted Moving Average..."` |
| `debug`       | `map<string, double>` | `{ "x0": 100, ... }`       |

All strings must be written in **English**.

---

## 📦 Call pattern

Functions must accept:
- `std::vector<Tick>` + field
- `std::vector<double>` directly
- Optional parameters:
  - `std::optional<int> n` (window)
  - `std::string direction = "desc"`

Use `prepare_y(...)` to standardize window extraction.

---

## 🧪 Python usage example
```python
from foggeett import wma

ticks = [
    {"price": 100},
    {"price": 102},
    {"price": 104},
]

res = wma(ticks, "price", 3)
print(res.acro)
print(res.value)
print(res.description)
print(res.debug)
```

---

## ✅ With this, we ensure:
- Consistent design
- Easy to maintain and debug
- Transparent integration with Python
- Testability and scalability

Every new function **must follow these guidelines**. No exceptions.

---

# 🟢🟡 PT-BR 📘 FUNCTIONS.md - Guia de Integração de Funções na Biblioteca foggeett

Este documento define o padrão oficial para a adição de novas funções (indicadores técnicos) na biblioteca C++ `foggeett`, integrada com Python via pybind11 e scikit-build-core.

Todos os contribuidores devem seguir rigorosamente este roteiro para garantir consistência, manutenção simples e integração fluida com o ecossistema Python.

---

## ✅ Etapas obrigatórias para adicionar uma nova função

### 1. Criar os arquivos da função
- Criar `NOME.cpp` e `NOME.hpp` dentro de `foggeett/functions/`
- Incluir os headers:
  ```cpp
  #include "NOME.hpp"
  #include "utils.hpp"
  #include "functions.hpp"
  ```
- Seguir o padrão de assinatura:
  ```cpp
  std::optional<KPIResult> nome(const std::vector<Tick>& ticks, const std::string& campo, std::optional<int> n, const std::string& direction);
  std::optional<KPIResult> nome(const std::vector<double>& values, std::optional<int> n, const std::string& direction);
  ```

### 2. Adicionar ao `functions.cpp`
- Se `functions.cpp` atua como roteador centralizado, incluir o `#include "NOME.hpp"` e definir o dispatch se aplicável.

### 3. Declarar no `bindings.cpp`
- No bloco `PYBIND11_MODULE(_core, m)`, adicionar:
  ```cpp
  m.def("nome",
      py::overload_cast<const std::vector<Tick>&, const std::string&, std::optional<int>, const std::string&>(&nome),
      py::arg("ticks"), py::arg("campo"), py::arg("n") = std::nullopt, py::arg("direction") = "desc");

  m.def("nome",
      py::overload_cast<const std::vector<double>&, std::optional<int>, const std::string&>(&nome),
      py::arg("values"), py::arg("n") = std::nullopt, py::arg("direction") = "desc");
  ```

### 4. Atualizar o `CMakeLists.txt`
- Adicionar o caminho do `.cpp` ao alvo `_core`:
  ```cmake
  foggeett/functions/NOME.cpp
  ```

### 5. Atualizar o `__init__.py` do pacote
- Para expor a função diretamente no namespace Python:
  ```python
  from ._core import nome
  ```

### 6. Importar no arquivo de testes
- Incluir `from foggeett import nome` no seu `test_functions.py`
- Criar testes unitários com `pytest` verificando:
  - `result.acro`
  - `result.value`
  - `result.description`
  - `result.debug`


---

## 📐 Convenção de nomenclatura e estrutura

| Campo         | Formato       | Exemplo                     |
|---------------|---------------|-----------------------------|
| `acro`        | `TAG_CAMPO`   | `SMA_PRI`, `WMA_Vol`, etc.  |
| `value`       | `double`      | `103.52`                    |
| `description` | `string`      | `"Weighted Moving Average..."` |
| `debug`       | `map<string, double>` | `{ "x0": 100, ... }`       |

Todas as strings devem estar em **inglês**.


---

## 📦 Padrão de chamadas

As funções devem aceitar:
- `std::vector<Tick>` + campo
- `std::vector<double>` diretamente
- Parâmetros opcionais:
  - `std::optional<int> n` (janela)
  - `std::string direction = "desc"`

Usar `prepare_y(...)` para padronizar a seleção da janela.


---

## 🧪 Exemplo de uso em Python
```python
from foggeett import wma

ticks = [
    {"price": 100},
    {"price": 102},
    {"price": 104},
]

res = wma(ticks, "price", 3)
print(res.acro)
print(res.value)
print(res.description)
print(res.debug)
```

---

## ✅ Com isso, garantimos:
- Consistência no design
- Facilita manutenção e debugging
- Integração transparente com Python
- Testabilidade e extensibilidade

Qualquer nova função **deve seguir essas diretrizes**. Nenhuma exceção.

