#include "cocineritos.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// PERSONAJES
const char STITCH = 'S';
const char REUBEN = 'R';
const char CAMBIAR = 'X';

// MOVIMIENTOS
#define CAMBIAR_PERSONAJE 'X'
#define ARRIBA 'W'
#define DERECHA 'D'
#define ABAJO 'S'
#define IZQUIERDA 'A'
#define MANIPULAR 'R'
#define CORTAR 'C'
#define COCINAR 'H'
#define DEJAR_EN_MESA 'T'

const int MOVER_DISTANCIA_MANHATTAN = 1;
const char FILA = 'F';
const char COLUMNA = 'C'; 

// ELEMENTOS:
const char PARED = '#';
const char FUEGO = 'F';
const int NUMERO_FUEGOS = 1;
#define MATAFUEGOS 'M'
const int NUMERO_MATAFUEGOS = 1;
const char NUMERO_APARECION_FUEGO = 15;
const char AGUJEROS = 'A';
const int NUMERO_AGUJEROS_STITCH = 10;
const int NUMERO_AGUJEROS_REUBEN = 10;
const char CUCHILLOS = 'C';
const int NUMERO_CUCHILLOS = 2;
const char HORNOS = 'H';
const int NUMERO_HORNOS = 2;
#define MAX_OBJETOS 50
const char SALIDA = 'P';
const char MESA = '_';
const char ESPACIO = ' ';

// COMIDAS
const int PRIMER_COMIDA = 0;
const char ENSALADA = 'E';
#define NUMERO_INGREDIENTES_ENSALADA 2
const char PIZZA= 'P';
#define NUMERO_INGREDIENTES_PIZZA 3
const int PRECIO_HASTA_PIZZA = 100;
const char HAMBURGUESA = 'H';
#define NUMERO_INGREDIENTES_HAMBURGUESA 4
const int PRECIO_HASTA_HAMBURGUESA = 150;
const char SANDWICH = 'S';
#define NUMERO_INGREDIENTES_SANDWICH 6
const int PRECIO_HASTA_SANDWICH = 150;

// INGREDIENTES
const char LECHUGA = 'L';
const char TOMATE  = 'T';
const char MILANESA  = 'I';
const char CARNE  = 'B';
const char PAN  = 'N';
const char JAMON  = 'J';
const char QUESO = 'Q';
const char MASA = 'O';
const char VACIO = 'V';

// COORDENADAS
const int CUADRANTE_STITCH = 1;
const int CUADRANTE_REUBEN = 2;
#define MAX_COLUMNAS 21
#define MAX_FILAS 21
// coordenadas que miden los rectangulos que forma cada cuadrante
const int COORDENADA_MIN_FILA = 0;
const int COORDENADA_MIN_COLUMNA = 0;
const int COORDENADA_MEDIA_FILA = 10;
const int COORDENADA_MEDIA_COLUMNA = 10;
const int COORDENADA_MAX_FILA = 20;
const int COORDENADA_MAX_COLUMNA = 20;
const int COORDENADA_RANGO_FILA_1 = 8;
const int COORDENADA_VALOR_MINIMO_FILA_1 = 1;
const int COORDENADA_RANGO_COLUMNA_1 = 18;
const int COORDENADA_VALOR_MINIMO_COLUMNA_1 = 1;
const int COORDENADA_RANGO_FILA_2 = 8;
const int COORDENADA_VALOR_MINIMO_FILA_2 = 11;
const int COORDENADA_RANGO_COLUMNA_2 = 18;
const int COORDENADA_VALOR_MINIMO_COLUMNA_2 = 1;

// ESTADO JUEGO
const int GANAR = 1;
const static int JUGANDO = 0;
const int PERDER = -1;

// FUNCIONES GENERALES

// PRE: Rango y valor_minimo son valores válidos.
// POS: Devuelve un número entero aleatorio dentro del rango .

int generar_numero_aleatorio(int rango, int valor_minimo){
    return rand() % rango + valor_minimo;
}

// PRE: Coordenadas a y b son validas.
// POS: True si las coordenadas a y b tienen el mismo valor de fila y columna, false en caso contrario.

bool coordenadas_iguales(coordenada_t a, coordenada_t b){
    return (a.fil == b.fil && a.col == b.col);
}

// PRE: Objeto válido, tope válido y número válido.
// POS: Elimina el elemento en la posición "numero" del vector "objeto" y actualiza el valor de "tope" restandolo en 1.

void eliminar_elemento_vector_objeto(objeto_t objeto[MAX_OBJETOS], int* tope, int numero){
    objeto_t auxiliar = objeto[numero];
    objeto[numero] = objeto[*tope-1];
    objeto[*tope-1] = auxiliar;
    (*tope)--;
}

// PRE: Comida válida, tope_comida válido, tope_ingredientes válido y número válido.
// POS: Elimina el ingrediente en la posición "numero" del vector "comida" y actualiza el valor de "tope_ingredientes" restandolo en 1.

void eliminar_elemento_vector_comida(comida_t comida[MAX_COMIDA], int tope_comida, int* tope_ingredientes, int numero){
    ingrediente_t auxiliar = comida[tope_comida-1].ingrediente[numero];
    comida[tope_comida-1].ingrediente[numero] =  comida[tope_comida-1].ingrediente[*tope_ingredientes-1];
    comida[tope_comida-1].ingrediente[*tope_ingredientes-1] = auxiliar;
    (*tope_ingredientes)--;
}

// PRE: Juego válido y coordenada_a_verificar válida.
// POS: Devuelve true si la coordenada_a_verificar está ocupada en los obstáculos, herramientas o ingredientes de comida del juego, false en caso contrario.

bool esta_ocupada_vectores(juego_t juego, coordenada_t coordenada_a_verificar){
    bool esta_ocupada = false;
    for (int i = 0; i < juego.tope_obstaculos; i++){
        if(coordenadas_iguales(coordenada_a_verificar, juego.obstaculos[i].posicion)){
            esta_ocupada = true;
        }
    }  
    for (int j = 0; j < juego.tope_herramientas; j++){
        if(coordenadas_iguales(coordenada_a_verificar, juego.herramientas[j].posicion)){
            esta_ocupada = true;
        }
    } 
    for (int k = 0; k < juego.comida[juego.tope_comida-1].tope_ingredientes; k++){
        if(coordenadas_iguales(coordenada_a_verificar, juego.comida[juego.tope_comida-1].ingrediente[k].posicion)){
            esta_ocupada = true;
        }
    }
    return esta_ocupada;
}

