/* 
 * File:   tablaSimbolos.h
 * Author: 
 *
 * Created on 1 de noviembre de 2015, 22:22
 */

void initTabla(); //Insercion de palabras reservadas en la tabla de simbolos

void imprimirTabla(); //Impresión de la tabla de símbolos

void insertarTabla(lexema lex); //Insertar un lexema en la tabla de simbolos

int buscarTabla(lexema lex); //Buscar un elemento en la tabla de simbolos

void liberarTabla(); //Libera la memoria de la tabla de simbolos
