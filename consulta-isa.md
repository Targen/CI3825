> Sabes que en el proyecto de S.O. creemos que terminamos las dos versiones.
> Para la versión de procesos obviamente es mas fácil verificar el resultado
> que para la parte de hilos (que parece estar haciéndolo bien), sin embargo
> me pregunto si estamos utilizando bien el mutex ya que nos costo un poco
> decidir en que parte lo usaríamos.

Como regla general, todo acceso a una variable compartida entre hilos debe hacerse habiendo reservado a su `mutex` asociado.  Hay algunas excepciones muy particulares: si un hilo únicamente desea *leer* de una variable compartida y no está particularmente interesado en que esa lectura esté sincronizada con respecto a una posible escritura en proceso por parte de otro hilo, podría hacerse la lectura sin reservar el `mutex` asociado a la variable compartida.

De esto no interpreten que “si solo voy a leer de la variable, no hace falta reservar su `mutex`”.  En cambio, significa “si no me importa mucho que esto funcione de forma predecible Y solo voy a leer, entonces no hace falta reservar su `mutex`”.  Por ejemplo, si lo que están haciendo es imprimir un mensaje de diagnóstico con el valor de la variable compartida, y lo quieren hacer rápidamente como ayuda al desarrollo, podrían dejar de reservar el `mutex`.  Si están haciendo cualquier cuenta con el valor de la variable que involucre al control del flujo de ejecución de sus hilos, es *probable* que sea importante reservar el `mutex` aunque solo hagan una lectura.

El criterio exacto depende mucho de cada situación particular.  Lo que sí es universalmente cierto es que si van a *escribir* a una variable compartida, *deben* haber reservado su `mutex`.



- - -



> Tenemos un arreglo de un struc que contiene en cada casilla todos los datos
> que una atracción necesita para funcionar (capacidad, tiempo, cola) y un
> dato llamado sig que es donde el coloca luego de hacer sleep() el numero de
> personas que se "bajaron" de la atracción e irán a la cola siguiente. 

Correcto: esas personas van a la atracción siguiente únicamente después de bajarse de la actual.  Apenas termine el `sleep` de una atracción, debe comunicarse a la siguiente que el número de personas que se acaban de bajar ahora estará encolado en la que le sigue.



- - -



> Cada hilo lee antes de empezar del ".sig" que corresponde a la casilla del
> hilo anterior a el (en el caso del primero, este lee del ultimo hilo) y
> añade ese numero a su cola, luego procede a hacer sleep. Luego cada hilo
> debe "borrar lo que leyo" pero para "borrarlo" debe accesar al .sig del otro
> hilo, por lo que esa parte la hemos colocado dentro del mutex  así:

Como el `.sig` se usa esencialmente como una variable de comunicación entre un productor y un consumidor, lo ideal sería que el consumidor “borre lo que leyó” inmediatamente después de leerlo.  En este caso no es inválido “borrarlo” después del `sleep` porque solo hay un consumidor asociado a ese productor; es decir que los visitantes salientes de una atracción siempre van a una única atracción particular que es fija para la atracción de la que salen.  En cualquier caso es preferible que la lectura a esa variable compartida suceda al mismo tiempo que la escritura, porque así se aseguran de que hay sincronización absoluta con una sola reserva del `mutex`, y el valor de la variable de comunicación tiene un significado uniforme fuera de una región crítica: siempre representa al número de personas que salieron de una atracción y aun no están en la atracción siguiente (ni en su cola, ni montados).

Como lo tienen ahorita, las cosas no son tan claras: es posible que ese número represente “esta cantidad de gente salió de una atracción a la siguiente, y todavía no han sido incorporados a la cola ni están montados”.  Pero también puede significar “esta cantidad de gente salió de una atracción, y algunos están en la cola de la siguiente, y otros ya están montados en la siguiente, y otros ya se bajaron y van a la que le sigue pero aun no se ha modificado el contador”.

