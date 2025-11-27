
#define _POSIX_C_SOURCE 200809L
#include "juego.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "lista.h"
#include "pila.h"
#include "abb.h"
#include "hash.h"
#include "cola.h"
#include "tp1.h"

static bool buscar_id_en_lista(void *elemento, void *extra);
static int comparador_pokemon_nombre(const void *a, const void *b);
static int comparador_pokemon_id(const void *a, const void *b);
static void juego_listar_ordenado(juego_t *juego,
				  int (*comparador)(const void *, const void *),
				  void (*accion)(pokemon_t *, void *),
				  void *ctx);
static int manejar_primera_carta(carta_t *carta_actual);
static int manejar_segunda_carta(juego_t *juego, carta_t *primera_carta,
				 carta_t *carta_actual);
static int procesar_par_correcto(juego_t *juego, carta_t *primera_carta,
				 carta_t *carta_actual);
static int procesar_par_incorrecto(juego_t *juego, carta_t *primera_carta,
				   carta_t *carta_actual);
static void mezclar_cartas(lista_t *cartas);
static carta_t *crear_carta(pokemon_t *pokemon, int posicion);
static void actualizar_posiciones_cartas(lista_t *cartas);

struct juego {
	lista_t *pokedex;
	hash_t *pokedex_por_nombre;
	lista_t *cartas;
	pila_t *historial_jugadas;
	bool partida_activa;
	int jugador_actual;
	int puntaje[2];
};

juego_t *juego_crear()
{
	juego_t *juego = malloc(sizeof(juego_t));
	if (!juego)
		return NULL;

	juego->pokedex = lista_crear();
	juego->pokedex_por_nombre = hash_crear(100);
	juego->cartas = lista_crear();
	juego->historial_jugadas = pila_crear();

	if (!juego->pokedex || !juego->pokedex_por_nombre || !juego->cartas ||
	    !juego->historial_jugadas) {
		lista_destruir(juego->pokedex);
		hash_destruir(juego->pokedex_por_nombre);
		lista_destruir(juego->cartas);
		pila_destruir(juego->historial_jugadas);
		free(juego);
		return NULL;
	}

	juego->partida_activa = false;
	juego->jugador_actual = 0;
	juego->puntaje[0] = 0;
	juego->puntaje[1] = 0;

	return juego;
}

bool insertar_pokemon_en_juego(struct pokemon *pokemon, void *contexto)
{
	juego_t *juego = (juego_t *)contexto;

	if (!pokemon || !juego) {
		return false;
	}

	struct pokemon *copia = malloc(sizeof(struct pokemon));
	if (!copia)
		return false;

	copia->id = pokemon->id;
	copia->nombre = strdup(pokemon->nombre);
	if (!copia->nombre) {
		free(copia);
		return false;
	}
	copia->tipo = pokemon->tipo;
	copia->ataque = pokemon->ataque;
	copia->defensa = pokemon->defensa;
	copia->velocidad = pokemon->velocidad;

	if (!lista_agregar(juego->pokedex, copia)) {
		free(copia->nombre);
		free(copia);
		return false;
	}

	if (!hash_insertar(juego->pokedex_por_nombre, copia->nombre, copia,
			   NULL)) {
		lista_eliminar_elemento(juego->pokedex,
					lista_cantidad(juego->pokedex) - 1);
		free(copia->nombre);
		free(copia);
		return false;
	}

	return true;
}

int juego_cargar_pokedex(juego_t *juego, const char *archivo)
{
	if (!juego || !archivo)
		return -1;

	tp1_t *tp1 = tp1_leer_archivo(archivo);
	if (!tp1)
		return -1;

	size_t cantidad = tp1_cantidad(tp1);

	size_t insertados =
		tp1_con_cada_pokemon(tp1, insertar_pokemon_en_juego, juego);

	int resultado = (insertados == cantidad) ? (int)cantidad : -1;

	tp1_destruir(tp1);

	return resultado;
}

size_t juego_cantidad_pokemones(juego_t *juego)
{
	if (!juego || !juego->pokedex)
		return 0;
	return lista_cantidad(juego->pokedex);
}

size_t juego_cantidad_cartas(juego_t *juego)
{
	if (!juego || !juego->cartas)
		return 0;
	return lista_cantidad(juego->cartas);
}

