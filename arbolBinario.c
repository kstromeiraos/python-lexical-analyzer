#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analizadorLexico.h"

struct celda {
    lexema info;
    struct celda *izq, *der;
};
typedef struct celda *abin;

void crea(abin *A) {
    A = NULL;
}

unsigned esVacio(abin A) {
    return (A == NULL);
}
//Necesito los prototipos para poder llamar a estas funciones
void supizq(abin *A);
void supder(abin *A);

abin izq(abin A) {
    return A->izq;
}

abin der(abin A) {
    return A->der;
}

void info(abin A, lexema *E) {
    *E = A->info;
}

void modifica(abin *A, lexema E) {
    (*A)->info = E;
}

void insizq(abin *A, lexema E) {
    abin aux;
    aux = (abin) malloc(sizeof (struct celda));
    aux->info = E;
    aux->izq = NULL;
    aux->der = NULL;

    if (esVacio(*A))
        *A = aux;
    else
        (*A)->izq = aux;
}

void insder(abin *A, lexema E) {
    abin aux;
    aux = (abin) malloc(sizeof (struct celda));
    aux->info = E;
    aux->izq = NULL;
    aux->der = NULL;
    if (esVacio(*A))
        *A = aux;
    else
        (*A)->der = aux;
}

void supizq(abin *A) {
    abin aux;
    aux = izq(*A);
    if (!esVacio(aux)) {
        supizq(&aux);
        supder(&aux);
        (*A)->izq = NULL;
        free(aux);
    }
}

void supder(abin *A) {
    abin aux;
    aux = der(*A);
    if (!esVacio(aux)) {
        supizq(&aux);
        supder(&aux);
        (*A)->der = NULL;
        free(aux);
    }
}

//Imprime un arbol
void imprimir(abin A) {
	if(A->izq) imprimir(A->izq);
	printf("Lexema: %s\t\t\tCódigo: %d\t\t\tLinea: %d\n",A->info.string, A->info.definicion, A->info.linea);
	if(A->der) imprimir(A->der);
}

//Elimina un arbol
void destruye(abin *A) {
    abin aux;
    aux = *A;
    if (!esVacio(aux)) {
        supizq(&aux);
        supder(&aux);
        free(aux);
        *A = NULL;
    }
}

//Busca un lexema en el arbol
int buscar(abin A, lexema E) {
    abin actual = A;
    
    while (!esVacio(actual)) {
        if (strcmp(E.string, actual->info.string) == 0) return actual->info.definicion; /* dato encontrado */
        else if (strcmp(E.string, actual->info.string) < 0) actual = actual->izq; /* Seguir */
        else if (strcmp(E.string, actual->info.string) > 0) actual = actual->der;
    }
    return 0; /* No está en árbol */
}

int insertar(abin *A, lexema E) {
    if (!(*A)) {
        (*A) = (abin)malloc(sizeof(lexema)*3);
        (*A)->izq = (*A)->der = NULL;
        (*A)->info.string = (char*) malloc(sizeof(E.string));
        (*A)->info.string = E.string;
        (*A)->info.definicion = E.definicion;
        (*A)->info.linea = E.linea;
        return E.definicion;
    }
    if (strcmp(E.string, (*A)->info.string) < 0)
        return insertar(&(*A)->izq, E);
    else if (strcmp(E.string, (*A)->info.string) > 0)
        return insertar(&(*A)->der, E);
    else
        return (*A)->info.definicion;
}