Es preferible que el significado de una variable compartida no sea tan complejo, y la simplicidad se logra si hacen toda la manipulación de esa variable dentro de una sola sección crítica.  Ahorita están leyendo *fuera* de una sección crítica, y están “borrando” lo que leyeron *dentro* de una sección crítica pero en otra parte del código que se ejecuta en otro momento.  Esto *no* invalida su solución, pero hace que sea relativamente sencillo introducir cambios que sí la invaliden; mi recomendación de hacer ambas operaciones (lectura y actualización) en una sola sección crítica no es una cuestión de correctitud, sino de mantenibilidad.  Es importante que el diseño de un sistema concurrente haga todo lo posible por facilitar la deducción de información sobre su estado.



- - -



```C
void *atracciones_Parque(void *arg) {
        double timehijo;
        int n, lee, pasa, montados; 
        int i = (int)(intptr_t)arg;
```

Acá están usando un truco típicamente conocido como *doble coerción* para evitar una advertencia del compilador. Ese es un truco que funciona perfectamente con el compilador GCC, y como es el que están usando, esto no está necesariamente mal. Yo suelo recomendar fuertemente que usen extensiones del proyecto GNU a los lenguajes y otros estándares con los que estén trabajando, pero siempre bajo dos condiciones importantes:

*   Que la extensión esté bien diseñada y facilite significativamente la seguridad, la eficiencia y/o la mantenibilidad de sus programas.
*   Que la extensión esté orientada a mejorar la portabilidad de sus programas. Sí, suena contradictorio, pero muchas extensiones, a pesar de no constituir comportamiento estandarizado ni realmente portable, son el origen de nuevas adiciones a los estándares técnicos que definen las tecnologías que usan. Estos casos típicamente son estándares *de facto*, y a veces hasta están incluidos en los borradores de las nuevas versiones de los estándares relevantes.

En cualquier caso, hacer que su programa dependa de comportamiento que escape a los estándares es una decisión que deben tomar a nivel de cada proyecto usando sus criterios como programadores. No es inválido hacerlo, pero es importante tener buenas razones para dejar de seguir un lineamiento establecido en un documento de estandarización.

Estos son dos párrafos relevantes del borrador más cercano que tengo (N1570, publicado el 2011‒04‒12) del documento final publicado como ISO/IEC 9899:2011, que es la versión más reciente del estándar internacional que define el lenguaje de programación C:

> ### 6.3.2.3 **Pointers**
>
> 1.  **A pointer to void may be converted to or from a pointer to any object type. A pointer to any object type may be converted to a pointer to void and back again; the result shall compare equal to the original pointer.**
> 2.  For any qualifier `q`, a pointer to a non‐`q`‐qualified type may be converted to a pointer to the `q`‐qualified version of the type; the values stored in the original and converted pointers shall compare equal.
> 3.  An integer constant expression with the value 0, or such an expression cast to type `void *`, is called a null pointer constant.) If a null pointer constant is converted to a pointer type, the resulting pointer, called a null pointer, is guaranteed to compare unequal to a pointer to any object or function.
> 4.  Conversion of a null pointer to another pointer type yields a null pointer of that type. Any two null pointers shall compare equal.
> 5.  **An integer may be converted to any pointer type. Except as previously specified, the result is implementation‐defined, might not be correctly aligned, might not point to an entity of the referenced type, and might be a trap representation.)**
> 6.  **Any pointer type may be converted to an integer type. Except as previously specified, the result is implementation‐defined. If the result cannot be represented in the integer type, the behavior is undefined. The result need not be in the range of values of any integer type.**
> 7.  A pointer to an object type may be converted to a pointer to a different object type. If the resulting pointer is not correctly aligned) for the referenced type, the behavior is undefined.  Otherwise, when converted back again, the result shall compare equal to the original pointer. When a pointer to an object is converted to a pointer to a character type, the result points to the lowest addressed byte of the object. Successive increments of the result, up to the size of the object, yield pointers to the remaining bytes of the object.

[El énfasis es mío.]

Además, POSIX.1‐2008 especifica esto en su descripción de `stdint.h`:

> The following type designates a signed integer type with the property that any valid pointer to void can be converted to this type, then converted back to a pointer to void, and the result will compare equal to the original pointer: intptr\_t

