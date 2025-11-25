#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/menu.h"
#include "src/juego.h"
#include "src/ansi.h"

#define MAX_BUFFER 256

typedef struct {
	juego_t *juego;
	char archivo_cargado[MAX_BUFFER];
	bool archivo_presente;
	estilo_personalizado_t *estilo_custom;
} contexto_juego_t;

/* ========== FUNCIONES AUXILIARES PRIVADAS ========== */

static void limpiar_pantalla(void)
{
	printf(ANSI_CLEAR_SCREEN "\033[H");
	fflush(stdout);
}

static void pausar(void)
{
	printf("\n" ANSI_COLOR_CYAN
	       "Presiona ENTER para continuar..." ANSI_COLOR_RESET);
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
}

static void limpiar_buffer(void)
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
}

static void eliminar_newline(char *str)
{
	size_t len = strlen(str);
	if (len > 0 && str[len - 1] == '\n') {
		str[len - 1] = '\0';
	}
}

static bool verificar_archivo_cargado(contexto_juego_t *ctx)
{
	if (!ctx->archivo_presente) {
		limpiar_pantalla();
		printf(ANSI_COLOR_RED
		       "\n✗ Primero debes cargar un archivo\n" ANSI_COLOR_RESET);
		pausar();
		return false;
	}
	return true;
}

static const char *obtener_color_por_indice(int indice)
{
	const char *colores[] = { ANSI_COLOR_RED,     ANSI_COLOR_GREEN,
				  ANSI_COLOR_BLUE,    ANSI_COLOR_YELLOW,
				  ANSI_COLOR_MAGENTA, ANSI_COLOR_CYAN,
				  ANSI_COLOR_WHITE };
	return (indice >= 1 && indice <= 7) ? colores[indice - 1] :
					      ANSI_COLOR_WHITE;
}

static void mostrar_menu_colores(void)
{
	printf("1) Rojo\n");
	printf("2) Verde\n");
	printf("3) Azul\n");
	printf("4) Amarillo\n");
	printf("5) Magenta\n");
	printf("6) Cyan\n");
	printf("7) Blanco\n");
}

