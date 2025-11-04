# Preparação do Ambiente de Execução OpenMP 

## Compiladores 

Para a utilização de um programa com OpenMP há várias opções de
linguagem e compiladores disponíveis. O OpenMP pode ser usado com as
linguagens Fortran, C e C++, para quais destacamos os seguintes
compiladores: GCC, PGI, Intel e LLVM.

### Compilador GCC 

O compilador **gcc** pode ser utilizado nos sistemas operacionais como
Linux, MacOSX, Windows e nas diversas versões de BSD. O suporte para o
OpenMP no compilador gcc teve o seguinte histórico de acordo com as suas
versões:


 |    Versão  |    Suporte  |      Linguagens |
 | ----------- | ------------ | ------------------- |
 |  GCC 4.2    | OpenMP 2.5   |  C/C++/Fortran |
 |  GCC 4.4    |  OpenMP 3.0  |  C/C++/Fortran |
 |  GCC 4.7    |  OpenMP 3.1  |  C/C++/Fortran |
 |  GCC 4.9    | OpenMP 4.0   | C/C++/Fortran |
 |   GCC 6     |  OpenMP 4.5  | C/C++ |
 |   GCC 7     |  OpenMP 4.5  | Fortran (parcial) |
 |   GCC 9.1   |  OpenMP 5.0  | C/C++ (parcial) |
 |   GCC 11    |  OpenMP 4.5  | Fortran         |
 |   GCC 12    |  OpenMP 5.1  | C/C++ (parcial) |
 |   GCC 13    |  OpenMP 5.2  | C/C++ (parcial)  |


O OpenMP 4.0 é totalmente suportado para C, C++ e Fortran desde o GCC 4.9.
O OpenMP 4.5 é totalmente suportado para C e C++ desde o GCC 6 e parcialmente para Fortran desde o GCC 7.
O OpenMP 5.0 é parcialmente suportado para C e C++ desde o GCC 9, com suporte ampliado no GCC 10.
A partir do GCC 11, o OpenMP 4.5 passou a ser totalmente suportado para Fortran, e o suporte ao OpenMP 5.0 foi ampliado para C, C++ e Fortran.
O GCC 12 introduziu o suporte inicial ao OpenMP 5.1 e expandiu a cobertura do OpenMP 5.0.
O GCC 13 implementa diversos recursos do OpenMP 5.2 e amplia o suporte às versões 5.0 e 5.1.


A seguir apresentamos o comando para a compilação do programa com o
compilador gcc:

```bash
$ gcc omp_hello.c -fopenmp -o teste
```

É necessário definir a quantidade de *threads* que serão utilizadas
antes da execução de um programa. Isso pode ser feito co uso da variável
de ambiente OMP_NUM_THREADS, cujo valor é definido com o seguinte
comando:

```bash
$ export OMP_NUM_THREADS=4
```

ou

```bash
$ setenv OMP_NUM_THREADS 4
```

A execução é bem simples, sem necessidade de comando adicionais, para
qualquer uma das versões de compilador:

```bash
$ ./teste
```

### Compilador PGI

O compilador PGI oferece suporte quase completo para o OpenMP 4.5 nas
linguagens C/C++/Fortran no sistema operacional Linux/x86-64 e
Linux/OpenPOWER. O *offloading* é implementado para execução no
hospedeiro com vários núcleos como padrão, assim como a região
**parallel** é paralelizada para todas as *threads* OpenMP.

As limitações conhecidas são as seguintes: as diretivas **simd** e
**declare simd** não tem efeito na geração de código.

O suporte para o OpenMP 3.1 é completo para as linguagens C/C++/Fortran
nos sistemas operacionais MacOS e Linux.

```bash
$ pgcc omp_hello.c -mp -o teste
```

### Compilador Intel

O compilador da Intel tem suporte para as linguagens C/C++/Fortran nos
sistemas operacionais Windows, Linux, and MacOSX.

| Versão                          | Suporte                          | Linguagens            |
|--------------------------------|----------------------------------|------------------------|
| Intel Compiler 12.0, 13.0, 14.0| OpenMP 3.1                        | C/C++/Fortran          |
| Intel Compiler 15.0, 16.0      | OpenMP 4.0                        | C/C++/Fortran          |
| Intel Compiler 17.0, 18.0, 19.0| OpenMP 4.5                        | C/C++/Fortran          |
| Intel Compiler 19.1             | OpenMP 5.0 (parcial)             | C/C++/Fortran          |
| Intel oneAPI Compiler 2021 (e posteriores) | GPU Offloading OpenMP 5.0 completo para C/C++ (2021.1) | C/C++/Fortran          |
| Intel Compiler 2025.0 (ifx)     | OpenMP 6.0 elementos implementados | C/C++/Fortran          |


