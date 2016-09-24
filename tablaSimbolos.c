/* 
 * File:   tablaSimbolos.c
 * Author: 
 *
 * Created on 14 de octubre de 2015, 12:11
 */

#include <stdio.h>
#include <stdlib.h>
#include "definiciones.h"
#include "arbolBinario.h"
#define TAM_MAXIMO 128

abin a;

void initTabla() { //Insercion de palabras reservadas en la tabla de simbolos
    
    a=NULL;
    crea(&a); //Iniciamos el árbol binario

    lexema lex;
    lex.string = (char*) malloc(TAM_MAXIMO);
    
    lex.linea = 0; //Las palabras reservadas tienen 0 como linea

    //Insertamos las palabras reservadas en la tabla de símbolos
    lex.string = "import";
    lex.definicion = IMPORT;
    insertar(&a, lex);
    lex.string = "as";
    lex.definicion = AS;
    insertar(&a, lex);
    lex.string = "def";
    lex.definicion = DEF;
    insertar(&a, lex);
    lex.string = "for";
    lex.definicion = FOR;
    insertar(&a, lex);
    lex.string = "in";
    lex.definicion = IN;
    insertar(&a, lex);
    lex.string = "if";
    lex.definicion = IF;
    insertar(&a, lex);
    lex.string = "elif";
    lex.definicion = ELIF;
    insertar(&a, lex);
    lex.string = "else";
    lex.definicion = ELSE;
    insertar(&a, lex);
    lex.string = "return";
    lex.definicion = RETURN;
    insertar(&a, lex);
    lex.string = "print";
    lex.definicion = PRINT;
    insertar(&a, lex);
    lex.string = "not";
    lex.definicion = NOT;
    insertar(&a, lex);
    lex.string = "len";
    lex.definicion = LEN;
    insertar(&a, lex);
    lex.string = "abs";
    lex.definicion = ABS;
    insertar(&a, lex);
    lex.string = "xrange";
    lex.definicion = XRANGE;
    insertar(&a, lex);
    lex.string = "sorted";
    lex.definicion = SORTED;
    insertar(&a, lex);
    lex.string = "min";
    lex.definicion = MIN;
    insertar(&a, lex);
}

void imprimirTabla(){
    imprimir(a);
}

void insertarTabla(lexema lex){
    insertar(&a, lex);   
}

int buscarTabla(lexema lex){
    return(buscar(a, lex));   
}

void liberarTabla(){
    destruye(&a);
}