size_t juego_buscar_por_nombre(juego_t *juego, const char *nombre,
			       pokemon_t **resultados, size_t max_resultados)
{
	if (!juego || !nombre || !resultados || max_resultados == 0)
		return 0;

	size_t encontrados = 0;
	size_t cantidad = lista_cantidad(juego->pokedex);

	for (size_t i = 0; i < cantidad && encontrados < max_resultados; i++) {
		pokemon_t *pokemon = lista_buscar_elemento(juego->pokedex, i);
		if (pokemon && strstr(pokemon->nombre, nombre)) {
			resultados[encontrados++] = pokemon;
		}
	}
	return encontrados;
}

pokemon_t *juego_buscar_por_id(juego_t *juego, int id)
{
	if (!juego) {
		return NULL;
	}

	void *busqueda[2];
	busqueda[0] = &id;
	busqueda[1] = NULL;

	lista_con_cada_elemento(juego->pokedex, buscar_id_en_lista, busqueda);
	return (pokemon_t *)busqueda[1];
}

void juego_listar_por_nombre(juego_t *juego,
			     void (*accion)(pokemon_t *, void *), void *ctx)
{
	juego_listar_ordenado(juego, comparador_pokemon_nombre, accion, ctx);
}

void juego_listar_por_id(juego_t *juego, void (*accion)(pokemon_t *, void *),
			 void *ctx)
{
	juego_listar_ordenado(juego, comparador_pokemon_id, accion, ctx);
}

bool juego_iniciar_partida(juego_t *juego, unsigned int semilla)
{
	if (!juego || juego_cantidad_pokemones(juego) < 9)
		return false;

	if (semilla == 0) {
		semilla = (unsigned int)time(NULL);
	}

	srand(semilla);

	lista_destruir(juego->cartas);
	juego->cartas = lista_crear();
	if (!juego->cartas)
		return false;

	juego_crear_cartas_memoria(juego);

	juego->partida_activa = true;
	juego->jugador_actual = 1;
	juego->puntaje[0] = 0;
	juego->puntaje[1] = 0;

	return true;
}

bool juego_partida_activa(juego_t *juego)
{
	if (!juego)
		return false;

	return (juego->cartas != NULL &&
		lista_cantidad(juego->cartas) == TOTAL_CARTAS);
}

int juego_jugador_actual(juego_t *juego)
{
	if (!juego)
		return 0;
	return juego->jugador_actual;
}

int juego_obtener_puntuacion(juego_t *juego, int jugador)
{
	if (!juego || jugador < 1 || jugador > 2)
		return 0;

	return juego->puntaje[jugador - 1];
}

size_t juego_obtener_tablero(juego_t *juego, carta_t *cartas)
{
	if (!juego || !cartas)
		return 0;
	if (!juego->cartas)
		return 0;

	lista_iterador_t *iterador = lista_iterador_crear(juego->cartas);
	if (!iterador)
		return 0;

	size_t contador = 0;
	while (lista_iterador_hay_mas_elementos(iterador)) {
		carta_t *carta = lista_iterador_obtener_actual(iterador);
		if (carta) {
			cartas[contador] = *carta;
			contador++;
		}
		lista_iterador_siguiente(iterador);
	}

	lista_iterador_destruir(iterador);
	return contador;
}

int juego_seleccionar_carta(juego_t *juego, int posicion)
{
	if (!juego || posicion < 0 || posicion >= TOTAL_CARTAS)
		return -1;
	if (!juego->cartas)
		return -1;

	carta_t *carta_actual =
		lista_buscar_elemento(juego->cartas, (size_t)posicion);
	if (!carta_actual)
		return -1;

	if (carta_actual->emparejada || carta_actual->descubierta)
		return -1;

	int cartas_descubiertas = 0;
	carta_t *primera_carta = NULL;
	size_t cantidad = lista_cantidad(juego->cartas);

	for (size_t i = 0; i < cantidad; i++) {
		carta_t *carta = lista_buscar_elemento(juego->cartas, i);
		if (carta && carta->descubierta && !carta->emparejada) {
			cartas_descubiertas++;
			if (!primera_carta)
				primera_carta = carta;
		}
	}

	if (cartas_descubiertas == 0) {
		return manejar_primera_carta(carta_actual);
	}

	if (cartas_descubiertas == 1 && primera_carta) {
		return manejar_segunda_carta(juego, primera_carta,
					     carta_actual);
	}

	return -1;
}

