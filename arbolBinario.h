/* 
 * File:   arbolBinario.h
 * Author: 
 *
 * Created on 1 de noviembre de 2015, 12:47
 */

#include "analizadorLexico.h"

//Interfaz del TAD abin
typedef void *abin; //TIPO OPACO, no sabemos cómo está construído

void crea(abin *A);
void destruye(abin *A);
unsigned esVacio(abin A);
abin izq(abin A);
abin der(abin A);
void info(abin A, lexema *E);
void insizq(abin *A, lexema E);
void insder(abin *A, lexema E);
void supizq(abin *A);
void supder(abin *A);
void modifica(abin *A, lexema E);
void imprimir(abin A);
void destruye(abin *A);
int buscar(abin A, lexema E);
int insertar(abin *A, lexema E);