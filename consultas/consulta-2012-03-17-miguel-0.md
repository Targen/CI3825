@@.pdf@.tex@@\fvset{frame=leftline,xleftmargin=6em}

>   Buenas noches Manuel, disculpa la molestia, te escribo para ver como puedo mover el offsett de un descriptor que fue abierto con open

Si están usando las primitivas de bajo nivel para la manipulación de archivos (`open`, `read`, `write`, `fcntl`, etc), usarían `lseek`.

Si están usando las funciones de la biblioteca estándar de C para manipulación de archivos (`fopen`, `fprintf`, `fscanf`, `feof`, etc), usarían `fseek`.

>   ya que para el proyecto es necesario colocar al principio en el makefile algunas cosas

Los ejemplos publicados junto con el enunciado, tanto en su sección introductoria como en el paquete de código de ejemplo distribuido con el documento de especificación del proyecto, siguen en efecto un cierto formato en sus Makefiles que se deriva de razones pedagógicas y de consistencia.

Sin embargo, el enunciado del proyecto no establece requerimientos sobre el código de los Makefiles generados más allá de que deben funcionar para el fin especificado y contener las directivas que sean necesarias para funcionar con el mecanismo de recursión y de archivos de colección por directorio.

En particular, el enunciado no especifica que las reglas deban ocurrir en los Makefiles generados en algún orden específico.

>   pero que dependen de unas que se hicieron mas abajo entonces luego de escribir en el archivo debo modificar algunas cosas pero debo hacerlo en el comienzo del archivo, como puedo hacer esto ? Es que solo consegui informacion sobre como mover el offset para descriptores del tipo FILE.

Si quieren generar los Makefiles con el mismo formato de los ejemplos (que, insisto, no es necesario según los requerimientos formales del enunciado), se me ocurren varias estrategias:

*   Determinen las dependencias de cada archivo generándolas todas en un solo archivo temporal que podrían crear con alguna de las funciones de la familia de mkstemp (pero que sean de las seguras, con “s”).  Luego creen el Makefile, escriben la primera parte, y copian el contenido del archivo temporal al final del Makefile.  Idealmente deberían borrar el archivo temporal al terminar de usarlo.  La creación de archivos temporales trae ciertas complicaciones que no son inmediatamente evidentes, así que no recomiendo muy fuertemente esta técnica.

*   Apliquen la misma técnica anterior pero guardando las dependencias generadas de cada archivo en archivos separados creados por el compilador (idealmente, los “.d”).  Esto es más sencillo porque no es necesario crear un archivo temporal, pero deberán leer muchos archivos en vez de uno solo.  Esta técnica es la que tenía en mente al redactar el documento de especificación del proyecto, pero en efecto es más compleja y que la que ustedes intentan aplicar, que no es menos válida.

*   Hagan varias pasadas sobre la estructura de directorios: por ejemplo, la primera para determinar qué archivos y subdirectorios existen (y así generar la primera parte del Makefile), y otra para generar las dependencias al final del Makefile.  Los criterios de corrección oficiales de las asignaciones anteriores del laboratorio han incluido que se haga una sola pasada por los datos, así que no recomiendo esta técnica.

*   No generen las dependencias a archivos (sea por archivos temporales explícitos o por la salida estándar redirigida a un archivo), sino por la salida estándar redirigida a un pipe que va a cada proceso de rautomake.  Esta técnica es sin duda alguna la más eficiente (ya que no depende de manipulación de archivos entre varios procesos, que es ineficiente por todo lo que saben de la teoría), pero la comunicación a través de pipes podría resultar incómoda, sobre todo porque no habría garantía alguna de atomicidad de las operaciones de lectura y escritura.

*    Generen el archivo con el orden indeseado, y luego léanlo a memoria e imprímanlo en el orden que desean.  Esto implica escribir código para reconocer hasta cierto punto el formato del archivo generado, así que podría ser complejo, y además es ineficiente tanto en tiempo como en memoria.  No recomiendo esta técnica.

Cualquiera de estas técnicas sirve para hacer lo que quieren hacer con varios grados de eficiencia, dificultad y robustez, pero insisto otra vez: aunque sería bonito porque los Makefiles generados serían más legibles, el enunciado NO requiere orden alguno entre las reglas del Makefile generado siempre que funcione como debe.

La única regla de Make en relación al orden que creo que les puede interesar es que la primera regla en aparecer es la que se activa por defecto.  Es posible ponerla sin dependencias al principio y especificar sus dependencias luego.  Por ejemplo, este Makefile compila la primera tarea si su código está todo en reportlog.c:

~~~{.Makefile .numberLines}
all:
reportlog: reportlog.c
all: reportlog
~~~

Omití las recetas porque las reglas implícitas de GNU Make hacen lo que deben hacer en este caso.
