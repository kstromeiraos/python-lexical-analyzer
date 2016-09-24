/* 
 * File:   analizadorLexico.h
 * Author: 
 *
 * Created on 18 de octubre de 2015, 0:17
 */

//Estructura en la que se almacena el componente léxico leído
typedef struct {
	char* string;
	int definicion;
	int linea;
} lexema; 

//Concatena el caracter actual al lexema
char* concatenar(char *cadena, char caracter);

//Asigna el caracter actual al principio del lexema
void asignarAlPrincipio(char *cadena, char caracter);

//Analizador léxico que devuelve componentes léxicos al analizador sintáctico
lexema siguienteCompLexico();