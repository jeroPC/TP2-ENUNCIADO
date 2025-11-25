#include <stdio.h>
#include "src/juego.h"

int main()
{
	printf("=== TEST SIMPLE DEL JUEGO ===\n\n");

	// 1. Crear juego
	printf("1. Creando juego... ");
	juego_t *juego = juego_crear();
	if (!juego) {
		printf("ERROR\n");
		return 1;
	}
	printf("OK\n");

	// 2. Cargar pokedex
	printf("2. Cargando pokedex... ");
	int cantidad = juego_cargar_pokedex(juego, "ejemplos/largo.csv");
	printf("Cargados: %d pokemones\n", cantidad);
	if (cantidad < 0) {
		printf("   ERROR al cargar\n");
		juego_destruir(juego);
		return 1;
	}

	// 3. Verificar cantidad
	printf("3. Verificando cantidad... ");
	size_t cant = juego_cantidad_pokemones(juego);
	printf("%zu pokemones en la pokedex\n", cant);

	if (cant < 9) {
		printf("   ERROR: Se necesitan al menos 9 pokemones\n");
		juego_destruir(juego);
		return 1;
	}

	// 4. Iniciar partida
	printf("4. Iniciando partida... ");
	bool iniciado = juego_iniciar_partida(juego, 42);
	if (!iniciado) {
		printf("ERROR\n");
		juego_destruir(juego);
		return 1;
	}
	printf("OK\n");

	// 5. Verificar cartas
	printf("5. Verificando cartas creadas... ");
	size_t cant_cartas = juego_cantidad_cartas(juego);
	printf("%zu cartas\n", cant_cartas);

	if (cant_cartas != TOTAL_CARTAS) {
		printf("   ERROR: Deberían ser %d cartas\n", TOTAL_CARTAS);
		juego_destruir(juego);
		return 1;
	}

	// 6. Verificar jugador actual
	printf("6. Jugador actual: %d\n", juego_jugador_actual(juego));

	// 7. Verificar si la partida terminó
	printf("7. Partida terminada? ");
	bool terminada = juego_partida_terminada(juego);
	printf("%s\n", terminada ? "SI" : "NO");

	if (terminada) {
		printf("   ERROR: La partida NO debería estar terminada al inicio\n");
		juego_destruir(juego);
		return 1;
	}

	// 8. Mostrar tablero
	printf("\n8. Estado del tablero:\n");
	carta_t cartas[TOTAL_CARTAS];
	size_t obtenidas = juego_obtener_tablero(juego, cartas);
	printf("   Cartas obtenidas: %zu\n", obtenidas);

	for (size_t i = 0; i < obtenidas; i++) {
		printf("   [%d] %s - %s - %s\n", cartas[i].posicion,
		       cartas[i].pokemon->nombre,
		       cartas[i].descubierta ? "DESCUBIERTA" : "OCULTA",
		       cartas[i].emparejada ? "EMPAREJADA" : "DISPONIBLE");
	}

	printf("\n=== TODAS LAS PRUEBAS PASARON ===\n");

	juego_destruir(juego);
	return 0;
}