// PRE: juego válido y coordenada_a_verificar válida.
// POS: Devuelve true si la coordenada_a_verificar está ocupada en los obstáculos, herramientas o ingredientes de comida del juego,
// o si coincide con la posición de Reuben en el juego, false en caso contrario.

bool esta_ocupada_vectores_y_reuben(juego_t juego, coordenada_t coordenada_a_verificar){
    return (esta_ocupada_vectores(juego, coordenada_a_verificar) || coordenadas_iguales(coordenada_a_verificar, juego.reuben.posicion));
}

// PRE: Juego válido y coordenada_a_verificar válida
// POS: Devuelve true si la coordenada_a_verificar está ocupada en los obstáculos, herramientas, ingredientes de comida del juego
// o si coincide con la posición de Reuben, la mesa o Stitch en el juego; de lo contrario, devuelve false.

bool esta_ocupada(juego_t juego, coordenada_t coordenada_a_verificar){
    return (esta_ocupada_vectores_y_reuben(juego, coordenada_a_verificar) || coordenadas_iguales(coordenada_a_verificar, juego.mesa) || coordenadas_iguales(coordenada_a_verificar, juego.stitch.posicion));
}

// PRE: Coordenada es un puntero válido a una estructura de coordenada, cuadrante_n es un valor válido que indica el cuadrante.
// POS: Modifica los valores de la coordenada para generar una coordenada aleatoria dependiendo del cuadrante en el que este.

void crear_coordenada_aleatoria(coordenada_t* coordenada, int cuadrante_n){
    if(cuadrante_n == CUADRANTE_STITCH){
        coordenada->fil =  generar_numero_aleatorio(COORDENADA_RANGO_FILA_1, COORDENADA_VALOR_MINIMO_FILA_1);
        coordenada->col =  generar_numero_aleatorio(COORDENADA_RANGO_COLUMNA_1, COORDENADA_VALOR_MINIMO_COLUMNA_1); 
    } 
    else if(cuadrante_n == CUADRANTE_REUBEN){
        coordenada->fil = generar_numero_aleatorio(COORDENADA_RANGO_FILA_2, COORDENADA_VALOR_MINIMO_FILA_2);
        coordenada->col = generar_numero_aleatorio(COORDENADA_RANGO_COLUMNA_2, COORDENADA_VALOR_MINIMO_COLUMNA_2);
    }
}

// PRE: Coordenada es válida, cuadrante_n es un valor válido que indica el cuadrante Stitch y Reuben, juego es un juego válido.
// POS: Inicializa los valores de coordenada generando una coordenada aleatoria en base al cuadrante_n.
// Si la coordenada generada está ocupada en los obstáculos, herramientas, ingredientes de comida del juego o coincide con las posiciones de los personajes,
// Se vuelve a generar una nueva coordenada aleatoria hasta encontrar una coordenada no ocupada.

void inicializar_coordenadas_vectores_y_personajes(coordenada_t* coordenada, int cuadrante_n, juego_t juego){
    crear_coordenada_aleatoria(coordenada, cuadrante_n);
    if(esta_ocupada_vectores(juego, *coordenada)){
        inicializar_coordenadas_vectores_y_personajes(coordenada, cuadrante_n, juego);
    }
}

// PRE: coordenada es un puntero válido a una estructura de coordenada, cuadrante_n es un valor válido que indica el cuadrante, juego es un juego válido.
// POS: Inicializa los valores de la estructura de coordenada apuntada por "coordenada" generando una coordenada aleatoria en base al cuadrante_n.
// Si la coordenada generada está ocupada en los obstáculos, herramientas, ingredientes de comida del juego, o coincide con las posiciones de Reuben o la puerta,
// se vuelve a generar una nueva coordenada aleatoria hasta encontrar una coordenada no ocupada.

void inicializar_coordenadas_puerta(coordenada_t* coordenada, int cuadrante_n, juego_t juego){
    crear_coordenada_aleatoria(coordenada, cuadrante_n);
    if(esta_ocupada_vectores_y_reuben(juego, *coordenada)){
        inicializar_coordenadas_puerta(coordenada, cuadrante_n, juego);
    }
}

// PRE: coordenada es un puntero válido a una estructura de coordenada, cuadrante_n es un valor válido que indica el cuadrante, juego es un juego válido.
// POS: Inicializa los valores de la estructura de coordenada apuntada por "coordenada" generando una coordenada aleatoria en base al cuadrante_n.
// Si la coordenada generada está ocupada en los obstáculos, herramientas, ingredientes de comida del juego, o coincide con las posiciones de los personajes o la puerta,
// se vuelve a generar una nueva coordenada aleatoria hasta encontrar una coordenada no ocupada.

void inicializar_coordenadas_general(coordenada_t* coordenada, int cuadrante_n, juego_t juego){
    crear_coordenada_aleatoria(coordenada, cuadrante_n);
    if(esta_ocupada(juego, *coordenada)){
        inicializar_coordenadas_general(coordenada, cuadrante_n, juego);
    }
}

// INICIALIZAR_JUEGO

// PRE: juego es un puntero válido a una estructura de juego, precio es un valor válido.
// POS: Inicializa los valores de las propiedades del juego.

void inicializar_datos(juego_t* juego, int precio){
    juego->personaje_activo = STITCH;
    juego->comida_actual = ENSALADA;
    juego->precio_total = 0;
    juego->movimientos = 0;
    juego->tope_paredes = 0;
    juego->tope_obstaculos = 0;
    juego->tope_herramientas = 0;
    juego->tope_comida = 0;
    juego->tope_comida_lista = 0;
    juego->mesa.fil = COORDENADA_MEDIA_FILA;
    juego->mesa.col = COORDENADA_MEDIA_COLUMNA;
}

// PRE: i y j son valores enteros válidos.
// POS: Devuelve true si la coordenada (i, j) cumple una condición específica relacionada con las coordenadas mínimas, máximas y medias.
// Devuelve false en caso contrario.

bool condicion_pared(int i, int j){
    return (i == COORDENADA_MIN_FILA || i == COORDENADA_MAX_FILA || j == COORDENADA_MIN_COLUMNA || j == COORDENADA_MAX_COLUMNA || (i == COORDENADA_MEDIA_FILA && j != COORDENADA_MEDIA_COLUMNA));
}

// PRE: paredes es un arreglo válido de coordenadas, tope es un puntero válido a un entero.
// POS: Inicializa el arreglo de coordenadas "paredes" con las coordenadas que cumplen la condición de ser paredes.
// Actualiza el valor apuntado por "tope" para reflejar la cantidad de coordenadas agregadas a "paredes".