size_t juego_obtener_ultimas_jugadas(juego_t *juego, jugada_t *jugadas,
				     size_t max_jugadas)
{
	if (!juego || !jugadas || max_jugadas == 0)
		return 0;
	if (!juego->historial_jugadas)
		return 0;

	pila_t *auxiliar = pila_crear();
	if (!auxiliar)
		return 0;

	size_t total = pila_cantidad(juego->historial_jugadas);
	size_t cantidad_copiar = (total < max_jugadas) ? total : max_jugadas;

	for (size_t i = 0; i < cantidad_copiar; i++) {
		jugada_t *jugada = pila_desapilar(juego->historial_jugadas);
		if (jugada)
			pila_apilar(auxiliar, jugada);
	}

	size_t copiadas = 0;
	while (pila_cantidad(auxiliar) > 0) {
		jugada_t *jugada = pila_desapilar(auxiliar);
		if (jugada) {
			jugadas[copiadas++] = *jugada;
			pila_apilar(juego->historial_jugadas, jugada);
		}
	}

	pila_destruir(auxiliar);
	return copiadas;
}

size_t juego_obtener_jugadas_jugador(juego_t *juego, int jugador,
				     jugada_t *jugadas, size_t max_jugadas)
{
	if (!juego || jugador < 1 || jugador > 2 || !jugadas ||
	    max_jugadas == 0)
		return 0;
	if (!juego->historial_jugadas)
		return 0;

	pila_t *auxiliar = pila_crear();
	if (!auxiliar)
		return 0;

	size_t copiadas = 0;
	size_t total = pila_cantidad(juego->historial_jugadas);

	for (size_t i = 0; i < total && copiadas < max_jugadas; i++) {
		jugada_t *jugada = pila_desapilar(juego->historial_jugadas);
		if (jugada) {
			pila_apilar(auxiliar, jugada);
			if (jugada->jugador == jugador) {
				jugadas[copiadas++] = *jugada;
			}
		}
	}

	while (pila_cantidad(auxiliar) > 0) {
		jugada_t *jugada = pila_desapilar(auxiliar);
		if (jugada)
			pila_apilar(juego->historial_jugadas, jugada);
	}

	pila_destruir(auxiliar);
	return copiadas;
}

bool juego_partida_terminada(juego_t *juego)
{
	if (!juego || !juego->cartas)
		return false;
	if (!juego->partida_activa)
		return false;

	size_t cantidad = lista_cantidad(juego->cartas);
	for (size_t i = 0; i < cantidad; i++) {
		carta_t *carta = lista_buscar_elemento(juego->cartas, i);
		if (carta && !carta->emparejada) {
			return false;
		}
	}

	return true;
}

int juego_obtener_ganador(juego_t *juego)
{
	if (!juego || !juego_partida_terminada(juego))
		return -1;

	if (juego->puntaje[0] > juego->puntaje[1]) {
		return 1;
	} else if (juego->puntaje[1] > juego->puntaje[0]) {
		return 2;
	}

	return 0;
}

void juego_finalizar_partida(juego_t *juego)
{
	if (!juego)
		return;

	if (juego->cartas) {
		size_t cantidad = lista_cantidad(juego->cartas);
		for (size_t i = 0; i < cantidad; i++) {
			carta_t *carta =
				lista_buscar_elemento(juego->cartas, i);
			free(carta);
		}
		lista_destruir(juego->cartas);
		juego->cartas = lista_crear();
	}

	if (juego->historial_jugadas) {
		while (pila_cantidad(juego->historial_jugadas) > 0) {
			jugada_t *jugada =
				pila_desapilar(juego->historial_jugadas);
			free(jugada);
		}
	}

	juego->partida_activa = false;
	juego->jugador_actual = 0;
	juego->puntaje[0] = 0;
	juego->puntaje[1] = 0;
}