Imagino que cuando escribieron la llamada a `pthread_create`, el último argumento (el `void *` que se le pasa a la función del hilo como parámetro) dice algo como `(void *)i`, donde `i` es el contador del ciclo en el que crean todos los hilos. En efecto, es necesario pasar al hilo algo que le diga cuál es su índice para que pueda hacer el resto del trabajo. El problema es que el lenguaje de programación C no provee ninguna garantía de que un valor entero pueda convertirse en un valor de tipo `void *` y luego volver a convertirse en un entero, y que el valor resultante sea igual al valor original.

Cuando escriben el código `(void *)i` en la llamada a `pthread_create`, eso es una conversión de `int` a `void *`. Sea cual fuere el valor de `i`, esa conversión es normada por ISO/IEC §6.3.2.3.5, que especifica que el valor resultante es **definido por la implementación**. En particular, nada asegura que el valor de `i` pueda luego ser recuperado a partir del valor resultante de esa conversión, que podría ser cualquier cosa.

Cuando escriben el código `(int)(intptr_t)arg` en la inicialización de `i`, están haciendo dos conversiones: la primera convierte a `arg`, un `void *`, en un `intptr_t` (que es un *tipo entero*), y la segunda convierte a ese nuevo valor de tipo `intptr_t` en un valor final de tipo `int`. La primera conversión es, entonces, del tipo referido en ISO/IEC 9899:2011 §6.3.2.3.6: se está convirtiendo un valor de tipo apuntador a `void` en un valor de un tipo entero. §6.3.2.3.6 dice que el valor resultante es **definido por la implementación**, y además que si el valor no puede representarse en ese tipo entero, **el comportamiento es indefinido**.

Esto último no sucede, y POSIX lo asegura: `intptr_t` es un tipo entero con signo con la propiedad de que cualquier apuntador a `void` puede convertirse en él (y si se vuelve a convertir en un apuntador a `void`, se tiene el apuntador original). Noten que este ciclo de conversiones es el contrario al que les interesaría: la idea es convertir un entero a `void *` y luego recuperar el entero; no nos interesa convertir un `void *` a entero y recuperar el `void *`. En cualquier caso, el valor de tipo `intptr_t` que resulta sigue siendo algo definido por la implementación sin ninguna garantía de que sea el mismo entero que habíamos pasado originalmente.

Si esto funciona para ustedes, es porque la versión de GCC que están usando hace lo que ustedes quieren que haga en la plataforma en la que lo están usando. Si tienen un documento de GCC, o de algún estándar que GCC obedezca, que especifique ese comportamiento, todo está bien dentro de la isla de no‐portabilidad en la que han decidido vivir. Si quieren programar de manera portable y consistente, deberían hacer algo totalmente diferente.

El mecanismo que les mostré en clase (y que está en la documentación) para pasar datos a un hilo consiste en tener un espacio de memoria que contenga los datos que se quieren pasar, y se le pasa al hilo *un apuntador a esos datos* convertido en un apuntador a `void`. La segunda oración de ISO/IEC 9899:2011 §6.3.2.3.1 dice que un apuntador a cualquier tipo de objeto puede convertirse en un apuntador a `void`, y si el resultado de eso se convierte en un apuntador del tipo de objeto original, entonces el apuntador resultante es igual al original. Es decir que la especificación del lenguaje de programación C sí asegura que la conversión cíclica que quieren hacer es válida **si el tipo original y final es un tipo de apuntador** (pero no lo especifica si es un tipo entero, que es lo que están usando ustedes.)

Lo que deberían estar haciendo es tener en alguna parte estable de la memoria una copia para cada iteración del valor del contador del ciclo, todas por separado e inmutables, y a cada hilo le pasan el apuntador a la copia correspondiente del valor del contador en cada iteración. Es tan sencillo como declarar, antes del inicio del ciclo, `int is[n];` (donde `n` es el número de atracciones) y al entrar a cada iteración asignar el valor del contador al elemento correspondiente del arreglo: `is[i] = i;`. Luego, cuando llaman a `pthread_create`, en su último parámetro deberían pasar `&is[i]`, y la inicialización de `i` en el inicio de la función de los hilos debería ser con la expresión `*((int *)arg)`; es decir, convierte el apuntador a `void` pasado al hilo en un apuntador a `int`, que es lo que era originalmente, y retorna el entero apuntado por él.