void inicializar_paredes(coordenada_t paredes[MAX_PAREDES], int* tope){
    for (int i = COORDENADA_MIN_FILA; i <= COORDENADA_MAX_FILA; i++){
        for (int j = COORDENADA_MIN_COLUMNA; j <= COORDENADA_MAX_COLUMNA; j++){
            if(condicion_pared(i, j)){
                paredes[*tope].fil = i;
                paredes[*tope].col = j;
                (*tope)++;
            }
        }
    } 
}

// PRE: Parámetros válidos.
// POS: Inicializa obstaculos según tipo, cantidad y cuadrante del juego. Incrementa "tope" en cantidad de obstaculos inicializadas.

void inicializar_obstaculos(objeto_t obstaculos[MAX_OBSTACULOS], char tipo, int cantidad_obstaculos, int* tope, int cuadrante_n, juego_t juego){
    for(int i = 0; i < cantidad_obstaculos; i++){
        if (tipo == FUEGO){
        inicializar_coordenadas_general(&(obstaculos[*tope].posicion), cuadrante_n, juego);
        } else {
        inicializar_coordenadas_vectores_y_personajes(&(obstaculos[*tope].posicion), cuadrante_n, juego);
        }
        obstaculos[*tope].tipo = tipo;
        (*tope)++;
    }
}

// PRE: Parámetros válidos.
// POS: Inicializa herramientas según tipo, cantidad y cuadrante del juego. Incrementa "tope" en cantidad de herramientas inicializadas.

void inicializar_herramientas(objeto_t herramientas[MAX_HERRAMIENTAS], char tipo, int cantidad_herramientas, int* tope, int cuadrante_n, juego_t juego){
    for(int i = 0; i < cantidad_herramientas; i++){
        if (tipo == MATAFUEGOS){
        inicializar_coordenadas_general(&(herramientas[*tope].posicion), cuadrante_n, juego);
        } else {
        inicializar_coordenadas_vectores_y_personajes(&(herramientas[*tope].posicion), cuadrante_n, juego);
        }
        herramientas[*tope].tipo = tipo;
        (*tope)++;
    }
}

// PRE: Parámetros válidos.
// POS: Inicializa un ingrediente de comida según tipo y cuadrante del juego en el arreglo de comida.
// Establece las propiedades de cocinado y corte como falsas. Incrementa el contador de ingredientes de la comida correspondiente.

void inicializar_ingredientes(comida_t comida[MAX_COMIDA], int i, char tipo_ingrediente, int cuadrante_ingrediente_n, int numero_comida, juego_t juego){
    comida[numero_comida].ingrediente[i].tipo = tipo_ingrediente;
    if (numero_comida == PRIMER_COMIDA){
        inicializar_coordenadas_vectores_y_personajes(&comida[numero_comida].ingrediente[i].posicion, cuadrante_ingrediente_n, juego);
    } else {
        inicializar_coordenadas_general(&comida[numero_comida].ingrediente[i].posicion, cuadrante_ingrediente_n, juego);
    }
    comida[numero_comida].ingrediente[i].esta_cocinado = false;
    comida[numero_comida].ingrediente[i].esta_cortado = false;
    (comida[numero_comida].tope_ingredientes)++;
}

// PRE: Parámetros válidos.
// POS: Inicializa una comida según tipo en el arreglo de comida. Establece el contador de ingredientes de la comida como cero.
// Incrementa el contador de comidas. Inicializa los ingredientes de la comida usando la función "inicializar_ingredientes".

void inicializar_comidas(comida_t comida[MAX_COMIDA], char tipo_comida, int NUMERO_INGREDIENTES, char tipo_ingrediente[MAX_INGREDIENTES], int cuadrante_ingrediente_n[MAX_INGREDIENTES], int* tope_comida, juego_t juego){
    (*tope_comida)++;
    comida[(*tope_comida)-1].tipo = tipo_comida;
    comida[(*tope_comida)-1].tope_ingredientes = 0;
    for (int i = 0; i < NUMERO_INGREDIENTES ; i++) {
        inicializar_ingredientes(comida, i, tipo_ingrediente[i], cuadrante_ingrediente_n[i], (*tope_comida)-1, juego);
    }
}

// PRE: Parámetros válidos.
// POS: Inicializa un personaje según tipo y cuadrante del juego. Inicializa la posición del personaje usando la función "inicializar_coordenadas_vectores_y_personajes".
// Establece el tipo de personaje y el objeto en mano como vacío.

void inicializar_personajes(personaje_t* personaje, char tipo, int cuadrante_n, juego_t juego){
    inicializar_coordenadas_vectores_y_personajes(&personaje->posicion, cuadrante_n, juego);
    personaje->tipo = tipo;
    personaje->objeto_en_mano = VACIO;
}

// IMPRIMIR_TERRENO

// PRE: Matriz válida de tamaño MAX_FILAS x MAX_COLUMNAS.
// POS: Rellena la matriz con espacios en blanco, dejándola vacía.

void imprimir_matriz_vacia(char matriz[MAX_FILAS][MAX_COLUMNAS]){
    for (int i = 0; i < MAX_FILAS; i++){
        for (int j = 0; j < MAX_COLUMNAS; j++){
            matriz[i][j] = ESPACIO;
        }  
    }
}

// PRE: Matriz y arreglo de paredes válidos. "tope" indica la cantidad de paredes válidas en el arreglo.
// POS: Marca las posiciones de las paredes en la matriz con el carácter PARED.

void imprimir_paredes(char matriz[MAX_FILAS][MAX_COLUMNAS], coordenada_t paredes[MAX_PAREDES], int tope){
    for (int i = 0; i < tope; i++){
        matriz[paredes[i].fil][paredes[i].col] = PARED;  
    }
}

// PRE: Matriz y arreglo de objetos válidos. "tope" indica la cantidad de objetos válidos en el arreglo.
// POS: Marca las posiciones de los objetos en la matriz, excepto aquellos que están en manos de Stitch o Reuben.

void imprimir_objetos_cuadrantes(char matriz[MAX_FILAS][MAX_COLUMNAS], objeto_t objeto[MAX_OBJETOS], int tope, personaje_t stitch, personaje_t reuben){
    for (int i = 0; i < tope; i++){
        if (objeto[i].tipo != stitch.objeto_en_mano && objeto[i].tipo != reuben.objeto_en_mano){
           matriz[objeto[i].posicion.fil][objeto[i].posicion.col] = objeto[i].tipo;
        }
    }
}

// PRE: Matriz y arreglo de objetos válidos. "tope" indica la cantidad de objetos válidos en el arreglo
// POS: Actualiza la matriz colocando en cada posición el tipo de ingrediente, excepto si es el tipo que están sosteniendo stitch o reuben.

