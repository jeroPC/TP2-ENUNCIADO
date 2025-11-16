#include "abb.h"
#include "abb_estructura_privada.h"
#include <stdlib.h>

static nodo_t *buscar_nodo_y_padre(abb_t *abb, void *dato, nodo_t **padre_out);

abb_t *abb_crear(int (*comparador)(const void *, const void *))
{
	if (!comparador)
		return NULL;
	abb_t *abb = calloc(1, sizeof(abb_t));
	if (!abb)
		return NULL;
	abb->comparador = comparador;
	return abb;
}

static nodo_t *crear_nodo(void *dato)
{
	nodo_t *nodo = malloc(sizeof(nodo_t));
	if (!nodo)
		return NULL;
	nodo->dato = dato;
	nodo->der = NULL;
	nodo->izq = NULL;

	return nodo;
}

static bool arbol_vacio(abb_t *abb, void *dato)
{
	if (!abb)
		return false;
	if (!abb->raiz) {
		abb->raiz = crear_nodo(dato);
		if (!abb->raiz)
			return false;
		abb->cantidad++;
		return true;
	}
	return false;
}

static bool arbol_insercion_con_elementos(abb_t *abb, void *dato)
{
	if (!abb)
		return false;
	nodo_t *actual = abb->raiz;

	while (actual) {
		int cmp = abb->comparador(dato, actual->dato);
		if (cmp <= 0) {
			if (!actual->izq) {
				actual->izq = crear_nodo(dato);
				if (!actual->izq)
					return false;
				abb->cantidad++;
				return true;
			}
			actual = actual->izq;
		} else {
			if (!actual->der) {
				actual->der = crear_nodo(dato);
				if (!actual->der)
					return false;
				abb->cantidad++;
				return true;
			}
			actual = actual->der;
		}
	}
	return false;
}

bool abb_insertar(abb_t *abb, void *dato)
{
	if (!abb)
		return false;
	if (arbol_vacio(abb, dato))
		return true;
	if (arbol_insercion_con_elementos(abb, dato))
		return true;
	return false;
}

bool abb_existe(abb_t *abb, void *dato)
{
	if (!abb)
		return false;
	nodo_t *padre = NULL;
	return buscar_nodo_y_padre(abb, dato, &padre) != NULL;
}

static nodo_t *buscar_nodo_y_padre(abb_t *abb, void *dato, nodo_t **padre_out)
{
	if (!abb || !padre_out)
		return NULL;
	nodo_t *actual = abb->raiz;
	*padre_out = NULL;
	int cmp = 0;
	while (actual) {
		cmp = abb->comparador(dato, actual->dato);
		if (cmp == 0)
			break;
		*padre_out = actual;
		if (cmp < 0)
			actual = actual->izq;
		else
			actual = actual->der;
	}
	return actual;
}

void *abb_buscar(abb_t *abb, void *dato)
{
	if (!abb)
		return NULL;
	nodo_t *padre = NULL;
	nodo_t *actual = buscar_nodo_y_padre(abb, dato, &padre);
	if (actual)
		return actual->dato;
	return NULL;
}

static void *abb_hoja_sin_hijos(abb_t *abb, void *dato)
{
	if (abb->raiz != NULL && abb->raiz->izq == NULL &&
	    abb->raiz->der == NULL &&
	    abb->comparador(dato, abb->raiz->dato) == 0 && abb->cantidad == 1) {
		void *dato_raiz = abb->raiz->dato;
		free(abb->raiz);
		abb->raiz = NULL;
		abb->cantidad = 0;
		return dato_raiz;
	}
	return NULL;
}

static void *eliminar_si_es_hoja_no_raiz(abb_t *abb, nodo_t *padre,
					 nodo_t *actual)
{
	if (!padre)
		return NULL;
	if (!actual->izq && !actual->der) {
		void *dato_nodo = actual->dato;
		if (padre->izq == actual)
			padre->izq = NULL;
		else
			padre->der = NULL;
		free(actual);
		abb->cantidad--;
		return dato_nodo;
	}
	return NULL;
}

static void *eliminar_si_tiene_un_hijo(abb_t *abb, nodo_t *padre,
				       nodo_t *actual)
{
	if (!actual)
		return NULL;
	if ((actual->izq && !actual->der) || (!actual->izq && actual->der)) {
		nodo_t *hijo = actual->izq ? actual->izq : actual->der;
		void *dato = actual->dato;
		if (!padre) {
			abb->raiz = hijo;
		} else if (padre->izq == actual) {
			padre->izq = hijo;
		} else {
			padre->der = hijo;
		}
		free(actual);
		abb->cantidad--;
		return dato;
	}
	return NULL;
}

static void *abb_ultima_hoja(abb_t *abb, void *dato)
{
	nodo_t *padre = NULL;
	nodo_t *actual = buscar_nodo_y_padre(abb, dato, &padre);
	if (!actual)
		return NULL;
	void *eliminado = eliminar_si_es_hoja_no_raiz(abb, padre, actual);
	if (eliminado)
		return eliminado;
	return eliminar_si_tiene_un_hijo(abb, padre, actual);
}

static void *abb_eliminacion_predecesor(abb_t *abb, nodo_t *nodo)
{
	nodo_t *padre = nodo;
	nodo_t *actual = nodo->izq;

	while (actual->der) {
		padre = actual;
		actual = actual->der;
	}

	void *predecesor = actual->dato;
	if (padre == nodo) {
		padre->izq = actual->izq;
	} else {
		padre->der = actual->izq;
	}
	free(actual);
	abb->cantidad--;
	return predecesor;
}

