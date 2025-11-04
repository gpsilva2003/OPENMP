# Controle de afinidade de *threads* no OpenMP

## Arquitetura dos processadores modernos

A importância do **controle de afinidade de *threads*** no **OpenMP** está diretamente relacionada à **arquitetura moderna dos processadores**, que incluem **múltiplos núcleos por soquete** e **diversos níveis de memória cache**. Cada **núcleo** é capaz de executar uma ou mais **threads** simultâneas (via *Simultaneous Multithreading*, SMT, como o *Hyper-Threading* da Intel), embora essa característica normalmente não seja ativada na execução de programas HPC, por conta do seu baixo desempenho.  Esses núcleos compartilham uma **hierarquia de memória cache**, geralmente organizada da seguinte forma:

* **L1 cache (nível 1)** – pequeno, muito rápido, exclusivo de cada núcleo;
* **L2 cache (nível 2)** – maior, ainda rápido, geralmente também exclusivo por núcleo;
* **L3 cache (nível 3)** – grande, mais lento, normalmente **compartilhado entre todos os núcleos de um mesmo soquete**;
* **Memória principal (RAM)** – compartilhada entre todos os soquetes do computador ou nó, com latência bem maior que as caches.

Além disso, em sistemas com mais de um soquete, a comunicação entre soquetes ocorre por meio de interconexões (como o **Intel QPI** ou **AMD Infinity Fabric**), que também introduzem **diferenças de latência e largura de banda** — essa característica é conhecida como **arquitetura NUMA (Non-Uniform Memory Access)**.

### Problema: movimentação não controlada de *threads*

Quando um programa paralelo em OpenMP é executado, o sistema operacional é responsável por **atribuir as *threads* aos núcleos físicos disponíveis**. Por padrão, essa atribuição é dinâmica: o sistema pode **mover *threads*** entre núcleos durante a execução (no que é conhecido como migração de *threads*). Essa movimentação pode causar **perda de desempenho**, pois:

* a *thread* pode perder os dados que estavam armazenados no cache do núcleo anterior (ocasionando um aumento de *cache misses*);
* os acessos à memória podem se tornar **não locais** (NUMA), aumentando a latência;
* o balanceamento de carga pode se tornar imprevisível, especialmente em aplicações sensíveis à localização dos dados.

### Solução: controle de afinidade de *threads*

O **controle de afinidade** permite **vincular (ou “fixar”) cada *thread*** a um núcleo específico do processador.
Em OpenMP, isso pode ser configurado de várias formas, por exemplo:

* Pela variável de ambiente:

  ```bash
  OMP_PROC_BIND=true
  OMP_PLACES=cores
  ```
* Ou programaticamente, em C/C++:

  ```c
  #pragma omp parallel proc_bind(spread)
  {
      // código paralelo
  }
  ```

Esse controle garante que cada *thread* permaneça sempre no mesmo núcleo, preservando o conteúdo do cache e reduzindo a latência no acesso à memória local.

### Benefícios práticos

O uso adequado da afinidade de *threads* proporciona:

* **Maior reutilização de cache** (menos falhas de cache);
* **Menor interferência entre *threads*** que compartilham dados;
* **Melhor previsibilidade de desempenho**;
* **Redução de custo em sistemas NUMA**, ao garantir que as *threads* acessem dados alocados na memória local do seu soquete.

---

## Variáveis de ambiente

O  posicionamento das *threads* pode ser controlado por meio de duas variáveis de ambiente:

* **OMP_PROC_BIND**: descreve como as *threads* são vinculadas aos "lugares" (`places`) do OpenMP;
* **OMP_PLACES**: descreve esses "lugares" em termos do *hardware* disponível.

Quando estiver experimentando essas variáveis, é recomendável definir **OMP_DISPLAY_ENV=true**, para que o OpenMP imprima, em tempo de execução, como interpretou sua especificação. Os exemplos nas seções seguintes mostram essa saída.

**Valores típicos de `OMP_PLACES`:**

* `socket`: as *threads* são vinculadas a um soquete, mas podem se mover entre núcleos dentro do soquete;
* `core`: threads são vinculadas a um núcleo, mas podem se mover entre hyperthreads dentro desse núcleo;
* `thread`: threads são vinculadas a um hyperthread específico.

**Valores típicos de `OMP_PROC_BIND`** (alguns dependem da implementação):

* `master`: threads são vinculadas ao mesmo local da thread mestre;
* `close`: threads subsequentes são atribuídas a locais próximos na lista de locais;
* `spread`: threads subsequentes são distribuídas o máximo possível entre os locais;
* `true`: threads são vinculadas ao local onde foram inicialmente colocadas;
* `false`: threads não são vinculadas ao local onde foram inicialmente colocadas.

Os valores `master`, `close` e `spread` são definidos pelo padrão OpenMP. Os demais dependem da implementação.

Não existe função em tempo de execução para definir a vinculação, mas a variável interna de controle correspondente (`ICV`) pode ser lida com a função **omp_get_proc_bind()**. A vinculação também pode ser definida usando a cláusula `proc_bind` na diretiva `parallel`, com os mesmos valores mencionados.

**Exemplo:**

Se você tem dois soquetes e define:

```bash
OMP_PLACES=sockets
```

A distribuição será:

* Thread 0 → socket 0
* Thread 1 → socket 1
* Thread 2 → socket 0
* Thread 3 → socket 1
  ... e assim por diante.