Este es un problema sutil que en principio podría no tener absolutamente ningún efecto, pero podría hacer que su programa falle misteriosamente si actualizan su compilador, o si cambian a una máquina de 64 bits, o si corren su programa con muchos hilos. Para este proyecto particular es una tontería de error, pero si llegan a implementar un sistema concurrente de escala considerable y que deba ser portable (como el navegador Web en el que seguramente están leyendo esto, o cualquiera de los cientos de paquetes sobre los que ese navegador está corriendo, como el sistema gráfico, las bibliotecas de redes, etc), este tipo de detalle se vuelve sumamente importante.



- - -



```C
        struct timeval ti, th;
        n = 0;
        lee = 0;
        gettimeofday(&ti, NULL);   // Instante inicial
```C

La página del manual del programador de Linux referente a `gettimeofday`, incluida en la versión 3.37 del proyecto de páginas de manual de Linux, dice

> CONFORMING TO

> SVr4, 4.3BSD.  POSIX.1-2001 describes gettimeofday() but not settimeofday().  POSIX.1-2008 marks gettimeofday() as obsolete, recommending the use of clock_gettime(2) instead.

`clock_gettime` tiene un funcionamiento muy similar a `gettimeofday`, pero es la llamada recomendada por POSIX.  El uso que les interesa es algo como esto:

```C
#include <stdio.h>    // para “perror”
#include <stdlib.h>   // para “exit”
#include <sysexits.h> // para “EX_SOFTWARE”
#include <time.h>     // para “clock_gettime” y “CLOCK_REALTIME”

/* … */

struct timespec ti, th;

/* … */

if (clock_gettime(CLOCK_REALTIME, &ti) != 0) {
    perror("clock_gettime");
    exit(EX_SOFTWARE);
}
```

La estructura `timespec` es muy similar a la estructura `timeval` usada por `gettimeofday`:

```C
struct timespec {
    time_t   tv_sec;        /* seconds */
    long     tv_nsec;       /* nanoseconds */
};
```

La única diferencia es que el segundo campo es de tipo `long` en vez de `suseconds_t`, se llama `tv_nsec` en vez de `tv_usec`, y representa nanosegundos en vez de microsegundos.



- - -



```C
        while (iteraciones - n > 0) {
```

Sobre lo que deben hacer los hilos trabajadores, el enunciado dice explícitamente así:

> Los procesos/hilos trabajadores deben:

> 1.  actualizar la cola de espera. La primera vez sólo actualizarán con la información del archivo y en lo sucesivo actualizarán la cola con los usuarios que llegan de la atracción anterior en el anillo,
> 2.  en cada iteración, montar tantos usuarios como la capacidad del aparato permita y de acuerdo a cuántos están en la cola (esta simulación es simplemente una resta al número de personas en la cola, la cola puede estar vacía),
> 3.  dormir durante el "tiempo de uso de la atracción",
> 4.  al despertar, enviar los usuarios que estaban montados a la siguiente atracción en el anillo, leer cuántos usuarios llegan de la atracción anterior y encolarlos en su cola,
> 5.  los pasos 2 a 4 se repiten durante el "nro. de iteraciones". Al finalizar todas las iteraciones, la atracción se cierra, e informa al proceso/hilo principal cuántos usuarios quedaron en la cola,
> 6.  además, al finalizar, cada trabajador debe imprimir el tiempo consumido durante su simulación. 



- - -



```C
                if (n == 0) {      
                        montados = d[i].numCola - d[i].capacidad;
                        d[i].numCola = montados;
                }
                else {  
                        if (i == 0) {
                                lee = d[atracciones-1].sig;
                                d[i].numCola = d[i].numCola + lee;
                                montados = d[i].numCola - d[i].capacidad;
                                d[i].numCola = montados;
                        } else {
                                lee = d[i-1].sig;
                                d[i].numCola = d[i].numCola + lee;
                                montados = d[i].numCola - d[i].capacidad;
                                d[i].numCola = montados;
                        }
                }
```

