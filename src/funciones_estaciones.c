/** @file funciones_server.c
 *  @brief Archivo con funciones extras del servidor.
 *
 *  Contiene funciones extras que necesita el servidor, pero que no están
 *  relacionadas directamente con la comunicación con el cliente.
 *
 *  @author Facundo Maero
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <unistd.h>
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

/**
* @brief Saltea lineas en un archivo.
*
* Dado un descriptor de archivo y un numero de lineas, las saltea leyéndolas
* y descartando el resultado.
*
* Crea un socket TCP con los argumentos que recibe. Utiliza el puerto 6020.
* @args *stream Descriptor del archivo donde se quieren saltar lineas.
* @args lines Cantidad de lineas a saltar.
*/
int 
skip_lines(FILE* stream, int lines){
    
    for (int i = 0; i < lines; ++i)
    {
        fscanf(stream, "%*[^\n]\n");
    }
    return 0;
}