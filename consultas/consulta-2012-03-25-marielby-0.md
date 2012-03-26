> Una cosita, nosotros debemos crear un makefile para compilar rautomake. Pero no entiendo como se usara ese makefile desde la carpeta multidoc por ejemplo para compilar ahi el rautomake??

`rautomake` se compila en su propio directorio: el que estén usando para desarrollar su proyecto que contiene su código fuente.  Ahí es que tiene que estar el `Makefile` que deben escribir igual que como hicieron en los otros proyectos.

Una vez compilado `rautomake`, su ejecutable será invocado desde otro directorio que no tiene nada que ver con el directorio de las fuentes de `rautomake`; por ejemplo, será invocado dentro de `multidoc`.

La ubicación del ejecutable no tiene nada que ver con la ubicación en la que ese ejecutable es invocado, claro.  Si ejecutan `/bin/ls` en su directorio de trabajo del proyecto, se ejecutará el programa `ls`, cuyo ejecutable está en `/bin`, pero no se ejecutará en `/bin` sino en su directorio de trabajo del proyecto, y su salida mostraría cosas como `Makefile` y `rautomake.c` (el contenido de donde fue ejecutado, porque eso es lo que hace `ls`), y **no** cosas como `ls`, `cat` y `bash` (que es lo que hay donde está el ejecutable, pero la ubicación del ejecutable no influye en absoluto sobre su ejecución).

Pero claro, la invocación no dirá `./rautomake` sino quizás `../rautomake`, o algo como

    /home/manuel/prepas/so1/proy2/grupo22/rautomake

> O es solo para compilar rautomake dentro de su propia carpeta??

Exactamente.
