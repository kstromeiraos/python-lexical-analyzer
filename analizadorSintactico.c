/* 
 * File:   main.c
 * Author: 
 *
 * Created on 14 de octubre de 2015, 12:09
 */

#include <stdio.h>
#include <stdlib.h>
#include "sistemaEntrada.h"
#include "definiciones.h"
#include "arbolBinario.h"
#include "tablaSimbolos.h"

/*
 * 
 */
int main(int argc, char** argv) {

    printf("******************************************************\n");
    printf("************ ANALIZADOR LEXICO PYTHON ****************\n");
    printf("******************************************************\n\n");
    //Inicializamos el archivo y la tabla de símbolos
    if (initArchivo() == -1) {
        return -1;
    };
    initTabla();

    //Imprimimos la tabla de simbolos inicial (palabras reservadas)
    printf("----------------------------------------------------------\n");
    printf("TABLA DE SIMBOLOS INICIAL\n");
    printf("----------------------------------------------------------\n");
    imprimirTabla();
    printf("----------------------------------------------------------");

    //Creación y reserva de memoria para el lexema leído
    lexema lex; //Componente léxico leído
    lex.string = (char *) malloc(sizeof (128)); //Reserva de memoria inicial

    int counter = 1; //Contador de componentes léxicos

    //Análisis léxico
    printf("\n\n----------------------------------------------------------\n");
    printf("ANALISIS LÉXICO\n");
    printf("----------------------------------------------------------\n");
    lex = siguienteCompLexico(); //Leemos el primer componente léxico
    while (lex.string[0] != EOF) {
        if (lex.definicion != -1) {
            if (lex.definicion == 10) {
                printf("\nLEX: %d || LINE: %d || ID: %d\n\\n", counter, lex.linea, lex.definicion);
            } else if (lex.definicion == INDENTACION) {
                printf("\nLEX: %d || LINE: %d || ID: %d\nINDENTACION", counter, lex.linea, lex.definicion);
            } else {
                printf("\nLEX: %d || LINE: %d || ID: %d\n%s", counter, lex.linea, lex.definicion, lex.string);
            }
            lex = siguienteCompLexico();
            counter++;
        } else {
            lex = siguienteCompLexico();
        }
    }
    printf("\n----------------------------------------------------------");

    //Imprimimos la tabla de simbolos final y liberamos la memoria
    printf("\n\n----------------------------------------------------------\n");
    printf("TABLA DE SIMBOLOS FINAL\n");
    printf("----------------------------------------------------------\n");
    imprimirTabla();
    printf("----------------------------------------------------------");

    //Liberamos la memoria
    liberarTabla();
    free(lex.string);

    return (EXIT_SUCCESS);
}

