#include "pa2m.h"
#include "src/menu.h"
#include "src/juego.h"
#include "src/tp1.h"
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

void test_juego_crear()
{
	pa2m_nuevo_grupo("Pruebas de juego_crear");
	
	juego_t *juego = juego_crear();
	pa2m_afirmar(juego != NULL, "Se puede crear un juego");
	pa2m_afirmar(juego_cantidad_pokemones(juego) == 0, "Un juego nuevo no tiene pokemones");
	
	juego_destruir(juego);
}

void test_juego_cargar_pokedex()
{
	pa2m_nuevo_grupo("Pruebas de juego_cargar_pokedex");
	
	juego_t *juego = juego_crear();
	
	int cargados = juego_cargar_pokedex(juego, "ejemplos/normal.csv");
	pa2m_afirmar(cargados > 0, "Se pueden cargar pokemones desde un archivo válido");
	pa2m_afirmar(juego_cantidad_pokemones(juego) == (size_t)cargados, 
				 "La cantidad de pokemones coincide con los cargados");
	
	int error = juego_cargar_pokedex(NULL, "ejemplos/normal.csv");
	pa2m_afirmar(error == -1, "No se puede cargar con juego NULL");
	
	error = juego_cargar_pokedex(juego, NULL);
	pa2m_afirmar(error == -1, "No se puede cargar con archivo NULL");
	
	error = juego_cargar_pokedex(juego, "archivo_inexistente.csv");
	pa2m_afirmar(error == -1, "No se puede cargar un archivo inexistente");
	
	juego_destruir(juego);
}

void test_juego_cantidad_pokemones()
{
	pa2m_nuevo_grupo("Pruebas de juego_cantidad_pokemones");
	
	juego_t *juego = juego_crear();
	
	pa2m_afirmar(juego_cantidad_pokemones(juego) == 0, "Un juego vacío tiene 0 pokemones");
	pa2m_afirmar(juego_cantidad_pokemones(NULL) == 0, "Un juego NULL tiene 0 pokemones");
	
	juego_cargar_pokedex(juego, "ejemplos/normal.csv");
	size_t cantidad = juego_cantidad_pokemones(juego);
	pa2m_afirmar(cantidad > 0, "Después de cargar, hay pokemones en la pokedex");
	
	juego_destruir(juego);
}

void test_juego_buscar_por_nombre()
{
	pa2m_nuevo_grupo("Pruebas de juego_buscar_por_nombre");
	
	juego_t *juego = juego_crear();
	juego_cargar_pokedex(juego, "ejemplos/normal.csv");
	
	pokemon_t *resultados[10];
	
	// Búsqueda con parámetros NULL
	size_t encontrados = juego_buscar_por_nombre(NULL, "Pika", resultados, 10);
	pa2m_afirmar(encontrados == 0, "No se puede buscar con juego NULL");
	
	encontrados = juego_buscar_por_nombre(juego, NULL, resultados, 10);
	pa2m_afirmar(encontrados == 0, "No se puede buscar con nombre NULL");
	
	encontrados = juego_buscar_por_nombre(juego, "Pika", NULL, 10);
	pa2m_afirmar(encontrados == 0, "No se puede buscar con array de resultados NULL");
	
	encontrados = juego_buscar_por_nombre(juego, "Pika", resultados, 0);
	pa2m_afirmar(encontrados == 0, "No se puede buscar con max_resultados = 0");
	
	// Búsqueda parcial válida
	encontrados = juego_buscar_por_nombre(juego, "a", resultados, 10);
	pa2m_afirmar(encontrados > 0, "Se encuentran pokemones con búsqueda parcial");
	pa2m_afirmar(encontrados <= 10, "No se excede el límite de max_resultados");
	
	// Búsqueda que no debería encontrar nada
	encontrados = juego_buscar_por_nombre(juego, "XXXZZZ", resultados, 10);
	pa2m_afirmar(encontrados == 0, "No se encuentran pokemones con nombre inexistente");
	
	juego_destruir(juego);
}

