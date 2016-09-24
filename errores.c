/* 
 * File:   errores.c
 * Author: 
 *
 * Created on 14 de octubre de 2015, 12:10
 */

#include <stdio.h>
#include <stdlib.h>

void imprimirError(int codigo, int linea, char* msg) {
    if (linea != -1) {
        printf("\n**ERROR %d en la linea %d**\n%s", codigo, linea, msg);
    } else {
        printf("\n**ERROR %d**\n%s", codigo, msg);
    }
}