Agora, se os dois sockets têm 16 núcleos no total, e você define:

```bash
OMP_PLACES=cores
OMP_PROC_BIND=close
```

A distribuição será:

* Thread 0 → core 0 (socket 0)
* Thread 1 → core 1 (socket 0)
* ...
* Thread 7 → core 7 (socket 0)
* Thread 8 → core 8 (socket 1)
  ... e assim por diante.

O valor `OMP_PROC_BIND=close` significa que as threads são atribuídas em sequência aos locais disponíveis.

Se usar:

```bash
OMP_PLACES=cores
OMP_PROC_BIND=spread
```

A distribuição será intercalada:

* Thread 0 → core 0 (socket 0)
* Thread 1 → core 8 (socket 1)
* Thread 2 → core 1 (socket 0)
* Thread 3 → core 9 (socket 1)
  ...
* Thread 15 → core 15 (socket 1)

Assim, `OMP_PLACES=cores` com `OMP_PROC_BIND=spread` é similar a `OMP_PLACES=sockets`, mas a diferença é que a última não vincula a thread a um núcleo específico, permitindo que o SO mova threads entre núcleos ou as agrupe em um mesmo núcleo, mesmo que outros estejam livres.

O valor `OMP_PROC_BIND=master` faz com que todas as threads fiquem no mesmo local da thread mestre — útil para equipes recursivas. Nesse caso, pode-se usar a cláusula `proc_bind(spread)` na equipe inicial e `proc_bind(master)` nas equipes internas.

---

## Efeitos da vinculação de threads

#### Exemplo 1: programa de cálculo de π (bound por computação)

| #threads | close/cores | spread/sockets | spread/cores |
| -------- | ----------- | -------------- | ------------ |
| 1        | 0.359       | 0.354          | 0.353        |
| 2        | 0.177       | 0.177          | 0.177        |
| 4        | 0.088       | 0.088          | 0.088        |
| 6        | 0.059       | 0.059          | 0.059        |
| 8        | 0.044       | 0.044          | 0.044        |
| 12       | 0.029       | 0.045          | 0.029        |
| 16       | 0.022       | 0.050          | 0.022        |

Observa-se quase escalonamento perfeito com `OMP_PLACES=cores`; com `OMP_PLACES=sockets`, ocorrem colisões ocasionais (duas *threads* no mesmo núcleo).

#### Exemplo 2: equação do calor (bound por largura de banda)

```math
x_{i}^{t+1} = 2x_i^t - x_{i-1}^t - x_{i+1}^t
```

| #threads | close/cores | spread/sockets | spread/cores |
| -------- | ----------- | -------------- | ------------ |
| 1        | 2.88        | 2.89           | 2.88         |
| 2        | 1.71        | 1.41           | 1.42         |
| 4        | 1.11        | 0.74           | 0.74         |
| 6        | 1.09        | 0.57           | 0.57         |
| 8        | 1.12        | 0.57           | 0.53         |
| 12       | 0.72        | 0.53           | 0.52         |
| 16       | 0.52        | 0.61           | 0.53         |

Para `OMP_PLACES=sockets`, o desempenho piora com número alto de *threads*, provavelmente por colisões. Para 6 ou 8 *threads*, `OMP_PROC_BIND=spread` fornece **o dobro de desempenho** em relação a `close`, pois distribui as *threads* entre soquetes e aumenta a largura de banda disponível por thread.

---

## Definição de lugares (`places`)

Há três valores predefinidos para `OMP_PLACES`:

* `sockets`
* `cores`
* `threads`

O valor `threads` é útil quando há **hyperthreading**, o que deve ser evitado em cargas HPC.`OMP_PLACES=cores` não garante vinculação a *hyperthread* específica, o que pode causar colisões. Com `OMP_PLACES=threads`, cada *thread* OpenMP é amarrada a um *hyperthread* específica.

Também existe uma sintaxe geral:

```text
<posição>:<número>:<passo>
```

**Exemplos:**

* `OMP_PLACES="{0:8:1},{8:8:1}"`
  Equivalente a dois soquetes com 8 núcleos cada: dois locais com 8 núcleos consecutivos.

* `OMP_PLACES="{0},{1},...,{15}"`
  Equivale a `OMP_PLACES=cores`.

* Em uma máquina com quatro soquetes:
  `OMP_PLACES="{0:4:8}:4:1"`
  Define lugares como 0,8,16,24 e os repete quatro vezes com passo 1. Atribui threads sequencialmente a esses núcleos distribuídos.

---

### Possibilidades de vinculação

Valores possíveis para `OMP_PROC_BIND`:

* `false`: sem vinculação;
* `true`: vincula a execução das *threads* ao mesmo núcleo;
* `master`: co-localiza *threads* com a *thread* mestre;
* `close`: coloca *threads* próximas da *thread* mestre conforme a  lista de locais;
* `spread`: distribui as *threads* o máximo possível, mas vincula ao núcleo em execução. 

Esses efeitos podem ser **locais** com uso da cláusula `proc_bind` na diretiva `parallel` do OpenMP.

**Configuração padrão segura:**

```bash
export OMP_PLACES=cores
export OMP_PROC_BIND=spread
```

Isso evita que o sistema operacional migre *threads*, o que **previne diversos problemas de escalabilidade**.

---