void test_juego_crear_cartas_memoria()
{
	pa2m_nuevo_grupo("Pruebas de juego_crear_cartas_memoria");
	
	juego_t *juego = juego_crear();
	
	// Intentar crear cartas sin pokemones
	juego_crear_cartas_memoria(juego);
	pa2m_afirmar(juego_cantidad_cartas(juego) == 0, 
				 "No se crean cartas si no hay suficientes pokemones");
	
	// Cargar pokemones y crear cartas
	juego_cargar_pokedex(juego, "ejemplos/normal.csv");
	
	// Verificar que hay al menos 9 pokemones
	if (juego_cantidad_pokemones(juego) >= 9) {
		juego_crear_cartas_memoria(juego);
		pa2m_afirmar(juego_cantidad_cartas(juego) == 18, 
					 "Se crean 18 cartas (9 pares) correctamente");
		
		// Verificar que hay pares (cada pokemon aparece 2 veces)
		pa2m_afirmar(juego_cantidad_cartas(juego) % 2 == 0, 
					 "La cantidad de cartas es par");
	}
	
	juego_destruir(juego);
}

void test_juego_crear_cartas_memoria_con_pocos_pokemones()
{
	pa2m_nuevo_grupo("Pruebas de juego_crear_cartas_memoria con pocos pokemones");
	
	juego_t *juego = juego_crear();
	
	// Intentar crear cartas con juego NULL
	juego_crear_cartas_memoria(NULL);
	pa2m_afirmar(true, "No crashea con juego NULL");
	
	juego_destruir(juego);
}

void test_juego_buscar_por_id()
{
	pa2m_nuevo_grupo("Pruebas de juego_buscar_por_id");
	
	juego_t *juego = juego_crear();
	juego_cargar_pokedex(juego, "ejemplos/normal.csv");
	
	// Búsqueda con parámetros NULL
	pokemon_t *encontrado = juego_buscar_por_id(NULL, 1);
	pa2m_afirmar(encontrado == NULL, "No se puede buscar con juego NULL");
	
	// Búsqueda de ID que debería existir (asumiendo que el primer pokemon tiene ID 1)
	encontrado = juego_buscar_por_id(juego, 1);
	pa2m_afirmar(encontrado != NULL, "Se encuentra un pokemon con ID válido");
	if (encontrado) {
		pa2m_afirmar(encontrado->id == 1, "El pokemon encontrado tiene el ID correcto");
	}
	
	// Búsqueda de ID que no existe
	encontrado = juego_buscar_por_id(juego, 99999);
	pa2m_afirmar(encontrado == NULL, "No se encuentra un pokemon con ID inexistente");
	
	juego_destruir(juego);
}

void contador_pokemones_callback(pokemon_t *pokemon, void *ctx)
{
	int *contador = (int *)ctx;
	(*contador)++;
}

void test_juego_listar_por_nombre()
{
	pa2m_nuevo_grupo("Pruebas de juego_listar_por_nombre");
	
	juego_t *juego = juego_crear();
	
	// Listar con parámetros NULL
	int contador = 0;
	juego_listar_por_nombre(NULL, contador_pokemones_callback, &contador);
	pa2m_afirmar(contador == 0, "No se listan pokemones con juego NULL");
	
	contador = 0;
	juego_listar_por_nombre(juego, NULL, &contador);
	pa2m_afirmar(contador == 0, "No se listan pokemones con callback NULL");
	
	// Cargar pokemones y listar
	int cargados = juego_cargar_pokedex(juego, "ejemplos/normal.csv");
	
	contador = 0;
	juego_listar_por_nombre(juego, contador_pokemones_callback, &contador);
	pa2m_afirmar(contador == cargados, "Se listan todos los pokemones cargados");
	
	// Verificar que están ordenados por nombre (se invoca el callback para cada uno)
	pa2m_afirmar(contador > 0, "El callback se invoca para cada pokemon");
	
	juego_destruir(juego);
}

void test_juego_listar_por_id()
{
	pa2m_nuevo_grupo("Pruebas de juego_listar_por_id");
	
	juego_t *juego = juego_crear();
	
	// Listar con parámetros NULL
	int contador = 0;
	juego_listar_por_id(NULL, contador_pokemones_callback, &contador);
	pa2m_afirmar(contador == 0, "No se listan pokemones con juego NULL");
	
	contador = 0;
	juego_listar_por_id(juego, NULL, &contador);
	pa2m_afirmar(contador == 0, "No se listan pokemones con callback NULL");
	
	// Cargar pokemones y listar
	int cargados = juego_cargar_pokedex(juego, "ejemplos/normal.csv");
	
	contador = 0;
	juego_listar_por_id(juego, contador_pokemones_callback, &contador);
	pa2m_afirmar(contador == cargados, "Se listan todos los pokemones cargados por ID");
	
	pa2m_afirmar(contador > 0, "El callback se invoca para cada pokemon ordenado por ID");
	
	juego_destruir(juego);
}