void imprimir_ingredientes(char matriz[MAX_FILAS][MAX_COLUMNAS], ingrediente_t ingrediente[MAX_INGREDIENTES], int tope, personaje_t stitch, personaje_t reuben){
    for (int i = 0; i < tope; i++){
        if(ingrediente[i].tipo != stitch.objeto_en_mano && ingrediente[i].tipo != reuben.objeto_en_mano){
            matriz[ingrediente[i].posicion.fil][ingrediente[i].posicion.col] = ingrediente[i].tipo;
        }
    }
}

// Matriz y personaje validos. 
// POS: Actualiza la matriz con el tipo de personaje en la posición correspondiente a las coordenadas del personaje.

void imprimir_personajes(char matriz[MAX_FILAS][MAX_COLUMNAS], personaje_t personaje){
    matriz[personaje.posicion.fil][personaje.posicion.col] = personaje.tipo;
}

// PRE: matriz es una matriz de caracteres válida de dimensiones MAX_FILAS x MAX_COLUMNAS.
// item es una estructura válida de tipo "coordenada_t" que contiene las coordenadas del item. tipo es un caracter válido.
// POS: Actualiza la matriz colocando el caracter tipo en la posición indicada por las coordenadas del item.

void imprimir_item_solitario(char matriz[MAX_FILAS][MAX_COLUMNAS], coordenada_t item, char tipo){
    matriz[item.fil][item.col] = tipo;
}

// PRE: matriz es una matriz de caracteres válida de dimensiones MAX_FILAS x MAX_COLUMNAS.
// POS: Imprime por pantalla los valores de la matriz, los eimprime fila por fila, seguidos de un salto de línea al final de cada fila.

void imprimir_matriz_con_valores(char matriz[MAX_FILAS][MAX_COLUMNAS]){
    for (int i = 0; i < MAX_FILAS; i++){
        for (int j = 0; j < MAX_COLUMNAS; j++){
            printf(" %c", matriz[i][j]);
        } 
        printf("\n");
    }
}

// REALIZAR JUGADA

// PRE: La función recibe un puntero a un carácter 'personaje_activo' válido.
// POS: El valor de la variable apuntada por 'personaje_activo' se actualiza cambiando al personaje opuesto.

void cambiar_personaje(char* personaje_activo){
    if (*personaje_activo == STITCH){
        *personaje_activo = REUBEN;
    } else if (*personaje_activo == REUBEN){
        *personaje_activo = STITCH;
    }
}

// PRE: fila_o_columna es un puntero válido que apunta a una variable entera.
// POS: La variable apuntada por fila_o_columna se incrementa en el valor de manhattan.

void mover_distancia_manhattan(int* fila_o_columna, int manhattan){
   *fila_o_columna += manhattan;
}

// PRE: obstaculos es un arreglo válido de objetos con capacidad para almacenar tope_obstaculos elementos.
// POS: Devuelve un valor booleano indicando si existe al menos un objeto de tipo FUEGO en el arreglo obstaculos.

bool estado_fuego_general(objeto_t obstaculos[MAX_OBSTACULOS], int tope_obstaculos){
    bool esta_iniciado = false;
    for (int i = 0; i < tope_obstaculos; i++){
        if (obstaculos[i].tipo == FUEGO){
            esta_iniciado = true;
        }
    }
    return esta_iniciado;
}

// PRE: movimientos es un puntero válido que apunta a una variable entera.
// POS: Incrementa el valor de la variable apuntada por movimientos en 1 si existe al menos un objeto de tipo FUEGO en el arreglo juego->obstaculos.

void aumentar_movimientos(int* movimientos, objeto_t obstaculos[MAX_OBSTACULOS], int tope_obstaculos){
    if (!estado_fuego_general(obstaculos, tope_obstaculos)){
        (*movimientos)++;
    }
}

// PRE: obstaculos es un arreglo válido de objetos de tipo objeto_t, tope_obstaculos es la cantidad actual de elementos en obstaculos,
// cuadrante_n es el cuadrante del personaje (CUADRANTE_STITCH o CUADRANTE_REUBEN), mesa es la coordenada de la mesa.
// POS: Devuelve true si hay fuego en el cuadrante y posición adecuada según el cuadrante del personaje, en caso contrario devuelve false.

bool estado_fuego_cuadrante(objeto_t obstaculos[MAX_OBSTACULOS], int tope_obstaculos, int cuadrante_n, coordenada_t mesa){
    bool esta_iniciado = false;
    for (int i = 0; i < tope_obstaculos; i++){
        if (obstaculos[i].tipo == FUEGO && cuadrante_n == CUADRANTE_STITCH && obstaculos[i].posicion.fil < mesa.fil){
            esta_iniciado = true;
        } else if (obstaculos[i].tipo == FUEGO && cuadrante_n == CUADRANTE_REUBEN && obstaculos[i].posicion.fil > mesa.fil){
            esta_iniciado = true;
        }
    }
    return esta_iniciado;
}

// PRE: coordenada_actual es una coordenada válida, movimiento_fil y movimiento_col representan los desplazamientos en filas y columnas respectivamente,
// movimientos es un puntero a la variable que almacena la cantidad de movimientos realizados, juego es una estructura válida de tipo juego_t.
// POS: Devuelve true si el movimiento a la coordenada_movida es posible, actualiza el contador de movimientos en caso de ser posible,
// en caso contrario devuelve false sin modificar el contador de movimientos.

bool estado_movimiento(coordenada_t coordenada_actual, int movimiento_fil, int movimiento_col, int* movimientos, juego_t juego){
    bool se_puede = true;
    coordenada_t coordenada_movida;
    coordenada_movida.fil = coordenada_actual.fil + movimiento_fil;
    coordenada_movida.col = coordenada_actual.col + movimiento_col;
    for(int i = 0; i < juego.tope_paredes; i++){
        if(coordenadas_iguales(coordenada_movida, juego.paredes[i])){
            se_puede = false;
        }
    }
    for(int i = 0; i < juego.tope_herramientas; i++){
        if(coordenadas_iguales(coordenada_movida, juego.herramientas[i].posicion) && juego.herramientas[i].tipo == HORNOS){
            se_puede = false;
        }
    }
    for(int i = 0; i < juego.tope_obstaculos; i++){
        if(coordenadas_iguales(coordenada_movida, juego.obstaculos[i].posicion) && juego.obstaculos[i].tipo == FUEGO){
            se_puede = false;
        }
    }
    if (coordenadas_iguales(coordenada_movida, juego.mesa)){
        se_puede = false;
    }
    if (se_puede == true){
        aumentar_movimientos(movimientos, juego.obstaculos, juego.tope_obstaculos);
    }
    
    return se_puede;
}

