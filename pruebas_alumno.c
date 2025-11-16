#include "pa2m.h"
#include "src/menu.h"
#include <string.h>

bool accion_valida(void *ctx_interno, void *ctx_externo)
{
	return true;
}

void pruebas_menu_crear()
{
	menu_t *menu = menu_crear("Menu Principal", ESTILO_MINIMALISTA);
	pa2m_afirmar(menu != NULL, "Se crea un menú con título personalizado");
	menu_destruir(menu);

	menu = menu_crear(NULL, ESTILO_BORDES);
	pa2m_afirmar(menu != NULL, "Se crea un menú con título NULL (usa MENU por defecto)");
	menu_destruir(menu);

	menu = menu_crear("Test", ESTILO_RETRO);
	pa2m_afirmar(menu != NULL, "Se crea un menú con estilo RETRO");
	menu_destruir(menu);
}

void pruebas_menu_agregar_opcion()
{
	menu_t *menu = menu_crear("Menu Test", ESTILO_MINIMALISTA);
	
	bool resultado = menu_agregar_opcion(menu, 'C', "Cargar archivo", accion_valida, NULL, NULL);
	pa2m_afirmar(resultado, "Se agrega una opción con acción correctamente");

	menu_t *submenu = menu_crear("Submenu", ESTILO_MINIMALISTA);
	resultado = menu_agregar_opcion(menu, 'B', "Buscar", NULL, submenu, NULL);
	pa2m_afirmar(resultado, "Se agrega una opción con submenú correctamente");

	resultado = menu_agregar_opcion(menu, 'J', "Jugar", accion_valida, NULL, NULL);
	pa2m_afirmar(resultado, "Se agrega otra opción con acción");

	resultado = menu_agregar_opcion(menu, 'C', "Cargar archivo modificado", accion_valida, NULL, NULL);
	pa2m_afirmar(resultado, "Se reemplaza una opción existente (misma tecla)");

	resultado = menu_agregar_opcion(NULL, 'X', "Test", accion_valida, NULL, NULL);
	pa2m_afirmar(!resultado, "No se puede agregar opción a un menú NULL");

	resultado = menu_agregar_opcion(menu, 'X', NULL, accion_valida, NULL, NULL);
	pa2m_afirmar(!resultado, "No se puede agregar opción con descripción NULL");

	resultado = menu_agregar_opcion(menu, 'X', "Test", NULL, NULL, NULL);
	pa2m_afirmar(!resultado, "No se puede agregar opción sin acción ni submenú");

	// Agregar múltiples opciones para probar expansión del array
	resultado = menu_agregar_opcion(menu, 'M', "Mostrar", accion_valida, NULL, NULL);
	pa2m_afirmar(resultado, "Se agrega opción 4");

	resultado = menu_agregar_opcion(menu, 'S', "Jugar con semilla", accion_valida, NULL, NULL);
	pa2m_afirmar(resultado, "Se agrega opción 5 (prueba expansión del array)");

	resultado = menu_agregar_opcion(menu, 'E', "Cambiar estilo", accion_valida, NULL, NULL);
	pa2m_afirmar(resultado, "Se agrega opción 6");

	resultado = menu_agregar_opcion(menu, 'Q', "Salir", accion_valida, NULL, NULL);
	pa2m_afirmar(resultado, "Se agrega opción 7");

	menu_destruir(submenu);
	menu_destruir(menu);
}

void pruebas_menu_cambiar_estilo_y_obtener()
{
	menu_t *menu = menu_crear("Menu Estilos", ESTILO_MINIMALISTA);
	pa2m_afirmar(menu_obtener_estilo(menu) == ESTILO_MINIMALISTA, "El estilo inicial es minimalista");

	menu_cambiar_estilo(menu);
	pa2m_afirmar(menu_obtener_estilo(menu) == ESTILO_BORDES, "El estilo cambia a bordes");

	menu_cambiar_estilo(menu);
	pa2m_afirmar(menu_obtener_estilo(menu) == ESTILO_RETRO, "El estilo cambia a retro");

	menu_cambiar_estilo(menu);
	pa2m_afirmar(menu_obtener_estilo(menu) == ESTILO_MINIMALISTA, "El estilo vuelve a minimalista (cíclico)");

	pa2m_afirmar(menu_obtener_estilo(NULL) == ESTILO_MINIMALISTA, "Obtener estilo de menú NULL retorna minimalista");

	menu_destruir(menu);
}

int main()
{
	pa2m_nuevo_grupo("============== PRUEBAS MENU_CREAR ===============");
	pruebas_menu_crear();

	pa2m_nuevo_grupo("============== PRUEBAS MENU_AGREGAR_OPCION ===============");
	pruebas_menu_agregar_opcion();

	pa2m_nuevo_grupo("============== PRUEBAS CAMBIAR Y OBTENER ESTILO ===============");
	pruebas_menu_cambiar_estilo_y_obtener();

	return pa2m_mostrar_reporte();
}
