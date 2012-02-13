#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

// Variable compartida.
int n;

// Variable mutex para “n”.
pthread_mutex_t mutex_stdout = PTHREAD_MUTEX_INITIALIZER;

// Datos pasados al hilo:
struct thread_info {
        pthread_t thread_id; // Identificador retornado por “pthread_create”.
        int thread_num;      // Número de hilo (definido por la aplicación).
        char * argv_string;  // Datos tomados del argumento de línea de comando.
};

#define LOCK(mutex)                                                                                                \
        do {                                                                                                       \
                int s = pthread_mutex_lock(&mutex);                                                                \
                if (s != 0) {                                                                                      \
                        fprintf(                                                                                   \
                                stderr,                                                                            \
                                "Error intentando entrar en la sección crítica del hilo %d; pthread_mutex_lock: ", \
                                tinfo->thread_num                                                                  \
                        );                                                                                         \
                        errno = s;                                                                                 \
                        perror(NULL);                                                                              \
                        exit(EX_SOFTWARE);                                                                         \
                }                                                                                                  \
        } while (0)

#define UNLOCK(mutex)                                                                                               \
        do {                                                                                                        \
                int s = pthread_mutex_unlock(&mutex);                                                               \
                if (s != 0) {                                                                                       \
                        fprintf(                                                                                    \
                                stderr,                                                                             \
                                "Error intentando salir de la sección crítica del hilo %d; pthread_mutex_unlock: ", \
                                tinfo->thread_num                                                                   \
                        );                                                                                          \
                        errno = s;                                                                                  \
                        perror(NULL);                                                                               \
                        exit(EX_SOFTWARE);                                                                          \
                }                                                                                                   \
        } while (0)

void lock(pthread_mutex_t * mutex, int thread_num) {
        int s = pthread_mutex_lock(mutex);
        if (s != 0) {
                fprintf(
                        stderr,
                        "Error intentando entrar en la sección crítica del hilo %d; pthread_mutex_unlock: ",
                        thread_num
                );
                errno = s;
                perror(NULL);
                exit(EX_SOFTWARE);
        }
}

void unlock(pthread_mutex_t * mutex, int thread_num) {
        int s = pthread_mutex_unlock(mutex);
        if (s != 0) {
                fprintf(
                        stderr,
                        "Error intentando salir de la sección crítica del hilo %d; pthread_mutex_unlock: ",
                        thread_num
                );
                errno = s;
                perror(NULL);
                exit(EX_SOFTWARE);
        }
}

// Función que se ejecutará en cada hilo:
void * thread_start(void * arg) {
        struct thread_info * tinfo = (struct thread_info *)arg;
        int * ret;

        // Crear un objeto en memoria dinámica para retornar al hilo principal el valor nuevo de la variable compartida:
        ret = calloc(1, sizeof(int));
        if (ret == NULL) {
                perror("calloc");
                exit(EX_OSERR);
        }

        //lock(&mutex_stdout, tinfo->thread_num);
        LOCK(mutex_stdout);
        { // Sección crítica:
                ++n;
                *ret = n;
                printf("Hilo %d (argv_string == %s): incrementé “n” a %d\n", tinfo->thread_num, tinfo->argv_string, n);
        }
        //unlock(&mutex_stdout, tinfo->thread_num);
        UNLOCK(mutex_stdout);

        // Retornar la dirección del objeto que este hilo reservó en memoria dinámica; otro hilo deberá liberar su memoria:
        pthread_exit(ret);
}

int main(int argc, char * argv[]) {
        int s, tnum, num_threads;
        struct thread_info * tinfo;
        void * res;

        // No se considera el argumento cero (que normalmente contiene el nombre del programa).
        num_threads = argc - 1;

        // Reservar memoria para los datos de todos los hilos:
        tinfo = calloc(num_threads, sizeof(struct thread_info));
        if (tinfo == NULL) {
                perror("No fue posible reservar memoria para los datos de todos los hilos; calloc");
                exit(EX_OSERR);
        }

        // Crear un hilo para cada argumento de línea de comando:
        for (tnum = 0; tnum < num_threads; ++tnum) {
                tinfo[tnum].thread_num = tnum + 1;
                tinfo[tnum].argv_string = argv[tnum + 1];

                // The pthread_create() call stores the thread ID into the corresponding element of tinfo[].
                s = pthread_create(&tinfo[tnum].thread_id, NULL, &thread_start, &tinfo[tnum]);
                if (s != 0) {
                        fprintf(stderr, "No fue posible crear el hilo %d; pthread_create: ", tnum + 1);
                        errno = s;
                        perror(NULL);
                        exit(EX_OSERR);
                }
        }

        // Esperar por la terminación de cada hilo en orden inverso al de creación e imprimir el dato que retorna.
        for (tnum = num_threads - 1; 0 <= tnum; --tnum) {
                s = pthread_join(tinfo[tnum].thread_id, &res);
                if (s != 0) {
                        fprintf(stderr, "No fue posible esperar por la terminación del hilo %d; pthread_join: ", tnum + 1);
                        errno = s;
                        perror(NULL);
                        exit(EX_OSERR);
                }

                if (res == PTHREAD_CANCELED) {
                        // Esto nunca pasa en este programa y solo está escrito acá por razones demostrativas.
                        printf("El hilo %d fue cancelado.\n", tinfo[tnum].thread_num);
                }
                else printf("El hilo principal hizo join con el hilo %d, que retornó el entero %d.\n", tinfo[tnum].thread_num, *((int *)res));

                // Liberar la memoria reservada por el hilo para retornar datos:
                free(res);
        }

        free(tinfo);
        exit(EX_OK);
}