// PRE: movimiento es un carácter válido (ARRIBA, DERECHA, ABAJO, IZQUIERDA),
// posicion es una coordenada_t válida que representa la posición actual del personaje, movimientos es un puntero a la variable que almacena la cantidad de movimientos realizados.
// POS: Realiza el movimiento del personaje en la dirección indicada por el parámetro movimiento, actualiza la posición del personaje y el contador de movimientos
// según la validación del estado de movimiento en la función estado_movimiento. Si el movimiento no es posible, no se realiza ninguna actualización.

void mover_personaje(char movimiento, coordenada_t* posicion, int* movimientos, juego_t juego){
    switch (movimiento)
    {
    case ARRIBA:
        if(estado_movimiento(*posicion, -MOVER_DISTANCIA_MANHATTAN, 0, movimientos, juego)){
            mover_distancia_manhattan(&posicion->fil, -MOVER_DISTANCIA_MANHATTAN);
        };
        break;
    case DERECHA:
        if(estado_movimiento(*posicion, 0, MOVER_DISTANCIA_MANHATTAN, movimientos, juego)){
            mover_distancia_manhattan(&posicion->col, MOVER_DISTANCIA_MANHATTAN);
        };
        break;
    case ABAJO:
        if(estado_movimiento(*posicion, MOVER_DISTANCIA_MANHATTAN, 0, movimientos, juego)){
            mover_distancia_manhattan(&posicion->fil, MOVER_DISTANCIA_MANHATTAN);
        };
        break;
    case IZQUIERDA:
        if(estado_movimiento(*posicion, 0, -MOVER_DISTANCIA_MANHATTAN, movimientos, juego)){
            mover_distancia_manhattan(&posicion->col, -MOVER_DISTANCIA_MANHATTAN);
        };
        break;
    }
}

// PRE: juego es válido, movimiento es un carácter válido que representa la dirección del movimiento.
// POS: Selecciona al personaje activo en el juego (STITCH o REUBEN) y realiza el movimiento indicado por el parámetro movimiento
// actualizando la posición del personaje y el contador de movimientos.

void seleccionar_y_mover_personaje(juego_t* juego, char movimiento){
    if (juego->personaje_activo == STITCH){
        mover_personaje(movimiento, &juego->stitch.posicion, &juego->movimientos, *juego);
    } else {
        mover_personaje(movimiento, &juego->reuben.posicion, &juego->movimientos, *juego);
    }
}

// PRE: juego es válido.
// POS: Inicializa el modo de fuego en el juego (fuego y matafuegos). 

void inicializar_modo_fuego(juego_t* juego){
    int resultado = generar_numero_aleatorio(CUADRANTE_REUBEN, CUADRANTE_STITCH);
    inicializar_obstaculos(juego->obstaculos, FUEGO, NUMERO_FUEGOS, &(juego->tope_obstaculos), resultado, *juego);
    inicializar_herramientas(juego->herramientas, MATAFUEGOS, NUMERO_MATAFUEGOS, &(juego->tope_herramientas), resultado, *juego);
}

// PRE: a y b son coordenadas válidas.
// POS: Calcula la distancia de Manhattan entre las coordenadas a y b y devuelve el resultado como un entero. La distancia de Manhattan se calcula
// sumando las diferencias absolutas entre las coordenadas en filas y columnas.

int calcular_distancia_manhattan(coordenada_t a, coordenada_t b){
    int distancia_fil = 0; 
    int distancia_col = 0;
    if (b.fil >= a.fil) {
        distancia_fil = b.fil - a.fil;
    } else {
        distancia_fil = a.fil - b.fil;
    }
    if (b.col >= a.col) {
        distancia_col = b.col - a.col;
    } else {
        distancia_col = a.col - b.col;
    }
    int distancia_total = distancia_fil + distancia_col;
    return distancia_total;
}

// PRE: juego es un juego válido y coordenada_a_verificar es una coordenada válida dentro del juego.
// POS: Devuelve el tipo de objeto que se encuentra debajo de la coordenada_a_verificar en el juego. Si no hay ningún objeto, devuelve ESPACIO.
// El tipo de objeto puede ser un obstáculo, una herramienta, un ingrediente de comida o la salida.

char objeto_abajo(juego_t juego, coordenada_t coordenada_a_verificar){
    char objeto_abajo = ESPACIO;
    for (int i = 0; i < juego.tope_obstaculos; i++){
        if(coordenadas_iguales(coordenada_a_verificar, juego.obstaculos[i].posicion)){
            objeto_abajo = juego.obstaculos[i].tipo;
        }
    }  
    for (int j = 0; j < juego.tope_herramientas; j++){
        if(coordenadas_iguales(coordenada_a_verificar, juego.herramientas[j].posicion)){
            objeto_abajo = juego.herramientas[j].tipo;
        }
    } 
    for (int k = 0; k < juego.comida[juego.tope_comida-1].tope_ingredientes; k++){
        if(coordenadas_iguales(coordenada_a_verificar, juego.comida[juego.tope_comida-1].ingrediente[k].posicion)){
            objeto_abajo = juego.comida[juego.tope_comida-1].ingrediente[k].tipo;
        }
    }
    if (coordenadas_iguales(coordenada_a_verificar, juego.salida)){
        objeto_abajo = SALIDA;
    }
    return objeto_abajo;
}

// PRE: comida es un arreglo válido de comidas, tope_comida es el número de comidas válidas en el arreglo, personaje es un puntero válido a un personaje y juego es un juego válido.
// POS: Realiza interacciones entre el personaje y los ingredientes de la última comida en comida, actualizando su estado según sus acciones.

void manipular_ingredientes(comida_t comida[MAX_COMIDA], int tope_comida, personaje_t* personaje, juego_t juego){
    if (personaje->objeto_en_mano == VACIO){
        for (int i = 0; i < comida[tope_comida-1].tope_ingredientes; i++){
            if (coordenadas_iguales(personaje->posicion, comida[tope_comida-1].ingrediente[i].posicion)){
                personaje->objeto_en_mano = comida[tope_comida-1].ingrediente[i].tipo;
            } 
        }
    } else if ((objeto_abajo(juego, personaje->posicion) == ESPACIO || objeto_abajo(juego, personaje->posicion) == personaje->objeto_en_mano) && personaje->objeto_en_mano != MATAFUEGOS){
        for (int i = 0; i < comida[tope_comida-1].tope_ingredientes; i++){
            if (comida[tope_comida-1].ingrediente[i].tipo == personaje->objeto_en_mano){
                comida[tope_comida-1].ingrediente[i].posicion = personaje->posicion;
            }
        }
        personaje->objeto_en_mano = VACIO; 
    }
}

