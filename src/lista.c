#include "lista.h"

typedef struct nodo {
	void *dato;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	nodo_t *primero;
	nodo_t *ultimo;
	size_t cantidad;
};

struct lista_iterador {
	nodo_t *actual;
};

lista_t *lista_crear()
{
	lista_t *lista = calloc(1, sizeof(lista_t));
	if (!lista)
		return NULL;
	return lista;
}

bool lista_vacia(lista_t *lista)
{
	if (!lista || lista->cantidad == 0)
		return true;
	return false;
}

size_t lista_cantidad(lista_t *lista)
{
	if (!lista)
		return 0;
	return lista->cantidad;
}

bool lista_agregar(lista_t *lista, void *dato)
{
	if (!lista)
		return false;
	nodo_t *nuevo = malloc(sizeof(nodo_t));
	if (!nuevo)
		return false;
	nuevo->dato = dato;
	nuevo->siguiente = NULL;

	if (!lista->primero) {
		lista->primero = nuevo;
		lista->ultimo = nuevo;
	} else {
		lista->ultimo->siguiente = nuevo;
		lista->ultimo = nuevo;
	}

	lista->cantidad++;
	return true;
}

static bool primera_posicion_insercion(lista_t *lista, void *elemento)
{
	if (!lista)
		return false;

	nodo_t *nuevo = malloc(sizeof(nodo_t));
	if (!nuevo)
		return false;

	nuevo->dato = elemento;
	nuevo->siguiente = lista->primero;
	lista->primero = nuevo;
	if (lista->cantidad == 0) {
		lista->ultimo = nuevo;
	}
	lista->cantidad++;
	return true;
}

static bool posicion_valida(lista_t *lista, size_t posicion)
{
	if (!lista)
		return false;
	return posicion < lista->cantidad;
}

bool lista_insertar(lista_t *lista, void *elemento, size_t posicion)
{
	if (!lista)
		return false;

	if (lista->cantidad == 0)
		return false;

	if (posicion == 0)
		return primera_posicion_insercion(lista, elemento);

	if (!posicion_valida(lista, posicion))
		return false;

	nodo_t *nuevo = malloc(sizeof(nodo_t));
	if (!nuevo)
		return false;
	nuevo->dato = elemento;

	nodo_t *actual = lista->primero;
	for (size_t i = 0; i < posicion - 1; i++) {
		actual = actual->siguiente;
	}
	nuevo->siguiente = actual->siguiente;
	actual->siguiente = nuevo;

	if (nuevo->siguiente == NULL) {
		lista->ultimo = nuevo;
	}
	lista->cantidad++;

	return true;
}

static void *eliminar_primero(lista_t *lista)
{
	if (!lista || !lista->primero)
		return NULL;
	nodo_t *nodo_a_eliminar = lista->primero;
	void *dato = nodo_a_eliminar->dato;
	lista->primero = nodo_a_eliminar->siguiente;
	free(nodo_a_eliminar);
	lista->cantidad--;
	if (!lista->cantidad)
		lista->ultimo = NULL;
	return dato;
}

static void *eliminar_en_posicion(lista_t *lista, size_t posicion)
{
	nodo_t *anterior = lista->primero;
	for (size_t i = 0; i < posicion - 1; i++) {
		anterior = anterior->siguiente;
	}

	nodo_t *a_eliminar = anterior->siguiente;
	void *dato = a_eliminar->dato;
	anterior->siguiente = a_eliminar->siguiente;
	free(a_eliminar);
	lista->cantidad--;
	if (anterior->siguiente == NULL) {
		lista->ultimo = anterior;
	}

	return dato;
}

void *lista_eliminar_elemento(lista_t *lista, size_t posicion)
{
	if (!lista || lista->cantidad <= posicion)
		return NULL;
	if (posicion == 0) {
		return eliminar_primero(lista);
	}

	return eliminar_en_posicion(lista, posicion);
}

int lista_buscar_posicion(lista_t *lista, void *elemento,
			  int (*comparador)(const void *, const void *))
{
	if (!lista || !comparador)
		return -1;

	nodo_t *actual = lista->primero;
	size_t i = 0;
	while (actual) {
		if (comparador(elemento, actual->dato) == 0)
			return (int)i;
		actual = actual->siguiente;
		i++;
	}
	return -1;
}

void *lista_buscar_elemento(lista_t *lista, size_t posicion)
{
	if (!lista || lista->cantidad <= posicion)
		return NULL;

	nodo_t *objetivo = lista->primero;
	size_t i = 0;
	while (i < posicion) {
		objetivo = objetivo->siguiente;
		i++;
	}

	return objetivo->dato;
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*f)(void *, void *),
			       void *extra)
{
	if (!lista || !f)
		return 0;

	nodo_t *actual = lista->primero;
	size_t i = 0;
	bool seguir = true;
	while (actual && i < lista->cantidad && seguir) {
		seguir = f(actual->dato, extra);
		actual = actual->siguiente;
		i++;
	}
	return i;
}

void lista_destruir_todo(lista_t *lista, void (*destructor)(void *))
{
	if (!lista)
		return;
	nodo_t *actual = lista->primero;
	while (actual) {
		nodo_t *siguiente = actual->siguiente;
		if (destructor)
			destructor(actual->dato);
		free(actual);
		actual = siguiente;
	}
	free(lista);
}

void lista_destruir(lista_t *lista)
{
	if (!lista)
		return;

	nodo_t *actual = lista->primero;
	while (actual) {
		nodo_t *siguiente = actual->siguiente;
		free(actual);
		actual = siguiente;
	}
	free(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista)
		return NULL;
	lista_iterador_t *it = malloc(sizeof(lista_iterador_t));
	if (!it)
		return NULL;
	it->actual = lista->primero;
	return it;
}

bool lista_iterador_hay_mas_elementos(lista_iterador_t *it)
{
	return it && it->actual;
}

void lista_iterador_siguiente(lista_iterador_t *it)
{
	if (!it || !it->actual)
		return;
	it->actual = it->actual->siguiente;
}

void *lista_iterador_obtener_actual(lista_iterador_t *it)
{
	if (!it || !it->actual)
		return NULL;
	return it->actual->dato;
}

void lista_iterador_destruir(lista_iterador_t *it)
{
	if (!it)
		return;
	free(it);
}