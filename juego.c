#include "cocineritos.h"
#include "chambuchito.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

const static int JUGANDO = 0;

void limpiar_pantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// PRE: movimiento es un puntero válido a un objeto de tipo char. juego es un puntero válido a un objeto de tipo juego_t.
// POS: Solicita al jugador que ingrese un movimiento, actualiza el juego según el movimiento ingresado y muestra el estado del juego.
// Continúa solicitando movimientos hasta que el estado del juego sea diferente de JUGANDO.

void preguntar_movimiento(char* movimiento, juego_t* juego){
    do {
        printf("Cantidad de movimientos: %i \n", juego->movimientos);
        if (juego->personaje_activo == 'S'){
            printf("Objeto en mano: %c \n", juego->stitch.objeto_en_mano);
        } else {
            printf("Objeto en mano: %c \n", juego->reuben.objeto_en_mano);
        }
        printf("Movimiento a realizar: ");
        scanf(" %c", movimiento);
        limpiar_pantalla();
        realizar_jugada(juego, *movimiento);
        if (estado_juego(*juego) == JUGANDO){
            imprimir_terreno(*juego);
        }
    } while (estado_juego(*juego) == JUGANDO);
}

int main(){
    srand ((unsigned)time(NULL));
    int precio = 0;
    calcular_precio_chambuchito(&precio);
    juego_t juego;
    inicializar_juego(&juego, precio);
    limpiar_pantalla();
    imprimir_terreno(juego);
    char movimiento = ' ';
    preguntar_movimiento(&movimiento, &juego);
    return 0;
}