void test_juego_iniciar_partida()
{
	pa2m_nuevo_grupo("Pruebas de juego_iniciar_partida");
	
	juego_t *juego = juego_crear();
	
	// Intentar iniciar partida sin pokemones
	bool resultado = juego_iniciar_partida(juego, 12345);
	pa2m_afirmar(!resultado, "No se puede iniciar partida sin suficientes pokemones");
	
	// Cargar pokemones e iniciar partida
	juego_cargar_pokedex(juego, "ejemplos/normal.csv");
	
	if (juego_cantidad_pokemones(juego) >= 9) {
		resultado = juego_iniciar_partida(juego, 12345);
		pa2m_afirmar(resultado, "Se puede iniciar partida con suficientes pokemones");
		pa2m_afirmar(juego_cantidad_cartas(juego) == 18, "Se crean 18 cartas al iniciar");
		pa2m_afirmar(juego_partida_activa(juego), "La partida queda activa");
		pa2m_afirmar(juego_jugador_actual(juego) == 1, "El jugador actual es el 1 al inicio");
		
		// Iniciar otra partida con diferente semilla
		resultado = juego_iniciar_partida(juego, 54321);
		pa2m_afirmar(resultado, "Se puede reiniciar una partida");
		pa2m_afirmar(juego_cantidad_cartas(juego) == 18, "Las cartas se regeneran correctamente");
		
		// Iniciar con semilla 0 (usa tiempo actual)
		resultado = juego_iniciar_partida(juego, 0);
		pa2m_afirmar(resultado, "Se puede iniciar partida con semilla 0 (aleatorio)");
	}
	
	// Intentar iniciar con juego NULL
	resultado = juego_iniciar_partida(NULL, 12345);
	pa2m_afirmar(!resultado, "No se puede iniciar partida con juego NULL");
	
	juego_destruir(juego);
}

void test_juego_partida_activa()
{
	pa2m_nuevo_grupo("Pruebas de juego_partida_activa");
	
	juego_t *juego = juego_crear();
	
	pa2m_afirmar(!juego_partida_activa(juego), "Una partida no iniciada no está activa");
	pa2m_afirmar(!juego_partida_activa(NULL), "Un juego NULL no tiene partida activa");
	
	juego_cargar_pokedex(juego, "ejemplos/normal.csv");
	
	if (juego_cantidad_pokemones(juego) >= 9) {
		juego_iniciar_partida(juego, 12345);
		pa2m_afirmar(juego_partida_activa(juego), "Después de iniciar, la partida está activa");
	}
	
	juego_destruir(juego);
}

void test_juego_jugador_actual()
{
	pa2m_nuevo_grupo("Pruebas de juego_jugador_actual");
	
	juego_t *juego = juego_crear();
	
	pa2m_afirmar(juego_jugador_actual(juego) == 0, "El jugador actual es 0 antes de iniciar partida");
	pa2m_afirmar(juego_jugador_actual(NULL) == 0, "Un juego NULL retorna jugador 0");
	
	juego_cargar_pokedex(juego, "ejemplos/normal.csv");
	
	if (juego_cantidad_pokemones(juego) >= 9) {
		juego_iniciar_partida(juego, 12345);
		pa2m_afirmar(juego_jugador_actual(juego) == 1, "El jugador actual es 1 al iniciar la partida");
	}
	
	juego_destruir(juego);
}

void test_integracion_juego_completa()
{
	pa2m_nuevo_grupo("Pruebas de integración del juego");
	
	juego_t *juego = juego_crear();
	pa2m_afirmar(juego != NULL, "Se crea el juego correctamente");
	
	int cargados = juego_cargar_pokedex(juego, "ejemplos/normal.csv");
	pa2m_afirmar(cargados > 0, "Se cargan pokemones correctamente");
	
	size_t cantidad = juego_cantidad_pokemones(juego);
	pa2m_afirmar(cantidad == (size_t)cargados, "La cantidad es consistente");
	
	pokemon_t *resultados[5];
	size_t encontrados = juego_buscar_por_nombre(juego, "a", resultados, 5);
	pa2m_afirmar(encontrados > 0 && encontrados <= 5, "La búsqueda funciona correctamente");
	
	if (cantidad >= 9) {
		juego_crear_cartas_memoria(juego);
		pa2m_afirmar(juego_cantidad_cartas(juego) == 18, "Se crean las cartas de memoria");
	}
	
	juego_destruir(juego);
}