Primero, un comentario de estilo: todo ese código se puede resumir así:

```C
                if (n != 0) d[i].numCola += d[(i == 0 ? atracciones : i) - 1].sig;
                montados = (d[i].numCola -= d[i].capacidad);
```

La mantenibilidad de dos líneas sin repetición seguramente es preferible a la de 17 con un montón de copias de lo mismo.  Si les toca hacer un cambio en esta parte del código, será de lejos preferible tener menos código que alterar.

Esta parte debería implementar los puntos 1 y 2 de la lista del enunciado que copié antes.

Acá aparece el problema que les mencioné antes: están accediendo a la variable compartida `sig` fuera de un `mutex`; la información que extraen de ella se usará para actualizar variables privadas (no compartidas) y también a la propia variable compartida (al “borrar” a los visitantes entrantes), pero esa última actualización no la están haciendo en este punto, por lo que la semántica de su variable compartida se complica.  Esto no es incorrecto en el caso particular de su código, pero sí es indeseable y peligroso para ustedes mismos: se vuelve más complejo razonar sobre el estado de su programa, que ya es suficientemente complejo por el solo hecho de involucrar concurrencia.  Lo ideal sería que acá mismo reservaran el mutex asociado a esta variable compartida particular, sumaran su valor a la cola del hilo actual, y la dejaran en cero.  Todo esto solo aplica, claro, para iteraciones que no sean la primera.

Un comentario de estilo: es importante que haya una correspondencia entre los símbolos usados para representar objetos de su programa y el significado de esos objetos.  `montados` debería representar el número de visitantes montados en la atracción en una iteración.  Si no representa eso, debería tener algún otro nombre.  La selección de símbolos (y los comentarios, y el espacio en blanco y demás) es arbitraria y es irrelevante para la semántica del programa, pero es importante para que el proceso de programación se les haga fácil a ustedes, que son los programadores.

En este caso particular, probablemente hubiera sido mejor asignar a `montados` el mínimo entre la capacidad de la atracción y la cantidad de visitantes en la cola, y luego restar ese número a la cola:

```C
#define MIN(a, b) ((a) <= (b) ? (a) : (b))

montados = MIN(d[i].capacidad, d[i].numCola);
d[i].numCola -= montados;
```

En efecto esto implementaría lo especificado en el segundo punto de la lista del enunciado.



- - -



```C
                sleep(d[i].tiempo);             
```

Esto en efecto implementa el tercer punto de la lista del enunciado que copié antes.



- - -



```C
                /* La cola tenia menos personas que capacidad*/
                if (d[i].numCola < 0) {      
                        pasa = d[i].capacidad - abs(d[i].numCola);
                        d[i].numCola = 0;
                }
```

Acá están compensando la deficiencia semántica del valor que decidieron almacenar antes: en realidad la cola nunca llega a tener una cantidad negativa de visitantes en espera (¿qué significaría “hay −3 personas en la cola”?), pero como le asignaron un número extraño a una variable que debería haber representado “la cantidad de visitantes en espera en la cola de la atracción `i`”, ahora tienen que aplicar una corrección también extraña.

Por cierto, el código `if (x < 0) { f(abs(x)); }` es exactamente equivalente a `if (x < 0) { f(-x); }`.



- - -



```C
                /* Seccion critica */
                pthread_mutex_lock( &mutex1 );
```

Acá veo que están usando un mismo `mutex` para todas las variables compartidas entre todos los hilos.  Deberían usar un `mutex` diferente para la comunicación entre cada par de hilos adyacentes.  Si usan un solo `mutex`, están restringiendo la ejecución concurrente de su programa mucho más de lo necesario, y eso desaprovecha la capacidad de concurrencia disponible en el sistema y le quita el sentido a escribir un programa concurrente; en efecto, usar un solo `mutex` hace que su programa resulte muy similar a un programa *serial*.



- - -



