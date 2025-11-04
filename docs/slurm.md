# Slurm

**Slurm** (Simple Linux Utility for Resource Management) é um sistema de gerenciamento de *clusters* e agendamento de tarefas de código aberto, que ajuda no gerenciamento de um *cluster* e todas as cargas de trabalho (ou **jobs**) que estão sendo executadas. Existem diversos agendadores de tarefas disponíveis, e você pode já estar familiarizado com algum caso tenha usado anteriormente um cluster HPC (computação de alto desempenho) compartilhado e local. O **Torque/PBS** é outro gerenciador de tarefas muito utilizado, cujos detalhes veremos em outro artigo.  

---

## O que o Slurm faz?

As principais responsabilidades de um agendador de tarefas como o Slurm são:

1. **Entender os recursos disponíveis** no cluster (ex: quantos nós de computação estão disponíveis, qual o tamanho de cada nó, quais tarefas estão sendo executadas).
2. **Colocar em fila e alocar tarefas**, de acordo com os recursos disponíveis e os solicitados no *script* (por exemplo, se você submete uma tarefa pedindo 1 tarefa com 4 vCPUs, o Slurm adiciona à fila e espera até que um nó com 4 vCPUs esteja disponível para executar a tarefa).
3. **Monitorar e relatar o status das tarefas** (em fila, em execução, com falha, concluídas com sucesso, etc.).

### Vantagens de clusters na nuvem

Uma das maiores vantagens de usar um cluster na nuvem é poder **escalar facilmente** (aumentar ou reduzir) o número de nós conforme a demanda, economizando custos — você só paga pelos nós que estão efetivamente em uso.

---

## Diretivas Slurm

O Slurm decide como alocar recursos para sua tarefa por meio de **diretivas** incluídas no início do script. Essas diretivas começam com `#SBATCH`.

Exemplo:

```bash
#SBATCH --job-name=alignment
```

Isso informa ao Slurm que o nome da tarefa será `alignment`. Você pode também usar a forma curta `-J alignment`.

Existem **mais de 100 diretivas possíveis**, mas geralmente são necessárias apenas algumas para fazer seu job rodar — especialmente na nuvem, onde o *cluster* é só seu.

### Diretivas comuns

* `--cpus-per-task`: número de vCPUs por tarefa. Ex: `--cpus-per-task=4`.
* `--gpus-per-node`: número de GPUs por nó. Ex: `--gpus-per-node=2`.
* `--job-name`: nome do job.
* `--mem` ou `--mem-per-cpu`: memória por nó ou por vCPU. ⚠️ No ParallelCluster, essas diretivas **não são suportadas** e podem causar falhas. Recomendamos omitir.
* `--nodes`: número mínimo de nós. Ex: `--nodes=2`.
* `--ntasks`: número total de tarefas. Ex: `--ntasks=16`.
* `--ntasks-per-node`: número de tarefas por nó.
* `--time`: tempo máximo de execução (formatos: `min`, `min:seg`, `hora:min:seg`, `dias-hora:min:seg`, etc).

---

## Exemplos de scripts Slurm

### Jobs de CPU única (não paralelos)

```bash
#!/bin/bash
#SBATCH --job-name=singlecpu
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1

sleep 30
echo "hello"
```

Esse script solicita 1 vCPU, aguarda 30 segundos e imprime "hello".

Para executar **vários comandos simples ao mesmo tempo**:

```bash
#!/bin/bash
#SBATCH --job-name=singlecputasks
#SBATCH --ntasks=2
#SBATCH --cpus-per-task=1

srun --ntasks=1 echo "I'm task 1"
srun --ntasks=1 echo "I'm task 2"
```

O Slurm poderá alocar as tarefas em nós diferentes, conforme disponibilidade.

---

### Jobs multithread (SMP)

Se seu comando usa múltiplas threads:

```bash
#!/bin/bash
#SBATCH --job-name=multithreaded
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8

mycommand --threads 8
```

**Atenção**: `--cpus-per-task` não pode exceder o número de vCPUs disponíveis em um nó.

Para rodar múltiplos comandos *multithread* em paralelo:

```bash
#!/bin/bash
#SBATCH --job-name=multithreadedtasks
#SBATCH --nodes=4
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=4

srun --ntasks=1 mycommand1 --threads 4
srun --ntasks=1 mycommand2 --threads 4
srun --ntasks=1 mycommand3 --threads 4
srun --ntasks=1 mycommand4 --threads 4
```

---

###  Jobs MPI

Para rodar um programa MPI com 16 vCPUs (distribuídas entre nós, conforme necessário):

```bash
#!/bin/bash
#SBATCH --job-name=simplempi
#SBATCH --ntasks=16

mpirun myscript
```

Para rodar em 2 nós com 8 vCPUs por nó:

```bash
#!/bin/bash
#SBATCH --job-name=nodempi
#SBATCH --ntasks=16
#SBATCH --ntasks-per-node=8

mpirun myscript
```

---

## Comandos Slurm

Uma vez que o script Slurm esteja pronto, use os seguintes comandos:

* **Submeter job:**

```bash
sbatch myslurmscript.sh
```

Saída:
`Submitted batch job 208` ← ID do job.

* **Verificar status do job:**

```bash
squeue --job 208
```

Ou, para ver todos os jobs:

```bash
squeue
```

* **Cancelar um job:**

```bash
scancel 208
```

* **Verificar status do cluster:**

```bash
sinfo
```

Útil para depuração se os jobs não estiverem sendo alocados corretamente.

---

Texto adaptado de https://blog.ronin.cloud/slurm-intro/
