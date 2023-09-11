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
	pokemon_t** pokemones;
	int cantidad_pokemones;
};

bool lineas_nulas(char linea_pokemon[MAX_CARACTERES], char lineas_ataques[MAX_ATAQUES][MAX_CARACTERES], FILE* archivo){
	return strlen(linea_pokemon) == 0 || strlen(lineas_ataques[0]) == 0 || strlen(lineas_ataques[1]) == 0 || strlen(lineas_ataques[2]) == 0;
}

void asignar_y_contar_datos(char linea_pokemon[MAX_CARACTERES], char lineas_ataques[MAX_ATAQUES][MAX_CARACTERES], int *datos_leidos, pokemon_t *nuevo_pokemon, char *tipo_pokemon, char tipos_ataques[MAX_ATAQUES]){
	*datos_leidos += sscanf(linea_pokemon, "%[^;];%c\n", nuevo_pokemon->nombre, tipo_pokemon);
	for (int i = 0; i < 3; i++){
		*datos_leidos += sscanf(lineas_ataques[i], "%[^;];%c;%u\n", nuevo_pokemon->ataques[i].nombre, &tipos_ataques[i], &(nuevo_pokemon->ataques[i].poder));
	}
}

bool validar_tipo(char tipo){
	return tipo == 'N' || tipo == 'F' || tipo == 'A' || tipo == 'P' || tipo == 'E' || tipo == 'R';
}

enum TIPO char_a_enum_tipo(char caracter_tipo){
	enum TIPO tipo_enum = NORMAL;
	switch (caracter_tipo)
	{
	case 'N':
		tipo_enum = NORMAL;
		break;
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

void asignar_tipos(pokemon_t *nuevo_pokemon, char tipo_pokemon, char tipos_ataques[MAX_ATAQUES]){
	nuevo_pokemon->tipo_pokemon = char_a_enum_tipo(tipo_pokemon);
	for (int i = 0; i < MAX_ATAQUES; i++){
		nuevo_pokemon->ataques[i].tipo = char_a_enum_tipo(tipos_ataques[i]);
	}
}

informacion_pokemon_t *pokemon_cargar_archivo(const char *path)
{
	if(path == NULL){ //esto podria ser innecesario, no lo se
		return NULL;
	}
	
	FILE* archivo = fopen(path, "r");
	
	if(archivo == NULL){
		return NULL;
	}

	informacion_pokemon_t *info_pokemon = calloc(1, sizeof(informacion_pokemon_t));

	bool seguir_leyendo = true;
	bool hay_pokemon_en_archivo = false;

	while (seguir_leyendo){
		char linea_pokemon[MAX_CARACTERES];
		char lineas_ataques[MAX_ATAQUES][MAX_CARACTERES];
		if(fscanf(archivo, "%s\n%s\n%s\n%s\n", linea_pokemon, lineas_ataques[0], lineas_ataques[1], lineas_ataques[2]) == 4){
			if(!lineas_nulas(linea_pokemon, lineas_ataques, archivo)){
				pokemon_t *nuevo_pokemon = malloc(sizeof(pokemon_t));
				char tipo_pokemon;
				char tipos_ataques[MAX_ATAQUES];
				int datos_leidos = 0;
				asignar_y_contar_datos(linea_pokemon, lineas_ataques, &datos_leidos, nuevo_pokemon, &tipo_pokemon, tipos_ataques);
				if(datos_leidos == 11 && validar_tipo(tipo_pokemon) && validar_tipo(tipos_ataques[0]) && validar_tipo(tipos_ataques[1]) && validar_tipo(tipos_ataques[2])){
					(info_pokemon->cantidad_pokemones)++;
					pokemon_t **puntero_auxiliar = realloc(info_pokemon->pokemones, (long unsigned int)(info_pokemon->cantidad_pokemones)*sizeof(pokemon_t));
					if(puntero_auxiliar != NULL){
						info_pokemon->pokemones = puntero_auxiliar;
						info_pokemon->pokemones[(info_pokemon->cantidad_pokemones)-1] = calloc(1, sizeof(pokemon_t));
						asignar_tipos(nuevo_pokemon, tipo_pokemon, tipos_ataques);
						*(info_pokemon->pokemones[(info_pokemon->cantidad_pokemones)-1]) = *nuevo_pokemon;
						if(info_pokemon->cantidad_pokemones == 1){
							hay_pokemon_en_archivo = true;
						}
					} else{
						seguir_leyendo = false;
						printf("ocurrio una falla al usar realloc\n");
					}
				} else{
					seguir_leyendo = false;
				}
				free(nuevo_pokemon);
			} else{
				seguir_leyendo = false;
			}
		} else{
			seguir_leyendo = false;
		}
	}

	if(hay_pokemon_en_archivo){
		return info_pokemon;
	} else{
		return NULL;
	}
}

pokemon_t *pokemon_buscar(informacion_pokemon_t *ip, const char *nombre)
{
	return NULL;
}

int pokemon_cantidad(informacion_pokemon_t *ip)
{
	return 0;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
	return NULL;
}

enum TIPO pokemon_tipo(pokemon_t *pokemon)
{
	return FUEGO;
}

const struct ataque *pokemon_buscar_ataque(pokemon_t *pokemon,
					   const char *nombre)
{
	return NULL;
}

int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *),
		     void *aux)
{
	return 0;
}

int con_cada_ataque(pokemon_t *pokemon,
		    void (*f)(const struct ataque *, void *), void *aux)
{
	return 0;
}

void pokemon_destruir_todo(informacion_pokemon_t *ip)
{
	free(ip);
}
