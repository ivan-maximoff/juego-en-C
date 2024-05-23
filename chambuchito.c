#include <stdio.h>
#include "chambuchito.h"

const int MEDIDA_MIN_CHAMBUCHITO = 15;
const int MEDIDA_MAX_CHAMBUCHITO = 30;
const float MEDIDA_MULTIPLICADA_POR = 0.3f;

const char PAN_BLANCO = 'B';
const char PAN_INTEGRAL = 'I';
const char PAN_AVENA_Y_MIEL = 'A';
const char PAN_QUESO_Y_OREGANO = 'Q';

const char QUESO_DAMBO = 'D';
const char QUESO_CHEDDAR = 'C';
const char QUESO_GRUYERE = 'G';
const char SIN_QUESO = 'S';

const int PRECIO_PAN_O_QUESO_BASICO = 5;
const int PRECIO_PAN_O_QUESO_ESPECIAL = 8;

const char PROTEINA_ROAST_BEEF = 'R';
const char PROTEINA_ATUN = 'A';
const char PROTEINA_SOJA = 'S';
const char PROTEINA_POLLITO = 'P';
const char NADA_PROTEINA = 'N';


const int PRECIO_PROTEINA_ROAST_BEEF = 7;
const int PRECIO_PROTEINA_ATUN = 9;
const int PRECIO_PROTEINA_POLLITO = 5;
const int PRECIO_PROTEINA_SOJA = 3;

const char CHAMBUCHITO_CALIENTE = 'S';
const char CHAMBUCHITO_FRIO = 'N';

// PRE: -
// POS: Muestra un mensaje preguntando por el largo del chambuchito y carga la variable con la respuesta valida del usuario.

void preguntar_medida(int* medida_chambuchito){

    printf("\n¿De cuantos centimetros desea su chambuchito? \nPuede ser desde 15 a 30 inclusive (solo escribir el numero entero).\n");
    scanf(" %i", medida_chambuchito);
    while(*medida_chambuchito < MEDIDA_MIN_CHAMBUCHITO || *medida_chambuchito > MEDIDA_MAX_CHAMBUCHITO ){
        printf("\nRecuerde escribir solo el numero entero y sin la unidad de cm desde 15 y 30 inclusive.\n");
        scanf(" %i", medida_chambuchito);
    }

};

// PRE: -
// POS: Muestra un mensaje preguntando por el tipo de pan y carga la variable con la respuesta valida del usuario.

void preguntar_pan(char* tipo_de_pan){

    printf("\n¿Qué pan desea en su chambuchito? Escriba la inicial correspondiente en MAYUSCULA: \nTenemos Blanco [B], Integral [I], Avena y Miel [A], Queso y Orégano [Q].\n");
    scanf(" %c", tipo_de_pan);
    while(*tipo_de_pan != PAN_BLANCO && *tipo_de_pan != PAN_INTEGRAL && *tipo_de_pan != PAN_AVENA_Y_MIEL && *tipo_de_pan != PAN_QUESO_Y_OREGANO){
        printf("\nRecuerde escribir solo la inicial en MAYUSCULA de los tipos de panes \nTenemos Blanco [B], Integral [I], Avena y Miel [A], Queso y Orégano [Q].\n");
        scanf(" %c", tipo_de_pan);
    }

};

// PRE: -
// POS: Muestra un mensaje preguntando por el tipo de queso y carga la variable con la respuesta valida del usuario.

void preguntar_queso(char* tipo_de_queso){

    printf("\n¿Qué queso quiere en su chambuchito? Escriba la inicial correspondiente en MAYUSCULA: \nPueden ser: Dambo [D], Cheddar [C], Gruyere [G], Sin Queso [S].\n");
    scanf(" %c", tipo_de_queso);
    while(*tipo_de_queso != QUESO_DAMBO && *tipo_de_queso != QUESO_CHEDDAR && *tipo_de_queso != QUESO_GRUYERE && *tipo_de_queso != SIN_QUESO){
        printf("\nRecuerde escribir solo la inicial en MAYUSCULA de los tipos de quesos\nPueden ser: Dambo [D], Cheddar [C], Gruyere [G], Sin Queso [S].\n");
        scanf(" %c", tipo_de_queso);
    }
};

// PRE: -
// POS: Muestra un mensaje preguntando por el tipo de proteina y carga la variable con la respuesta valida del usuario.

void preguntar_proteina(char* tipo_de_proteina){

    printf("\n¿Qué proteina quiere en su chambuchito? Escriba la inicial correspondiente en MAYUSCULA: \nPueden ser: Roast Beef [R], Atún [A], Soja [S], Pollito [P], Nada de prote [N].\n");
    scanf(" %c", tipo_de_proteina);
    while(*tipo_de_proteina != PROTEINA_ROAST_BEEF && *tipo_de_proteina != PROTEINA_ATUN && *tipo_de_proteina != PROTEINA_POLLITO && *tipo_de_proteina != PROTEINA_SOJA && *tipo_de_proteina != NADA_PROTEINA){
        printf("\nRecuerde escribir solo la inicial en MAYUSCULA de los tipos de proteinas\nPueden ser: Roast Beef [R], Atún [A], Soja [S], Pollito [P], Nada de prote [N].\n");
        scanf(" %c", tipo_de_proteina);
    }
};

