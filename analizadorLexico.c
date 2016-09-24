/*
 * File:   analizadorlexico.c
 * Author:
 *
 * Created on 14 de octubre de 2015, 12:10
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sistemaEntrada.h"
#include "definiciones.h"
#include "analizadorLexico.h"
#include "tablaSimbolos.h"
#include "errores.h"

#define TAM_MAXIMO_LEXEMA 128 //Tamaño máximo del lexema permitido

size_t bloque = 8; //Tamaño del bloque inicial que se reservará

int indentacion = 0; //Variable para comprobar la indentación
int linea = 1; //Variable para guardar el número de línea
size_t memoria = 0; //Variable para comprobar cuando se debe realizar un realloc
int flagMemoriaExcedida = 0; //Variable para comprobar que el tamaño del lexema excede el máximo permitido

//Concatena el caracter actual al lexema

char* concatenar(char *cadena, char caracter) {
    int i;

    memoria++;

    if (memoria == TAM_MAXIMO_LEXEMA-1) { //Si la memoria usada es el máximo permitido, se produce un error
        imprimirError(TAM_EXCEDIDO, linea, "Tamaño maximo del lexema excedido");
        flagMemoriaExcedida = 1;
    }

    //Si la memoria usada llega a la reserva inicial, se reserva el doble de la actual
    if (memoria == bloque - 1) {
        bloque *= 2;
            cadena = (char*) realloc(cadena, (bloque * sizeof (char)));
    }

    /* Encontrar el fin de cadena */
    for (i = 0; cadena[i] != '\0'; i++)
        ;

    /* Añadimos el caracter */
    cadena[i++] = caracter;

    /* Añadimos el caracter de fin de cadena */
    cadena[i] = '\0';

    return cadena;
}

//Asigna el caracter actual al principio del lexema

void asignarAlPrincipio(char *cadena, char caracter) {
    memoria = 2;

    /* Añadimos el caracter */
    cadena[0] = caracter;

    /* Añadimos el caracter de fin de cadena */
    cadena[1] = '\0';
}