Compile com `-Qopenmp` no Windows, ou apenas `-qopenmp` or `–fiopenmp` no Linux ou Mac OSX. Compile com `-fiopenmp -fopenmp-targets=spir64` no Windows e Linux para suporte de  *offloading*. 

```bash
$ icc omp_hello.c -qopenmp -o teste
$ icc omp_hello.c -fiopenmp -o teste
```

Para a geração de um relatório com diagnóstico de paralelização, compile
o programa com os comandos a seguir:

```bash
    $ icc -o myprog myprog.c -qopenmp -openmp_report2
```

Há suporte para todas as facilidades que não requerem *offloading* da
versão 4.5 do OpenMP. O suporte para as construções que resultam em
*offloading* que executem no hospedeiro estão disponíveis na versão 7.0.
O suporte para *offloading* para dispositivos GPU está disponível na
versão 8.0. O suporte para o padrão OpenMP 5.0 está em pleno
desenvolvimento.

### Compilador Clang 

O compilador **Clang**, do projeto **LLVM**, é um compilador de código aberto
que possui **suporte completo ao OpenMP 4.5**. O Clang oferece **offloading** 
para as arquiteturas **x86_64**, **ARM de 64 bits** e **PowerPC de 64 bits**, 
além de possuir **suporte básico para dispositivos CUDA**.
Atualmente, o **Clang implementa quase todas as funcionalidades do OpenMP 5.0** 
e **boa parte das versões 5.1 e 5.2**.
No que se refere ao offloading para **GPUs**, o Clang dá suporte aos 
**dispositivos NVIDIA (via CUDA)** e **AMD GPUs** (por meio do *runtime* LLVM/ROCm), 
embora em alguns casos o suporte ainda seja parcial ou em desenvolvimento.

Ele pode ser instalado a partir dos fontes disponível em
<http://llvm.org/releases/download.html> ou com uso de pacotes
pré-compilados nos sistemas operacionais disponíveis nos seus
repositórios. Para seu correto funcionamento com o OpenMP o pacote de
desenvolvimento deve ser instalado junto com o compilador e as
bibliotecas de execução.

### Fedora, CentOS e similares 

Para instalação do Clang do diretamente a partir repositório:

```bash
$ sudo dnf install clang
```

### Ubuntu, Debian e similares 

Para instalação do Clang diretamente do repositório:

```bash
$ sudo apt-get install clang
```

O Clang foi projetado para oferecer suporte à linguagens de programação
C e C++ e sua execução é simples:

```bash
$ clang omp_hello.c -fopenmp -o teste
$ clang++ omp_hello.c -fopenmp -o teste
```

Como informação adicional, o GCC e o Clang usam bibliotecas de execução
OpenMP diferentes: libgomp e libomp respectivamente.

O engenho de execução do Clang é o LLVM OpenMP, baseado no engenho de
execução Intel OpenMP (código aberto) e está disponível em
https://www.openmprtl.org/.

## Variáveis de ambiente 

### Compilador PGI 

O compilador PGI utiliza-se das variáveis de ambiente conforme definido
no padrão OpenMP. Atualmente a variável de ambiente
OMP_DYNAMIC não produz nenhum efeito. O valor padrão para a variável
OMP_THREAD_LIMIT é 64.

### Compilador GCC 

-   **GOMP_CPU_AFFINITY**

    Fixa *threads* a processadores específicos. A variável deve conter
    uma lista de processadores separados por espaço ou vírgula. Esta
    lista pode conter diferentes tipos de entradas: números de
    processadores iniciando em '0', uma faixa de processadores (M-N) ou
    uma faixa de processadores com um salto (M-N:S). Se ambas as
    variáveis de ambiente GOMP_CPU_AFFINITY e OMP_PROC_BIND estiverem
    definidas, então OMP_PROC_BIND tem maior precedência.

    ```bash
    $ export GOMP_CPU_AFFINITY="0 3 1-2 4-15:2"
    $ export GOMP_CPU_AFFINITY=0
    ```

-   **GOMP_DEBUG**

    Habilita a saída de depuração. A variável deve ser definida como '0'
    (desabilitado, o valor padrão) ou '1'.

-   **GOMP_STACKSIZE**

    Define o tamanho padrão da pilha das *threads*. Tem a mesma sintaxe
    e comportamento da variável de ambiente OMP_STACKSIZE.
