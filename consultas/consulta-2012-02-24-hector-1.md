1.  > Ahora tengo otro problema: Yo deseo pasar por parametros a la funcion que llama a ejecutar el  `pthread_create`  3 datos, para ello declaro un vector de enteros de tamaño 3 --> `int args[3];`

    Bien, pero hay dos detalles: uno tonto y uno importante.

    El tonto es que eso no es un vector sino un arreglo.  Cuando trabajen con C++ (en alguna otra materia, claro) seguramente trabajarán con una plantilla de clases definida en la biblioteca estándar de C++ (ISO/IEC 14882:2011 §23.3.6 [vector]) llamada `vector` que esencialmente produce arreglos que ajustan su capacidad automáticamente cuando les es necesario.  Por otra parte, desde el punto de vista matemático, un vector es un elemento de un álgebra con ciertas propiedades; los arreglos de C no tienen esas propiedades.  Eso es un arreglo, no un vector.

    El detalle importante es que eso es exactamente *un* arreglo.  Cuando escriben esa declaración, el compilador de C sabrá que debe reservar en la memoria espacio para exactamente tres enteros.  Si pretenden que haya tres enteros *por hilo* en vez de tres enteros *en total*, tendrán que hacer algo ligeramente diferente.



    - - -



2.  > y al crear el hilo utilizo: `pthread_create(&trabajadores[i], NULL, iterar, (void *)args )`.

    Eso en principio no está mal siempre que `args` sea un apuntador a un objeto de cualquier tipo cuyo tiempo de vida sea al menos hasta que todos los hilos que tengan un apuntador a él hayan terminado de usarlo, y que si varios hilos lo usan, se asegure la exclusión mutua cuando hace falta.

    Si los apuntadores que pasan a los hilos apuntan a objetos reservados en el *heap* (o a un lugar en la pila que sepan que no será desempilado hasta que esos hilos hayan terminado), y si los apuntadores pasados a cada hilo son a objetos *distintos*, y si esos objetos solo son usados por un hilo a la vez, no tendrán problemas.

    En cambio, si los apuntadores son todos a un mismo objeto, tendrán problemas.



    - - -



3.  > Luego cuando voy a recuperar los datos en iterar (la funcion que ejecuta el hilo) los recupero de la siguiente manera:

    >     int atraccion = *((int *)(datos));
    >     int capacidad = *((int *)(datos) + 1);
    >     int tiempoUso = *((int *)(datos) + 2);

    > donde la firma de la funcion es la siguiente: `void *iterar(void *datos)`

    Perfecto.

    También sirve así, que es exactamente equivalente pero resulta bastante más obvio:

        #define DATOS ((int *)datos)
        int atraccion = DATOS[0];
        int capacidad = DATOS[1];
        int tiempoUso = DATOS[2];

    O igual de obvio pero con menos memoria, así:

        #define DATOS ((int *)datos)
        #define atraccion (DATOS[0])
        #define capacidad (DATOS[1])
        #define tiempoUso (DATOS[2])

    Aunque ya esto es ligeramente distinto, porque ya no son variables locales con copias de los valores en el arreglo a cuyo inicio apunta lo que fue pasado como parámetro, sino que se usa directamente el arreglo apuntado.  Si todo está bien, no habría diferencia: igual el arreglo apuntado por el parámetro de cada hilo debería ser totalmente independiente del usado con los demás hilos.  Y si no es así, tendrán problemas.

    También está el problema de que esos símbolos tomarían ese significado en todo el archivo (más precisamente, en toda la unidad de traducción, que incluye a todos los archivos incluidos), así que si usan esos nombres en cualquier otra parte del código, pasarían cosas malas.  Todo depende de cuánto les agrade usar macros.

    En cualquier caso, la forma que usaron está bien.



    - - -



4.  > pero resulta que la funcion siempre recibe los ultimos datos enviados en el `pthread_create`, NO toma en cuenta los primeros, parece que los perdiera.

    Porque seguramente no están creando un objeto independiente para pasar los datos a cada hilo, sino que en cada iteración del ciclo de creación de hilos están *sobreescribiendo* los datos pasados al anterior.  Recuerden que la función recibe un *apuntador*; es decir, el pasaje de parámetros es por *referencia*, no por valor.  Los valores en su arreglo **no** se copian al momento de llamar a `pthread_create`.



    - - -



5.  > He verificado que los datos son enviados distintos (con un `printf`) antes del `pthread_create`. ¿Qué puede estar sucediendo?

    Que no hay sincronización entre el momento en el que los datos del hilo son sobreescritos y el momento en el que el hijo copia esos datos a sus variables locales.  La mejor manera de resolver el problema es tener objetos separados para el pasaje de parámetros a cada hilo; otra manera bastante más complicada y que no ahorra casi nada es usar primitivas de sincronización para sincronizar esos dos eventos, pero realmente no vale la pena hacerlo así por ahorrarse tres enteros por hilo.
