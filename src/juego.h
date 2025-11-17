#ifndef JUEGO_H
#define JUEGO_H

#include <stdbool.h>
#include <stddef.h>
#include "tp1.h"

#define MAX_NOMBRE_POKEMON 256
#define MAX_TIPO 5
#define TOTAL_CARTAS 18
#define MAX_JUGADAS_MOSTRADAS 5

/* Usamos las definiciones de tp1.h */
typedef struct pokemon pokemon_t;

/* Estructura de una carta en el juego */
typedef struct carta {
    int posicion;           // Posición en el tablero (0-17)
    pokemon_t *pokemon;     // Puntero al pokémon
    bool descubierta;       // Si la carta está visible
    bool emparejada;        // Si ya fue emparejada
} carta_t;

/* Estructura de una jugada */
typedef struct jugada {
    int jugador;            // 1 o 2
    int carta1;             // Posición de la primera carta
    int carta2;             // Posición de la segunda carta
    bool acierto;           // true si fue un par válido
    char nombre_pokemon[MAX_NOMBRE_POKEMON]; // Nombre del pokémon encontrado
} jugada_t;

/* Estado del juego */
typedef struct juego juego_t;

/* Crea un nuevo juego vacío.
 * 
 * Devuelve un puntero al juego creado o NULL en caso de error.
 */
juego_t *juego_crear();

/* Carga pokemones desde un archivo CSV.
 * El formato esperado es: ID,NOMBRE,TIPO,ATAQUE,DEFENSA,VELOCIDAD
 * 
 * Devuelve la cantidad de pokemones cargados, o -1 en caso de error.
 */
int juego_cargar_pokedex(juego_t *juego, const char *archivo);

/* Obtiene la cantidad de pokemones en la pokedex.
 * 
 * Devuelve la cantidad de pokemones o 0 si el juego es NULL.
 */
size_t juego_cantidad_pokemones(juego_t *juego);

/* Obtiene la cantidad de cartas creadas en el juego.
 * 
 * Devuelve la cantidad de cartas o 0 si el juego es NULL.
 */
size_t juego_cantidad_cartas(juego_t *juego);

/* Busca pokemones por nombre (búsqueda parcial).
 * Los resultados se almacenan en el array resultados.
 * 
 * Devuelve la cantidad de pokemones encontrados.
 */
size_t juego_buscar_por_nombre(juego_t *juego, const char *nombre, 
                               pokemon_t **resultados, size_t max_resultados);

/* Busca un pokémon por ID exacto.
 * 
 * Devuelve el pokémon encontrado o NULL si no existe.
 */
pokemon_t *juego_buscar_por_id(juego_t *juego, int id);

/* Muestra todos los pokemones ordenados por nombre.
 * La función callback se invoca para cada pokémon.
 * 
 * callback: función que recibe el pokémon y un contexto
 * ctx: contexto pasado al callback
 */
void juego_listar_por_nombre(juego_t *juego, 
                            void (*callback)(pokemon_t *, void *), 
                            void *ctx);

/* Muestra todos los pokemones ordenados por ID.
 * La función callback se invoca para cada pokémon.
 * 
 * callback: función que recibe el pokémon y un contexto
 * ctx: contexto pasado al callback
 */
void juego_listar_por_id(juego_t *juego, 
                        void (*callback)(pokemon_t *, void *), 
                        void *ctx);

/* Inicia una nueva partida con pokemones aleatorios.
 * Selecciona 9 pokemones al azar y crea 18 cartas (9 pares).
 * 
 * semilla: semilla para el generador aleatorio (0 = tiempo actual)
 * 
 * Devuelve true si se pudo iniciar la partida, false en caso de error.
 */
bool juego_iniciar_partida(juego_t *juego, unsigned int semilla);

/* Verifica si hay una partida en curso.
 * 
 * Devuelve true si hay una partida activa, false en caso contrario.
 */
bool juego_partida_activa(juego_t *juego);

/* Obtiene el jugador actual (1 o 2).
 * 
 * Devuelve el número del jugador actual.
 */
int juego_jugador_actual(juego_t *juego);

/* Obtiene la puntuación de un jugador.
 * 
 * jugador: 1 o 2
 * 
 * Devuelve la puntuación del jugador o 0 si es inválido.
 */
int juego_obtener_puntuacion(juego_t *juego, int jugador);

/* Obtiene el estado actual del tablero.
 * 
 * cartas: array donde se almacenarán las cartas (mínimo TOTAL_CARTAS)
 * 
 * Devuelve la cantidad de cartas copiadas.
 */
size_t juego_obtener_tablero(juego_t *juego, carta_t *cartas);

/* Intenta seleccionar una carta.
 * Si es la primera carta, se marca como seleccionada.
 * Si es la segunda, se verifica si forman un par.
 * 
 * posicion: posición de la carta (0-17)
 * 
 * Devuelve:
 *  - 0: carta seleccionada correctamente (esperando segunda carta)
 *  - 1: par formado correctamente (punto para el jugador)
 *  - -1: error (posición inválida, carta ya emparejada, etc)
 *  - -2: no es un par (cambio de turno)
 */
int juego_seleccionar_carta(juego_t *juego, int posicion);

/* Obtiene las últimas jugadas realizadas.
 * 
 * jugadas: array donde se almacenarán las jugadas
 * max_jugadas: tamaño máximo del array
 * 
 * Devuelve la cantidad de jugadas copiadas.
 */
size_t juego_obtener_ultimas_jugadas(juego_t *juego, jugada_t *jugadas, 
                                     size_t max_jugadas);

/* Obtiene todas las jugadas de un jugador específico.
 * 
 * jugador: 1 o 2
 * jugadas: array donde se almacenarán las jugadas
 * max_jugadas: tamaño máximo del array
 * 
 * Devuelve la cantidad de jugadas copiadas.
 */
size_t juego_obtener_jugadas_jugador(juego_t *juego, int jugador,
                                     jugada_t *jugadas, size_t max_jugadas);

/* Verifica si la partida ha terminado.
 * Una partida termina cuando no quedan más cartas sin emparejar.
 * 
 * Devuelve true si la partida terminó, false en caso contrario.
 */
bool juego_partida_terminada(juego_t *juego);

/* Obtiene el ganador de la partida.
 * Solo válido si la partida ha terminado.
 * 
 * Devuelve:
 *  - 1 o 2: el jugador ganador
 *  - 0: empate
 *  - -1: la partida no ha terminado
 */
int juego_obtener_ganador(juego_t *juego);

/* Finaliza la partida actual y limpia el estado.
 * No destruye la pokedex cargada.
 */
void juego_finalizar_partida(juego_t *juego);

/* Crea las cartas de memoria para el juego.
 * Selecciona 9 pokemones al azar y crea 18 cartas (9 pares).
 */
void juego_crear_cartas_memoria(juego_t *juego);

/* Destruye el juego y libera toda la memoria asociada.
 * Incluye la pokedex y el estado de la partida.
 * 
 * Si el juego es NULL, no hace nada.
 */
void juego_destruir(juego_t *juego);

/* Convierte un string de tipo a enum */
enum tipo_pokemon juego_tipo_desde_string(const char *tipo_str);

/* Convierte un enum de tipo a string */
const char *juego_tipo_a_string(enum tipo_pokemon tipo);

#endif // JUEGO_H