void *abb_eliminar(abb_t *abb, void *dato)
{
	if (!abb)
		return NULL;
	void *abb_sin_hijos = abb_hoja_sin_hijos(abb, dato);
	if (abb_sin_hijos)
		return abb_sin_hijos;

	void *eliminado = abb_ultima_hoja(abb, dato);
	if (eliminado)
		return eliminado;

	nodo_t *padre = NULL;
	nodo_t *actual = buscar_nodo_y_padre(abb, dato, &padre);
	if (!actual)
		return NULL;

	if (actual->izq && actual->der) {
		void *dato_eliminado = actual->dato;
		actual->dato = abb_eliminacion_predecesor(abb, actual);
		return dato_eliminado;
	}

	return NULL;
}

void *abb_raiz(abb_t *abb)
{
	if (!abb || !abb->raiz)
		return NULL;
	return abb->raiz->dato;
}

size_t abb_cantidad(abb_t *abb)
{
	if (!abb)
		return 0;
	return abb->cantidad;
}

bool abb_esta_vacio(abb_t *abb)
{
	return (!abb || abb->cantidad == 0);
}

typedef struct {
	void *user_data;
	size_t max_elementos;
	size_t *elemento_actual;
	bool cortar;
	bool (*funcion_usuario)(void *, void *);
} contexto_recorrido_t;

static void procesar_nodo(nodo_t *nodo, contexto_recorrido_t *contexto,
			  size_t *contador)
{
	if (contexto->funcion_usuario) {
		if (!contexto->funcion_usuario(nodo->dato, contexto->user_data))
			contexto->cortar = true;
		(*contador)++;
	} else if (contexto->elemento_actual) {
		if (*(contexto->elemento_actual) < contexto->max_elementos) {
			((void **)contexto
				 ->user_data)[*(contexto->elemento_actual)] =
				nodo->dato;
			(*(contexto->elemento_actual))++;
			(*contador)++;
			if (*(contexto->elemento_actual) >=
			    contexto->max_elementos)
				contexto->cortar = true;
		} else {
			contexto->cortar = true;
		}
	}
}

static size_t abb_recorrer(nodo_t *nodo, contexto_recorrido_t *contexto,
			   enum abb_recorrido tipo)
{
	if (!nodo ||
	    (contexto->elemento_actual &&
	     *(contexto->elemento_actual) >= contexto->max_elementos) ||
	    contexto->cortar)
		return 0;
	size_t contador = 0;
	switch (tipo) {
	case ABB_PREORDEN:
		procesar_nodo(nodo, contexto, &contador);
		if (contexto->cortar)
			return contador;
		contador += abb_recorrer(nodo->izq, contexto, tipo);
		if (contexto->cortar)
			return contador;
		contador += abb_recorrer(nodo->der, contexto, tipo);
		break;
	case ABB_INORDEN:
		contador += abb_recorrer(nodo->izq, contexto, tipo);
		if (contexto->cortar)
			return contador;
		procesar_nodo(nodo, contexto, &contador);
		if (contexto->cortar)
			return contador;
		contador += abb_recorrer(nodo->der, contexto, tipo);
		break;
	case ABB_POSTORDEN:
		contador += abb_recorrer(nodo->izq, contexto, tipo);
		if (contexto->cortar)
			return contador;
		contador += abb_recorrer(nodo->der, contexto, tipo);
		if (contexto->cortar)
			return contador;
		procesar_nodo(nodo, contexto, &contador);
		break;
	}
	return contador;
}

size_t abb_con_cada_elemento(abb_t *abb, enum abb_recorrido tipo_recorrido,
			     bool (*f)(void *, void *), void *extra)
{
	if (!abb || !f)
		return 0;
	contexto_recorrido_t contexto = { .user_data = extra,
					  .max_elementos = (size_t)-1,
					  .elemento_actual = NULL,
					  .cortar = false,
					  .funcion_usuario = f };
	return abb_recorrer(abb->raiz, &contexto, tipo_recorrido);
}

size_t abb_vectorizar(abb_t *abb, enum abb_recorrido tipo_recorrido,
		      size_t cant, void **vector)
{
	if (!abb || !vector || cant == 0)
		return 0;
	size_t pos = 0;
	contexto_recorrido_t contexto = { .user_data = vector,
					  .max_elementos = cant,
					  .elemento_actual = &pos,
					  .cortar = false,
					  .funcion_usuario = NULL };
	return abb_recorrer(abb->raiz, &contexto, tipo_recorrido);
}

static void abb_destruir_nodos(nodo_t *nodo)
{
	if (!nodo)
		return;
	abb_destruir_nodos(nodo->izq);
	abb_destruir_nodos(nodo->der);
	free(nodo);
}

void abb_destruir(abb_t *abb)
{
	if (!abb)
		return;
	abb_destruir_nodos(abb->raiz);
	free(abb);
}

static void abb_destruir_con_destructor(nodo_t *nodo,
					void (*destructor)(void *))
{
	if (!nodo)
		return;
	abb_destruir_con_destructor(nodo->izq, destructor);
	abb_destruir_con_destructor(nodo->der, destructor);
	if (destructor)
		destructor(nodo->dato);
	free(nodo);
}

void abb_destruir_todo(abb_t *abb, void (*destructor)(void *))
{
	if (!abb)
		return;
	abb_destruir_con_destructor(abb->raiz, destructor);
	free(abb);
}