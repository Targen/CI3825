1.  > \-En el caso del proyecto, basta con tener UN pipe para comunicarse con el padre verdad?. Inicialmente crei que se necesitaba uno por cada uno, pero me estaba dando muchos errores, y al tener uno solo me parece que funciona. Pero no se con seguridad si es asi y cual es el motivo por el que basta uno?

    Cada trabajador debe tener abiertos dos *pipe*s a los que podrá escribir: uno para reportar al padre cuántos visitantes quedaron en su cola cuando finalice sus iteraciones, y otro para escribir al trabajador que maneja la siguiente atracción la cantidad de visitantes que salen de cada una de sus iteraciones.  (Esos otros procesos evidentemente deben tener abierto el otro lado del *pipe* para leer esos datos, excepto en el caso de trabajadores que ya hayan terminado).

    En el caso de los *pipe*s usados para la comunicación entre trabajadores, no es viable ninguna solución que utilice menos que un *pipe* por trabajador: si hubiera menos que esa cantidad de *pipe*s, sería posible que un trabajador lea datos que *no* son para él.  Si la longitud de los datos escritos por cada trabajador llegara a ser mayor que `PIPE_BUF` bytes (que no es menor que 512 según POSIX.1-2001), podría suceder, además, que los mensajes de varios trabajadores estén solapados.  Como son mensajes cortos, podría asegurarse que esto no pase sin ningún problema (y en ese caso, POSIX asegura que la escritura se haga atómicamente), pero esto no elimina el problema de que los trabajadores lean (y consuman) mensajes que *no* estaban destinados a ellos.  En esos casos se podría volver a escribir el mensaje al *pipe* compartido para que esté disponible al trabajador al que correspondía (si en el mensaje se incluye su identificación), pero dependiendo de otros detalles de la implementación, el algoritmo resultante podría causar inanición: el mensaje podría no llegar nunca a su destino.

    En el caso de los *pipe*s usados para la comunicación con el padre al finalizar, sí es viable que se utilice menos que un *pipe* por trabajador.  La escritura de los mensajes sería atómica porque los mensajes producidos al finalizar son cortos: incluyen simplemente un entero en forma binaria que mide exactamente 4 bytes (u 8, en una máquina de 64 bits) que es menos que 512, y si lo escriben en forma de texto, tendría los dígitos decimales correspondientes al entero seguidos de un delimitador (un byte que no sea un dígito), que también sería menos que 512 bytes (en el caso de 32 bits, el mayor entero sin signo es 4294967295, y en el caso de 64 bits es 18446744073709551615, que tienen 10 y 20 dígitos decimales, respectivamente; al añadir el delimitador, 11 y 21 son, claro, aun menores que 512).  Esto asegura que los mensajes de los trabajadores no se mezclen en el *pipe* compartido, y como solo el padre leerá, el problema del párrafo anterior no existe.



    - - -



2.  > \-Si el padre hace un wait para esperar a que los hijos terminen, espera hasta que terminen todos sus hijos verdad? y luego es que poodria leer la informacion e imprimir lo que corresponde?. Es que me confunde que el enunciado dice "a medida que los trabajadores terminen, imprimir el numero de personas que quedaron en  la cola ...", en realidad imprimira, despues de que todos terminen cuando salga del wait... o estoy equivocada??

    POSIX.1‐2008, en su especificación de las funciones `wait` y `waitpid`, dice

    > The `wait()` and `waitpid()` functions shall obtain status information pertaining to one of the caller's child processes. Various options permit status information to be obtained for child processes that have terminated or stopped. If status information is available for two or more child processes, the order in which their status is reported is unspecified.

    Las funciones `wait` y `waitpid` retornan cuando *un* hijo cambia de estado (aunque también pueden retornar si son interrumpidas por una señal).  Si quieren esperar por la terminación de *todos* los hijos, deben llamar repetidamente a `wait` hasta que todos los hijos indiquen que terminaron.  Si usan `waitpid`, asegúrense de que el cambio de estado fue en efecto una terminación (porque dependiendo de las opciones que le especifiquen, `waitpid` puede retornar el identificador de un proceso que se haya hecho detener o continuar con señales).  Recuerden también que `wait` y `waitpid` pueden retornar si son interrumpidos por señales, en cuyo caso sus códigos de retorno indicarían errores (que no deben causar que el programa aborte, sino que se vuelva a ejecutar la llamada).

    Cuando `wait` retorna indicando que un hijo terminó, pueden imprimir inmediatamente la información indicada en el *pipe* antes de continuar el ciclo de `wait` para esperar por el retorno del siguiente trabajador (en caso de que falten).  Esto es lo que indica esa parte del enunciado.



    - - -