```C
                if (i == 0) {
                        d[atracciones-1].sig = d[atracciones-1].sig - lee;              
                } else {
                        d[i-1].sig = d[i-1].sig - lee;
                }       
                if (montados < 0) {                    
                        d[i].sig = d[i].sig + pasa;
                } else {
                        d[i].sig = d[i].sig + d[i].capacidad;   
                }
```

Acá también hay reducciones considerables que se pueden aplicar:

```C
                d[(i == 0 ? atracciones ? i)-1].sig -= lee;
                d[i].sig += (montados < 0 ? pasa : d[i].capacidad);
```

La primera de esas líneas hace lo que les sugiero que hagan no acá sino antes de hacer `sleep`; si la hacen antes, evidentemente deben hacerla en una sección crítica; es decir, habiendo reservado el `mutex` correspondiente a esa variable compartida.

La segunda de esas líneas, si se fijan bien, utiliza *otra* variable compartida.  También debe ir en una sección crítica, pero el `mutex` sería el correspondiente a `d[i].sig`, que no es el mismo `mutex` referido en el párrafo anterior.

Estas dos cosas en efecto implementan lo requerido en el cuarto elemento de la lista del enunciado.



- - -



```C
                pthread_mutex_unlock( &mutex1 );
                n = n+1;                
        } 
        /* Se imprime lo correspondiente por cada Trabajador */
        gettimeofday(&th, NULL);   // Instante final
        timehijo = (th.tv_sec - ti.tv_sec)*1000 + (th.tv_usec - ti.tv_usec)/1000.0;
        printf("Trabajador \t %d \t %g ms\n",(int)pthread_self(), timehijo);
        pthread_exit((void*)pthread_self());
}
```

Al pasar el resultado de `pthread_self()` a la llamada a `pthread_exit`, están retornando el identificador del hilo a quien haya llamado a `pthread_join` con el identificador de ese hilo… pero si llamaron a `pthread_join` con ese identificador, ya tenían ese identificador, así que ¿qué sentido tiene pasarlo como valor de retorno?

Acá también tienen una llamada a `gettimeofday` que sería preferible sustituir por `clock_gettime`.



- - -



> De esa forma creemos que se evita que dos hilos toquen el .sig de la misma
> casilla del arreglo. Le reste el "lee" porque ya es posible que el hilo
> anterior haya escrito nuevamente y así solo se resta lo que ya se monto en
> la cola.

> Sin importar si eso esta bien o mal, que espero que este bien
> :) quería confirmar que cuando corres la versión de hilos, si colocas que
> todas las atracciones duren la misma cantidad de tiempo, el resultado varia
> dependiendo del orden en que se ejecuten los hilos (ya que ellos no se
> esperan).

Lo ideal sería que el problema se hubiera especificado de una forma que hiciera que las corridas fueran independientes de cualquier condición de carrera.  Esto hubiera sido posible si estuviera especificado (o fuera compatible con la especificación y con las aclaratorias de Yudith) que los hilos deban esperar a que haya suficientes visitantes en sus colas para llenar la capacidad de sus atracciones antes de comenzar una de sus iteraciones.

El problema con esta idea es que no es trivial definir el problema de esta manera y evitar que ocurra interbloqueo, y además sería bastante más complejo programar la interacción entre los hilos: habría que programar un verdadero sistema de productores y consumidores con variables de condición para hacerlo bien.  Sería un problema adecuado para la cadena de operativos, donde se resuelven estas cosas con herramientas más avanzadas que evitan todos los problemas de intentar hacer esto con hilos y memoria compartida.

Es importante entender que la intención de la programación concurrente es que esto **no** ocurra: un programa concurrente bien diseñado **no** debería estar sujeto a condiciones de carrera.  Sin embargo, el alcance de los objetivos de este proyecto debe limitarse para que sea viable para un curso de este nivel, y uno de los sacrificios necesarios es el buen diseño de la concurrencia.  En general, cuando en su ejercicio profesional en computación requiera que diseñen o implementen un sistema concurrente, su preocupación es absolutamente válida: el resultado **no** debería variar dependiendo del orden en que se ejecuten los hilos **a menos que** ese orden sea controlado mediante primitivas de sincronización, o que ese orden no afecte el resultado del cómputo del sistema.
