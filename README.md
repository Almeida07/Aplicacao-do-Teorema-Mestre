# Aplicação do Teorema Mestre: análise teórica e experimental

Trabalho em grupo que aplica o Teorema Mestre a três algoritmos recursivos em C, cada um enquadrado em um dos três casos do teorema, e compara a complexidade prevista com medições reais de tempo e de número de operações.

## Problemas analisados

| # | Recorrência | a | b | f(n) | Caso | Complexidade | Algoritmo |
|---|-------------|---|---|------|------|--------------|-----------|
| 1 | T(n) = 4T(n/2) + Θ(n) | 4 | 2 | n | 1 | Θ(n²) | Multiplicação recursiva de polinômios com 4 subprodutos |
| 2 | T(n) = 3T(n/3) + Θ(n) | 3 | 3 | n | 2 | Θ(n log n) | Merge sort em 3 partes |
| 3 | T(n) = 2T(n/2) + Θ(n²) | 2 | 2 | n² | 3 | Θ(n²) | Contagem quadrática de pares com recursão nas duas metades |

Restrições do enunciado atendidas: a ≠ 2 nos Casos 1 e 2, a ≠ b no Problema 1, e os três problemas são diferentes entre si.

## Estrutura do repositório

```
.
├── p1.c                  # Problema 1 (Caso 1)
├── p2.c                  # Problema 2 (Caso 2)
├── p3.c                  # Problema 3 (Caso 3)
├── analise.py            # agrega o CSV, ajusta as curvas teóricas e gera tabelas/gráficos
├── resultados.csv        # medições de todos os experimentos (um único arquivo)
└── relatorio_teorema_mestre.pdf   # relatório completo (análise teórica, indução, resultados)
```

## Requisitos

- Compilador C (GCC; os resultados do relatório foram obtidos com GCC 13.3.0 e `-O2`) em Linux ou ambiente compatível (usa `clock_gettime`).
- Python 3 com `pandas`, `numpy` e `matplotlib` (somente para `analise.py`).

## Como compilar e executar

```bash
# compilar
gcc -O2 -Wall -o p1 p1.c
gcc -O2 -Wall -o p2 p2.c
gcc -O2 -Wall -o p3 p3.c

# criar o CSV com o cabeçalho (os programas só acrescentam linhas)
echo "problema,n,repeticao,tempo_s,operacoes" > resultados.csv

# executar: ./pN n_min n_max repeticoes   (n dobra a cada experimento)
./p1 64 4096 10
./p2 1024 4194304 10
./p3 64 8192 10
```

Cada programa recebe o menor tamanho de entrada, o maior e o número de repetições por tamanho. O valor de `n` dobra a cada experimento, e cada execução acrescenta uma linha a `resultados.csv`. No Problema 1, `n` deve ser potência de 2 (o código assume essa divisão).

Para gerar as tabelas e os gráficos:

```bash
python3 analise.py
```

O script lê `resultados.csv` e produz `fig1.png`, `fig2.png`, `fig3.png` (tempo × n em escala log-log e razão medido/g(n)) e `tab1.csv`, `tab2.csv`, `tab3.csv` (estatísticas por tamanho e erro em relação à curva teórica).

## Formato do `resultados.csv`

| Coluna | Descrição |
|--------|-----------|
| `problema` | 1, 2 ou 3 |
| `n` | tamanho da entrada |
| `repeticao` | número da repetição (1 a 10) |
| `tempo_s` | tempo de execução em segundos (`CLOCK_MONOTONIC`) |
| `operacoes` | contador de operações elementares do algoritmo |

## Metodologia resumida

- Para cada tamanho, o algoritmo é executado 10 vezes, com entradas geradas por `rand()` e semente fixa (`srand(42)`).
- Para cada problema, ajusta-se por mínimos quadrados uma constante `c` tal que tempo (ou número de operações) ≈ `c·g(n)`, com `g(n)` = n², n·log₂ n e n², respectivamente.
- A comparação usa o erro relativo, a razão medido/g(n) e a inclinação da reta em escala log-log.

## Resultados resumidos

| Problema | Complexidade prevista | Inclinação log-log do tempo | Faixa do erro do tempo |
|----------|-----------------------|-----------------------------|------------------------|
| 1 | Θ(n²) | 2,012 | −8,8% a +0,4% |
| 2 | Θ(n log n) | 1,091 | −3,7% a +19,9% |
| 3 | Θ(n²) | 2,020 | −10,0% a +0,1% |

O número de operações coincide com as fórmulas exatas das recorrências dos contadores (por exemplo, `(11/3)n² − 4n + 4/3` no Problema 1). Os detalhes, as demonstrações por indução e a discussão das divergências estão no relatório.

## Limitações

- As medições do relatório foram feitas em uma máquina virtual com 1 CPU virtual (Intel Xeon 2,80 GHz), sujeita a ruído de outras cargas.
- Só foram testados tamanhos em potência de 2 e uma única semente de dados.
- Os tempos absolutos dependem da máquina; vale repetir os experimentos no seu ambiente.

## Integrantes

- Mateus Almeida e Rafael Borduni
