#include "menu.h"
#include "ansi.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



typedef struct opcion_menu {
    char tecla;
    char *descripcion;
    accion_menu_t accion;
    menu_t *submenu;
    void *ctx_interno;
} opcion_menu_t;

struct menu {
    char *titulo;
    estilo_menu_t estilo;
    opcion_menu_t *opciones; // Array dinámico de opciones
    size_t cantidad_opciones;
    size_t capacidad_opciones;
};

menu_t *menu_crear(const char *titulo, estilo_menu_t estilo){
    menu_t *menu = malloc(sizeof(menu_t));
    if(!menu) return NULL;

    if(titulo){
        menu->titulo = strdup(titulo);
    }else{
        menu->titulo = strdup("MENU");
    }
    if (!menu->titulo) {
        free(menu);
        return NULL;
    }

    menu->estilo = estilo;
    menu->cantidad_opciones = 0;
    menu->capacidad_opciones = 4; // capacidad inicial
    menu->opciones = malloc(menu->capacidad_opciones * sizeof(opcion_menu_t));
    if (!menu->opciones) {
        free(menu->titulo);
        free(menu);
        return NULL;
    }

    return menu;
}


static bool opcion_menu_asignar(opcion_menu_t *opcion, char tecla, const char *descripcion,
                                accion_menu_t accion, menu_t *submenu, void *ctx_interno) {
    opcion->tecla = tecla;
    opcion->accion = accion;
    opcion->submenu = submenu;
    opcion->ctx_interno = ctx_interno;
    opcion->descripcion = strdup(descripcion);
    return opcion->descripcion != NULL;
}


bool menu_agregar_opcion(menu_t *menu, char tecla, const char *descripcion,
                        accion_menu_t accion, menu_t *submenu, 
                        void *ctx_interno) {
    if (!menu || !descripcion || (!accion && !submenu)) return false;

    for (size_t i = 0; i < menu->cantidad_opciones; i++) {
        if (menu->opciones[i].tecla == tecla) {
            free(menu->opciones[i].descripcion);
            return opcion_menu_asignar(&menu->opciones[i], tecla, descripcion, accion, submenu, ctx_interno);
        }
    }

    if (menu->cantidad_opciones == menu->capacidad_opciones) {
        size_t nueva_capacidad = menu->capacidad_opciones * 2;
        opcion_menu_t *nuevo = realloc(menu->opciones, nueva_capacidad * sizeof(opcion_menu_t));
        if (!nuevo) return false;
        menu->opciones = nuevo;
        menu->capacidad_opciones = nueva_capacidad;
    }

    if (!opcion_menu_asignar(&menu->opciones[menu->cantidad_opciones], tecla, descripcion, accion, submenu, ctx_interno))
        return false;
    menu->cantidad_opciones++;

    return true;
}



void menu_cambiar_estilo(menu_t *menu){
    if(!menu) return ;
    menu->estilo = (menu->estilo +1) % TOTAL_ESTILOS;
}



estilo_menu_t menu_obtener_estilo(menu_t *menu){
    if(!menu) return ESTILO_MINIMALISTA;
    return menu->estilo;
}

static void mostrar_estilo_minimalista(menu_t *menu) {
    printf("\n%s\n\n", menu->titulo);
    for (size_t i = 0; i < menu->cantidad_opciones; i++) {
        printf("%c) %s\n", menu->opciones[i].tecla, menu->opciones[i].descripcion);
    }
}

static void mostrar_estilo_bordes(menu_t *menu) {
    printf("\n" ANSI_COLOR_CYAN "╔══════════════════════════════════════╗\n");
    printf("║ " ANSI_COLOR_BOLD "%s" ANSI_COLOR_RESET ANSI_COLOR_CYAN " ║\n", menu->titulo);
    printf("╠══════════════════════════════════════╣\n" ANSI_COLOR_RESET);
    
    for (size_t i = 0; i < menu->cantidad_opciones; i++) {
        printf(ANSI_COLOR_CYAN "║ " ANSI_COLOR_YELLOW "%c" ANSI_COLOR_RESET ") %s\n", 
               menu->opciones[i].tecla, menu->opciones[i].descripcion);
    }
    
    printf(ANSI_COLOR_CYAN "╚══════════════════════════════════════╝\n" ANSI_COLOR_RESET);
}

static void mostrar_estilo_retro(menu_t *menu) {
    printf("\n" ANSI_COLOR_GREEN "+--------------------------------------+\n");
    printf("| " ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET ANSI_COLOR_GREEN " |\n", menu->titulo);
    printf("+--------------------------------------+\n" ANSI_COLOR_RESET);
    
    for (size_t i = 0; i < menu->cantidad_opciones; i++) {
        printf(ANSI_COLOR_GREEN "| " ANSI_COLOR_MAGENTA "[%c]" ANSI_COLOR_RESET " %s\n", 
               menu->opciones[i].tecla, menu->opciones[i].descripcion);
    }
    
    printf(ANSI_COLOR_GREEN "+--------------------------------------+\n" ANSI_COLOR_RESET);
}


void menu_mostrar(menu_t *menu){
    if(!menu) return;

    switch(menu->estilo) {
        case ESTILO_MINIMALISTA:
            mostrar_estilo_minimalista(menu);
            break;
        case ESTILO_BORDES:
            mostrar_estilo_bordes(menu);
            break;
        case ESTILO_RETRO:
            mostrar_estilo_retro(menu);
            break;
        default:
            mostrar_estilo_minimalista(menu);
            break;
    }
}


void menu_ejecutar(menu_t *menu ,void *ctx_externo){
    if(!menu) return ;
    bool continuar = true;
    while(continuar){
        menu_mostrar(menu);
        printf("\nselecciona una opcion:" );
        char opcion;
        scanf("%c",&opcion);
        
        bool encontrada = false;
        size_t i;
        for(i = 0; i < menu->cantidad_opciones ;i++)
            if(menu->opciones[i].tecla == opcion){
                encontrada = true;
                if(menu->opciones[i].accion){
                    continuar = menu->opciones[i].accion(menu->opciones[i].ctx_interno, ctx_externo);
                }else if(menu->opciones[i].submenu){
                    menu_ejecutar(menu->opciones[i].submenu, ctx_externo);
                }else{
                    printf("error");
                }
            }
        if (!encontrada){
            printf("Opción inválida.\n");
        }
    }
   
}








void menu_destruir(menu_t *menu) {
    if (!menu) return;

    // Liberar todas las descripciones de las opciones
    for (size_t i = 0; i < menu->cantidad_opciones; i++) {
        free(menu->opciones[i].descripcion);
    }

    free(menu->opciones);
    free(menu->titulo);
    free(menu);
}