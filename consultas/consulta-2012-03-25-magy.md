> Mira, si no hago el fork, el programa me dice cuales son los archivos del nivel en el que estoy y me dice cuales son los directorios. Si hago el fork, me da la misma información, repitiéndola muchas veces y después se queda pegado.

>       if(S_ISDIR(statbuf.st_mode)){
>           
>          pidhijo = fork();
>        
>            if (pidhijo < 0) {
>         printf("Fork\n");
>         perror("fork");
>         exit(1);
>            }
>            
>            if (pidhijo == 0){
>       printf("Directorio %s\n", currentPath); (Esto es lo único que no comento en la salida "buena")
>            //dircheck(dip, dit, statbuf, currentPath, depth, argv); (esta es la
>       función recursiva)
>            }
>       }

> Qué estoy haciendo mal? T_T

Cuando ejecutan `fork`, el proceso se duplica y continúa su ejecución en exactamente el mismo código en exactamente el mismo lugar, con (casi) exactamente los mismos descriptores de archivo y (casi) el mismo entorno.  Las diferencias exactas entre el padre y el hijo están enumeradas en la página de manual de `fork`, pero la que les interesa es básicamente que el retorno de `fork` es diferente (0 en el hijo, y el PID del hijo en el padre).

Lo típico que se hace luego de hacer `fork` es obligar a que el hijo se vaya por un camino de ejecución totalmente diferente al del padre y que nunca vayan a terminar ejecutando las mismas cosas.  En esencia es algo así:

~~~{.C .numberLines}
    /* código del padre */
    switch (fork()) {
        case -1: perror("fork"); exit(EX_OSERR);
        case 0: /* código del hijo */ exit(EX_OK);
        default: break;
    }
    /* el resto del código del padre */
~~~

Lo más importante es el segundo `exit`: el hijo nunca debería salir de ahí y continuar su flujo de ejecución por donde pasó el padre.

Esto no es una regla universal en el sentido de que sea necesario para que las cosas funcionen, pero es extremadamente difícil escribir código que funcione bien y no trabaje así.  Por ejemplo, este es un programa en C que imprime los números del 1 al 1000 en un orden no determininístico usando forks no estructurados:

~~~{.C .numberLines}
    #include <unistd.h>
    #include <stdio.h>
    #include <stdlib.h>

    int main() {
        int i = 0, base = 0, incr = 10;
        while (i < 10) {
            switch (incr > 0 ? fork() : 1) {
                case -1: perror("fork"); exit(EXIT_FAILURE);
                case 0: incr /= 10; i = 0; break;
                default:
                    if (incr) ++i; else { printf("%d\n", base); i = 10; }
                    base += incr; break;
            }
        }
    }
~~~

Analizar un programa así es sumamente difícil, y es muy fácil equivocarse y hacer que el programa haga `fork` infinitamente produciendo algo llamado *fork bomb*: el programa no para de duplicarse y se satura la tabla de procesos del sistema, lo que dificulta ejecutar cualquier programa, como `kill`, para terminar su ejecución (y el sistema deja de funcionar).

La mejor manera de evitar estos problemas es nunca escribir un `fork` no estructurado: el código del hijo debería ejecutarlo únicamente el hijo, y el código del padre debería ejecutarlo únicamente el padre.

Claro, hay muchos problemas donde el hijo debe ejecutar lo mismo que el padre con otros parámetros: cualquier función recursiva podría ejecutarse con una sola invocación por proceso haciendo las invocaciones recursivas en hijos.  Este proyecto tiene una estructura recursiva que hace que esta técnica sea muy natural.  Pero en cualquier caso habría una frontera bien definida entre el código del padre y el código del hijo: el hijo solo ejecuta la llamada recursiva y su ejecución termina al salir de la función, o la función nunca retorna.
