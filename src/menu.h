#ifndef MENU_H
#define MENU_H

#include <stdbool.h>

/* Estilos predefinidos para el menú */
typedef enum {
	ESTILO_MINIMALISTA = 0,
	ESTILO_BORDES = 1,
	ESTILO_RETRO = 2,
	ESTILO_PERSONALIZADO = 99, // Indicador de estilo personalizado
	TOTAL_ESTILOS = 3
} estilo_menu_t;

/* Estructura para definir un estilo personalizado.
 * Permite al usuario crear estilos visuales propios sin modificar menu.c
 */
typedef struct estilo_personalizado {
	char *borde_superior; // Ej: "╔═══════╗"
	char *borde_inferior; // Ej: "╚═══════╝"
	char *separador; // Ej: "─────────"
	char *prefijo_opcion; // Ej: "  ▸ "
	char *color_titulo; // Color ANSI para el título
	char *color_opciones; // Color ANSI para las opciones
	char *color_teclas; // Color ANSI para las teclas
	char *color_reset; // ANSI reset
} estilo_personalizado_t;

typedef struct menu menu_t;

/* Tipo de función que se ejecuta al seleccionar una opción.
 * - ctx_interno: contexto interno de la opción (opcional)
 * - ctx_externo: contexto externo pasado al ejecutar el menú
 * 
 * Retorna true si se debe continuar mostrando el menú, false para salir.
 */
typedef bool (*accion_menu_t)(void *ctx_interno, void *ctx_externo);

/* Crea un nuevo menú con el título dado y estilo inicial.
 * Si el título es NULL, se asigna "MENU" por defecto.
 *
 * Devuelve un puntero al menú creado o NULL en caso de error.
 */
menu_t *menu_crear(const char *titulo, estilo_menu_t estilo);

/* Agrega una nueva opción al menú.
 * - m: el menú donde agregar la opción
 * - tecla: carácter que identifica la opción ('C', 'B', etc)
 * - descripcion: texto descriptivo de la opción
 * - accion: función a ejecutar (NULL si es un submenú)
 * - submenu: submenú a abrir (NULL si no es submenú)
 * - ctx_interno: contexto interno opcional para la acción
 *
 * Si ya existe una opción con esa tecla, se reemplaza.
 * No puede ser NULL ni accion ni submenu al mismo tiempo.
 *
 * Devuelve true si se agregó correctamente, false en caso de error.
 */
bool menu_agregar_opcion(menu_t *menu, char tecla, const char *descripcion,
			 accion_menu_t accion, menu_t *submenu,
			 void *ctx_interno);

/* Cambia el estilo visual del menú.
 * El estilo se aplica de forma cíclica (0, 1, 2, 0, 1, 2...).
 *
 * Si el menú es NULL, no hace nada.
 */
void menu_cambiar_estilo(menu_t *menu);

/* Obtiene el estilo actual del menú.
 *
 * Devuelve el estilo actual o ESTILO_MINIMALISTA si el menú es NULL.
 */
estilo_menu_t menu_obtener_estilo(menu_t *menu);

/* ======== FUNCIONES PARA ESTILOS PERSONALIZADOS ======== */

/* Crea un nuevo estilo personalizado.
 * Todos los parámetros son opcionales (pueden ser NULL).
 * Si son NULL, se usan valores por defecto simples.
 * 
 * Los strings son copiados internamente.
 * 
 * Devuelve un puntero al estilo o NULL en caso de error.
 */
estilo_personalizado_t *
estilo_crear(const char *borde_superior, const char *borde_inferior,
	     const char *separador, const char *prefijo_opcion,
	     const char *color_titulo, const char *color_opciones,
	     const char *color_teclas);

/* Destruye un estilo personalizado y libera su memoria.
 * Si el estilo es NULL, no hace nada.
 */
void estilo_destruir(estilo_personalizado_t *estilo);

/* Establece un estilo personalizado en el menú.
 * El menú NO toma ownership del estilo (no lo destruye).
 * El estilo debe mantenerse válido mientras el menú lo use.
 * 
 * Devuelve true si se aplicó correctamente, false en caso de error.
 */
bool menu_establecer_estilo_personalizado(menu_t *menu,
					  estilo_personalizado_t *estilo);

/* Muestra el menú por consola con el estilo configurado.
 *
 * Si el menú es NULL, no imprime nada.
 */
void menu_mostrar(menu_t *menu);

/* Ejecuta el loop principal del menú:
 * 1. Muestra el menú
 * 2. Lee la opción del usuario
 * 3. Ejecuta la acción correspondiente
 * 4. Repite hasta que una acción retorne false
 *
 * - m: el menú a ejecutar
 * - ctx_externo: contexto que se pasa a todas las acciones
 *
 * Si el menú es NULL, no hace nada.
 */
void menu_ejecutar(menu_t *menu, void *ctx_externo);

/* Destruye el menú y libera toda la memoria asociada.
 * NO destruye los submenús automáticamente (deben destruirse manualmente).
 *
 * Si el menú es NULL, no hace nada.
 */
void menu_destruir(menu_t *menu);

#endif // MENU_H