// PRE: -
// POS: Muestra un mensaje preguntando por la temperatura del chambuchito y carga la variable con la respuesta valida del usuario.

void preguntar_temperatura(char* temperatura_chambuchito){

    printf("\n¿Desea su chambuchito caliente? Escriba la inicial correspondiente en MAYUSCULA: \nPueden ser:  [S] para sí y [N] para no.\n");
    scanf(" %c", temperatura_chambuchito);
    while(*temperatura_chambuchito != CHAMBUCHITO_CALIENTE && *temperatura_chambuchito != CHAMBUCHITO_FRIO ){
        printf("\nRecuerde escribir solo la inicial en MAYUSCULA del sí o el no.\n");
        scanf(" %c", temperatura_chambuchito);
    }
};

// PRE: Debe recibir las variables con valores de caracteres iguales a [B], [I], [A] o [Q].
// POS: Categoriza el tipo de pan segun el seleccionado por el usuario y le asigna su respectivo valor.

int calcular_precio_pan(int* precio_pan, char tipo_de_pan){
    if(tipo_de_pan == PAN_BLANCO || tipo_de_pan == PAN_INTEGRAL){
        *precio_pan = PRECIO_PAN_O_QUESO_BASICO;
    } else {
        *precio_pan = PRECIO_PAN_O_QUESO_ESPECIAL;
    };

    return (*precio_pan);
};

// PRE: Debe recibir las variables con valores validos de caracteres iguales a [D], [C], [G] o [S].
// POS: Categoriza el tipo de queso segun el seleccionado por el usuario y le asigna su respectivo valor.

int calcular_precio_queso(int* precio_queso, char tipo_de_queso){
    if(tipo_de_queso == QUESO_DAMBO || tipo_de_queso == QUESO_CHEDDAR){
        *precio_queso = PRECIO_PAN_O_QUESO_BASICO;
    } else if (tipo_de_queso == QUESO_GRUYERE){
        *precio_queso = PRECIO_PAN_O_QUESO_ESPECIAL;
    }

    return (*precio_queso);
};

// PRE: Debe recibir las variables con valores validos de caracteres iguales a [R], [A], [S], [P] o [N].
// POS: Categoriza el tipo de proteina segun el seleccionado por el usuario y le asigna su respectivo valor.

int calcular_precio_proteina(int* precio_proteina, char tipo_de_proteina){
    if (tipo_de_proteina == PROTEINA_ROAST_BEEF){
        *precio_proteina = PRECIO_PROTEINA_ROAST_BEEF;
    } else if (tipo_de_proteina == PROTEINA_ATUN){
        *precio_proteina = PRECIO_PROTEINA_ATUN;
    } else if (tipo_de_proteina == PROTEINA_POLLITO){
        *precio_proteina = PRECIO_PROTEINA_POLLITO;
    } else if (tipo_de_proteina == PROTEINA_SOJA){
        *precio_proteina = PRECIO_PROTEINA_SOJA;
    }

    return (*precio_proteina);
};

// PRE: Debe recibir las variables con los valores asignados y validados por las otras funciones.
// POS: Calcula el precio final de chambuchito segun todas las elecciones del usuario.

int calcular_precio_final(int* precio_final, int precio_pan, int precio_queso, int precio_proteina, int medida_chambuchito){
    *precio_final = (int)((float)((precio_pan + precio_queso + precio_proteina) * medida_chambuchito )* MEDIDA_MULTIPLICADA_POR);

    return(*precio_final);
};

void calcular_precio_chambuchito(int* precio_final){

    printf("¡Bienvenidos a nuestra tienda de chambuchitos premium!\n");

    int medida_chambuchito = 0;
    char tipo_de_pan = ' ';
    char tipo_de_queso = ' ';
    char tipo_de_proteina = ' ';
    char temperatura_chambuchito = ' ';
    int precio_pan = 0;
    int precio_queso = 0;
    int precio_proteina = 0;

    preguntar_medida(&medida_chambuchito);
    preguntar_pan(&tipo_de_pan);
    preguntar_queso(&tipo_de_queso);
    preguntar_proteina(&tipo_de_proteina);
    if (tipo_de_proteina != PROTEINA_ATUN){preguntar_temperatura(&temperatura_chambuchito);}

    calcular_precio_pan(&precio_pan, tipo_de_pan);
    calcular_precio_queso(&precio_queso, tipo_de_queso);
    calcular_precio_proteina(&precio_proteina, tipo_de_proteina);
    calcular_precio_final(precio_final, precio_pan, precio_queso, precio_proteina, medida_chambuchito);

    printf("\nEl precio de su chambuchito es -%i-\nPuede esperar su pedido en la zona de entrega.\n", *precio_final);
}