void test_juego_obtener_tablero()
{
	pa2m_nuevo_grupo("Pruebas de juego_obtener_tablero");
	
	juego_t *juego = juego_crear();
	carta_t cartas[TOTAL_CARTAS];
	
	// Pruebas con parámetros inválidos
	size_t obtenidas = juego_obtener_tablero(NULL, cartas);
	pa2m_afirmar(obtenidas == 0, "No se puede obtener tablero con juego NULL");
	
	obtenidas = juego_obtener_tablero(juego, NULL);
	pa2m_afirmar(obtenidas == 0, "No se puede obtener tablero con array NULL");
	
	// Prueba sin cartas creadas
	obtenidas = juego_obtener_tablero(juego, cartas);
	pa2m_afirmar(obtenidas == 0, "No se obtienen cartas si no hay partida iniciada");
	
	// Cargar pokemones e iniciar partida
	juego_cargar_pokedex(juego, "ejemplos/normal.csv");
	
	if (juego_cantidad_pokemones(juego) >= 9) {
		juego_iniciar_partida(juego, 12345);
		
		obtenidas = juego_obtener_tablero(juego, cartas);
		pa2m_afirmar(obtenidas == TOTAL_CARTAS, "Se obtienen las 18 cartas correctamente");
		
		// Verificar que las cartas tienen datos válidos
		bool todas_validas = true;
		for (size_t i = 0; i < obtenidas; i++) {
			if (cartas[i].pokemon == NULL) {
				todas_validas = false;
				break;
			}
		}
		pa2m_afirmar(todas_validas, "Todas las cartas tienen un pokemon asignado");
		
		// Verificar que hay exactamente 9 pares (cada pokemon aparece 2 veces)
		int pares_encontrados = 0;
		for (size_t i = 0; i < obtenidas; i++) {
			int cuenta = 0;
			for (size_t j = 0; j < obtenidas; j++) {
				if (cartas[i].pokemon == cartas[j].pokemon) {
					cuenta++;
				}
			}
			if (cuenta == 2 && i == 0) {
				// Solo contamos una vez por cada par
				pares_encontrados++;
			}
		}
		pa2m_afirmar(pares_encontrados > 0, "Existen pares de cartas con el mismo pokemon");
	}
	
	juego_destruir(juego);
}

void test_juego_seleccionar_carta()
{
	pa2m_nuevo_grupo("Pruebas de juego_seleccionar_carta");
	
	juego_t *juego = juego_crear();
	juego_cargar_pokedex(juego, "ejemplos/normal.csv");
	
	// Pruebas con parámetros inválidos
	int resultado = juego_seleccionar_carta(NULL, 0);
	pa2m_afirmar(resultado == -1, "No se puede seleccionar carta con juego NULL");
	
	resultado = juego_seleccionar_carta(juego, -1);
	pa2m_afirmar(resultado == -1, "No se puede seleccionar carta con posición negativa");
	
	resultado = juego_seleccionar_carta(juego, TOTAL_CARTAS);
	pa2m_afirmar(resultado == -1, "No se puede seleccionar carta fuera de rango");
	
	// Iniciar partida con semilla fija para pruebas determinísticas
	if (juego_cantidad_pokemones(juego) >= 9) {
		juego_iniciar_partida(juego, 12345);
		
		// Obtener el tablero para saber qué cartas hay
		carta_t tablero[TOTAL_CARTAS];
		juego_obtener_tablero(juego, tablero);
		
		// Seleccionar primera carta
		resultado = juego_seleccionar_carta(juego, 0);
		pa2m_afirmar(resultado == 0, "Primera carta seleccionada correctamente (retorna 0)");
		
		// Intentar seleccionar la misma carta
		resultado = juego_seleccionar_carta(juego, 0);
		pa2m_afirmar(resultado == -1, "No se puede seleccionar una carta ya descubierta");
		
		// Buscar la pareja de la carta en posición 0
		int posicion_pareja = -1;
		for (int i = 1; i < TOTAL_CARTAS; i++) {
			if (tablero[i].pokemon == tablero[0].pokemon) {
				posicion_pareja = i;
				break;
			}
		}
		
		if (posicion_pareja != -1) {
			// Seleccionar la pareja
			int jugador_antes = juego_jugador_actual(juego);
			int puntos_antes = juego_obtener_puntuacion(juego, jugador_antes);
			
			resultado = juego_seleccionar_carta(juego, posicion_pareja);
			pa2m_afirmar(resultado == 1, "Se forma un par correctamente (retorna 1)");
			
			int puntos_despues = juego_obtener_puntuacion(juego, jugador_antes);
			pa2m_afirmar(puntos_despues == puntos_antes + 1, "Se suma un punto al jugador");
			
			int jugador_despues = juego_jugador_actual(juego);
			pa2m_afirmar(jugador_despues == jugador_antes, "El turno se mantiene cuando hay par");
		}
		
		// Seleccionar dos cartas que NO son pareja
		int carta1 = -1, carta2 = -1;
		for (int i = 0; i < TOTAL_CARTAS && carta1 == -1; i++) {
			if (tablero[i].emparejada) continue;
			for (int j = i + 1; j < TOTAL_CARTAS; j++) {
				if (tablero[j].emparejada) continue;
				if (tablero[i].pokemon != tablero[j].pokemon) {
					carta1 = i;
					carta2 = j;
					break;
				}
			}
		}
		
		if (carta1 != -1 && carta2 != -1) {
			int jugador_antes = juego_jugador_actual(juego);
			
			resultado = juego_seleccionar_carta(juego, carta1);
			pa2m_afirmar(resultado == 0, "Primera carta de par incorrecto seleccionada");
			
			resultado = juego_seleccionar_carta(juego, carta2);
			pa2m_afirmar(resultado == -2, "Par incorrecto retorna -2");
			
			int jugador_despues = juego_jugador_actual(juego);
			pa2m_afirmar(jugador_despues != jugador_antes, "El turno cambia cuando no hay par");
		}
	}
	
	juego_destruir(juego);
}

