Caso de prueba `empty`
======================

Este caso de prueba verifica el funcionamiento de `rautomake` para el caso de una jerarquía de archivos en la que no se deba generar ningún `Makefile`.

Este caso de prueba no contiene archivos ni subdirectorios con permisos restrictivos; por lo tanto, no evalúa el manejo de errores de permisos.

Consiste de estos archivos:

*   `README.md`          : Este archivo.
*   `Makefile.caso`      : Un `Makefile` que automatiza la generación del caso de prueba.
*   `gen.sh`             : Un shell script que genera los archivos y directorios del caso de prueba.



Instrucciones para `Makefile` generado por `rautomake`
------------------------------------------------------

Debe correrse la implementación de `rautomake` a corregir en el directorio actual; no debería generar `Makefile`s.

    make -f Makefile.caso distclean # Limpiar
    make -f Makefile.caso           # Generar archivos de prueba
    /camino/a/rautomake             # Ejecutar proyecto del estudiante
    find . -type f -iname makefile  # No debería imprimir nada



Salida esperada
---------------

No deberían generarse `Makefile`s, así que el comando final no debería encontrar archivos.