void juego_crear_cartas_memoria(juego_t *juego)
{
	if (!juego || lista_cantidad(juego->pokedex) < 9)
		return;

	size_t total = lista_cantidad(juego->pokedex);
	size_t *indices = malloc(sizeof(size_t) * total);
	if (!indices)
		return;

	for (size_t i = 0; i < total; i++) {
		indices[i] = i;
	}

	// Fisher-Yates shuffle on indices
	for (size_t i = total - 1; i > 0; i--) {
		size_t j = (size_t)(rand() % (int)(i + 1));
		size_t temp = indices[i];
		indices[i] = indices[j];
		indices[j] = temp;
	}

	int pares_creados = 0;
	for (size_t i = 0; i < total && pares_creados < 9; i++) {
		size_t idx = indices[i];
		pokemon_t *pokemon = lista_buscar_elemento(juego->pokedex, idx);
		if (!pokemon)
			continue;

		carta_t *carta1 = crear_carta(pokemon, 0);
		carta_t *carta2 = crear_carta(pokemon, 0);

		if (carta1 && carta2) {
			if (lista_agregar(juego->cartas, carta1) &&
			    lista_agregar(juego->cartas, carta2)) {
				pares_creados++;
			} else {
				free(carta1);
				free(carta2);
			}
		} else {
			free(carta1);
			free(carta2);
		}
	}

	free(indices);
	mezclar_cartas(juego->cartas);
	actualizar_posiciones_cartas(juego->cartas);
}

void juego_destruir(juego_t *juego)
{
	if (!juego)
		return;

	if (juego->pokedex) {
		size_t cantidad = lista_cantidad(juego->pokedex);
		for (size_t i = 0; i < cantidad; i++) {
			pokemon_t *poke =
				lista_buscar_elemento(juego->pokedex, i);
			if (poke) {
				free(poke->nombre);
				free(poke);
			}
		}
		lista_destruir(juego->pokedex);
	}

	hash_destruir(juego->pokedex_por_nombre);

	if (juego->cartas) {
		size_t cantidad = lista_cantidad(juego->cartas);
		for (size_t i = 0; i < cantidad; i++) {
			carta_t *carta =
				lista_buscar_elemento(juego->cartas, i);
			free(carta);
		}
		lista_destruir(juego->cartas);
	}

	if (juego->historial_jugadas) {
		while (pila_cantidad(juego->historial_jugadas) > 0) {
			jugada_t *jugada =
				pila_desapilar(juego->historial_jugadas);
			free(jugada);
		}
		pila_destruir(juego->historial_jugadas);
	}

	free(juego);
}

static const struct {
	enum tipo_pokemon tipo;
	const char *str;
} conversion_tipos[] = {
	{ TIPO_ELEC, "ELEC" }, { TIPO_FUEG, "FUEG" }, { TIPO_PLAN, "PLAN" },
	{ TIPO_AGUA, "AGUA" }, { TIPO_NORM, "NORM" }, { TIPO_FANT, "FANT" },
	{ TIPO_PSI, "PSI" },   { TIPO_LUCH, "LUCH" }
};

enum tipo_pokemon juego_tipo_desde_string(const char *tipo_str)
{
	if (!tipo_str)
		return TIPO_NORM;

	for (size_t i = 0;
	     i < sizeof(conversion_tipos) / sizeof(conversion_tipos[0]); i++) {
		if (strcmp(tipo_str, conversion_tipos[i].str) == 0) {
			return conversion_tipos[i].tipo;
		}
	}

	return TIPO_NORM;
}

const char *juego_tipo_a_string(enum tipo_pokemon tipo)
{
	for (size_t i = 0;
	     i < sizeof(conversion_tipos) / sizeof(conversion_tipos[0]); i++) {
		if (conversion_tipos[i].tipo == tipo) {
			return conversion_tipos[i].str;
		}
	}
	return "NORM";
}

// FUNCIONES AUXILIARES 

static bool buscar_id_en_lista(void *elemento, void *extra)
{
	pokemon_t *pokemon = (pokemon_t *)elemento;
	void **busqueda = (void **)extra;
	int *id_buscado = (int *)busqueda[0];

	if (pokemon && pokemon->id == *id_buscado) {
		busqueda[1] = pokemon;
		return false;
	}
	return true;
}

static void mezclar_cartas(lista_t *cartas)
{
	size_t n = lista_cantidad(cartas);
	if (n == 0)
		return;

	carta_t **array = malloc(sizeof(carta_t *) * n);
	if (!array)
		return;

	for (size_t i = 0; i < n; i++) {
		array[i] = lista_buscar_elemento(cartas, i);
	}

	// Mezclar usando Fisher-Yates
	for (size_t i = n - 1; i > 0; i--) {
		size_t j = (size_t)(rand() % (int)(i + 1));
		carta_t *tmp = array[i];
		array[i] = array[j];
		array[j] = tmp;
	}

	while (lista_cantidad(cartas) > 0) {
		lista_eliminar_elemento(cartas, 0);
	}

	for (size_t i = 0; i < n; i++) {
		lista_agregar(cartas, array[i]);
	}

	free(array);
}