lexema siguienteCompLexico() {

    int estado = 0; //Estado actual
    int subestado = 0; //Subestado
    char actual; //Caracter actual
    lexema lex; //Estructura donde se almacena el componente léxico
    
    //Se inicializan estas variables, la memoria a 0 porque aun no se ha leido nada y el flag de memoria excedida porque aun no se ha excedido
    flagMemoriaExcedida = 0;
    memoria = 0;

    //Reserva de memoria inicial para la estructura
    lex.string = NULL;
    lex.string = (char *) malloc(bloque * sizeof (char));

    while (1 == 1) {
        switch (estado) {
            case 0: //Estado inicial

                //Leemos el primer caracter y lo asignamos al principio del lexema
                actual = siguienteCaracter();
                asignarAlPrincipio(lex.string, actual);

                //Leemos una indentación
                /*Si la bandera de indentación está activada, leemos la indentación y la devolvemos*/
                if (indentacion == 1) {
                    actual = siguienteCaracter();
                    while (actual == ' ') {
                        lex.string=concatenar(lex.string, actual);
                        actual = siguienteCaracter();
                    }
                    retrocederPosicion(); //Cuando se lee algo distinto a una indentación, retrocedemos
                    indentacion = 0; //Desactivamos la bandera de indentaciones
                    if (flagMemoriaExcedida == 1) {
                        lex.definicion = -1;
                        return lex;
                    }
                    lex.linea = linea;
                    lex.definicion = INDENTACION;
                    return lex;
                }

                //Leemos un \n
                if (actual == '\n') {
                    linea++; //Aumentamos una línea

                    actual = siguienteCaracter();

                    /*Si después de leer un \n hay un espacio, se trata de una 
                    indentación, por lo que activamos la bandera que detecta 
                    indentaciones. Si no hay un espacio, simplemente devolvemos
                     el \n */
                    if (actual != ' ') {
                        if (actual == EOF) {
                            asignarAlPrincipio(lex.string, actual);
                            return lex;
                        }
                        //Si no es un espacio, volvemos a la posición anterior y devolvemos el \n
                        retrocederPosicion();
                        lex.linea = linea;
                        lex.definicion = (int) lex.string[0];
                        return lex;
                    } else {
                        //Si es un espacio, volvemos a la posición anterior, activamos la bandera de indentación y devolvemos el \n
                        retrocederPosicion();
                        indentacion = 1;
                        lex.linea = linea;
                        lex.definicion = (int) lex.string[0];
                        return lex;
                    }
                }

                //Leemos un espacio
                if (actual == ' ') {
                    break;
                }

                //Leemos un punto
                if (actual == '.') {
                    actual = siguienteCaracter();

                    if ('0' <= actual && actual <= '9') { //Comprobamos si el punto es el inicio de un número decimal
                        retrocederPosicion();
                        estado = 30; //Vamos al estado "leído un dígito"
                    } else { //Si no es el inicio de un número decimal, volvemos a la posicion anterior y devolvemos el punto
                        retrocederPosicion();
                        lex.linea = linea;
                        lex.definicion = (int) lex.string[0];
                        return lex; //Leído punto
                    }
                }

                //Leemos dos puntos
                if (actual == ':') {
                    lex.linea = linea;
                    lex.definicion = (int)lex.string[0];
                    return lex; //Leído dos puntos
                }

                //Leemos una coma
                if (actual == ',') {
                    lex.linea = linea;
                    lex.definicion = (int) lex.string[0];
                    return lex; //Leída una coma
                }

                //Leemos un mayor
                if (actual == '<') {
                    actual = siguienteCaracter();
                    if (actual == '=') {
                        lex.string=concatenar(lex.string, actual);
                        lex.linea = linea;
                        lex.definicion = MENOR_O_IGUAL;
                        return lex; //Leído <=
                    }
                    retrocederPosicion();
                    lex.linea = linea;
                    lex.definicion = (int) lex.string[0];
                    return lex; //Leído un <
                }

                //Leemos un menor
                if (actual == '>') {
                    actual = siguienteCaracter();
                    if (actual == '=') {
                        lex.string=concatenar(lex.string, actual);
                        lex.linea = linea;
                        lex.definicion = MAYOR_O_IGUAL;
                        return lex; //Leído >=
                    }
                    retrocederPosicion();
                    lex.linea = linea;
                    lex.definicion = (int) lex.string[0];
                    return lex; //Leído un >
                }

                //Leemos un operador
                /* Después de leer un operador, comprobamos si hay un = u otro operador
                ya que de haberlo sería un componente léxico distinto */
                if (actual == '+') {
                    actual = siguienteCaracter();
                    if (actual == '=') {
                        lex.string=concatenar(lex.string, actual);
                        lex.linea = linea;
                        lex.definicion = MAS_IGUAL;
                        return lex; //Leído +=
                    }
                    if (actual == '+') {
                        lex.string=concatenar(lex.string, actual);
                        lex.linea = linea;
                        lex.definicion = MAS_MAS;
                        return lex; //Leído ++
                    }
                    retrocederPosicion();
                    lex.linea = linea;
                    lex.definicion = (int) lex.string[0];
                    return lex; //Leído un +
                }

                //Leemos un operador
                if (actual == '-') {
                    actual = siguienteCaracter();
                    if (actual == '=') {
                        lex.string=concatenar(lex.string, actual);
                        lex.linea = linea;
                        lex.definicion = MENOS_IGUAL;
                        return lex; //Leído -=
                    }
                    if (actual == '-') {
                        lex.string=concatenar(lex.string, actual);
                        lex.linea = linea;
                        lex.definicion = MENOS_MENOS;
                        return lex; //Leído --
                    }
                    retrocederPosicion();
                    lex.linea = linea;
                    lex.definicion = (int) lex.string[0];
                    return lex; //Leído un -
                }

                //Leemos un operador
                if (actual == '*') {
                    actual = siguienteCaracter();
                    if (actual == '=') {
                        lex.string=concatenar(lex.string, actual);
                        lex.linea = linea;
                        lex.definicion = POR_IGUAL;
                        return lex; //Leído *=
                    }
                    if (actual == '*') {
                        lex.string=concatenar(lex.string, actual);
                        lex.linea = linea;
                        lex.definicion = POR_POR;
                        return lex; //Leído **
                    }
                    retrocederPosicion();
                    lex.linea = linea;
                    lex.definicion = (int) lex.string[0];
                    return lex; //Leído un *
                }

                //Leemos un operador
                if (actual == '/') {
                    actual = siguienteCaracter();
                    if (actual == '=') {
                        lex.string=concatenar(lex.string, actual);
                        lex.linea = linea;
                        lex.definicion = ENTRE_IGUAL;
                        return lex; //Leído un /=
                    }
                    if (actual == '/') {
                        lex.string=concatenar(lex.string, actual);
                        lex.linea = linea;
                        lex.definicion = ENTRE_ENTRE;
                        return lex; //Leído un //
                    }
                    retrocederPosicion();
                    lex.linea = linea;
                    lex.definicion = (int) lex.string[0];
                    return lex; //Leído un /
                }

                //Leemos una llave
                if (actual == '{' || actual == '}') {
                    lex.linea = linea;
                    lex.definicion = (int) lex.string[0];
                    return lex; //Leída una llave
                }

                //Leemos un paréntesis
                if (actual == '(' || actual == ')') {
                    lex.linea = linea;
                    lex.definicion = (int) lex.string[0];
                    return lex; //Leído un paréntesis
                }

                //Leemos un corchete
                if (actual == '[' || actual == ']') {
                    lex.linea = linea;
                    lex.definicion = (int) lex.string[0];
                    return lex; //Leído un corchete
                }

                //Leemos un igual
                if (actual == '=') {
                    lex.linea = linea;
                    lex.definicion = (int) lex.string[0];
                    return lex; //Leído un igual
                }

                //Leemos un comentario
                /*Si leemos un comentario, ignoramos el texto hasta encontrar un
                /n */
                if (actual == '#') {
                    while (actual != '\n') {
                        actual = siguienteCaracter();
                    }

                    asignarAlPrincipio(lex.string, actual);
                    //Al encontrar un /n, aumentamos el contador de líneas
                    linea++;

                    actual = siguienteCaracter();

                    /*Si después de leer un \n hay un espacio, se trata de una 
                    indentación, por lo que activamos la bandera que detecta 
                    indentaciones. Si no hay un espacio, simplemente devolvemos
                     el \n */
                    if (actual != ' ') {
                        //Si no es un \n, volvemos a la posición anterior y devolvemos el \n
                        retrocederPosicion();
                        lex.linea = linea;
                        lex.definicion = (int) lex.string[0];
                        return lex;
                    } else {
                        //Si es un \n, volvemos a la posición anterior, activamos la bandera de indentación y devolvemos el \n
                        retrocederPosicion();
                        indentacion = 1;
                        lex.linea = linea;
                        lex.definicion = (int) lex.string[0];
                        return lex;
                    }
                }

                //Leemos una comilla doble
                if (actual == '"') {
                    estado = 10;
                }

                //Leemos una letra o una _
                if (isalpha(actual) || actual == '_') {
                    estado = 20;
                }

                //Leemos un dígito
                if ('0' <= actual && actual <= '9') {
                    estado = 30;
                }

                //Leemos una comilla simple
                if (actual == '\'') {
                    estado = 40;
                }

                //Si el archivo termina, devolvemos EOF
                if (actual == EOF) {
                    lex.linea = linea;
                    return lex;
                }

                if (estado == 0) { //Si no se ha cambiado la variable estado y se ha llegado aquí, es que el caracter no se reconoce por ningún autómata, por lo que se lanza un error
                    lex.definicion = -1;
                    imprimirError(CARACTER_NO_RECONOCIDO, linea, "Caracter no reconocido");
                    return lex;
                    break;
                }

                break;

            case 10: //Leída una comilla doble
                actual = siguienteCaracter();
                lex.string=concatenar(lex.string, actual);

                //Leemos dos comillas dobles
                if (actual == '"') {
                    subestado = 0;
                }//Leemos un caracter
                else {
                    subestado = 1;
                }

                switch (subestado) {
                    case 0: //Leídas dos comillas dobles
                        actual = siguienteCaracter();
                        //Leemos tres comillas dobles (string documental)
                        if (actual == '"') {
                            lex.string=concatenar(lex.string, actual);
                            int num_comillas = 0; //Número de comillas

                            while (num_comillas != 3) { //Leemos caracteres hasta encontrar tres comillas
                                actual = siguienteCaracter();
                                if (actual == '\n') {
                                    linea++;
                                }
                                lex.string=concatenar(lex.string, actual);
                                if (actual == '"') {
                                    num_comillas++;
                                } else {
                                    num_comillas = 0;
                                }

                            }
                            if (flagMemoriaExcedida == 1) { //Si se excede el tamaño máximo del lexema, el código del lexema se pone a -1, lo que se interpreta como que se ha producido un error al leer el lexema
                                lex.definicion = -1;
                                return lex;
                            }
                            lex.linea = linea;
                            lex.definicion = COMENTARIO_DOCUMENTACION;
                            return lex; //Devolvemos el lexema string documental

                        }//Leemos un string vacío (dos comillas) y otro caracter
                        else {
                            retrocederPosicion();
                            lex.linea = linea;
                            lex.definicion = STRING;
                            return lex; //Devolvemos el contenido del string
                        }

                        break;
                    case 1: //Leída una comilla y un caracter
                        actual = siguienteCaracter();
                        while (actual != '"') {
                            lex.string=concatenar(lex.string, actual);
                            actual = siguienteCaracter();
                        }
                        lex.string=concatenar(lex.string, actual);
                        //retrocederPosicion();
                        if (flagMemoriaExcedida == 1) {
                            lex.definicion = -1;
                            return lex;
                        }
                        lex.linea = linea;
                        lex.definicion = STRING;
                        return lex;
                        break;
                }

            case 20: //Leída una letra

                while (isalnum(actual) || actual == '_') {
                    actual = siguienteCaracter();
                    if (isalnum(actual) || actual == '_') {
                        lex.string=concatenar(lex.string, actual);
                    }
                }

                retrocederPosicion(); //Cuando se lee algo distinto a un alfanum o una _ retrocedemos una posición

                if (flagMemoriaExcedida == 1) {
                    lex.definicion = -1;
                    return lex;
                }
                lex.linea = linea;

                lex.definicion = buscarTabla(lex);
                //Lo insertamos en la tabla de símbolos si no está
                if (lex.definicion == 0) {
                    lex.definicion = IDENTIFICADOR;
                    insertarTabla(lex);
                } else {
                    return lex;
                }
                return lex;

                break;

            case 30: //Leido un dígito
                if (actual == '0') {
                    actual = siguienteCaracter();
                    if (actual == 'x') {
                        lex.string=concatenar(lex.string, actual);
                        subestado = 1; //El número es hexadecimal
                    } else {
                        retrocederPosicion();
                        subestado = 0;
                    }
                } else {
                    subestado = 0; //El número es un numero no hexadecimal
                }

                switch (subestado) {
                    case 0: //Lector de números no hexadecimales
                        actual = siguienteCaracter();

                        while ('0' <= actual && actual <= '9') {
                            lex.string=concatenar(lex.string, actual);
                            actual = siguienteCaracter();
                        }

                        if (actual == '.') {
                            lex.string=concatenar(lex.string, actual);
                            actual = siguienteCaracter();
                        }

                        while ('0' <= actual && actual <= '9') {
                            lex.string=concatenar(lex.string, actual);
                            actual = siguienteCaracter();
                        }

                        if (actual == 'e') {
                            lex.string=concatenar(lex.string, actual);
                            actual = siguienteCaracter();
                            if (actual == '-') {
                                lex.string=concatenar(lex.string, actual);
                                actual = siguienteCaracter();
                            }

                        }

                        while ('0' <= actual && actual <= '9') {
                            lex.string=concatenar(lex.string, actual);
                            actual = siguienteCaracter();
                        }

                        if (actual == '.') {
                            lex.string=concatenar(lex.string, actual);
                            actual = siguienteCaracter();
                        }

                        while ('0' <= actual && actual <= '9') {
                            lex.string=concatenar(lex.string, actual);
                            actual = siguienteCaracter();
                        }

                        retrocederPosicion(); //Cuando se lee algo distinto a un numero retrocedemos una posición

                        if (flagMemoriaExcedida == 1) {
                            lex.definicion = -1;
                            return lex;
                        }

                        lex.linea = linea;
                        lex.definicion = NUMERO;

                        return lex;

                        break;

                    case 1: //Lector de números hexadecimales
                        actual = siguienteCaracter();

                        while (('0' <= actual && actual <= '9') || (actual == 'A') || (actual == 'B') || (actual == 'C') || (actual == 'D') || (actual == 'E') || (actual == 'F')|| (actual == 'a') || (actual == 'b') || (actual == 'c') || (actual == 'd') || (actual == 'e') || (actual == 'f')) {
                            lex.string=concatenar(lex.string, actual);
                            actual = siguienteCaracter();
                        }

                        retrocederPosicion(); //Cuando se lee algo distinto a un numero retrocedemos una posición

                        if (flagMemoriaExcedida == 1) {
                            lex.definicion = -1;
                            return lex;
                        }

                        lex.linea = linea;
                        lex.definicion = NUMERO_HEXADECIMAL;

                        return lex;

                        break;
                }

                break;

                //Leída una comilla simple
            case 40:
                actual = siguienteCaracter();

                if (actual == '\'') {
                    lex.string=concatenar(lex.string, actual);
                    lex.definicion = STRING;
                    lex.linea = linea;
                    return lex;
                }

                while (actual != '\'') {
                    lex.string=concatenar(lex.string, actual);
                    actual = siguienteCaracter();
                }

                lex.string=concatenar(lex.string, actual);

                if (flagMemoriaExcedida == 1) {
                    lex.definicion = -1;
                    return lex;
                }

                lex.linea = linea;
                lex.definicion = STRING;

                return lex;

                break;
        }
    }
}


