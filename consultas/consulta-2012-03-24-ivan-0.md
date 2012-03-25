>   Tenemos dudas con el uso de la función exec que nos genere el archivo ".d" con las dependencias de cada archivo ".c":

>   estábamos intentando con: execl("/usr/bin/gcc","gcc","-E","-MMD",nombre,NULL); donde "nombre" es el nombre del archivo.  pero captando el error nos dice que no encuentra el archivo.(errno=2)

>   Intentamos pasando el archivo como un char, abriendo el archivo y pasando su fd y nada, sigue sin encontrar el archivo.  (Ya verificamos que estamos en el directorio correcto)

El proceso de `gcc` que se crea buscará un archivo cuyo camino sea lo que haya en `nombre`.  Si ese camino es relativo, lo buscará desde el directorio de trabajo de ese proceso, que es el mismo directorio de trabajo de su padre (a menos que lo cambien con `chdir`).

Si `nombre` contenía algo como `parser.c`, entonces se buscará un archivo llamado `parser.c` en el directorio de trabajo del proceso.  Si `parser.c` estaba en un directorio anidado en alguna parte de donde corrieron `rautomake` y no en la raíz, y si nunca cambiaron el directorio de trabajo, no lo va a conseguir.

La solución, claro, es que cada vez que visitan un directorio, cambien el directorio de trabajo del proceso a ese directorio que visitan.

También podrían ir concatenando las componentes de camino a los nombres que le pasan a GCC, pero entonces generaría las dependencias con componentes de caminos, y los `Makefile`s de cada subdirectorio procesan los caminos a archivos desde donde está el `Makefile`, así que tendrían que arreglarlos… no es una buena solución.



- - -



> Por otro lado, el proyecto debe ser realizado totalmente en lenguaje C
> correcto? nada de shell script?

En efecto.  Es válido que usen shell scripts para cualquier tarea de automatización de generación de código o compilación (aunque debe poder compilarse ejecutando `make` en el directorio de su proyecto), pero el propio código del proyecto debe terminar reduciéndose a archivos en el lenguaje de programación C.  Además, el enunciado especifica ciertas restricciones sobre la implementación: debe usarse `fork` y `exec` en algunas partes para hacer ciertas cosas.  Es decir que no se vale que su programa consista de `if (fork() == 0) execl("/bin/bash", "bash", "-c", "un script que resuelva todo", NULL);`.

Este proyecto es notablemente sencillo si se hace con un shell script, así que es natural que quieran resolverlo al menos parcialmente con esas herramientas que son las más adecuadas para el problema.  Eso, junto a las razones para no usar Make recursivamente que se detallan en el paper referido en el enunciado, hace que este proyecto no sea software muy bien diseñado para uso real.  Sus objetivos son pedagógicos y no prácticos: la idea es hacer algo interesante que les enseñe a trabajar con árboles de procesos y jerarquías de archivos usando C, solidificar su entendimiento de Make e introducir el concepto de programas que generan programas, que es fundamental para la carrera.

Hacer este proyecto en C no tiene sentido práctico real, pero es un buen ejercicio.
