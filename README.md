<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP1

## Repositorio de Tomás Hevia - 110934 - thevia@fi.uba.ar

- Para compilar:

gcc pokemon.c -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g -o pokemon

- Para ejecutar:

./pokemon

- Para ejecutar con valgrind:
make valgrind-chanutron
---
##  Funcionamiento

Explicación de cómo funcionan las estructuras desarrolladas en el TP y el funcionamiento general del mismo.

Aclarar en esta parte todas las decisiones que se tomaron al realizar el TP, cosas que no se aclaren en el enunciado, fragmentos de código que necesiten explicación extra, etc.

Incluír **EN TODOS LOS TPS** los diagramas relevantes al problema (mayormente diagramas de memoria para explicar las estructuras, pero se pueden utilizar otros diagramas si es necesario).

El codigo de pokemon.c realiza diferentes acciones con el archivo que recibe, lo primero que hace es leer el archivo y, si cumple con el formato pedido en la funcion, guarda los pokemones en la memoria del programa y en otras funciones permite realizar con estos pokemones diferentes acciones.
El archivo debe tener el siguiente formato para que la funcion pokemon_cargar_archivo logre almacenar los pokemones en memoria:
nombre;tipo
ataque1;tipo;poder
ataque2;tipo;poder
ataque3;tipo;poder
nombre;tipo
ataque1;tipo;poder
ataque2;tipo;poder
ataque3;tipo;poder

pokemon.c incluye 3 bibliotecas: ataque.h, pokemon.h y tipo.h

en pokemon.c se usan dos estructuras que son fundamentales para el funcionamiento del programa, estas son pokemon_t e informacion_pokemon_t.
pokemon_t representa a un pokemon, que tiene un nombre, un tipo y 3 ataques.
los ataques estan definidos en ataque.h y se conforman por el nombre, el tipo y el poder.
informacion_pokemon_t contiene un puntero a un vector de pokemones (los pokemones son de tipo pokemon_t) y la cantidad de pokemones. 
a informacion_pokemon_t lo usamos para guardar a los pokemones en la memoria.


Funcion pokemon_cargar_archivo:

esta funcion recibe un string que representa el nombre de un archivo, en la funcion se corrobora que el string no sea nulo y se intenta abrir el archivo, si no se abre la funcion devolvera null y si se abre continuara ejecutandose.

luego se reserva lugar en memoria dinamica para un informacion_pokemon_t que es donde se van a guardar los pokemones, esto se inicializa con calloc (todas las variables en 0)

luego usa un bucle while para ir leyendo el archivo y guardando los datos del mismo, se saldra del bucle cuando alguna linea sea invalida, tenga algun dato invalido, falle alguna asignacion de memoria o si el archivo se termina de leer.

dentro del while se leen y guardan 4 lineas del archivo, la primera de estas lineas representan el nombre y tipo del pokemon y las otras los ataques (nombre;tipo;poder)
se usa un fscanf para leer el archivo y guardar los strings.
si no se leen correctamente, se termina la iteracion, si se leen bien se sigue ejecutando.

lo siguiente que realiza la funcion es guardar en el heap un pokemon_t nuevo_pokemon usando malloc.
luego, con sscanf, se leen las lineas guardadas y los datos se asignan en nuevo_pokemon.
se validan que los datos sean de la cantidad correcta (11) y que los tipos del pokemon y los ataques sean validos, si esto no se cumple, se termina la iteracion, si se cumple continua.

luego de validar los datos, queda confirmado que hay un pokemon para asignar a info_pokemon asi que se realiza esta asignacion (reservando memoria en el heap para este pokemon que se asigna) y se aumenta en 1 la cantidad_pokemones.
la memoria para el pokemon se reserva en info_pokemon->pokemones con realloc (se usa un puntero auxiliar para el caso de que realloc falle, si falla se deja de iterar y el pokemon no se guarda en el vector) entonces el pokemon asignado queda guardado en el vector pokemones y esta en el heap.
uso realloc aca para poder ir aumentando el lugar en memoria del vector info_pokemon->pokemones a medida que se necesitan ir agregando pokemones nuevos al vector.

la funcion itera mientras se lee el archivo y asignan correctamente los pokemones en info_pokemon. cuando algo falla o el archivo termina, se corta la iteracion y lo siguiente que hace la funcion es chequear si el booleano hay_pokemon_en_archivo es verdadero o falso (lo inicializo en falso y en el momento que se asigna el primer pokemon se cambia a true).
si es true, devuelve informacion_pokemon_t *info_pokemon (contiene un vector de pokemones y la cantidad de pokemones que hay)
si es false, devuelve null y libera la memoria ocupada por info_pokemon.


despues las demas funciones tienen un funcionamiento mas simple:

pokemon_buscar: busca un pokemon segun su nombre y lo devuelve, si no lo encuentra devuelve null.

pokemon_cantidad: devuelve la cantidad de pokemones que hay en ip

pokemon_nombre: devuelve el nombre del pokemon recibido

pokemon_tipo: devuelve el tipo del pokemon recibido

pokemon_buscar_ataque: busca el ataque en el pokemon segun el nombre que recibe. si lo encuentra lo devuelve y sino devuelve null

con_cada_pokemon: ordena los pokemones y les aplica la funcion que recibe a todos ellos, devuelve la cantidad de pokemones a los que se les aplico la funcion

con_cada_ataque: le aplica la funcion f a todos los ataques del pokemon, devuelve la cantidad de ataques a los que les aplico la funcion.
todas estas funciones chequean que los datos que reciben no sean null, si lo son devuelven null, 0 o NORMAL, segun la funcion.

pokemon_destruir_todo: libera toda la memoria que se usa durante la ejecucion del programa, mediante free()





### Por ejemplo:

El programa funciona abriendo el archivo pasado como parámetro y leyendolo línea por línea. Por cada línea crea un registro e intenta agregarlo al vector. La función de lectura intenta leer todo el archivo o hasta encontrar el primer error. Devuelve un vector con todos los registros creados.

<div align="center">
<img width="70%" src="img/diagrama1.svg">
</div>

En el archivo `sarasa.c` la función `funcion1` utiliza `realloc` para agrandar la zona de memoria utilizada para conquistar el mundo. El resultado de `realloc` lo guardo en una variable auxiliar para no perder el puntero original en caso de error:

```c
int *vector = realloc(vector_original, (n+1)*sizeof(int));

if(vector == NULL)
    return -1;
vector_original = vector;
```


<div align="center">
<img width="70%" src="img/diagrama2.svg">
</div>

---

## Respuestas a las preguntas teóricas
Incluír acá las respuestas a las preguntas del enunciado (si aplica).