// PRE: -
// POS: Devuelve true si el tipo de ingrediente es apto para ser cortado en la fila especificada, o false en caso contrario.

bool estado_cortar(char tipo){
    bool se_puede_cortar = false;
    if((tipo == LECHUGA || tipo == TOMATE || tipo == JAMON || tipo == QUESO || tipo == PAN)){
        se_puede_cortar = true;
    }
    return se_puede_cortar;
}

// PRE: -
// POS: Devuelve true si el tipo de ingrediente es apto para ser cocinado, o false en caso contrario.

bool estado_cocinar(char tipo){
    bool se_puede_cocinar = false;
    if((tipo == CARNE || tipo == MASA || tipo == MILANESA)){
        se_puede_cocinar = true;
    };
    return se_puede_cocinar;
}

// PRE: -
// POS: Corta el ingrediente en la posición del personaje si cumple las condiciones necesarias. 
// Actualiza el atributo 'esta_cortado' del ingrediente correspondiente en el array 'comida'.

void cortar_ingrediente(comida_t comida[MAX_COMIDA], int tope_comida, personaje_t personaje, juego_t juego){
    int i = 0;
    while (i < comida[tope_comida-1].tope_ingredientes && objeto_abajo(juego, personaje.posicion) == CORTAR){
        if(comida[tope_comida-1].ingrediente[i].tipo == personaje.objeto_en_mano && estado_cortar(personaje.objeto_en_mano)){
            comida[tope_comida-1].ingrediente[i].esta_cortado = true;
        }
        i++;
    }
}

// PRE: Comida y herramientas válidos. 'tope_comida' y 'personaje' son índices y personajes válidos, respectivamente.
// POS: Cocina ingredientes si se cumplen las condiciones. Actualiza 'esta_cocinado' en comida.

void cocinar_ingrediente(comida_t comida[MAX_COMIDA], int tope_comida, objeto_t herramientas[MAX_HERRAMIENTAS], int tope_herramientas, personaje_t personaje){
    for (int i = 0; i < tope_herramientas; i++){
        if (herramientas[i].tipo == HORNOS){
            for (int j = 0; j < comida[tope_comida-1].tope_ingredientes; j++){
                if(comida[tope_comida-1].ingrediente[j].tipo == personaje.objeto_en_mano && estado_cocinar(personaje.objeto_en_mano) && calcular_distancia_manhattan(personaje.posicion, herramientas[i].posicion) == 1){
                    comida[tope_comida-1].ingrediente[j].esta_cocinado = true;
                }
            }
        }
    }
}

// PRE: personaje, mesa, comida, y juego son válidos.
// POS: Mueve ingredientes a la mesa si el personaje tiene un objeto en la mano y cumple las condiciones.
// Si el personaje no tiene un objeto en la mano y hay un objeto en la mesa, lo recoge.

void manipular_ingrediente_en_mesa(personaje_t* personaje, coordenada_t mesa, comida_t comida[MAX_COMIDA], int tope_comida, juego_t juego){
    if (personaje->objeto_en_mano != VACIO && personaje->objeto_en_mano != MATAFUEGOS && calcular_distancia_manhattan(personaje->posicion, mesa) < 2 && objeto_abajo(juego, mesa) == ESPACIO){
        for (int i = 0; i < comida[tope_comida-1].tope_ingredientes; i++){
            if(comida[tope_comida-1].ingrediente[i].tipo == personaje->objeto_en_mano){
                comida[tope_comida-1].ingrediente[i].posicion = mesa;
                comida[tope_comida-1].ingrediente[i].tipo = personaje->objeto_en_mano;
                personaje->objeto_en_mano = VACIO;
            }
        }
    }
    if (personaje->objeto_en_mano == VACIO && calcular_distancia_manhattan(personaje->posicion, mesa) < 2 && objeto_abajo(juego, mesa) != ESPACIO){
        for (int i = 0; i < comida[tope_comida-1].tope_ingredientes; i++){
            if (comida[tope_comida-1].ingrediente[i].posicion.fil == mesa.fil){
                personaje->objeto_en_mano = comida[tope_comida-1].ingrediente[i].tipo;
                comida[tope_comida-1].ingrediente[i].posicion = personaje->posicion;
            }
        }
    }
}

// PRE: personaje, obstaculos, herramientas, tope_obstaculos, tope_herramientas y movimientos son válidos.
// POS: Si el personaje tiene un matafuegos y cumple las condiciones, apaga los fuegos cercanos.
// Elimina el fuego de los obstáculos y el matafuegos de las herramientas.
// Actualiza el estado del objeto en la mano del personaje a VACIO y reinicia el contador de movimientos.

void apagar_fuego(personaje_t* personaje, objeto_t obstaculos[MAX_OBSTACULOS], int* tope_obstaculos, objeto_t herramientas[MAX_HERRAMIENTAS], int* tope_herramientas, int* movimientos){
    int i = 0;
    while (i < (*tope_obstaculos) && (personaje->objeto_en_mano == MATAFUEGOS)){
        if (calcular_distancia_manhattan(personaje->posicion, obstaculos[i].posicion) < 3 && obstaculos[i].tipo == FUEGO){
            eliminar_elemento_vector_objeto(obstaculos, tope_obstaculos, i);
            for (int j = 0; j < (*tope_herramientas); j++){
                if (herramientas[j].tipo == MATAFUEGOS){
                    eliminar_elemento_vector_objeto(herramientas, tope_herramientas, j);
                }
            }
            personaje->objeto_en_mano = VACIO;
            (*movimientos) = 0;
        }
        i++;
    }
}

// PRE: personaje, herramientas y juego son válidos.
// POS: Si hay un matafuegos debajo del personaje y el personaje no tiene ningún objeto en la mano, el personaje agarra el matafuegos.

void agarrar_matafuegos(personaje_t* personaje, objeto_t herramientas[MAX_HERRAMIENTAS], int* tope_herramientas, juego_t juego){
    if(objeto_abajo(juego, personaje->posicion) == MATAFUEGOS && personaje->objeto_en_mano == VACIO){
        personaje->objeto_en_mano = MATAFUEGOS;
    }
}

// PRE: tipo, ingrediente y tope_comida son válidos.
// POS: Devuelve true si el tipo de ingrediente es válido y está cortado, de lo contrario devuelve false.

