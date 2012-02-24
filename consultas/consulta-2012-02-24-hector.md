1.  >   Para el proyecto yo utilizo lo siguiente para esperar los hilos que creo:

    >       pthread_join(trabajadores[i],(void **)&(tiempoCorrida)),

    >   donde: `trabajadores` es un arreglo --> `pthread_t trabajadores[numAtrac];` y `tiempoCorrida` un apuntador a entero --> `int *tiempoCorrida;`

    Para no caer en comportamiento indefinido, asegúrense de que ese apuntador apunte a un entero en la memoria cuyo tiempo de vida no termine antes de que ocurra el `pthread_join`; es decir que deberían haber hecho algo como `tiempoCorrida = malloc(sizeof(int));` o `tiempoCorrida = calloc(1, sizeof(int));` dentro del hilo (y no olviden hacer el correspondiente `free` en el hilo principal cuando ya hayan usado ese valor), o quizás algo como `tiempoCorrida = algo;` donde `algo` es la dirección de un objeto local de otra función o un objeto global, y que ese objeto siga existiendo cuando ejecutan `pthread_join`.



- - -



2.  >   Pero sucede que cuando se ejecuta esa instruccion `(pthread_join(trabajadores[i],(void **)&(tiempoCorrida)) )` se guinda el programa y no termina

    >   ¿A qué se debe esto? ¿Que puedo estar haciendo mal?

    >   Cuando coloco esta instruccion en comentarios `//`, el main termina.

    >   En la funcion que mando a ejecutar con `pthread_create(&trabajadores[i], NULL, (void *)iterar, (void*)args )` (`iterar`) hago un `pthread_exit(tiempoCorrida);`, donde igualmente `tiempoCorrida` es un apuntador a entero.

    En principio, `pthread_join` solo retorna cuando ese hijo sale, ya sea porque su función principal retornó, o porque en alguna parte de su ejecución llamó a `pthread_exit`.  Si no tienen comportamiento indefinido y `pthread_join` no retorna, es porque el hilo con el que pretenden sincronizarse sencillamente no está saliendo.

    Sospecho que el ciclo en el que hacen `sleep` debe estar iterando infinitamente, o quizás le están pasando un argumento a `sleep` que es más grande de lo que ustedes creen y debería ser; ¿están manejando los tiempos indicados en la entrada en segundos como indicó Yudith hace unas semanas por e‐mail?  ¿Están pasando en la entrada tiempos como 1000 y 10000, como los que estaban en los ejemplos del enunciado, e interpretándolos como segundos?  En ese último caso, deberían cambiar esas entrada a 1 y 10.

    Podrían usar `gdb` para poner un *breakpoint* en el punto de su programa donde deberían llamar a `pthread_exit`, o simplemente imprimir algo ahí para verificar si se llama o no.  Lo más probable es que nunca se esté ejecutando.