static estilo_personalizado_t *crear_estilo_interactivo(void)
{
	limpiar_pantalla();
	printf(ANSI_COLOR_BOLD ANSI_COLOR_MAGENTA
	       "\n╔═══════════════════════════════════════════════╗\n");
	printf("║  CREADOR DE ESTILOS PERSONALIZADOS           ║\n");
	printf("╚═══════════════════════════════════════════════╝\n" ANSI_COLOR_RESET);

	char borde_superior[256];
	char borde_inferior[256];
	char separador[256];
	char prefijo[64];

	printf("\n" ANSI_COLOR_YELLOW "Ejemplos de bordes:\n" ANSI_COLOR_RESET);
	printf("  ╔══════════════════════════════════════╗\n");
	printf("  ╭━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╮\n");
	printf("  +--------------------------------------+\n");
	printf("  ========================================\n");
	printf("\n" ANSI_COLOR_CYAN
	       "Ingresa los caracteres para el borde superior: " ANSI_COLOR_RESET);
	if (!fgets(borde_superior, sizeof(borde_superior), stdin)) {
		return NULL;
	}
	borde_superior[strcspn(borde_superior, "\n")] = 0;

	printf(ANSI_COLOR_CYAN
	       "Ingresa los caracteres para el borde inferior: " ANSI_COLOR_RESET);
	if (!fgets(borde_inferior, sizeof(borde_inferior), stdin)) {
		return NULL;
	}
	borde_inferior[strcspn(borde_inferior, "\n")] = 0;

	printf("\n" ANSI_COLOR_YELLOW
	       "Ejemplos de separadores:\n" ANSI_COLOR_RESET);
	printf("  ╠══════════════════════════════════════╣\n");
	printf("  ├────────────────────────────────────┤\n");
	printf("  +--------------------------------------+\n");
	printf("  ----------------------------------------\n");
	printf("\n" ANSI_COLOR_CYAN
	       "Ingresa los caracteres para el separador: " ANSI_COLOR_RESET);
	if (!fgets(separador, sizeof(separador), stdin)) {
		return NULL;
	}
	separador[strcspn(separador, "\n")] = 0;

	printf("\n" ANSI_COLOR_YELLOW
	       "Ejemplos de prefijos:\n" ANSI_COLOR_RESET);
	printf("  '> ' '* ' '⚡ ' '▸ ' '→ ' '• '\n");
	printf("\n" ANSI_COLOR_CYAN
	       "Ingresa el prefijo de las opciones: " ANSI_COLOR_RESET);
	if (!fgets(prefijo, sizeof(prefijo), stdin)) {
		return NULL;
	}
	prefijo[strcspn(prefijo, "\n")] = 0;

	printf("\n" ANSI_COLOR_BOLD
	       "Selecciona el color del título:\n" ANSI_COLOR_RESET);
	mostrar_menu_colores();
	printf(ANSI_COLOR_CYAN "Opción: " ANSI_COLOR_RESET);

	int opcion_titulo;
	if (scanf("%d", &opcion_titulo) != 1) {
		limpiar_buffer();
		return NULL;
	}
	limpiar_buffer();

	printf("\n" ANSI_COLOR_BOLD
	       "Selecciona el color de las opciones:\n" ANSI_COLOR_RESET);
	mostrar_menu_colores();
	printf(ANSI_COLOR_CYAN "Opción: " ANSI_COLOR_RESET);

	int opcion_opciones;
	if (scanf("%d", &opcion_opciones) != 1) {
		limpiar_buffer();
		return NULL;
	}
	limpiar_buffer();

	printf("\n" ANSI_COLOR_BOLD
	       "Selecciona el color de las teclas:\n" ANSI_COLOR_RESET);
	mostrar_menu_colores();
	printf(ANSI_COLOR_CYAN "Opción: " ANSI_COLOR_RESET);

	int opcion_teclas;
	if (scanf("%d", &opcion_teclas) != 1) {
		limpiar_buffer();
		return NULL;
	}
	limpiar_buffer();

	const char *color_titulo = obtener_color_por_indice(opcion_titulo);
	const char *color_opciones = obtener_color_por_indice(opcion_opciones);
	const char *color_teclas = obtener_color_por_indice(opcion_teclas);

	printf("\n" ANSI_COLOR_CYAN
	       "¿Usar negrita en el título? (s/n): " ANSI_COLOR_RESET);
	char negrita_titulo;
	if (scanf(" %c", &negrita_titulo) != 1)
		negrita_titulo = 'n';
	limpiar_buffer();

	printf(ANSI_COLOR_CYAN
	       "¿Usar negrita en las teclas? (s/n): " ANSI_COLOR_RESET);
	char negrita_teclas;
	if (scanf(" %c", &negrita_teclas) != 1)
		negrita_teclas = 'n';
	limpiar_buffer();

	char *color_titulo_final = malloc(128);
	char *color_teclas_final = malloc(128);

	if (!color_titulo_final || !color_teclas_final) {
		free(color_titulo_final);
		free(color_teclas_final);
		return NULL;
	}

	snprintf(color_titulo_final, 128, "%s%s",
		 (negrita_titulo == 's' || negrita_titulo == 'S') ?
			 ANSI_COLOR_BOLD :
			 "",
		 color_titulo);

	snprintf(color_teclas_final, 128, "%s%s",
		 (negrita_teclas == 's' || negrita_teclas == 'S') ?
			 ANSI_COLOR_BOLD :
			 "",
		 color_teclas);

	estilo_personalizado_t *estilo = estilo_crear(
		borde_superior, borde_inferior, separador, prefijo,
		color_titulo_final, color_opciones, color_teclas_final);

	free(color_titulo_final);
	free(color_teclas_final);

	if (estilo) {
		printf(ANSI_COLOR_GREEN
		       "\n✓ ¡Estilo personalizado creado exitosamente!\n" ANSI_COLOR_RESET);
	} else {
		printf(ANSI_COLOR_RED
		       "\n✗ Error al crear el estilo personalizado\n" ANSI_COLOR_RESET);
	}

	pausar();
	return estilo;
}

/* ========== ACCIONES DEL MENÚ ========== */