bool estado_cortado_ingrediente(char tipo, ingrediente_t ingrediente, int tope_comida){
    bool estado_cortar = false;
    if((tipo == LECHUGA || tipo == TOMATE || tipo == JAMON || tipo == QUESO || tipo == PAN) && ingrediente.tipo == tipo && ingrediente.esta_cortado == true){
        estado_cortar = true;
    }
    return estado_cortar;
}

// PRE: tipo, ingrediente y tope_comida son válidos.
// POS: Devuelve true si el tipo de ingrediente es válido y está cocinado, de lo contrario devuelve false.

bool estado_cocinado_ingrediente(char tipo, ingrediente_t ingrediente, int tope_comida){
    bool estado_cocinado = false;
    if((tipo == CARNE || tipo == MASA || tipo == MILANESA) && ingrediente.tipo == tipo && ingrediente.esta_cocinado == true){
        estado_cocinado = true;
    }
    return estado_cocinado;
}

// PRE: reuben, comida, comida_lista, tope_comida_lista, tope_comida y juego son válidos.
// POS: Si Reuben está en la posición de la salida y no tiene un matafuegos en la mano, se entrega el ingrediente cortado o cocinado en la lista de comida_lista. 
// El ingrediente se elimina de la comida actual y se actualiza el tope_comida_lista y el objeto en la mano de Reuben.

void entregar_ingrediente(personaje_t* reuben, comida_t comida[MAX_COMIDA], ingrediente_t comida_lista[MAX_INGREDIENTES], int* tope_comida_lista, int tope_comida, juego_t juego){
    if (objeto_abajo(juego, reuben->posicion) == SALIDA && reuben->objeto_en_mano != MATAFUEGOS){
        for (int i = 0; i < comida[tope_comida-1].tope_ingredientes; i++){
            if (estado_cortado_ingrediente(reuben->objeto_en_mano, comida[tope_comida-1].ingrediente[i], tope_comida) || estado_cocinado_ingrediente(reuben->objeto_en_mano,comida[tope_comida-1].ingrediente[i], tope_comida)){
                comida_lista[*tope_comida_lista] = comida[tope_comida-1].ingrediente[i];
                (*tope_comida_lista)++;
                eliminar_elemento_vector_comida(comida, tope_comida, &(comida[tope_comida-1].tope_ingredientes), i);
                reuben->objeto_en_mano = VACIO;
            }
        }
    }
}

// PRE: juego y personaje son válidos. movimiento es un valor válido.
// POS: Realiza la jugada correspondiente al personaje elegido y movimiento.

void realizar_jugada_personaje_elegido(juego_t* juego, personaje_t* personaje, char movimiento){
    switch(movimiento) {
        case CAMBIAR_PERSONAJE:
            cambiar_personaje(&juego->personaje_activo);
            break;
        case ARRIBA:
        case DERECHA:
        case ABAJO:
        case IZQUIERDA:
            seleccionar_y_mover_personaje(juego, movimiento);
            break;
        case MANIPULAR:
            manipular_ingredientes(juego->comida, juego->tope_comida, personaje, *juego);
            break;
        case CORTAR:
            if (juego->personaje_activo == STITCH && !(estado_fuego_cuadrante(juego->obstaculos, juego->tope_obstaculos, CUADRANTE_STITCH, juego->mesa))){
                cortar_ingrediente(juego->comida, juego->tope_comida, *personaje, *juego);
            }
            break;
        case COCINAR:
            if (juego->personaje_activo == REUBEN && !(estado_fuego_cuadrante(juego->obstaculos, juego->tope_obstaculos, CUADRANTE_REUBEN, juego->mesa))){
                cocinar_ingrediente(juego->comida, juego->tope_comida, juego->herramientas, juego->tope_herramientas, *personaje);
            }
            break;
        case DEJAR_EN_MESA:
            if (juego->personaje_activo == STITCH && !(estado_fuego_cuadrante(juego->obstaculos, juego->tope_obstaculos, CUADRANTE_STITCH, juego->mesa))){
                manipular_ingrediente_en_mesa(&juego->stitch, juego->mesa, juego->comida, juego->tope_comida, *juego);
            } else if (juego->personaje_activo == REUBEN && !(estado_fuego_cuadrante(juego->obstaculos, juego->tope_obstaculos, CUADRANTE_REUBEN, juego->mesa))){
                manipular_ingrediente_en_mesa(&juego->reuben, juego->mesa, juego->comida, juego->tope_comida, *juego);
            }
            break;
        case MATAFUEGOS:
            apagar_fuego(personaje, juego->obstaculos, &juego->tope_obstaculos, juego->herramientas, &juego->tope_herramientas, &juego->movimientos);
            break;
        default:
            break;
    }
}

// PRE: juego es valido.
// POS: Agrega comida al juego según las reglas establecidas. Si se alcanza la cantidad necesaria de ingredientes para una comida específica
// y la comida actual coincide con esa comida, se inicializa la comida correspondiente y se reinicia el contador de ingredientes agregados. 
// El tipo de comida actual se actualiza según la comida inicializada.

void agregar_comida(juego_t* juego){
    if(juego->tope_comida_lista == NUMERO_INGREDIENTES_ENSALADA && juego->comida_actual == ENSALADA){
        char tipo_ingrediente_pizza[MAX_INGREDIENTES] = {JAMON, QUESO, MASA};
        int cuadrante_ingrediente_pizza[MAX_INGREDIENTES] = {CUADRANTE_STITCH, CUADRANTE_STITCH, CUADRANTE_REUBEN};
        inicializar_comidas(juego->comida, PIZZA, NUMERO_INGREDIENTES_PIZZA, tipo_ingrediente_pizza, cuadrante_ingrediente_pizza, &juego->tope_comida, *juego);
        juego->tope_comida_lista = 0;
        juego->comida_actual = PIZZA;
    }
    if(juego->tope_comida_lista == NUMERO_INGREDIENTES_PIZZA && juego->comida_actual == PIZZA){
        char tipo_ingrediente_hamburguesa[MAX_INGREDIENTES] = {PAN, CARNE, LECHUGA, TOMATE};
        int cuadrante_ingrediente_hamburguesa[MAX_INGREDIENTES] = {CUADRANTE_STITCH, CUADRANTE_REUBEN, CUADRANTE_STITCH, CUADRANTE_STITCH};
        inicializar_comidas(juego->comida, HAMBURGUESA, NUMERO_INGREDIENTES_HAMBURGUESA, tipo_ingrediente_hamburguesa, cuadrante_ingrediente_hamburguesa, &juego->tope_comida, *juego);
        juego->tope_comida_lista = 0;
        juego->comida_actual = HAMBURGUESA;
    }
    if(juego->tope_comida_lista == NUMERO_INGREDIENTES_HAMBURGUESA && juego->comida_actual == HAMBURGUESA){
        char tipo_ingrediente_sandwich[MAX_INGREDIENTES] = {MILANESA, PAN, TOMATE, LECHUGA, JAMON, QUESO};
        int cuadrante_ingrediente_sandwich[MAX_INGREDIENTES] = {CUADRANTE_REUBEN, CUADRANTE_STITCH, CUADRANTE_STITCH, CUADRANTE_STITCH, CUADRANTE_STITCH, CUADRANTE_STITCH};
        inicializar_comidas(juego->comida, SANDWICH, NUMERO_INGREDIENTES_SANDWICH, tipo_ingrediente_sandwich, cuadrante_ingrediente_sandwich, &juego->tope_comida, *juego);
        juego->tope_comida_lista = 0;
        juego->comida_actual = SANDWICH;
    }
}

