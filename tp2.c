#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "src/menu.h"
#include "src/ansi.h"

// Estructura de contexto para pasar datos entre acciones
typedef struct {
    int contador;
    char nombre[50];
} contexto_t;

// Acción simple: Saludar
bool accion_saludar(void *ctx_interno, void *ctx_externo) {
    contexto_t *ctx = (contexto_t *)ctx_externo;
    printf(ANSI_COLOR_GREEN "\n¡Hola");
    if (ctx && ctx->nombre[0] != '\0') {
        printf(" %s", ctx->nombre);
    }
    printf("! Esta es una opción de prueba.\n" ANSI_COLOR_RESET);
    printf("\nPresiona ENTER para continuar...");
    getchar();
    return true;
}

// Acción con contexto interno: Mostrar mensaje personalizado
bool accion_mensaje_personalizado(void *ctx_interno, void *ctx_externo) {
    char *mensaje = (char *)ctx_interno;
    printf(ANSI_COLOR_YELLOW "\n%s\n" ANSI_COLOR_RESET, mensaje);
    printf("\nPresiona ENTER para continuar...");
    getchar();
    return true;
}

// Acción: Incrementar contador
bool accion_incrementar(void *ctx_interno, void *ctx_externo) {
    contexto_t *ctx = (contexto_t *)ctx_externo;
    if (ctx) {
        ctx->contador++;
        printf(ANSI_COLOR_CYAN "\n✓ Contador incrementado: %d\n" ANSI_COLOR_RESET, ctx->contador);
    }
    printf("\nPresiona ENTER para continuar...");
    getchar();
    return true;
}

// Acción: Mostrar contador
bool accion_mostrar_contador(void *ctx_interno, void *ctx_externo) {
    contexto_t *ctx = (contexto_t *)ctx_externo;
    if (ctx) {
        printf(ANSI_COLOR_MAGENTA "\n📊 Valor del contador: %d\n" ANSI_COLOR_RESET, ctx->contador);
    }
    printf("\nPresiona ENTER para continuar...");
    getchar();
    return true;
}

// Acción: Cambiar estilo del menú
bool accion_cambiar_estilo(void *ctx_interno, void *ctx_externo) {
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
            default:
                printf("DESCONOCIDO\n");
        }
        printf(ANSI_COLOR_RESET);
    }
    printf("\nPresiona ENTER para continuar...");
    getchar();
    return true;
}

// Acción: Pedir nombre
bool accion_pedir_nombre(void *ctx_interno, void *ctx_externo) {
    contexto_t *ctx = (contexto_t *)ctx_externo;
    if (ctx) {
        printf("\nIngresa tu nombre: ");
        fgets(ctx->nombre, sizeof(ctx->nombre), stdin);
        // Eliminar el salto de línea
        size_t len = strlen(ctx->nombre);
        if (len > 0 && ctx->nombre[len-1] == '\n') {
            ctx->nombre[len-1] = '\0';
        }
        printf(ANSI_COLOR_GREEN "✓ Nombre guardado: %s\n" ANSI_COLOR_RESET, ctx->nombre);
    }
    printf("\nPresiona ENTER para continuar...");
    getchar();
    return true;
}

// Acción: Salir del submenú
bool accion_volver(void *ctx_interno, void *ctx_externo) {
    printf(ANSI_COLOR_YELLOW "\n← Volviendo al menú anterior...\n" ANSI_COLOR_RESET);
    return false;
}

// Acción: Salir del programa
bool accion_salir(void *ctx_interno, void *ctx_externo) {
    printf(ANSI_COLOR_RED "\n¡Hasta luego! Saliendo del programa...\n" ANSI_COLOR_RESET);
    return false;
}

int main() {
    printf(ANSI_COLOR_BOLD ANSI_COLOR_CYAN);
    printf("\n╔═══════════════════════════════════════════╗\n");
    printf("║   PRUEBA DEL TDA MENÚ - TP2 ALGO2        ║\n");
    printf("╚═══════════════════════════════════════════╝\n");
    printf(ANSI_COLOR_RESET "\n");

    // Crear contexto externo para compartir datos
    contexto_t contexto = {0};
    contexto.contador = 0;
    contexto.nombre[0] = '\0';

    // Crear menú principal
    menu_t *menu_principal = menu_crear("MENÚ PRINCIPAL", ESTILO_MINIMALISTA);
    if (!menu_principal) {
        fprintf(stderr, "Error al crear el menú principal\n");
        return 1;
    }

    // Crear submenú de opciones
    menu_t *submenu_opciones = menu_crear("SUBMENÚ - OPCIONES", ESTILO_BORDES);
    if (!submenu_opciones) {
        fprintf(stderr, "Error al crear el submenú\n");
        menu_destruir(menu_principal);
        return 1;
    }

    // Agregar opciones al submenú
    menu_agregar_opcion(submenu_opciones, 'I', "Incrementar contador", 
                        accion_incrementar, NULL, NULL);
    menu_agregar_opcion(submenu_opciones, 'M', "Mostrar contador", 
                        accion_mostrar_contador, NULL, NULL);
    menu_agregar_opcion(submenu_opciones, 'N', "Ingresar nombre", 
                        accion_pedir_nombre, NULL, NULL);
    menu_agregar_opcion(submenu_opciones, 'V', "Volver al menú principal", 
                        accion_volver, NULL, NULL);

    // Mensajes personalizados con contexto interno
    char *msg1 = "¡Este mensaje usa contexto interno!";
    char *msg2 = "Los contextos permiten pasar datos a las acciones.";

    // Agregar opciones al menú principal
    menu_agregar_opcion(menu_principal, 'S', "Saludar", 
                        accion_saludar, NULL, NULL);
    menu_agregar_opcion(menu_principal, 'O', "Ir al submenú de opciones", 
                        NULL, submenu_opciones, NULL);
    menu_agregar_opcion(menu_principal, 'E', "Cambiar estilo del menú", 
                        accion_cambiar_estilo, NULL, menu_principal);
    menu_agregar_opcion(menu_principal, '1', "Mensaje 1 (con contexto interno)", 
                        accion_mensaje_personalizado, NULL, msg1);
    menu_agregar_opcion(menu_principal, '2', "Mensaje 2 (con contexto interno)", 
                        accion_mensaje_personalizado, NULL, msg2);
    menu_agregar_opcion(menu_principal, 'Q', "Salir del programa", 
                        accion_salir, NULL, NULL);

    // Ejecutar el menú principal pasando el contexto externo
    menu_ejecutar(menu_principal, &contexto);

    // Liberar memoria
    menu_destruir(submenu_opciones);
    menu_destruir(menu_principal);

    printf(ANSI_COLOR_GREEN "\n✓ Programa finalizado correctamente.\n" ANSI_COLOR_RESET);
    return 0;
}