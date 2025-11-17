#include "juego.h"
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "pila.h"
#include "abb.h"
#include "hash.h"
#include "cola.h"
#include "tp1.h"


struct juego {
    lista_t *pokedex;              // Lista de todos los pokemones
    hash_t *pokedex_por_nombre;    // Búsqueda rápida por nombre
    lista_t *cartas;               // Mazo de cartas
    pila_t *historial_jugadas;     // Historial (para deshacer)
    bool partida_activa;        
    int jugador_actual;         
    int puntaje[2];             
};


juego_t *juego_crear(){
    juego_t *juego= malloc(sizeof(juego_t));
    if(!juego) return NULL;

    juego->pokedex = lista_crear();
    juego->pokedex_por_nombre = hash_crear(100);
    juego->cartas = lista_crear();
    juego->historial_jugadas = pila_crear();

    if (!juego->pokedex || !juego->pokedex_por_nombre || 
        !juego->cartas || !juego->historial_jugadas) {
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

/* Carga pokemones desde un archivo CSV.
 * El formato esperado es: ID,NOMBRE,TIPO,ATAQUE,DEFENSA,VELOCIDAD
 * 
 * Devuelve la cantidad de pokemones cargados, o -1 en caso de error.
 */



bool insertar_pokemon_en_juego(struct pokemon *pokemon, void *contexto) {
    juego_t *juego = (juego_t *)contexto;
    
    size_t pos = lista_cantidad(juego->pokedex);
    if (!lista_insertar(juego->pokedex, pokemon,pos))
        return false;
    
    // Insertar en el hash para búsqueda rápida por nombre
    if (!hash_insertar(juego->pokedex_por_nombre, pokemon->nombre, pokemon, NULL))
        return false;
    
    return true;
}


int juego_cargar_pokedex(juego_t *juego, const char *archivo){
    if(!juego || !archivo) return -1;

    tp1_t *tp1 = tp1_leer_archivo(archivo);
    if(!tp1) return -1;

    size_t cantidad = tp1_cantidad(tp1);

    size_t insertados = tp1_con_cada_pokemon(tp1, insertar_pokemon_en_juego,juego);

    if (insertados == cantidad) {
        return (int)cantidad;
    }

    return -1;

}