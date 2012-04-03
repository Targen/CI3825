Caso de prueba `indirect`
=========================

Este caso de prueba verifica el funcionamiento de `rautomake` para el caso de un proyecto con subdirectorios vacíos pero que contienen indirectamente a algún archivo cuyo nombre termina en `.c`.

Este caso de prueba no contiene archivos ni subdirectorios con permisos restrictivos; por lo tanto, no evalúa el manejo de errores de permisos.

Consiste de estos archivos:

*   `README.md`          : Este archivo.
*   `Makefile.ref`       : `Makefile` de referencia.
*   `1`, `1/2`, `1/2/3`  : Subdirectorios anidados hasta llegar al 100.
*   `1/…/100`/indirect.h : Declaración de un procedimiento del programa.
*   `1/…/100`/indirect.c : Definición de un procedimiento del programa.
*   `main.c`             : Fuente del procedimiento principal del programa.



Instrucciones para `Makefile` de referencia
-------------------------------------------

Puede usarse el Makefile de referencia para compilar el programa y correrlo:

    make -f Makefile.ref
    ./indirect

Para limpiar lo que esto genera y volver al estado inicial del caso de prueba, se puede ejecutar la orden

    make -f Makefile.ref clean

El `Makefile` de referencia no tiene por qué ser similar ni mucho menos idéntico a los `Makefiles` generados por una implementación de `rautomake`.



Instrucciones para `Makefile` generado por `rautomake`
------------------------------------------------------

Debe correrse la implementación de `rautomake` a corregir en el directorio actual; debería generar un `Makefile` en el directorio actual capaz de compilar el ejecutable `indirect` mediante llamadas recursivas de cada Makefile al de su subdirectorio hasta llegar al que contiene el archivo compilable.

    /camino/a/rautomake             # Ejecutar proyecto del estudiante
    make                            # Compilar con el Makefile generado
    ./indirect                      # Ejecutar el archivo compilado



Salida esperada
---------------

El ejecutable final compilado debería imprimir los caracteres `42` seguidos de un fin de línea.
