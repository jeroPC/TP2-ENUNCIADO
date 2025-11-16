#include "lista.h"
#include "pila.h"

struct pila {
	lista_t *lista;
};

pila_t *pila_crear()
{
	pila_t *pila = calloc(1, sizeof(pila_t));
	if (!pila)
		return NULL;

	pila->lista = lista_crear();
	if (!pila->lista) {
		free(pila);
		return NULL;
	}
	return pila;
}

bool pila_apilar(pila_t *pila, void *elemento)
{
	if (!pila || !elemento)
		return false;
	return lista_agregar(pila->lista, elemento);
}

void *pila_desapilar(pila_t *pila)
{
	if (!pila || lista_vacia(pila->lista))
		return NULL;
	size_t ultima_posicion = lista_cantidad(pila->lista) - 1;
	return lista_eliminar_elemento(pila->lista, ultima_posicion);
}

void *pila_ver_primero(pila_t *pila)
{
	if (!pila || lista_vacia(pila->lista))
		return NULL;
	size_t posicion = lista_cantidad(pila->lista) - 1;
	return lista_buscar_elemento(pila->lista, posicion);
}

size_t pila_cantidad(pila_t *pila)
{
	if (!pila)
		return 0;
	return lista_cantidad(pila->lista);
}

void pila_destruir(pila_t *pila)
{
	if (!pila)
		return;
	lista_destruir(pila->lista);
	free(pila);
}