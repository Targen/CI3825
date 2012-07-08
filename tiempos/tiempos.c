#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>



// Calcular el tiempo absoluto y guardarlo en la estructura apuntada por el parámetro que nos pasan:
void tiempo_absoluto(struct timespec * t) {
        if (clock_gettime(CLOCK_REALTIME, t) == -1) {
                perror("clock_gettime");
                exit(EX_OSERR);
        }
}



// Dados dos tiempos absolutos, calcular su diferencia en segundos como un «double».
// Si el segundo es nulo, simplemente se convierte el primero en «double».
double diferencia(struct timespec * tf, struct timespec * ti) {
        return
                // Los segundos:
                difftime(tf->tv_sec, (ti ? ti->tv_sec : 0))
                // OJO: Existen sistemas donde podría ser inválido hacer la resta directa entre los tiempos en segundos, porque «time_t» no necesariamente es un tipo aritmético.  En sistemas POSIX sí, y claro, las funciones «clock_*» son de POSIX, así que acá podría hacerse, pero mejor hacerlo de la manera más general con la función «difftime».

                // Y los nanosegundos:
                + 1e-9 * (tf->tv_nsec - (ti ? ti->tv_nsec : 0))
        ;
}



int main() {
        struct timespec t_inicial;
        struct timespec t_final  ;



        tiempo_absoluto(&t_inicial); // Guardar en «t_inicial» el tiempo absoluto actual.

        // Hacer algo que consuma tiempo: dormir por (al menos) dos segundos y medio.
        struct timespec t_duerme = { .tv_sec = 2, .tv_nsec = 500e6 };
        if (clock_nanosleep(CLOCK_REALTIME, 0, &t_duerme, NULL) == -1) {
                perror("nanosleep");
                exit(EX_OSERR);
        }

        tiempo_absoluto(&t_final); // Guardar en «t_final» el tiempo absoluto actual.



        // Mostrar la diferencia de tiempos:
        printf
                ( "Intenté dormir por %lf segundos.\n"
                  "En realidad tardé %lf segundos.\n"
                , diferencia(&t_duerme, NULL      )
                , diferencia(&t_final , &t_inicial)
                )
        ;
}
