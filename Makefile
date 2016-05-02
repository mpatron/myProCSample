###############################################################################
## ATTENTION : A le faire une fois, mais le faire au moins une fois.
##
## Il faut modifier le fichier $(ORACLE_HOME)/precomp/lib/env_precomp.mk
## au niveau des variables SYS_INCLUDE_PATH et CPLUS_SYS_INCLUDE
## Dans le fichier /opt/oracle/product/11.2.0/dbhome_1/precomp/admin/pcscfg.cfg
## la variable sys_include doit être modifier pour être conforme au system
###############################################################################

## Definition de environment Oracle
include $(ORACLE_HOME)/precomp/lib/env_precomp.mk
USERID=scott/tiger
#PROCPLSFLAGS=sqlcheck=semantics userid=$(USERID) code=ANSI_C mode=ANSI
PROCPLSFLAGS=sqlcheck=full userid=$(USERID) code=ANSI_C mode=ORACLE char_map=VARCHAR2
PROCFLAGS=$(PROCPLSFLAGS) $(SYS_INCLUDE)

## Definition de environment C
DEBUG=yes
CC=gcc
#CFLAGS=-W -Wall -Wextra -ansi -pedantic
CFLAGS=-W -Wall -Wextra -std=c99 -pedantic-errors
LDFLAGS=-L $(ORACLE_HOME)/lib -l clntsh
ifeq ($(DEBUG),yes)
 CFLAGS_DEBUG=-g
else
 CFLAGS_DEBUG=
endif
EXEC=myhello
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o) $(SRC_PROC:.pc=.o)
SRC_PROC= $(wildcard *.pc)
OBJ_PROC= $(SRC_PROC:.pc=.c)
LIS_PROC= $(SRC_PROC:.pc=.lis)

# Reset the default goal.
.DEFAULT_GOAL :=all
.SUFFIXES: .h .c .o .pc

all: $(EXEC)
ifeq ($(DEBUG),yes)
	@echo "Génération en mode debug"
else
	@echo "Génération en mode release"
endif
#  helloworld.o
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.c: %.pc
	$(PROC) $(PROCFLAGS) iname=$< oname=$@

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(CFLAGS_DEBUG)

main.o: hello.h helloworld.h

## Liste des targets sans régle
.PHONY: clean cleanAll

clean:
	@rm -rf *.o $(OBJ_PROC) $(LIS_PROC) pcscfg.cfg
cleanAll: clean
	@rm -rf $(EXEC) tp*
