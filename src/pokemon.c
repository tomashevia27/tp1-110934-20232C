#include <stdlib.h>
#include "pokemon.h"
#include "tipo.h"
#include <stdio.h>
#include "ataque.h"
#include <string.h>
#include <stdbool.h>

#define MAX_ATAQUES 3
#define MAX_CARACTERES 255

struct pokemon {
	char nombre[20];
	enum TIPO tipo_pokemon;
	struct ataque ataques[MAX_ATAQUES];
};

struct info_pokemon {
	pokemon_t **pokemones;
	int cantidad_pokemones;
};

void pokemon_destruir_todo(informacion_pokemon_t *ip)
{
	if (ip != NULL) {
		for (int i = 0; i < ip->cantidad_pokemones; i++) {
			free(ip->pokemones[i]);
		}
		free(ip->pokemones);
		free(ip);
	}
}

int leer_lineas(FILE *archivo, char linea_pokemon[MAX_CARACTERES],
		char lineas_ataques[MAX_ATAQUES][MAX_CARACTERES])
{
	return fscanf(archivo, "%s\n%s\n%s\n%s\n", linea_pokemon,
		      lineas_ataques[0], lineas_ataques[1], lineas_ataques[2]);
}

bool validar_tipo(char tipo)
{
	return tipo == 'N' || tipo == 'F' || tipo == 'A' || tipo == 'P' ||
	       tipo == 'E' || tipo == 'R';
}

bool tipos_validos(char tipo_pokemon, char tipos_ataques[MAX_ATAQUES])
{
	return validar_tipo(tipo_pokemon) && validar_tipo(tipos_ataques[0]) &&
	       validar_tipo(tipos_ataques[1]) && validar_tipo(tipos_ataques[2]);
}

enum TIPO char_a_enum_tipo(char caracter_tipo)
{
	enum TIPO tipo_enum = NORMAL;
	switch (caracter_tipo) {
	case 'F':
		tipo_enum = FUEGO;
		break;
	case 'A':
		tipo_enum = AGUA;
		break;
	case 'P':
		tipo_enum = PLANTA;
		break;
	case 'E':
		tipo_enum = ELECTRICO;
		break;
	case 'R':
		tipo_enum = ROCA;
		break;
	}
	return tipo_enum;
}

void asignar_tipos(pokemon_t *nuevo_pokemon, char tipo_pokemon,
		   char tipos_ataques[MAX_ATAQUES])
{
	nuevo_pokemon->tipo_pokemon = char_a_enum_tipo(tipo_pokemon);
	for (int i = 0; i < MAX_ATAQUES; i++) {
		nuevo_pokemon->ataques[i].tipo =
			char_a_enum_tipo(tipos_ataques[i]);
	}
}

bool asignar_y_validar_datos(char linea_pokemon[MAX_CARACTERES],
			     char lineas_ataques[MAX_ATAQUES][MAX_CARACTERES],
			     int *datos_leidos, pokemon_t *nuevo_pokemon,
			     char *tipo_pokemon,
			     char tipos_ataques[MAX_ATAQUES])
{
	*datos_leidos += sscanf(linea_pokemon, "%[^;];%c\n",
				nuevo_pokemon->nombre, tipo_pokemon);
	for (int i = 0; i < MAX_ATAQUES; i++) {
		*datos_leidos += sscanf(lineas_ataques[i], "%[^;];%c;%u\n",
					nuevo_pokemon->ataques[i].nombre,
					&tipos_ataques[i],
					&(nuevo_pokemon->ataques[i].poder));
	}

	bool datos_validos = false;
	if (*datos_leidos == 11 &&
	    tipos_validos(*tipo_pokemon, tipos_ataques)) {
		asignar_tipos(nuevo_pokemon, *tipo_pokemon, tipos_ataques);
		datos_validos = true;
	}

	return datos_validos;
}

pokemon_t *crear_nuevo_pokemon(char linea_pokemon[MAX_CARACTERES],
			       char lineas_ataques[MAX_ATAQUES][MAX_CARACTERES])
{
	pokemon_t *nuevo_pokemon = malloc(sizeof(pokemon_t));
	if (nuevo_pokemon == NULL) {
		printf("error al asignar memoria con malloc para nuevo_pokemon");
		return NULL;
	}
	char tipo_pokemon;
	char tipos_ataques[MAX_ATAQUES];
	int datos_leidos = 0;
	if (asignar_y_validar_datos(linea_pokemon, lineas_ataques,
				    &datos_leidos, nuevo_pokemon, &tipo_pokemon,
				    tipos_ataques)) {
		return nuevo_pokemon;
	} else {
		free(nuevo_pokemon);
		return NULL;
	}
}

void asignar_pokemon_a_info_pokemon(informacion_pokemon_t *info_pokemon,
				    pokemon_t *nuevo_pokemon,
				    bool *seguir_leyendo)
{
	(info_pokemon->cantidad_pokemones)++;
	pokemon_t **puntero_auxiliar =
		realloc(info_pokemon->pokemones,
			(long unsigned int)(info_pokemon->cantidad_pokemones) *
				sizeof(pokemon_t));
	if (puntero_auxiliar != NULL) {
		info_pokemon->pokemones = puntero_auxiliar;
		info_pokemon->pokemones[(info_pokemon->cantidad_pokemones) - 1] =
			calloc(1, sizeof(pokemon_t));
		*(info_pokemon
			  ->pokemones[(info_pokemon->cantidad_pokemones) - 1]) =
			*nuevo_pokemon;
	} else {
		*seguir_leyendo = false;
		printf("ocurrio una falla al usar realloc\n");
	}
}

