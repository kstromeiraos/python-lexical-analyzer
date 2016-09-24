/* 
 * File:   sistemaEntrada.c
 * Author: 
 *
 * Created on 14 de octubre de 2015, 12:11
 */

/*
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "errores.h"
#define SIZE 128 //Tamaño del buffer

FILE* archivo; //Puntero al archivo a leer

//Declaramos los dos buffers para leer el archivo
char buffer1[SIZE + 1];
char buffer2[SIZE + 1];

int posicion_buffer1 = 0; //Posición del buffer 1
int posicion_buffer2 = 0; //Posición del buffer 2

int bufferActual = 1; //Variable que indica el buffer que se está leyendo actualmente
int archivoLeido = 0; //Variable que indica si el archivo se ha terminado de leer

int bufferFlag = 0; //Bandera para evitar sobreescrituras en el buffer

void cargarBuffer1() { //Funcion para cargar el primer buffer
    int count = 0; //Cuenta el número de bytes leídos, lo usamos para detectar EOF

    //Leemos el primer buffer
    count = fread(&buffer1, sizeof (char), SIZE, archivo);

    if (count != 128) { //Si no se lee el buffer completo, el archivo se ha acabado, por lo que se pone EOF después del último elemento leído y se pone la bandera de archivo leído a 1
        buffer1[count] = EOF;
        archivoLeido = 1;
    }
}

void cargarBuffer2() { //Funcion para cargar el segundo buffer
    int count = 0; //Cuenta el número de bytes leídos, lo usamos para detectar EOF

    //Leemos el segundo buffer
    count = fread(&buffer2, sizeof (char), SIZE, archivo);

    if (count != 128) { //Si no se lee el buffer completo, el archivo se ha acabado, por lo que se pone EOF después del último elemento leído y se pone la bandera de archivo leído a 1
        buffer2[count] = EOF;
        archivoLeido = 1;
    }

}

int initArchivo() {
    archivo = fopen("wilcoxon.py", "r"); //Leemos el archivo

    if(archivo == NULL){ //Detectamos si ha habido un error al abrir el archivo
        imprimirError(ERROR_ABRIR_ARCHIVO, -1, "Error al abrir el archivo");
        return -1;
    }
    
    cargarBuffer1(); //Cargamos el primer buffer

    //Asignamos el centinela al último elemento del buffer
    buffer1[SIZE] = EOF;
    buffer2[SIZE] = EOF;
    
    return 0;
}

char siguienteCaracter() {
    //Lectura de caracteres en el buffer 1
    if (bufferActual == 1) {
        if (buffer1[posicion_buffer1] != EOF) { //Si el buffer1 no se ha terminado de leer, se retorna el caracter
            posicion_buffer1++;
            return buffer1[posicion_buffer1 - 1];
        } else if (archivoLeido == 0 && buffer1[posicion_buffer1] == EOF) { //Si se ha llenado el buffer1, se carga el buffer2 y se cambia el buffer actual
            if (bufferFlag == 0) {
                cargarBuffer2();
            } else {
                bufferFlag = 0;
            }
            bufferActual = 2;
            posicion_buffer1 = 0;
        } else if (archivoLeido == 1 && buffer1[posicion_buffer1] == EOF) { //Si se ha terminado de leer el archivo
            return buffer1[posicion_buffer1];
        }
    }

    //Lectura de caracteres en el buffer 2
    if (bufferActual == 2) {
        if (buffer2[posicion_buffer2] != EOF) { //Si el buffer2 no se ha terminado de leer, se retorna el caracter
            posicion_buffer2++;
            //printf("\nP:%d  C:%c", posicion_buffer2-1, buffer2[posicion_buffer2 - 1]);
            return buffer2[posicion_buffer2 - 1];
        } else if (archivoLeido == 0 && buffer2[posicion_buffer2] == EOF) { //Si se ha llenado el buffer1, se carga el buffer2 y se cambia el buffer actual
            if (bufferFlag == 0) {
                cargarBuffer1();
            } else {
                bufferFlag = 0;
            }
            bufferActual = 1;
            posicion_buffer2 = 0;

            posicion_buffer1++;
            return buffer1[posicion_buffer1-1];
            //siguienteCaracter();
        } else if (archivoLeido == 1 && buffer2[posicion_buffer2] == EOF) { //Si se ha terminado de leer el archivo
            return buffer2[posicion_buffer2];
        }
    }
}

void retrocederPosicion() { //Retrocedemos en el buffer que se esté usando
    if (bufferActual == 1 && posicion_buffer1 != 0) {
        posicion_buffer1--;
    } else if ((bufferActual == 1 && posicion_buffer1 == 0)) {
        bufferActual = 2;
        posicion_buffer2 = SIZE - 1;
        bufferFlag = 1;
    }
    if (bufferActual == 2 && posicion_buffer2 != 0) {
        posicion_buffer2--;
    } else if ((bufferActual == 2 && posicion_buffer2 == 0)) {
        bufferActual = 1;
        posicion_buffer1 = SIZE - 1;
        bufferFlag = 1;
    }
    return;
}
