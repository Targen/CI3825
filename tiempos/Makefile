SHELL  = bash
CC     = gcc
COPTS  = -std=gnu9x
LDLIBS = rt



# Objetivos auxiliares (no asociadas a archivos reales con esos nombres):
.PHONY: all clean

all: tiempos

clean:
	rm -f ./*.o tiempos



# Reglas genéricas:
%.o: %.c
	$(CC) $(COPTS) -c $<



# Programa principal:
tiempos: tiempos.o
	$(CC) $(COPTS) -o $@ $^ $(LDLIBS:%=-l%)