static bool accion_cargar_archivo(void *ctx_interno, void *ctx_externo)
{
	(void)ctx_interno;
	contexto_juego_t *ctx = (contexto_juego_t *)ctx_externo;

	limpiar_pantalla();
	printf(ANSI_COLOR_BOLD ANSI_COLOR_CYAN
	       "\n═══ CARGAR ARCHIVO ═══\n" ANSI_COLOR_RESET);

	printf("\nIngresa el nombre del archivo (ej: ejemplos/normal.csv): ");
	char archivo[MAX_BUFFER];
	if (!fgets(archivo, MAX_BUFFER, stdin)) {
		printf(ANSI_COLOR_RED
		       "\n✗ Error al leer el nombre del archivo\n" ANSI_COLOR_RESET);
		pausar();
		return true;
	}

	eliminar_newline(archivo);

	int cantidad = juego_cargar_pokedex(ctx->juego, archivo);

	if (cantidad > 0) {
		printf(ANSI_COLOR_GREEN
		       "\n✓ Archivo cargado exitosamente: %d pokemones\n" ANSI_COLOR_RESET,
		       cantidad);
		strncpy(ctx->archivo_cargado, archivo, MAX_BUFFER - 1);
		ctx->archivo_cargado[MAX_BUFFER - 1] = '\0';
		ctx->archivo_presente = true;
	} else {
		printf(ANSI_COLOR_RED
		       "\n✗ Error al cargar el archivo\n" ANSI_COLOR_RESET);
	}

	pausar();
	return true;
}

static void mostrar_pokemon(pokemon_t *pokemon, void *ctx)
{
	(void)ctx;
	printf("  [ID: %3d] %-15s | Tipo: %-4s | ATK: %3d | DEF: %3d | VEL: %3d\n",
	       pokemon->id, pokemon->nombre, juego_tipo_a_string(pokemon->tipo),
	       pokemon->ataque, pokemon->defensa, pokemon->velocidad);
}

static bool accion_buscar_nombre(void *ctx_interno, void *ctx_externo)
{
	(void)ctx_interno;
	contexto_juego_t *ctx = (contexto_juego_t *)ctx_externo;

	if (!verificar_archivo_cargado(ctx))
		return true;

	limpiar_pantalla();
	printf(ANSI_COLOR_BOLD ANSI_COLOR_CYAN
	       "\n═══ BUSCAR POR NOMBRE ═══\n" ANSI_COLOR_RESET);

	printf("\nIngresa el nombre (o parte del nombre): ");
	char nombre[MAX_BUFFER];
	if (!fgets(nombre, MAX_BUFFER, stdin)) {
		printf(ANSI_COLOR_RED
		       "\n✗ Error al leer el nombre\n" ANSI_COLOR_RESET);
		pausar();
		return true;
	}

	eliminar_newline(nombre);

	pokemon_t *resultados[20];
	size_t encontrados =
		juego_buscar_por_nombre(ctx->juego, nombre, resultados, 20);

	if (encontrados > 0) {
		printf(ANSI_COLOR_GREEN
		       "\n✓ Se encontraron %zu pokemon(es):\n\n" ANSI_COLOR_RESET,
		       encontrados);
		for (size_t i = 0; i < encontrados; i++) {
			mostrar_pokemon(resultados[i], NULL);
		}
	} else {
		printf(ANSI_COLOR_YELLOW
		       "\n⚠ No se encontraron pokemones con ese nombre\n" ANSI_COLOR_RESET);
	}

	pausar();
	return true;
}

static bool accion_buscar_id(void *ctx_interno, void *ctx_externo)
{
	(void)ctx_interno;
	contexto_juego_t *ctx = (contexto_juego_t *)ctx_externo;

	if (!verificar_archivo_cargado(ctx))
		return true;

	limpiar_pantalla();
	printf(ANSI_COLOR_BOLD ANSI_COLOR_CYAN
	       "\n═══ BUSCAR POR ID ═══\n" ANSI_COLOR_RESET);

	printf("\nIngresa el ID: ");
	int id;
	if (scanf("%d", &id) != 1) {
		limpiar_buffer();
		printf(ANSI_COLOR_RED "\n✗ ID inválido\n" ANSI_COLOR_RESET);
		pausar();
		return true;
	}
	limpiar_buffer();

	pokemon_t *pokemon = juego_buscar_por_id(ctx->juego, id);

	if (pokemon) {
		printf(ANSI_COLOR_GREEN
		       "\n✓ Pokemon encontrado:\n\n" ANSI_COLOR_RESET);
		mostrar_pokemon(pokemon, NULL);
	} else {
		printf(ANSI_COLOR_YELLOW
		       "\n⚠ No se encontró un pokemon con ID %d\n" ANSI_COLOR_RESET,
		       id);
	}

	pausar();
	return true;
}

