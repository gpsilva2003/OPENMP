# Exemplos OpenMP

Este repositório tem os exemplos em OpenMP do livro "Programação Paralela e Distribuída".
Todos os arquivos podem ser compilados com uso do Makefile usando o comando make. 
Para executar os programas compilados podem ser o comando "make run PROG=nomedoprograma". 

## Preparação do Ambiente de Execução OpenMP 

### Compiladores 

Para a utilização de um programa com OpenMP há várias opções de
linguagem e compiladores disponíveis. O OpenMP pode ser usado com as
linguagens Fortran, C e C++, para quais destacamos os seguintes
compiladores: GCC, PGI, Intel e LLVM.

#### Compilador GCC 

O compilador **gcc** pode ser utilizado nos sistemas operacionais como
Linux, MacOSX, Windows e nas diversas versões de BSD. O suporte para o
OpenMP no compilador gcc teve o seguinte histórico de acordo com as suas
versões:


 |    Versão  |    Suporte  |      Linguagens |
 | ----------- | ------------ | ------------------- |
 |  GCC 4.2.0  | OpenMP 2.5   |  C/C++/Fortran |
 |  GCC 4.4.0 |  OpenMP 3.0   |  C/C++/Fortran |
 |  GCC 4.7.0 |  OpenMP 3.1   |  C/C++/Fortran |
 |  GCC 4.9.1 | OpenMP 4.0    | C/C++/Fortran |
 |   GCC 6.1  |  OpenMP 4.5   |      C/C++ |
 |   GCC 7.1  |  OpenMP 4.5   | Fortran (parcial) |
 |   GCC 9.1  |  OpenMP 5.0   | C/C++ (parcial) |


A seguir apresentamos o comando para a compilação do programa com o
compilador gcc:

``` {style="verbatim"}
$ gcc omp_hello.c -fopenmp -o teste
```

É necessário definir a quantidade de *threads* que serão utilizadas
antes da execução de um programa. Isso pode ser feito co uso da variável
de ambiente OMP_NUM_THREADS, cujo valor é definido com o seguinte
comando:

``` {style="verbatim"}
$ export OMP_NUM_THREADS=4
```

ou

``` {style="verbatim"}
$ setenv OMP_NUM_THREADS 4
```

A execução é bem simples, sem necessidade de comando adicionais, para
qualquer uma das versões de compilador:

``` {style="verbatim"}
$ ./teste
```

#### Compilador PGI

O compilador PGI oferece suporte quase completo para o OpenMP 4.5 nas
linguagens C/C++/Fortran no sistema operacional Linux/x86-64 e
Linux/OpenPOWER. O *offloading* é implementado para execução no
hospedeiro com vários núcleos como padrão, assim como a região
**parallel** é paralelizada para todas as *threads* OpenMP.

As limitações conhecidas são as seguintes: as diretivas **simd** e
**declare simd** não tem efeito na geração de código.

O suporte para o OpenMP 3.1 é completo para as linguagens C/C++/Fortran
nos sistemas operacionais MacOS e Linux.

``` {style="verbatim"}
$ pgcc omp_hello.c -mp -o teste
```

#### Compilador Intel

O compilador da Intel tem suporte para as linguagens C/C++/Fortran nos
sistemas operacionais Windows, Linux, and MacOSX.


  |            Versão             |     Suporte      |       Linguagens |
  | ------------------------------- | ---------------- | ------------------------- |
  | Intel Compiler 12.0,13.0,14.0  |   OpenMP 3.1      |    C/C++/Fortran |
  |   Intel Compiler 15.0,16.0     |   OpenMP 4.0      |    C/C++/Fortran |
  | Intel Compiler 17.0,18.0,19.0  |    OpenMP 4.5     |     C/C++/Fortran |
  |      Intel Compiler 19.1     |     OpenMP 5.0     | C/C++/Fortran (parcial) |
  |    Intel Compiler 2021.1.2   |   GPU offloading   |     C/C++/Fortran |


``` {style="verbatim"}
$ icc omp_hello.c -qopenmp -o teste
$ icc omp_hello.c -openmp -o teste
```

Para a geração de um relatório com diagnóstico de paralelização, compile
o programa com os comandos a seguir:

    $ icc -o myprog myprog.c -openmp -openmp_report2

Há suporte para todas as facilidades que não requerem *offloading* da
versão 4.5 do OpenMP. O suporte para as construções que resultam em
*offloading* que executem no hospedeiro estão disponíveis na versão 7.0.
O suporte para *offloading* para dispositivos GPU está disponível na
versão 8.0. O suporte para o padrão OpenMP 5.0 está em pleno
desenvolvimento.

#### Compilador Clang 

O compilador Clang do LLVM é um compilador de código aberto, que tem
suporte completo para o OpenMP 4.5. O compilador Clang suporta
*offloading* para arquiteturas x86_64, ARM de 64 bits e PowerPC de 64
bits e tem suporte básico para dispositivos Cuda.

Ele pode ser instalado a partir dos fontes disponível em
<http://llvm.org/releases/download.html> ou com uso de pacotes
pré-compilados nos sistemas operacionais disponíveis nos seus
repositórios. Para seu correto funcionamento com o OpenMP o pacote de
desenvolvimento deve ser instalado junto com o compilador e as
bibliotecas de execução.

#### Fedora, CentOS e similares 

Para instalação do Clang do diretamente a partir repositório:

``` {style="verbatim"}
$ sudo dnf install clang
```

#### Ubuntu, Debian e similares 

Para instalação do Clang diretamente do repositório:

``` {style="verbatim"}
$ sudo apt-get install clang
```

O Clang foi projetado para oferecer suporte à linguagens de programação
C e C++ e sua execução é simples:

``` {style="verbatim"}
$ clang omp_hello.c -fopenmp -o teste
$ clang++ omp_hello.c -fopenmp -o teste
```

Como informação adicional, o GCC e o Clang usam bibliotecas de execução
OpenMP diferentes: libgomp e libomp respectivamente.

O engenho de execução do Clang é o LLVM OpenMP, baseado no engenho de
execução Intel OpenMP (código aberto) e está disponível em
https://www.openmprtl.org/.

### Variáveis de ambiente 

#### Compilador PGI 

O compilador PGI utiliza-se das variáveis de ambiente conforme definido
no padrão OpenMP. Atualmente a variável de ambiente
OMP_DYNAMIC não produz nenhum efeito. O valor padrão para a variável
OMP_THREAD_LIMIT é 64.

#### Compilador GCC 

-   **GOMP_CPU_AFFINITY**

    Fixa *threads* a processadores específicos. A variável deve conter
    uma lista de processadores separados por espaço ou vírgula. Esta
    lista pode conter diferentes tipos de entradas: números de
    processadores iniciando em '0', uma faixa de processadores (M-N) ou
    uma faixa de processadores com um salto (M-N:S). Se ambas as
    variáveis de ambiente GOMP_CPU_AFFINITY e OMP_PROC_BIND estiverem
    definidas, então OMP_PROC_BIND tem maior precedência.

    ``` {style="verbatim"}
    $ export GOMP_CPU_AFFINITY="0 3 1-2 4-15:2"
    $ export GOMP_CPU_AFFINITY=0
    ```

-   **GOMP_DEBUG**

    Habilita a saída de depuração. A variável deve ser definida como '0'
    (desabilitado, o valor padrão) ou '1'.

-   **GOMP_STACKSIZE**

    Define o tamanho padrão da pilha das *threads*. Tem a mesma sintaxe
    e comportamento da variável de ambiente OMP_STACKSIZE.