3.  > \-Se que el padre debe cerrar el descriptor que no vaya a utilizar, en este caso el de escritura. Pero tambien deberia cerrar los descriptores de los pipes del anilllo porque el no los usa?

    Sí, pero solo cuando haya terminado de crear a todos sus hijos.  Si los cierran antes, los hijos serán creados con todos los *pipe*s cerrados, y no podrán usarlos.  Si no los cierran, se les dificultará detectar en los trabajadores que el anterior ya terminó y no enviará más visitantes, y que el siguiente terminó y no recibirá más visitantes.



    - - -



4.  > \- Y finalmente los resultados de los tiempos deben estar expresados en microsegundos?..es que vi que pusiste que usaramos clock_gettime, yo ya lo habia hecho con gettimeofday. ¿Lo puedo dejar con gettimeofday o lo cambio a clock_gettime y transformo a ms??

    La profesora Yudith envió una comunicación hace varias semanas indicando una modificación al enunciado: los tiempos serán en segundos, en vez de en microsegundos que es lo que se intuía del enunciado (aunque no estaba especificado).  Copio acá su contenido:

    >   Una aclaratoria para el Proyecto

    >   de Yudith C. Cardinale V. - martes, 7 de febrero de 2012, 18:46

    >   Estimados estudiantes

    >   Para facilitarles el manejo de los tiempos, consideren que el parámetro que corresponde al tiempo de simulación de cada atracción, estará dado en segundos y no en ms como se intuye en el ejemplo de prueba del enunciado.

    >   Así cada trabajador dormirá por el tiempo indicado, si usan la llamada al sistema sleep:

    >       #include <unistd.h>
    >       unsigned int sleep(unsigned int seconds);

    >   Saludos

    >   -yudith

    No es inválido que usen `gettimeofday`; mi sugerencia es para aumentar la calidad y portabilidad de su proyecto, pero no es un requerimiento del enunciado que su programa se ajuste a las recomendaciones de las últimas versiones de POSIX.  El requerimiento, si mal no recuerdo, es que funcionen en el LDC.  En cualquier caso, POSIX.1‐2008 especifica a `gettimeofday`, pero indica que las aplicaciones *no deberían* usarla, no que *no deben* usarla.  La distinción es sutil: POSIX, al igual que muchísimos otros documentos de estandarización (como RFCs, especificaciones de lenguajes de programación, de protocolos de comunicación, de formatos de archivos, etc) utilizan los términos “should” y “should not”, entre otros, para especificar *recomendaciones normativas* cuyo cumplimiento no es un requerimiento para ser conforme a la especificación, mientras que usan los términos “must”, “shall”, “must not” y “shall not”, entre otros, para especificar *requerimientos normativos*, que sí constituyen requerimientos para ser conforme a la especificación.  Hay un documento que especifica una de las formas comunes de estas convenciones de documentación y muchos otros se basan en él: [RFC 2119](https://www.ietf.org/rfc/rfc2119).  POSIX no hace referencia a este documento, pero en su sección [A.1.5](http://pubs.opengroup.org/onlinepubs/9699919799/xrat/V4_xbd_chap01.html#tag_21_01_05) define reglas muy similares.

    En cualquier caso, el uso de `gettimeofday` es muy similar al de `clock_gettime` (y eso no es sorprendente: `clock_gettime` fue diseñada específicamente para sustituir a `gettimeofday`, que tenía deficiencias en su diseño relacionadas con el manejo de zonas horarias).  Lo importante de este asunto es que manejen el tiempo de cada atracción como una cantidad de **segundos**.