static bool accion_mostrar_nombre(void *ctx_interno, void *ctx_externo)
{
	(void)ctx_interno;
	contexto_juego_t *ctx = (contexto_juego_t *)ctx_externo;

	if (!verificar_archivo_cargado(ctx))
		return true;

	limpiar_pantalla();
	printf(ANSI_COLOR_BOLD ANSI_COLOR_CYAN
	       "\n═══ LISTADO POR NOMBRE ═══\n\n" ANSI_COLOR_RESET);

	juego_listar_por_nombre(ctx->juego, mostrar_pokemon, NULL);

	pausar();
	return true;
}

static bool accion_mostrar_id(void *ctx_interno, void *ctx_externo)
{
	(void)ctx_interno;
	contexto_juego_t *ctx = (contexto_juego_t *)ctx_externo;

	if (!verificar_archivo_cargado(ctx))
		return true;

	limpiar_pantalla();
	printf(ANSI_COLOR_BOLD ANSI_COLOR_CYAN
	       "\n═══ LISTADO POR ID ═══\n\n" ANSI_COLOR_RESET);

	juego_listar_por_id(ctx->juego, mostrar_pokemon, NULL);

	pausar();
	return true;
}

static void dibujar_tablero(juego_t *juego)
{
	carta_t cartas[TOTAL_CARTAS];
	size_t cantidad = juego_obtener_tablero(juego, cartas);

	printf("\n" ANSI_COLOR_BOLD
	       "╔════════════════════ TABLERO ════════════════════╗\n" ANSI_COLOR_RESET);

	for (size_t i = 0; i < cantidad; i++) {
		if (i % 6 == 0)
			printf("║ ");

		if (cartas[i].emparejada) {
			printf(ANSI_COLOR_GREEN " ✓✓ " ANSI_COLOR_RESET);
		} else if (cartas[i].descubierta) {
			printf(ANSI_COLOR_YELLOW "%3s " ANSI_COLOR_RESET,
			       cartas[i].pokemon->nombre);
		} else {
			printf(ANSI_COLOR_CYAN "[%2d]" ANSI_COLOR_RESET,
			       cartas[i].posicion);
		}

		if (i % 6 == 5)
			printf(" ║\n");
	}

	printf("╚═════════════════════════════════════════════════╝\n");
}

static void mostrar_ultimas_jugadas(juego_t *juego)
{
	jugada_t jugadas[MAX_JUGADAS_MOSTRADAS];
	size_t cantidad = juego_obtener_ultimas_jugadas(juego, jugadas,
							MAX_JUGADAS_MOSTRADAS);

	if (cantidad > 0) {
		printf("\n" ANSI_COLOR_BOLD
		       "═══ ÚLTIMAS JUGADAS ═══\n" ANSI_COLOR_RESET);
		for (size_t i = 0; i < cantidad; i++) {
			printf("  J%d: Cartas %d-%d → %s %s\n",
			       jugadas[i].jugador, jugadas[i].carta1,
			       jugadas[i].carta2, jugadas[i].nombre_pokemon,
			       jugadas[i].acierto ?
				       ANSI_COLOR_GREEN "✓" ANSI_COLOR_RESET :
				       ANSI_COLOR_RED "✗" ANSI_COLOR_RESET);
		}
	}
}

