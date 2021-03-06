#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

// Variables compartidas:

// • Una pila de enteros con capacidad limitada.
int stack_capacity;
int * stack;
int stack_size;

// • Un booleano para indicar que no se empilarán más datos.
int done;

pthread_mutex_t mutex_stack = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_stack_readable = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_stack_writable = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex_done = PTHREAD_MUTEX_INITIALIZER;

void * productor(void * arg) {
        int times = *((int *)arg);
        int n = 0;
        int s;

        int i;
        for (i = 0; i < times; ++i) {
                s = pthread_mutex_lock(&mutex_stack);
                if (s != 0) {
                        // Si el código del programa está bien, esto nunca debería suceder.  Sin embargo, esta verificación puede ayudar a detectar errores de programación.
                        errno = s;
                        perror("Error intentando entrar en la sección crítica del productor; pthread_mutex_lock: ");
                        exit(EX_SOFTWARE);
                }

                // Mientras NO haya espacio libre en la pila…
                while (!(stack_size < stack_capacity)) {
                        // …esperamos a que se libere espacio.  Cuando se entra a esta función, atómicamente se libera el mutex y se comienza a esperar por un signal sobre la condición.
                        s = pthread_cond_wait(&cond_stack_writable, &mutex_stack);
                        if (s != 0) {
                                // Si el código del programa está bien, esto nunca debería suceder.  Sin embargo, esta verificación puede ayudar a detectar errores de programación.
                                errno = s;
                                perror("Error intentando entrar en la sección crítica del productor; pthread_mutex_lock: ");
                                exit(EX_SOFTWARE);
                        }
                        // Al ocurrir un signal sobre esta condición, esta función adquiere de nuevo el mutex y retorna.  Si otro productor no se nos adelantó, la condición del ciclo no se cumplirá (porque seremos los primeros en ver el nuevo espacio disponible en la pila) y saldremos del ciclo.
                }

                { // Sección crítica:
                        ++n;
                        stack[stack_size] = n;
                        printf("Productor: empilé %d en la posición %d.\n", n, stack_size);
                        ++stack_size;
                        pthread_cond_broadcast(&cond_stack_readable); // Como empilé un valor, ahora hay un dato disponible para leer en la pila.

                        // Si estamos en la última iteración del ciclo, hay que avisar al consumidor que terminamos de escribir todos los datos.
                        if (i == times - 1) done = 1;
                }

                s = pthread_mutex_unlock(&mutex_stack);
                if (s != 0) {
                        // Si el código del programa está bien, esto nunca debería suceder.  Sin embargo, esta verificación puede ayudar a detectar errores de programación.
                        errno = s;
                        perror("Error intentando salir de la sección crítica del productor; pthread_mutex_unlock: ");
                        exit(EX_SOFTWARE);
                }
        }

        pthread_exit(NULL);
}

void * consumidor(void * arg) {
        int n = 0;
        int s;

        while (1) {
                s = pthread_mutex_lock(&mutex_stack);
                if (s != 0) {
                        // Si el código del programa está bien, esto nunca debería suceder.  Sin embargo, esta verificación puede ayudar a detectar errores de programación.
                        errno = s;
                        perror("Error intentando entrar en la sección crítica del consumidor; pthread_mutex_lock: ");
                        exit(EX_SOFTWARE);
                }

                // Mientras NO haya datos en la pila…
                while (!(stack_size > 0)) {
                        // …primero, verificamos si el productor nos avisó que ya no escribirá más datos a la pila.
                        // Aunque no está indicado explícitamente, al ejecutar esto tenemos el mutex, así que estamos dentro de una sección crítica.
                        if (done) {
                                s = pthread_mutex_unlock(&mutex_stack);
                                if (s != 0) {
                                        // Si el código del programa está bien, esto nunca debería suceder.  Sin embargo, esta verificación puede ayudar a detectar errores de programación.
                                        errno = s;
                                        perror("Error intentando salir de la sección crítica del consumidor; pthread_mutex_unlock: ");
                                        exit(EX_SOFTWARE);
                                }

                                pthread_exit(NULL);
                        }

                        // …luego, si no nos salimos, esperamos a que alguien inserte datos.  Cuando se entra a esta función, atómicamente se libera el mutex y se comienza a esperar por un signal sobre la condición.
                        s = pthread_cond_wait(&cond_stack_readable, &mutex_stack);
                        if (s != 0) {
                                // Si el código del programa está bien, esto nunca debería suceder.  Sin embargo, esta verificación puede ayudar a detectar errores de programación.
                                errno = s;
                                perror("Error intentando entrar en la sección crítica del consumidor; pthread_mutex_lock: ");
                                exit(EX_SOFTWARE);
                        }
                        // Al ocurrir un signal sobre esta condición, esta función adquiere de nuevo el mutex y retorna.  Si otro consumidor no se nos adelantó, la condición del ciclo no se cumplirá (porque seremos los primeros en ver el nuevo dato disponible en la pila) y saldremos del ciclo.
                }

                { // Sección crítica:
                        n = stack[stack_size - 1];
                        printf("Consumidor: desempilé %d de la posición %d.\n", n, stack_size - 1);
                        --stack_size;
                        pthread_cond_broadcast(&cond_stack_writable); // Como desempilé un valor, ahora hay un espacio disponible para escribir en la pila.
                }

                s = pthread_mutex_unlock(&mutex_stack);
                if (s != 0) {
                        // Si el código del programa está bien, esto nunca debería suceder.  Sin embargo, esta verificación puede ayudar a detectar errores de programación.
                        errno = s;
                        perror("Error intentando salir de la sección crítica del consumidor; pthread_mutex_unlock: ");
                        exit(EX_SOFTWARE);
                }
        }

        pthread_exit(NULL);
}

