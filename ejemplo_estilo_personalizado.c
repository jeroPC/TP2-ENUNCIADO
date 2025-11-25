#include "src/menu.h"
#include "src/ansi.h"
#include <stdio.h>

bool opcion_salir(void *ctx_interno, void *ctx_externo)
{
	(void)ctx_interno;
	(void)ctx_externo;
	printf("\n¡Adiós!\n");
	return false; // Salir del menú
}

bool opcion_mensaje(void *ctx_interno, void *ctx_externo)
{
	(void)ctx_externo;
	char *mensaje = (char *)ctx_interno;
	printf("\n%s\n\n", mensaje);
	printf("Presiona ENTER para continuar...");
	getchar();
	return true;
}

int main()
{
	// Crear estilo personalizado sin modificar menu.c
	estilo_personalizado_t *mi_estilo = estilo_crear(
		"╔═══════════════════════════════════════╗", // borde superior
		"╚═══════════════════════════════════════╝", // borde inferior
		"─────────────────────────────────────────", // separador
		"  ▸ ", // prefijo opción
		ANSI_COLOR_BOLD ANSI_COLOR_MAGENTA, // color título
		ANSI_COLOR_CYAN, // color opciones
		ANSI_COLOR_YELLOW // color teclas
	);

	if (!mi_estilo) {
		fprintf(stderr, "Error al crear estilo personalizado\n");
		return 1;
	}

	// Crear menú con estilo minimalista inicial
	menu_t *menu = menu_crear("MI MENÚ PERSONALIZADO", ESTILO_MINIMALISTA);
	if (!menu) {
		estilo_destruir(mi_estilo);
		return 1;
	}

	// Agregar opciones
	menu_agregar_opcion(menu, '1', "Ver información", opcion_mensaje, NULL,
			    "Esta es una opción de ejemplo");
	menu_agregar_opcion(menu, '2', "Otra opción", opcion_mensaje, NULL,
			    "Segunda opción del menú");
	menu_agregar_opcion(menu, '3', "Más opciones", opcion_mensaje, NULL,
			    "Tercera opción disponible");
	menu_agregar_opcion(menu, 'Q', "Salir", opcion_salir, NULL, NULL);

	// Aplicar el estilo personalizado creado externamente
	printf("\n=== MENÚ CON ESTILO MINIMALISTA ===\n");
	menu_mostrar(menu);

	printf("\n\n=== APLICANDO ESTILO PERSONALIZADO ===\n");
	menu_establecer_estilo_personalizado(menu, mi_estilo);
	menu_mostrar(menu);

	printf("\n\n=== CAMBIANDO A ESTILO RETRO ===\n");
	menu_cambiar_estilo(menu);
	menu_cambiar_estilo(menu); // Cambiar dos veces para llegar a RETRO
	menu_mostrar(menu);

	printf("\n\n=== VOLVIENDO AL ESTILO PERSONALIZADO ===\n");
	menu_establecer_estilo_personalizado(menu, mi_estilo);
	menu_mostrar(menu);

	// Limpiar
	menu_destruir(menu);
	estilo_destruir(mi_estilo);

	printf("\n\n✅ El usuario creó un estilo personalizado SIN modificar menu.c\n");

	return 0;
}