static void ejecutar_partida(juego_t *juego)
{
	while (!juego_partida_terminada(juego)) {
		limpiar_pantalla();

		int jugador = juego_jugador_actual(juego);
		printf(ANSI_COLOR_BOLD ANSI_COLOR_MAGENTA
		       "\n╔═══════════════════════════════════════╗\n");
		printf("║     JUEGO DE MEMORIA POKEMON          ║\n");
		printf("╠═══════════════════════════════════════╣\n");
		printf("║  Jugador 1: %2d pts | Jugador 2: %2d pts  ║\n",
		       juego_obtener_puntuacion(juego, 1),
		       juego_obtener_puntuacion(juego, 2));
		printf("║  Turno: Jugador %d                      ║\n",
		       jugador);
		printf("╚═══════════════════════════════════════╝\n" ANSI_COLOR_RESET);

		dibujar_tablero(juego);
		mostrar_ultimas_jugadas(juego);

		printf("\n" ANSI_COLOR_CYAN
		       "Selecciona una carta (0-17): " ANSI_COLOR_RESET);
		int posicion;
		if (scanf("%d", &posicion) != 1) {
			limpiar_buffer();
			printf(ANSI_COLOR_RED
			       "\n✗ Posición inválida\n" ANSI_COLOR_RESET);
			pausar();
			continue;
		}
		limpiar_buffer();

		int resultado = juego_seleccionar_carta(juego, posicion);

		if (resultado == -1) {
			printf(ANSI_COLOR_RED
			       "\n✗ Carta inválida o ya seleccionada\n" ANSI_COLOR_RESET);
			pausar();
		} else if (resultado == 0) {
			printf(ANSI_COLOR_YELLOW
			       "\n→ Primera carta seleccionada\n" ANSI_COLOR_RESET);
			pausar();
		} else if (resultado == 1) {
			limpiar_pantalla();
			dibujar_tablero(juego);
			printf(ANSI_COLOR_GREEN
			       "\n✓ ¡Par encontrado! El Jugador %d gana un punto\n" ANSI_COLOR_RESET,
			       jugador);
			pausar();
		} else if (resultado == -2) {
			limpiar_pantalla();
			dibujar_tablero(juego);
			printf(ANSI_COLOR_RED
			       "\n✗ No es un par. Turno del Jugador %d\n" ANSI_COLOR_RESET,
			       juego_jugador_actual(juego));
			pausar();
		}
	}

	limpiar_pantalla();
	printf(ANSI_COLOR_BOLD ANSI_COLOR_MAGENTA
	       "\n╔═══════════════════════════════════════╗\n");
	printf("║        ¡PARTIDA TERMINADA!            ║\n");
	printf("╚═══════════════════════════════════════╝\n" ANSI_COLOR_RESET);

	int ganador = juego_obtener_ganador(juego);
	int p1 = juego_obtener_puntuacion(juego, 1);
	int p2 = juego_obtener_puntuacion(juego, 2);

	printf("\n" ANSI_COLOR_BOLD "PUNTUACIÓN FINAL:\n" ANSI_COLOR_RESET);
	printf("  Jugador 1: %d puntos\n", p1);
	printf("  Jugador 2: %d puntos\n", p2);

	if (ganador == 0) {
		printf(ANSI_COLOR_YELLOW "\n🤝 ¡EMPATE!\n" ANSI_COLOR_RESET);
	} else {
		printf(ANSI_COLOR_GREEN
		       "\n🏆 ¡GANADOR: Jugador %d!\n" ANSI_COLOR_RESET,
		       ganador);
	}

	pausar();
}

static bool accion_jugar(void *ctx_interno, void *ctx_externo)
{
	(void)ctx_interno;
	contexto_juego_t *ctx = (contexto_juego_t *)ctx_externo;

	if (!verificar_archivo_cargado(ctx))
		return true;

	if (juego_cantidad_pokemones(ctx->juego) < 9) {
		limpiar_pantalla();
		printf(ANSI_COLOR_RED
		       "\n✗ Se necesitan al menos 9 pokemones para jugar\n" ANSI_COLOR_RESET);
		pausar();
		return true;
	}

	limpiar_pantalla();
	if (!juego_iniciar_partida(ctx->juego, 0)) {
		printf(ANSI_COLOR_RED
		       "\n✗ Error al iniciar la partida\n" ANSI_COLOR_RESET);
		pausar();
		return true;
	}

	ejecutar_partida(ctx->juego);
	juego_finalizar_partida(ctx->juego);

	return true;
}