static carta_t *crear_carta(pokemon_t *pokemon, int posicion)
{
	carta_t *carta = malloc(sizeof(carta_t));
	if (!carta)
		return NULL;

	carta->posicion = posicion;
	carta->pokemon = pokemon;
	carta->descubierta = false;
	carta->emparejada = false;
	return carta;
}

static void actualizar_posiciones_cartas(lista_t *cartas)
{
	size_t cantidad = lista_cantidad(cartas);
	for (size_t i = 0; i < cantidad; i++) {
		carta_t *carta = lista_buscar_elemento(cartas, i);
		if (carta)
			carta->posicion = (int)i;
	}
}

static int comparador_pokemon_nombre(const void *a, const void *b)
{
	const pokemon_t *p1 = (const pokemon_t *)a;
	const pokemon_t *p2 = (const pokemon_t *)b;
	return strcmp(p1->nombre, p2->nombre);
}

static int comparador_pokemon_id(const void *a, const void *b)
{
	const pokemon_t *p1 = (const pokemon_t *)a;
	const pokemon_t *p2 = (const pokemon_t *)b;
	return p1->id - p2->id;
}

static void juego_listar_ordenado(juego_t *juego,
				  int (*comparador)(const void *, const void *),
				  void (*accion)(pokemon_t *, void *),
				  void *ctx)
{
	if (!juego || !juego->pokedex || !accion)
		return;

	abb_t *abb = abb_crear(comparador);
	if (!abb)
		return;

	size_t cantidad = lista_cantidad(juego->pokedex);
	for (size_t i = 0; i < cantidad; i++) {
		pokemon_t *poke = lista_buscar_elemento(juego->pokedex, i);
		abb_insertar(abb, poke);
	}

	pokemon_t **vector = malloc(sizeof(pokemon_t *) * cantidad);
	if (!vector) {
		abb_destruir(abb);
		return;
	}
	abb_vectorizar(abb, ABB_INORDEN, cantidad, (void **)vector);

	for (size_t i = 0; i < cantidad; i++)
		accion(vector[i], ctx);

	free(vector);
	abb_destruir(abb);
}

static int manejar_primera_carta(carta_t *carta_actual)
{
	carta_actual->descubierta = true;
	return 0;
}

static int manejar_segunda_carta(juego_t *juego, carta_t *primera_carta,
				 carta_t *carta_actual)
{
	carta_actual->descubierta = true;

	int pos_primera = -1, pos_actual = -1;
	size_t cantidad = lista_cantidad(juego->cartas);
	for (size_t i = 0; i < cantidad; i++) {
		carta_t *c = lista_buscar_elemento(juego->cartas, i);
		if (c == primera_carta)
			pos_primera = (int)i;
		if (c == carta_actual)
			pos_actual = (int)i;
	}

	bool es_par = (primera_carta->pokemon == carta_actual->pokemon);

	jugada_t *jugada = malloc(sizeof(jugada_t));
	if (jugada) {
		jugada->jugador = juego->jugador_actual;
		jugada->carta1 = pos_primera;
		jugada->carta2 = pos_actual;
		jugada->acierto = es_par;
		strncpy(jugada->nombre_pokemon, primera_carta->pokemon->nombre,
			MAX_NOMBRE_POKEMON - 1);
		jugada->nombre_pokemon[MAX_NOMBRE_POKEMON - 1] = '\0';
		pila_apilar(juego->historial_jugadas, jugada);
	}

	if (es_par) {
		return procesar_par_correcto(juego, primera_carta,
					     carta_actual);
	} else {
		return procesar_par_incorrecto(juego, primera_carta,
					       carta_actual);
	}
}

static int procesar_par_correcto(juego_t *juego, carta_t *primera_carta,
				 carta_t *carta_actual)
{
	primera_carta->emparejada = true;
	carta_actual->emparejada = true;
	juego->puntaje[juego->jugador_actual - 1]++;
	return 1;
}

static int procesar_par_incorrecto(juego_t *juego, carta_t *primera_carta,
				   carta_t *carta_actual)
{
	primera_carta->descubierta = false;
	carta_actual->descubierta = false;
	juego->jugador_actual = (juego->jugador_actual == 1) ? 2 : 1;
	return -2;
}
