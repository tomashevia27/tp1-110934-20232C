<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP1

## Repositorio de Tomás Hevia - 110934 - thevia@fi.uba.ar

- Para compilar:

```bash
gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g src/*.c pruebas_chanutron.o -o pruebas_chanutron
```

- Para ejecutar:

```bash
./pruebas_chanutron
```

- Para ejecutar con valgrind:
```bash
make valgrind-chanutron
```
---

##  Funcionamiento

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

esta funcion lo que hace es leer el archivo y devuelve un puntero a informacion_pokemon_t donde guarda los datos.

primero se chequea que path no sea nulo y luego se abre el archivo (chequeando que no sea null).
se reserva en memoria espacio para informacion_pokemon_t info_pokemon y para pokemon_t pokemon_leido
luego se entra en un bucle while, donde mediante la funcion leer_pokemon se lee el archivo y se le asignan los datos a pokemon_leido, si la funcion devuelve null, se sale del bucle. luego se asigna el pokemon_leido en el vector info_pokemon->pokemones, se usa realloc para ir agrandando el vector pokemones a medida que se agreguen nuevos pokemones, se usa un puntero auxiliar para esto.
si el realloc falla, se cierra el archivo, se libera pokemon_leido y se sale del bucle.

la funcion leer_pokemon utiliza fscanf para leer el nombre y tipo del pokemon, y los tres ataques (nombre;tipo;poder) validando que el archivo tenga el formato correcto y devuelva datos validos (como el tipo por ejemplo), si falla en algun momento (formato del archivo invalio, fin del archivo, tipos incorrectos) la funcion devuelve null y libera el espacio reservado para pokemon_leido, si todo sale bien devuelve un puntero a pokemon_t pokemon_leido

luego del while se chequea que haya al menos un pokemon guardado en info_pokemon, si no lo hay se libera info_pokemon y devuelve null. si hay al menos un pokemon se devuelve un puntero a info_pokemon.


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


## Respuestas a las preguntas teóricas

para ordenar los pokemones alfabeticamente, uso el metodo de ordenamiento Burbujeo.
lo que hace este algoritmo es ir comparando los nombres de los pokemones de a pares "empujando" hacia el fondo del vector al pokemon con nombre "mas grande" alfabeticamente. por ejemplo si compara pokemones[0].nombre = "Pikachu" y pokemones[1].nombre = "Charmander" lo que hace es intercambiar de posicion a estos pokemones, luego continua comparando a Pikachu con el pokemon en la posicion 2, luego la 3 y asi continua hasta el final.
cuando termina de comparar, arranca otra vez desde el inicio, esta vez llega a comparar hasta el anteultimo pokemon.
luego arranca devuelta y llega hasta el anteultimo.
asi itera hasta terminar comparando todos los pokemones y quedan ordenados.
El costo computacional es de n².


DIAGRAMA DE MEMORIA:

<div align="center">
<img width="70%" src="img/diagrama-tp1.svg">
</div>