static bool accion_jugar_semilla(void *ctx_interno, void *ctx_externo)
{
	(void)ctx_interno;
	contexto_juego_t *ctx = (contexto_juego_t *)ctx_externo;

	if (!verificar_archivo_cargado(ctx))
		return true;

	limpiar_pantalla();
	printf(ANSI_COLOR_BOLD ANSI_COLOR_CYAN
	       "\n═══ JUGAR CON SEMILLA ═══\n" ANSI_COLOR_RESET);

	printf("\nIngresa la semilla (número entero): ");
	unsigned int semilla;
	if (scanf("%u", &semilla) != 1) {
		limpiar_buffer();
		printf(ANSI_COLOR_RED
		       "\n✗ Semilla inválida\n" ANSI_COLOR_RESET);
		pausar();
		return true;
	}
	limpiar_buffer();

	if (!juego_iniciar_partida(ctx->juego, semilla)) {
		printf(ANSI_COLOR_RED
		       "\n✗ Error al iniciar la partida\n" ANSI_COLOR_RESET);
		pausar();
		return true;
	}

	ejecutar_partida(ctx->juego);
	juego_finalizar_partida(ctx->juego);

	return true;
}

static bool accion_volver(void *ctx_interno, void *ctx_externo)
{
	(void)ctx_interno;
	(void)ctx_externo;
	return false;
}

static bool accion_salir(void *ctx_interno, void *ctx_externo)
{
	(void)ctx_interno;
	(void)ctx_externo;
	limpiar_pantalla();
	printf(ANSI_COLOR_GREEN
	       "\n¡Gracias por jugar! Hasta pronto.\n" ANSI_COLOR_RESET);
	return false;
}

static bool accion_cambiar_estilo(void *ctx_interno, void *ctx_externo)
{
	(void)ctx_externo;
	menu_t *menu = (menu_t *)ctx_interno;
	if (menu) {
		menu_cambiar_estilo(menu);
		estilo_menu_t estilo = menu_obtener_estilo(menu);
		printf(ANSI_COLOR_BLUE "\n✓ Estilo cambiado a: ");
		switch (estilo) {
		case ESTILO_MINIMALISTA:
			printf("MINIMALISTA\n");
			break;
		case ESTILO_BORDES:
			printf("BORDES\n");
			break;
		case ESTILO_RETRO:
			printf("RETRO\n");
			break;
		case ESTILO_PERSONALIZADO:
			printf("PERSONALIZADO\n");
			break;
		default:
			printf("DESCONOCIDO\n");
		}
		printf(ANSI_COLOR_RESET);
	}
	pausar();
	return true;
}

static bool accion_estilo_personalizado(void *ctx_interno, void *ctx_externo)
{
	menu_t *menu = (menu_t *)ctx_interno;
	contexto_juego_t *ctx = (contexto_juego_t *)ctx_externo;

	if (menu && ctx && ctx->estilo_custom) {
		menu_establecer_estilo_personalizado(menu, ctx->estilo_custom);
		printf(ANSI_COLOR_GREEN
		       "\n✓ Estilo predefinido aplicado\n" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_YELLOW
		       "Este estilo fue creado externamente sin modificar menu.c\n" ANSI_COLOR_RESET);
	} else {
		printf(ANSI_COLOR_RED
		       "\n✗ Error: No hay estilo personalizado disponible\n" ANSI_COLOR_RESET);
	}
	pausar();
	return true;
}

static bool accion_crear_estilo_interactivo(void *ctx_interno,
					    void *ctx_externo)
{
	menu_t *menu = (menu_t *)ctx_interno;
	contexto_juego_t *ctx = (contexto_juego_t *)ctx_externo;

	if (!ctx) {
		printf(ANSI_COLOR_RED
		       "\n✗ Error: Contexto no válido\n" ANSI_COLOR_RESET);
		pausar();
		return true;
	}

	estilo_personalizado_t *nuevo_estilo = crear_estilo_interactivo();

	if (nuevo_estilo) {
		if (ctx->estilo_custom) {
			estilo_destruir(ctx->estilo_custom);
		}

		ctx->estilo_custom = nuevo_estilo;

		if (menu) {
			menu_establecer_estilo_personalizado(
				menu, ctx->estilo_custom);
			limpiar_pantalla();
			printf(ANSI_COLOR_GREEN
			       "\n✓ Tu estilo personalizado ha sido aplicado al menú\n" ANSI_COLOR_RESET);
			pausar();
		}
	}

	return true;
}

/* ========== MAIN ========== */

