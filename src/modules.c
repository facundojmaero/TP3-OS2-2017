#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include <getopt.h>
#define MBYTE 1024
#define PALABRA 50

struct Modulo
{
    char name[PALABRA];
    int size;
    int instances_loaded;
    char status[PALABRA];
};

void getData(struct Modulo modulos[], int lines);
int parseFile(char before[], char after[], char* buff);
int count_modules();
void printPage(struct Modulo modulos[], int cant);

int main (int argc, char* argv[]){
    printf("Content-Type: text/html\n\n");
    int lines = count_modules();
    if(lines == -1){
        printf("Error count_modules\n");
        exit(EXIT_FAILURE);
    }

    struct Modulo modulos[lines];
    getData(modulos, lines);

    // for (int i = 0; i < lines; ++i)
    // {
    //     printf("%s %d %d %s\n", modulos[i].name, modulos[i].size, modulos[i].instances_loaded, modulos[i].status);
    // }

    printPage(modulos, lines);

	return 0;
}

void printPage(struct Modulo modulos[], int cant){
    printf(
        "<html>"
        "<title>Drivers and Modules</title>"
        "<meta charset=\"UTF-8\">"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
        "<link rel=\"stylesheet\" href=\"https://www.w3schools.com/w3css/4/w3.css\">"
        "<link rel=\"stylesheet\" href=\"https://fonts.googleapis.com/css?family=Lato\">"
        "<link rel=\"stylesheet\" href=\"https://fonts.googleapis.com/css?family=Montserrat\">"
        "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">"
        "<style>"
        "body,h1,h2,h3,h4,h5,h6 {font-family: \"Lato\", sans-serif}"
        ".w3-bar,h1,button {font-family: \"Montserrat\", sans-serif}"
        ".fa-anchor,.fa-coffee {font-size:200px}"
        "</style>"
        "<body>"

        // <!-- Navbar -->
        "<div>"
        "<div class=\"w3-bar w3-red w3-card-2 w3-left-align w3-large\">"
        "<a href=\"../index.html\" class=\"w3-bar-item w3-button w3-padding-large w3-white\">Home</a>"
        "<a href=\"ksamp.cgi\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">System Info</a>"
        "<a href=\"../stations.html\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">Stations</a>"
        "<a href=\"modules.cgi\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">Drivers and Modules</a>"
        "<a href=\"#\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">Documentation</a>"
        "</div>"
        "</div>"
        // "<!-- Header -->"
        "<header class=\"w3-container w3-red w3-left-align\" style=\"padding:0px 16px\">"
        "<h1 class=\"w3-padding-8 w3-xxlarge w3-third\">Módulos del Kernel</h1>"
        "<h1 class=\"w3-padding-8 w3-xlarge w3-twothird\"></h1>"
        "</header>"
        );

    printf(
        "<div class=\"w3-half w3-container w3-padding\">"
        "<table class=\"w3-table w3-striped w3-bordered\">"
        "<tr>"
        "<th>Nro</th>"
        "<th>Nombre</th>"
        "<th>Tamaño</th>"
        "<th>Nro Instancias</th>"
        "<th>Estado</th>"
        "</tr>"
        );

    for (int i = 0; i < cant; ++i)
    {
        printf(
            "<tr>"
            "<td>%d</td>"
            "<td>%s</td>"
            "<td>%d Kb</td>"
            "<td>%d</td>"
            "<td>%s</td>"
            "</tr>",
            i+1,modulos[i].name, modulos[i].size/1024,
            modulos[i].instances_loaded, modulos[i].status
            );
    }
        
    printf(
        "</table>"
        "</div>"
        "</body>"
        "</html>"
        );
}

int count_modules(){
    char buffer[256];
    int lines = 0;
    FILE* fp = fopen("/proc/modules","r");
    if (fp == NULL){
        return -1;
    }
    while(fgets(buffer, 256, fp) != NULL){
        lines++;
    }
    fclose(fp);
    return lines;
}

void getData(struct Modulo modulos[], int lines){

    char buffer[256];

    FILE* fp = fopen("/proc/modules","r");
    if (fp == NULL){
        return;
    }
    for (int i = 0; i < lines; ++i)
    {
        fgets(buffer, 256, fp);
        sscanf( buffer, "%s %d %d %*s %s", modulos[i].name, &modulos[i].size, &modulos[i].instances_loaded, modulos[i].status );
    }
    fclose(fp);
}