// ESTADO JUEGO

// PRE: estado_juego y juego son valores válidos.
// POS: Si hay agujeros debajo de los personajes 'stitch' o 'reuben' en el juego, establece el estado del juego en "PERDER".

int perder(int estado_juego, juego_t juego){
    if(objeto_abajo(juego, juego.stitch.posicion) == AGUJEROS || objeto_abajo(juego, juego.reuben.posicion) == AGUJEROS){
        estado_juego = PERDER;
    }
    return estado_juego;
}

// PRE: estado_juego, tope, y precio son valores válidos. comida_actual es un carácter válido.
// POS: Si se cumplen las condiciones necesarias para ganar el juego, establece el estado del juego en "GANAR".

int ganar(int estado_juego, int tope, int precio, char comida_actual){
    if (precio <= PRECIO_HASTA_PIZZA && comida_actual == HAMBURGUESA){
        estado_juego = GANAR;
    } else if (precio <= PRECIO_HASTA_HAMBURGUESA && comida_actual == SANDWICH){
        estado_juego = GANAR;
    } else if (tope == NUMERO_INGREDIENTES_SANDWICH && precio > PRECIO_HASTA_SANDWICH && comida_actual == SANDWICH){
        estado_juego = GANAR;
    }
    return estado_juego;
}

// FUNCIONES PRINCIPALES

void inicializar_juego(juego_t* juego, int precio){
    inicializar_datos(juego, precio);
    inicializar_paredes(juego->paredes, &juego->tope_paredes);
    inicializar_obstaculos(juego->obstaculos, AGUJEROS, NUMERO_AGUJEROS_STITCH, &(juego->tope_obstaculos), CUADRANTE_STITCH, *juego);
    inicializar_obstaculos(juego->obstaculos, AGUJEROS, NUMERO_AGUJEROS_REUBEN, &(juego->tope_obstaculos), CUADRANTE_REUBEN, *juego);
    inicializar_herramientas(juego->herramientas, CUCHILLOS, NUMERO_CUCHILLOS, &(juego->tope_herramientas), CUADRANTE_STITCH, *juego);
    inicializar_herramientas(juego->herramientas, HORNOS, NUMERO_HORNOS, &(juego->tope_herramientas), CUADRANTE_REUBEN, *juego);  
    char tipo_ingrediente_ensalada[MAX_INGREDIENTES] = {LECHUGA, TOMATE};
    int cuadrante_ingrediente_ensalada[MAX_INGREDIENTES] = {CUADRANTE_STITCH, CUADRANTE_STITCH};
    inicializar_comidas(juego->comida, ENSALADA, NUMERO_INGREDIENTES_ENSALADA, tipo_ingrediente_ensalada, cuadrante_ingrediente_ensalada, &juego->tope_comida, *juego);
    inicializar_personajes(&(juego->stitch), STITCH, CUADRANTE_STITCH, *juego);
    inicializar_personajes(&(juego->reuben), REUBEN, CUADRANTE_REUBEN, *juego);
    inicializar_coordenadas_puerta(&(juego->salida), CUADRANTE_REUBEN, *juego);
}

void realizar_jugada(juego_t* juego, char movimiento){  
    if(juego->personaje_activo == STITCH){
        realizar_jugada_personaje_elegido(juego, &juego->stitch, movimiento);
    } else {
        realizar_jugada_personaje_elegido(juego, &juego->reuben, movimiento);
    }
    if (juego->personaje_activo == REUBEN && !(juego->obstaculos[juego->tope_obstaculos-1].tipo == FUEGO)){
        entregar_ingrediente(&juego->reuben, juego->comida, juego->comida_lista, &juego->tope_comida_lista, juego->tope_comida, *juego);
    }
    agregar_comida(juego);
    if(juego->movimientos == NUMERO_APARECION_FUEGO && !estado_fuego_general(juego->obstaculos, juego->tope_obstaculos)){
        inicializar_modo_fuego(juego);
    }
    if (juego->personaje_activo == STITCH){
        agarrar_matafuegos(&juego->stitch, juego->herramientas, &juego->tope_herramientas, *juego);
    } else {
        agarrar_matafuegos(&juego->reuben, juego->herramientas, &juego->tope_herramientas, *juego);
    }
}

void imprimir_terreno(juego_t juego){
    char matriz[MAX_FILAS][MAX_COLUMNAS];
    imprimir_matriz_vacia(matriz);
    imprimir_paredes(matriz, juego.paredes, juego.tope_paredes);
    imprimir_item_solitario(matriz, juego.mesa, MESA);
    imprimir_objetos_cuadrantes(matriz, juego.obstaculos, juego.tope_obstaculos, juego.stitch, juego.reuben);
    imprimir_objetos_cuadrantes(matriz, juego.herramientas, juego.tope_herramientas, juego.stitch, juego.reuben);
    imprimir_ingredientes(matriz, juego.comida[(juego.tope_comida)-1].ingrediente, juego.comida[(juego.tope_comida)-1].tope_ingredientes, juego.stitch, juego.reuben);
    imprimir_personajes(matriz, juego.stitch);
    imprimir_personajes(matriz, juego.reuben);
    imprimir_item_solitario(matriz, juego.salida, SALIDA);
    imprimir_matriz_con_valores(matriz);
};

int estado_juego(juego_t juego){
    int estado_juego = JUGANDO;
    estado_juego = perder(estado_juego, juego);
    estado_juego = ganar(estado_juego, juego.tope_comida_lista, juego.precio_total, juego.comida_actual);
    return estado_juego;
}