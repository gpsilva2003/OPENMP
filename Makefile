#
# Makefile 
# Compila e gera os executáveis de todos os arquivos com extensão .c
# presentes no diretório
#
CC=pgcc
#CC=gcc-13
#CC=icc
#CC=nvc

#CFLAGS =  -Ofast -qopenmp -w3 -diag-disable:remark

ifeq ($(CC),gcc)
	CFLAGS = -O3 -fopenmp -Wall
else
	CFLAGS = -fast -mp -Minfo=all -Mneginfo=all
endif
LDLIBS=-lm

LIBS=-lm
RM=rm -vf
MV=mv
BINDIR=./bin/
SRCDIR=./src/
PROG=teste

vpath %.c ./src/

SRCFILES= $(wildcard src/*.c)
OBJFILES= $(patsubst src/%.c, %.o, $(SRCFILES))
_PROGS= $(patsubst src/%.c, %, $(SRCFILES))
PROGFILES=$(addprefix $(BINDIR),$(_PROGS))

.PHONY: all clean run

all: $(PROGFILES)

$(BINDIR)%: $(SRCDIR)%.c
	$(CC) $(INC) $< $(CFLAGS) -o $@ $(LIBS)
run:  
	$(BINDIR)$(PROG)

list:
	ls $(BINDIR)*
clean:
	$(RM) $(PROGFILES) *~
## eof Makefile