int main(int argc, char * argv[]) {
        int s;
        pthread_t tid_productor, tid_consumidor;
        int times;

        if (argc != 3) {
                fprintf(
                        stderr,
                        "Uso: %s productos tamaño\n"
                        "\n"
                        "Crea dos hilos:\n"
                        "• Un productor que empila “productos” productos a una pila de tamaño “tamaño”, y\n"
                        "• Un consumidor que desempila aquello que el productor empiló hasta que la pila quede vacía y el productor haya avisado que no empilará más productos.\n",
                        argv[0]
                );
                exit(EX_USAGE);
        }

        {
                unsigned long int times_ul;
                errno = 0;
                times_ul = strtoul(argv[1], NULL, 0);
                if (errno != 0 || times_ul > INT_MAX) {
                        fprintf(stderr, "El primer argumento de línea de comandos debería ser un entero positivo menor que %d; strtoul: ", INT_MAX);
                        perror(NULL);
                        exit(EX_USAGE);
                }
                times = times_ul;
        }

        {
                unsigned long int stack_capacity_ul;
                errno = 0;
                stack_capacity_ul = strtoul(argv[2], NULL, 0);
                if (errno != 0 || stack_capacity_ul > INT_MAX) {
                        fprintf(stderr, "El segundo argumento de línea de comandos debería ser un entero positivo menor que %d; strtoul: ", INT_MAX);
                        perror(NULL);
                        exit(EX_USAGE);
                }
                stack_capacity = stack_capacity_ul;
        }

        fprintf(stderr, "%d\n%d\n", times, stack_capacity); // DEBUG

        // Inicializar las variables compartidas:
        done = 0;
        stack = calloc(stack_capacity, sizeof(int));
        if (stack == NULL) {
                perror("No fue posible reservar memoria para la pila; calloc");
                exit(EX_OSERR);
        }
        stack_size = 0;

        // Crear los hilos productor y consumidor:
        s = pthread_create(&tid_productor, NULL, &productor, &times);
        if (s != 0) {
                errno = s;
                perror("No fue posible crear hilo productor; pthread_create: ");
                exit(EX_OSERR);
        }

        s = pthread_create(&tid_consumidor, NULL, &consumidor, NULL);
        if (s != 0) {
                errno = s;
                perror("No fue posible crear hilo consumidor; pthread_create: ");
                exit(EX_OSERR);
        }

        // Esperar por la terminación de los hilos:
        s = pthread_join(tid_productor, NULL);
        if (s != 0) {
                errno = s;
                perror("No fue posible esperar por la terminación del hilo productor; pthread_join: ");
                exit(EX_OSERR);
        }

        s = pthread_join(tid_consumidor, NULL);
        if (s != 0) {
                errno = s;
                perror("No fue posible esperar por la terminación del hilo consumidor; pthread_join: ");
                exit(EX_OSERR);
        }

        exit(EX_OK);
}
