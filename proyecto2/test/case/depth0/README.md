Caso de prueba `depth0`
=======================

Este caso de prueba verifica el funcionamiento de `rautomake` para el caso de un proyecto simple sin subdirectorios.

Este caso de prueba no contiene archivos ni subdirectorios con permisos restrictivos; por lo tanto, no evalúa el manejo de errores de permisos.

Consiste de estos archivos:

*   `README.md`          : Este archivo.
*   `Makefile.caso`      : Un `Makefile` que automatiza la generación del caso de prueba.
*   `Makefile.ref.header`: Encabezado del `Makefile` de referencia que se genera.
*   `gen.sh`             : Un shell script que genera los archivos `.c` y `.h` del caso de prueba.



Instrucciones para `Makefile` de referencia
-------------------------------------------

Para generar los archivos de este caso de prueba se debe ejecutar la orden

    make -f Makefile.caso

Luego de eso, puede usarse el Makefile de referencia que se genera para compilar el programa y correrlo:

    make -f Makefile.ref
    ./depth0

Para limpiar lo que esto genera, se puede ejecutar la orden

    make -f Makefile.ref clean

Y para volver al estado inicial del caso de prueba, se puede ejecutar

    make -f Makefile.caso distclean

El `Makefile` de referencia no tiene por qué ser similar ni mucho menos idéntico a los `Makefiles` generados por una implementación de `rautomake`.



Instrucciones para `Makefile` generado por `rautomake`
------------------------------------------------------

Con el código de prueba ya generado debe correrse la implementación de `rautomake` a corregir en el directorio actual; debería generar un `Makefile` capaz de compilar el ejecutable `depth0` como lo hace el Makefile de referencia generado por el caso de prueba.

    make -f Makefile.caso distclean # Limpiar
    make -f Makefile.caso           # Generar código de prueba
    /camino/a/rautomake             # Ejecutar proyecto del estudiante
    make                            # Compilar con el Makefile generado
    ./depth0                        # Ejecutar el archivo compilado



Salida esperada
---------------

El ejecutable final compilado debería imprimir los números enteros del 1 al 20 en notación posicional en base decimal con los dígitos de ASCII, seguido cada uno por un fin de línea.
