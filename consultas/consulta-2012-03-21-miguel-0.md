>   Si yo coloco
>
>       .PHONY: all force
>
>   al principio de todos los makefiles esta mal? Osea que siempre coloco el force sin distincion... ES correcto de sta manera ?

No hay ningún problema siempre que eso no entre en conflicto con alguna otra regla que se utilice.  Sería posible, claro, que el directorio principal del proyecto o alguno de sus subdirectorios se llamaran precisamente como alguno de esos dos *targets* y eso podría producir algún comportamiento indeseado.  No se me ocurre un caso que produzca un comportamiento incompatible con el enunciado, pero podría suceder.

Esos dos nombres de *targets* son, por supuesto, totalmente arbitrarios.  Podrían generar nombres distintos si determinan que en algún caso particular eso pueda producir algún problema.  Lo que hace que esas reglas hagan lo que hacen es la forma en que se usan:

*   `all` es el nombre que típicamente se le da a la primera regla normal que ocurre en un Makefile, que es la que se requiere implícitamente cuando `make` se ejecuta sin parámetros que indiquen *targets* (que es como se hacen las invocaciones recursivas en los ejemplos del documento del enunciado).  Hasta podrían hacer que las invocaciones recursivas usen una regla explícitamente y no importaría el orden en absoluto, ni siquiera para la primera regla.

*   `force` es como se me ocurrió[^1] nombrar a una regla que fuerza la evaluación de las llamadas recursivas a `make` para subdirectorios.  El enunciado ni siquiera requiere que usen esa técnica exacta para hacer las llamadas recursivas.

Es válido que intenten reproducir el formato exacto de los ejemplos del documento del enunciado, pero no es de ninguna manera necesario: los requerimientos son los que se especifican en el texto del enunciado, y los ejemplos son solo informativos.

[^1]: O como se le ocurrió al que escribió el artículo del que saqué la idea; ya ni recuerdo.
