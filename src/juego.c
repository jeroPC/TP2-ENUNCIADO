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

// Declaraciones adelantadas de funciones auxiliares
static bool buscar_id_en_lista(void *elemento, void *extra);
int comparador_pokemon_nombre(const void *a, const void *b);
int comparador_pokemon_id(const void *a, const void *b);
void juego_listar_ordenado(juego_t *juego,
                           int (*comparador)(const void *, const void *),
                           void (*accion)(pokemon_t *, void *),
                           void *ctx);

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
    
    if (!pokemon || !juego) {
        return false;
    }
    
    // Agregar al final de la lista
    if (!lista_agregar(juego->pokedex, pokemon))
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


 //perro
/* Obtiene la cantidad de pokemones en la pokedex.
 * 
 * Devuelve la cantidad de pokemones o 0 si el juego es NULL.
 */
size_t juego_cantidad_pokemones(juego_t *juego){
    if(!juego || !juego->pokedex ) return 0;
    return lista_cantidad(juego->pokedex);
}

/* Obtiene la cantidad de cartas creadas en el juego.
 * 
 * Devuelve la cantidad de cartas o 0 si el juego es NULL.
 */
size_t juego_cantidad_cartas(juego_t *juego){
    if(!juego || !juego->cartas) return 0;
    return lista_cantidad(juego->cartas);
}




/* Busca pokemones por nombre (búsqueda parcial).
 * Los resultados se almacenan en el array resultados.
 * 
 * Devuelve la cantidad de pokemones encontrados.
 */
size_t juego_buscar_por_nombre(juego_t *juego, const char *nombre, 
                               pokemon_t **resultados, size_t max_resultados){
                                
    if (!juego || !nombre || !resultados || max_resultados == 0)
    return 0;

    size_t encontrados = 0;
    size_t cantidad = lista_cantidad(juego->pokedex);

    for(size_t i = 0 ; i < cantidad && encontrados < max_resultados; i++){
        pokemon_t *pokemon = lista_buscar_elemento(juego->pokedex , i);
         if (pokemon && strstr(pokemon->nombre, nombre)) { 
            resultados[encontrados++] = pokemon;
        }
    }
    return encontrados;
}
    

pokemon_t *juego_buscar_por_id(juego_t *juego, int id){
    if(!juego){
        return NULL;
    }

    void *busqueda[2];
    busqueda[0] = &id;
    busqueda [1] = NULL;

    lista_con_cada_elemento(juego->pokedex, buscar_id_en_lista, busqueda);
    return (pokemon_t *)busqueda[1];
}


void juego_listar_por_nombre(juego_t *juego,
                            void (*accion)(pokemon_t *, void *),
                            void *ctx) {
    juego_listar_ordenado(juego, comparador_pokemon_nombre, accion, ctx);
}

void juego_listar_por_id(juego_t *juego,
                        void (*accion)(pokemon_t *, void *),
                        void *ctx) {
    juego_listar_ordenado(juego, comparador_pokemon_id, accion, ctx);
}


/* Inicia una nueva partida con pokemones aleatorios.
 * Selecciona 9 pokemones al azar y crea 18 cartas (9 pares).
 * 
 * semilla: semilla para el generador aleatorio (0 = tiempo actual)
 * 
 * Devuelve true si se pudo iniciar la partida, false en caso de error.
 */
bool juego_iniciar_partida(juego_t *juego, unsigned int semilla) {
    if (!juego || juego_cantidad_pokemones(juego) < 9)
        return false;
    
    // Si semilla es 0, usa el tiempo actual (no determinístico)
    if (semilla == 0) {
        semilla = (unsigned int)time(NULL);
    }
    
    // Inicializar el generador con la semilla
    srand(semilla);
    
    // Limpiar cartas anteriores si las hay
    lista_destruir(juego->cartas);
    juego->cartas = lista_crear();
    if (!juego->cartas)
        return false;
    
    // Crear las cartas usando rand() internamente
    juego_crear_cartas_memoria(juego);
    
    // Inicializar estado de la partida
    juego->partida_activa = true;
    juego->jugador_actual = 1;
    juego->puntaje[0] = 0;
    juego->puntaje[1] = 0;
    
    return true;
}