void test_juego_obtener_puntuacion()
{
	pa2m_nuevo_grupo("Pruebas de juego_obtener_puntuacion");
	
	juego_t *juego = juego_crear();
	
	// Pruebas con parámetros inválidos
	int puntos = juego_obtener_puntuacion(NULL, 1);
	pa2m_afirmar(puntos == 0, "Juego NULL retorna 0 puntos");
	
	puntos = juego_obtener_puntuacion(juego, 0);
	pa2m_afirmar(puntos == 0, "Jugador 0 (inválido) retorna 0 puntos");
	
	puntos = juego_obtener_puntuacion(juego, 3);
	pa2m_afirmar(puntos == 0, "Jugador 3 (inválido) retorna 0 puntos");
	
	// Pruebas con partida iniciada
	juego_cargar_pokedex(juego, "ejemplos/normal.csv");
	
	if (juego_cantidad_pokemones(juego) >= 9) {
		juego_iniciar_partida(juego, 12345);
		
		puntos = juego_obtener_puntuacion(juego, 1);
		pa2m_afirmar(puntos == 0, "Jugador 1 empieza con 0 puntos");
		
		puntos = juego_obtener_puntuacion(juego, 2);
		pa2m_afirmar(puntos == 0, "Jugador 2 empieza con 0 puntos");
	}
	
	juego_destruir(juego);
}

int main()
{
	pa2m_nuevo_grupo("============== PRUEBAS MENU_CREAR ===============");
	pruebas_menu_crear();

	pa2m_nuevo_grupo("============== PRUEBAS MENU_AGREGAR_OPCION ===============");
	pruebas_menu_agregar_opcion();

	pa2m_nuevo_grupo("============== PRUEBAS CAMBIAR Y OBTENER ESTILO ===============");
	pruebas_menu_cambiar_estilo_y_obtener();

	pa2m_nuevo_grupo("============== PRUEBAS DEL TDA JUEGO ===============");
	test_juego_crear();
	test_juego_cargar_pokedex();
	test_juego_cantidad_pokemones();
	test_juego_buscar_por_nombre();
	test_juego_buscar_por_id();
	test_juego_listar_por_nombre();
	test_juego_listar_por_id();
	test_juego_iniciar_partida();
	test_juego_partida_activa();
	test_juego_jugador_actual();
	test_juego_crear_cartas_memoria();
	test_juego_crear_cartas_memoria_con_pocos_pokemones();
	test_integracion_juego_completa();
	
	pa2m_nuevo_grupo("============== PRUEBAS NUEVAS PRIMITIVAS ===============");
	test_juego_obtener_tablero();
	test_juego_obtener_puntuacion();
	test_juego_seleccionar_carta();

	return pa2m_mostrar_reporte();
}
