> Con respecto a la verificacion de permisos en los archivos regulares y directorios... se verifica q los permisos los tengan los tres tipos de usuarios? es decir OWNER, GROUP, OTHERS o alguno de estos en especifico?

La idea es verificar si el proceso tiene permiso de lectura sobre los `.c` y de lectura, escritura y búsqueda sobre los directorios a visitar.  Los detalles del funcionamiento de los permisos de procesos sobre la jerarquía de sistemas de archivos en Linux están bien descritos en las páginas `credentials` y `path_resolution` del manual del programador de Linux.  Les recomiendo que las lean: les darán una visión general y precisa de los mecanismos involucrados en el proceso sobre el que preguntan.

Específicamente, la idea sería verificar si el usuario efectivo del proceso tiene los permisos requeridos; si no, verificar si el grupo efectivo o alguno de los grupos retornados por `getgroups` tiene los permisos requeridos; si no, verificar si el archivo o directorio otorga los permisos requeridos universalmente.

Esta verificación es precisamente la que implementan las llamadas al sistema de apertura de archivos; si la verificación fracasa, típicamente producen un código de error como valor retornado o en la variable global `errno`; el código más común es `EACCES`.

El enunciado no especifica si el chequeo debe hacerse explícitamente o si puede hacerse con la verificación de errores implícita en las llamadas al sistema.

Hacer explícita la verificación es un buen ejercicio que les enseñaría a trabajar con permisos y les permitiría producir mensajes de error detallados (y eso mejora la calidad de la implementación), pero recuerden que la verificación que las llamadas al sistema implementan ya está hecha, es fácil de usar (porque se reduce a verificar valores de retorno y de `errno`) y es presumible que está libre de errores.  En cualquier caso, tienen que hacer la verificación de todos los otros errores de las llamadas al sistema relevantes, pero algunos de esos errores no deberían causar la terminación de `rautomake`.

Como siempre, la decisión que tomen debería estar orientada a aumentar la calidad y reducir el esfuerzo.

- - -

> PS: a la final movieron la fecha del proyecto al jueves?

Las profesoras a cargo del curso decidieron que si reciben una carta solicitando la prórroga con las firmas de más de la mitad de los inscritos, harán que la fecha de entrega sea 16 horas más tarde de la que dice en el enunciado: quedaría para el mediodía del miércoles 2012‒03‒28 en la hora legal de Venezuela.  El anuncio oficial está en el foro del sistema Moodle.