bool juego_partida_activa(juego_t *juego) {
    if (!juego) return false;
    
    return (juego->cartas != NULL && lista_cantidad(juego->cartas) == TOTAL_CARTAS);
}

int juego_jugador_actual(juego_t *juego) {
    if (!juego)
        return 0;
    return juego->jugador_actual;
}



















//FUNCIONES AUXILIARES :




static bool buscar_id_en_lista(void *elemento, void *extra) {
    pokemon_t *pokemon = (pokemon_t *)elemento;
    void **busqueda = (void **)extra;
    int *id_buscado = (int *)busqueda[0];
    
    // Si encontramos el ID, lo guardamos en busqueda[1] y cortamos
    if (pokemon && pokemon->id == *id_buscado) {
        busqueda[1] = pokemon;
        return false; // Cortar recorrido
    }
    return true; // Seguir buscando
}




void mezclar_cartas(lista_t *cartas) {
    size_t n = lista_cantidad(cartas);
    for (size_t i = n - 1; i > 0; i--) {
        size_t j = (size_t)(rand() % (int)(i + 1));
        void *tmp = lista_buscar_elemento(cartas, i);
        void *otro = lista_buscar_elemento(cartas, j);
        lista_eliminar_elemento(cartas, i);
        lista_insertar(cartas, otro, i);
        lista_eliminar_elemento(cartas, j);
        lista_insertar(cartas, tmp, j);
    }
}

//aleatoriedad de cartas de pokemon 
void juego_crear_cartas_memoria(juego_t *juego) {
    if (!juego || lista_cantidad(juego->pokedex) < 9)
        return;

    size_t total = lista_cantidad(juego->pokedex);
    bool usados[total];
    memset(usados, 0, sizeof(usados)); //Establece todos los elementos del arreglo usados en cero (false).

    // Selecciona 9 pokemones distintos al azar
    for (int i = 0; i < 9; ) {
        size_t idx = (size_t)(rand() % (int)total);
        if (!usados[idx]) {
            struct pokemon *pk = lista_buscar_elemento(juego->pokedex, idx);
            lista_insertar(juego->cartas, pk, lista_cantidad(juego->cartas));
            lista_insertar(juego->cartas, pk, lista_cantidad(juego->cartas)); // Duplicado
            usados[idx] = true;
            i++;
        }
    }
    mezclar_cartas(juego->cartas);

}




int comparador_pokemon_nombre(const void *a, const void *b) {
    const pokemon_t *p1 = (const pokemon_t *)a;
    const pokemon_t *p2 = (const pokemon_t *)b;
    return strcmp(p1->nombre, p2->nombre);
}

int comparador_pokemon_id(const void *a, const void *b) {
    const pokemon_t *p1 = (const pokemon_t *)a;
    const pokemon_t *p2 = (const pokemon_t *)b;
    return p1->id - p2->id;
}

void juego_listar_ordenado(juego_t *juego,
                           int (*comparador)(const void *, const void *),
                           void (*accion)(pokemon_t *, void *),
                           void *ctx) {
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



void juego_destruir(juego_t *juego){
    if(!juego) return;

    // Destruir la lista de pokemones (sin destruir los pokemones, eso lo hace tp1)
    lista_destruir(juego->pokedex);
    
    // Destruir el hash (sin destruir los pokemones, ya están en la lista)
    hash_destruir(juego->pokedex_por_nombre);
    
    // Destruir la lista de cartas (sin destruir los pokemones)
    lista_destruir(juego->cartas);
    
    // Destruir la pila de historial (debería destruir las jugadas si las hay)
    pila_destruir(juego->historial_jugadas);
    
    // Liberar la estructura del juego
    free(juego);
}