int main(int argc, char *argv[])
{
	contexto_juego_t contexto = { 0 };
	contexto.juego = juego_crear();
	contexto.archivo_presente = false;

	contexto.estilo_custom =
		estilo_crear("╭━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╮",
			     "╰━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╯",
			     "├────────────────────────────────────┤", "  ⚡ ",
			     ANSI_COLOR_BOLD ANSI_COLOR_CYAN, ANSI_COLOR_WHITE,
			     ANSI_COLOR_YELLOW ANSI_COLOR_BOLD);

	if (!contexto.juego) {
		fprintf(stderr, "Error al crear el juego\n");
		estilo_destruir(contexto.estilo_custom);
		return 1;
	}

	if (!contexto.estilo_custom) {
		fprintf(stderr, ANSI_COLOR_YELLOW
			"⚠ Advertencia: No se pudo crear el estilo personalizado\n" ANSI_COLOR_RESET);
	}

	if (argc > 1) {
		int cantidad = juego_cargar_pokedex(contexto.juego, argv[1]);
		if (cantidad > 0) {
			strncpy(contexto.archivo_cargado, argv[1],
				MAX_BUFFER - 1);
			contexto.archivo_presente = true;
			printf(ANSI_COLOR_GREEN
			       "✓ Archivo cargado: %d pokemones\n" ANSI_COLOR_RESET,
			       cantidad);
		}
	}

	menu_t *menu_principal =
		menu_crear("MENÚ PRINCIPAL - POKEMON MEMORY", ESTILO_BORDES);
	if (!menu_principal) {
		juego_destruir(contexto.juego);
		return 1;
	}

	menu_t *submenu_buscar = menu_crear("BUSCAR POKEMON", ESTILO_RETRO);
	menu_agregar_opcion(submenu_buscar, 'N', "Buscar por nombre",
			    accion_buscar_nombre, NULL, NULL);
	menu_agregar_opcion(submenu_buscar, 'I', "Buscar por ID",
			    accion_buscar_id, NULL, NULL);
	menu_agregar_opcion(submenu_buscar, 'A', "Volver al menú anterior",
			    accion_volver, NULL, NULL);

	menu_t *submenu_mostrar = menu_crear("MOSTRAR POKEDEX", ESTILO_RETRO);
	menu_agregar_opcion(submenu_mostrar, 'N', "Mostrar por nombre",
			    accion_mostrar_nombre, NULL, NULL);
	menu_agregar_opcion(submenu_mostrar, 'I', "Mostrar por ID",
			    accion_mostrar_id, NULL, NULL);
	menu_agregar_opcion(submenu_mostrar, 'A', "Volver al menú anterior",
			    accion_volver, NULL, NULL);

	menu_agregar_opcion(menu_principal, 'C', "Cargar archivo",
			    accion_cargar_archivo, NULL, NULL);
	menu_agregar_opcion(menu_principal, 'B', "Buscar pokemon", NULL,
			    submenu_buscar, NULL);
	menu_agregar_opcion(menu_principal, 'M', "Mostrar pokedex", NULL,
			    submenu_mostrar, NULL);
	menu_agregar_opcion(menu_principal, 'J', "Jugar (semilla aleatoria)",
			    accion_jugar, NULL, NULL);
	menu_agregar_opcion(menu_principal, 'S', "Jugar con semilla",
			    accion_jugar_semilla, NULL, NULL);
	menu_agregar_opcion(menu_principal, 'E', "Cambiar estilo de menú",
			    accion_cambiar_estilo, NULL, menu_principal);
	menu_agregar_opcion(menu_principal, 'P', "Aplicar estilo predefinido",
			    accion_estilo_personalizado, NULL, menu_principal);
	menu_agregar_opcion(menu_principal, 'X',
			    "Crear tu propio estilo (Interactivo)",
			    accion_crear_estilo_interactivo, NULL,
			    menu_principal);
	menu_agregar_opcion(menu_principal, 'Q', "Salir", accion_salir, NULL,
			    NULL);

	menu_ejecutar(menu_principal, &contexto);

	menu_destruir(submenu_buscar);
	menu_destruir(submenu_mostrar);
	menu_destruir(menu_principal);
	estilo_destruir(contexto.estilo_custom);
	juego_destruir(contexto.juego);

	return 0;
}
