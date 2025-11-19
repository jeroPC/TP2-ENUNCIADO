#include <stdio.h>
#include "src/juego.h"

int main() {
    juego_t *juego = juego_crear();
    juego_cargar_pokedex(juego, "ejemplos/normal.csv");
    
    pokemon_t *poke = juego_buscar_por_id(juego, 1);
    if (poke) {
        printf("Encontrado: ID=%d, Nombre=%s\n", poke->id, poke->nombre);
    } else {
        printf("No encontrado\n");
    }
    
    juego_destruir(juego);
    return 0;
}