informacion_pokemon_t *pokemon_cargar_archivo(const char *path)
{
	if (path == NULL) {
		return NULL;
	}

	FILE *archivo = fopen(path, "r");

	if (archivo == NULL) {
		return NULL;
	}

	informacion_pokemon_t *info_pokemon =
		calloc(1, sizeof(informacion_pokemon_t));
	if (info_pokemon == NULL) {
		printf("error al asignar memoria con calloc para info_pokemon");
		fclose(archivo);
		return NULL;
	}

	bool seguir_leyendo = true;

	while (seguir_leyendo) {
		char linea_pokemon[MAX_CARACTERES];
		char lineas_ataques[MAX_ATAQUES][MAX_CARACTERES];
		if (leer_lineas(archivo, linea_pokemon, lineas_ataques) == 4) {
			pokemon_t *nuevo_pokemon = crear_nuevo_pokemon(
				linea_pokemon, lineas_ataques);
			if (nuevo_pokemon != NULL) {
				asignar_pokemon_a_info_pokemon(info_pokemon,
							       nuevo_pokemon,
							       &seguir_leyendo);
				free(nuevo_pokemon);
			} else {
				seguir_leyendo = false;
			}
		} else {
			seguir_leyendo = false;
		}
	}

	fclose(archivo);

	if (info_pokemon->cantidad_pokemones >= 1) {
		return info_pokemon;
	} else {
		pokemon_destruir_todo(info_pokemon);
		return NULL;
	}
}

void recorrer_y_buscar_nombre(informacion_pokemon_t *ip, const char *nombre,
			      int *indice_pokemon_encontrado)
{
	int i = 0;
	while (*indice_pokemon_encontrado == -1 && i < ip->cantidad_pokemones) {
		if (strcmp(ip->pokemones[i]->nombre, nombre) == 0) {
			*indice_pokemon_encontrado = i;
		}
		i++;
	}
}

pokemon_t *pokemon_buscar(informacion_pokemon_t *ip, const char *nombre)
{
	if (ip == NULL || nombre == NULL) {
		return NULL;
	}

	int indice_pokemon_encontrado = -1;
	recorrer_y_buscar_nombre(ip, nombre, &indice_pokemon_encontrado);

	return (indice_pokemon_encontrado != -1) ?
		       ip->pokemones[indice_pokemon_encontrado] :
		       NULL;
}

int pokemon_cantidad(informacion_pokemon_t *ip)
{
	if (ip == NULL) {
		return 0;
	}

	return ip->cantidad_pokemones;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
	if (pokemon == NULL) {
		return NULL;
	}

	return pokemon->nombre;
}

enum TIPO pokemon_tipo(pokemon_t *pokemon)
{
	if (pokemon == NULL) {
		return NORMAL;
	}

	return pokemon->tipo_pokemon;
}

void recorrer_y_buscar_ataque(pokemon_t *pokemon, const char *nombre,
			      int *indice_ataque_encontrado)
{
	int i = 0;
	while (*indice_ataque_encontrado == -1 && i < MAX_ATAQUES) {
		if (strcmp(pokemon->ataques[i].nombre, nombre) == 0) {
			*indice_ataque_encontrado = i;
		}
		i++;
	}
}

const struct ataque *pokemon_buscar_ataque(pokemon_t *pokemon,
					   const char *nombre)
{
	int indice_ataque_encontrado = -1;
	recorrer_y_buscar_ataque(pokemon, nombre, &indice_ataque_encontrado);

	return (indice_ataque_encontrado != -1) ?
		       &(pokemon->ataques[indice_ataque_encontrado]) :
		       NULL;
}

void ordenar_pokemones(informacion_pokemon_t *ip)
{
	for (int i = 0; i < ip->cantidad_pokemones; i++) {
		for (int j = 0; j < ip->cantidad_pokemones - i - 1; j++) {
			if (strcmp(ip->pokemones[j]->nombre,
				   ip->pokemones[j + 1]->nombre) > 0) {
				pokemon_t aux = *(ip->pokemones[j]);
				*(ip->pokemones[j]) = *(ip->pokemones[j + 1]);
				*(ip->pokemones[j + 1]) = aux;
			}
		}
	}
}

int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *),
		     void *aux)
{
	if (ip == NULL || f == NULL) {
		return 0;
	}

	ordenar_pokemones(ip);

	for (int i = 0; i < ip->cantidad_pokemones; i++) {
		f(ip->pokemones[i], aux);
	}

	return ip->cantidad_pokemones;
}

int con_cada_ataque(pokemon_t *pokemon,
		    void (*f)(const struct ataque *, void *), void *aux)
{
	if (pokemon == NULL || f == NULL) {
		return 0;
	}

	int i = 0;
	while (i < MAX_ATAQUES) {
		f(&(pokemon->ataques[i]), aux);
		i++;
	}

	return i;
}
