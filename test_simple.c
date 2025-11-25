#include "src/juego.h"
#include "src/tp1.h"
#include <stdio.h>

int main()
{
	printf("Creando juego...\n");
	juego_t *juego = juego_crear();
	if (!juego) {
		printf("Error al crear juego\n");
		return 1;
	}

	printf("Cargando pokedex...\n");
	int cargados = juego_cargar_pokedex(juego, "ejemplos/normal.csv");
	printf("Pokemones cargados: %d\n", cargados);
	printf("Cantidad en pokedex: %zu\n", juego_cantidad_pokemones(juego));

	juego_destruir(juego);
	return 0;
}
