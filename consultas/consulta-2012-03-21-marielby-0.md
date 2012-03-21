@@.pdf@.tex@@\fvset{frame=leftline,xleftmargin=6em}

> - Te escribo para comentarte una duda que tengo.. Al usar la opcion de gcc -MMD, sabes que se crea un archivo .d, no hay problema con que esos archivos queden dentro del proyecto de software, en el ejemplo dentro de las carpertas de multidoc??

El documento del enunciado del proyecto especifica esto en §2.2.b:

>   rautomake podrá suponer que no existe ningún archivo dentro del directorio donde es ejecutado, ni dentro de ninguno de sus subdirectorios, cuyo nombre termine en “.d”. Rautomake no será ejecutado en un directorio que tenga algún subdirectorio directo o indirecto cuyo nombre termine en “.d”.

El propósito de ese texto es asegurarles que cualquier archivo con un nombre así puede ser descartado; es decir, que no es parte del proyecto que debe poder compilarse con sus Makefiles generados, y no es necesario para su funcionamiento, porque en principio no existían; si ya existen, están en libertad de sobreescribirlos, borrarlos o lo que sea.  Así se les permite usar los archivos .d para obtener las dependencias.  Hay otros métodos, pero ese es uno que es perfectamente válido usar (de hecho es el que a mí se me había ocurrido, pero varios han descubierto soluciones alternativas también válidas, y en algunos casos un poco mejores y más simples).

El enunciado no establece requerimientos sobre los archivos que su proyecto genere aparte de que se deben crear Makefiles en ciertas condiciones, y que el proyecto sobre el que corre debe poder compilarse.

Hubiera sido bueno especificar que no puede modificar ningún archivo que el proyecto tuviera originalmente (con la excepción especial de los .d, porque con esos pueden hacer lo que quieran), pero se me pasó.  Pero claro, si borran el proyecto entonces no podrían al final compilarlo con sus Makefiles.

- - -

> - Es necesario que el PHONY este al principio del makefile para que este funcione??

El *target* especial `.PHONY` de GNU Make hace que la evaluación de las recetas de sus prerequisitos se efectúe siempre que ocurra en una regla que se procesa sin que se intente ver si hay un archivo que se llama como ese *target* para verificar si está al día.

La idea es más o menos esta: en un `Makefile` se suele incluir, por ejemplo, un *target* llamado `clean` cuya receta elimina todos los archivos del proyecto que no sean parte de su código fuente.  El detalle está en que no se pretende considerar que pueda haber un archivo cuyo **nombre** sea `clean`, en contraste con las reglas usuales para compilar.  Por ejemplo, en la regla

~~~{.Makefile .numberLines}
parser.o: parser.c
	$(CC) $(CFLAGS) -c $<
~~~

se le indica a Make que si el archivo llamado `parser.o` es más viejo que el archivo `parser.c`, o si `parser.o` no existe, entonces se debe (re)generar ejecutando la receta.  El cambio, en el caso de la regla

~~~{.Makefile .numberLines}
clean:
	rm -f ./*.o multidoc
~~~

no se desea indicarle a Make que verifique si hay un archivo llamado `clean` que quizás deba actualizarse ejecutando la receta, sino que se desea ejecutar el comando incondicionalmente.  Para eso es que se usa el *target* especial `.PHONY`.

Como se desea hacer la entrada a los subdirectorios incondicionalmente (porque solo ellos sabrán si algo debe regenerarse, porque solo ellos conocen las dependencias de los archivos que están en ellos), es necesario que las ejecuciones recursivas se ejecuten incondicionalmente (aunque en el enunciado final eliminaron lo que había escrito que requería eso, pero seguramente será evaluado; ya saben cómo es esto…).

Aunque `.PHONY` es un *target* especial que tiene una semántica distinta de la de los *target*s usuales, sus reglas, al igual que todas las demás reglas, pueden ocurrir en el Makefile en cualquier orden.

- - -

> -El orden del resto de las reglas que se colocan en el makefile es irrelevante mientras esten correctas?

En efecto.  Make lee el `Makefile` completo y **luego** hace el análisis sobre el grafo de dependencias resultante, así que el orden no importa si el Makefile es correcto en algún orden.
