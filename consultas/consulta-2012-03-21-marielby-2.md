>   Estoy tratando de ir leyendo por lotes, pero en algun momento sale un error que dice:
>
>       ** glibc detected ** ./rautomake: malloc(): memory corruption
>
>   Y no logro saber que es lo que pasa. Por que motivos puede salir ese error?

El funcionamiento del mecanismo de memoria dinámica con `malloc` y `free` requiere mantener ciertos datos sobre los lotes de memoria que se han reservado, y esos datos suelen ubicarse cerca de los propios lotes de memoria dinámica que se reservaron en el *heap*.

Cuando se escribe a un espacio de memoria que está fuera de las fronteras del lote que se reservó suceden cosas malas: si el espacio de memoria accedido estaba fuera de los rangos válidos para el proceso, el sistema operativo envía al proceso la señal SIGFAULT que produce el típico *segmentation fault*; sin embargo, es posible que el acceso indebido se mantenga dentro de la memoria del proceso (así que no habrá *segfault*) pero se podría sobreescribir alguno de los datos propios de la implementación de `malloc`, `free` u otras partes del entorno de ejecución del lenguaje C.  Eso es lo que produce ese error: es un *buffer overflow*.

Hay dos fuentes probables para estos errores: o tienen un error *off‐by‐one* (cuando se pasan del límite por una posición, porque a alguna condición había que ponerle `- 1` o `+ 1`, o comparar con `<=` en vez de `<`, o algo así), están escribiendo a un *buffer* de tamaño fijo usando un mecanismo para la lectura del archivo a copiar que no especifica la cantidad de datos máxima a leer, así que se están escribiendo datos del archivo más allá del límite del buffer.
