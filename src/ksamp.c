#include "../include/ksamp.h"

    /**
    * @brief Punto de inicio del programa
    *
    * Iniciado al ingresar el comando ./ksamp en consola.
    * Admite parametros extra, con los que realiza funciones avanzadas.
    * En base de estos parametros llama a las funciones correspondientes.
    * @see checkInput()
    * @see printData()
    * @see getStats()
    * @see calculateInterval(double values[])
    */
int main (int argc, char* argv[]){
    printf("Content-Type: text/html\n\n");
    struct Datos datos;
    struct Interval interval;
    
    getData(&datos);
    calculateInterval(&interval);
    printPage(datos, interval);
    // printData(datos, interval);
	return 0;
}

void printPage(struct Datos datos, struct Interval data){
    long ut = datos.uptime;
    //Imports
    printf(""
    "<html>"
    "<title>System Information</title>"
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
    "<body>");

    //<!-- Navbar -->
    printf(""
    "<div>"
    "  <div class=\"w3-bar w3-red w3-card-2 w3-left-align w3-large\">"
    "    <a href=\"../index.html\" class=\"w3-bar-item w3-button w3-padding-large w3-white\">Home</a>"
    "    <a href=\"ksamp.cgi\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">System Info</a>"
    "    <a href=\"../stations.html\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">Stations</a>"
    "    <a href=\"modules.cgi\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">Drivers and Modules</a>"
    "    <a href=\"#\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">Documentation</a>"
    "  </div>"
    "</div>");

    // <!-- Header -->
    printf(""
    "<header class=\"w3-container w3-red w3-left-align\" style=\"padding:0px 16px\">"
    "    <h1 class=\"w3-padding-8 w3-xxlarge\">System Info</h1>"
    "</header>");

    //Lista
    printf(""
    "<ul class=\"w3-ul w3-card-4\">"
    "  <li class=\"w3-padding-16\">"
    "    <img src=\"../icons/host.png\" class=\"w3-left w3-circle w3-margin-right\" style=\"width:50px\">"
    "    <span class=\"w3-large\">Hostname</span><br>"
    "    <span>%s</span>"
    "  </li>"
    "  <li class=\"w3-padding-16\">"
    "    <img src=\"../icons/cpu.png\" class=\"w3-left w3-circle w3-margin-right\" style=\"width:50px\">"
    "    <span class=\"w3-large\">CPU Model</span><br>"
    "    <span>%s</span>"
    "  </li>"
    "  <li class=\"w3-padding-16\">"
    "    <img src=\"../icons/cpu2.png\" class=\"w3-left w3-circle w3-margin-right\" style=\"width:50px\">"
    "    <span class=\"w3-large\">CPU Usage</span><br>"
    "    <span>%s %s %s</span>"
    "  </li>"
    "  <li class=\"w3-padding-16\">"
    "    <img src=\"../icons/ram.png\" class=\"w3-left w3-circle w3-margin-right\" style=\"width:50px\">"
    "    <span class=\"w3-large\">Memoria RAM</span><br>"
    "    <span>%i MB/%i MB</span>"
    "  </li>"
    "  <li class=\"w3-padding-16\">"
    "    <img src=\"../icons/clock.png\" class=\"w3-left w3-circle w3-margin-right\" style=\"width:50px\">"
    "    <span class=\"w3-large\">Hora y Fecha</span><br>"
    "    <span>%s - %s</span>"
    "  </li>"
    "  <li class=\"w3-padding-16\">"
    "    <img src=\"../icons/uptime.png\" class=\"w3-left w3-circle w3-margin-right\" style=\"width:50px\">"
    "    <span class=\"w3-large\">Uptime</span><br>"
    "    <span>%ldD %ld:%02ld:%02ld</span>"
    "  </li>"
    "  <li class=\"w3-padding-16\">"
    "    <img src=\"../icons/kernel.jpg\" class=\"w3-left w3-circle w3-margin-right\" style=\"width:50px\">"
    "    <span class=\"w3-large\">Linux Kernel Version</span><br>"
    "    <span>%s</span>"
    "  </li>"
    "</ul>"

    "</body>"
    "</html>",
    datos.hostname,
    datos.cpu, data.load1Min, data.load5Min, data.load15Min,
    data.memDisp/MBYTE,data.memTot/MBYTE,
    datos.hora,datos.fecha,
    ut/day, (ut%day)/hour,(ut%hour)/minute,ut%minute,
    datos.kernel
    );
}

/**
    * @brief Imprime los datos basicos del sistema.
    *
    * Muestra el modelo de cpu, la version de kernel del sistema, el tiempo que
    * la pc estuvo encendida, y la cantidad de sistemas de archivos soportados por el kernel.
    * @param datos Estructura de datos con la informacion necesaria para imprimir
    */
void printData(struct Datos datos, struct Interval data){

    long ut = datos.uptime;

    printf("Hostname: %s",datos.hostname);
    printf("Hora y Fecha actual: %s - %s",datos.hora,datos.fecha);
    printf("CPU:%s",datos.cpu);
    printf("Version del Kernel de Linux: %s\n",datos.kernel);
    printf("Uptime: %ldD %ld:%02ld:%02ld \n",ut/day, (ut%day)/hour,(ut%hour)/minute,ut%minute);
    printf("Memoria disponible/total: %i MB/%i MB\n",data.memDisp/MBYTE,data.memTot/MBYTE);
    printf("Promedio de carga del sistema: %s %s %s\n",  data.load1Min, data.load5Min, data.load15Min);
}

void getData(struct Datos *datos){

    char buffer[256];
    char fecha [20];
    char hora [20];

    fp = fopen("/proc/sys/kernel/hostname","r");
    if (fp == NULL){
        return;
    } 
    if(fgets(buffer, 256, fp) == NULL){
        printf("Error fgets\n");
    }
    fclose(fp);
    strcpy(datos->hostname,buffer);

    fp = fopen("/proc/driver/rtc","r");
    if (fp == NULL){
        return;
    } 
    
    if(fgets(buffer, 100, fp) == NULL){
        printf("Error fgets\n");
    }
    strcpy(hora,buffer+11);
    if(fgets(buffer, 100, fp) == NULL){
        printf("Error fgets\n");
    }
    strcpy(fecha,buffer+11);

    hora[strcspn(hora, "\n")] = 0;

    strcpy(datos->hora,hora);
    strcpy(datos->fecha,fecha);

    fclose(fp);

    strcpy(path,"/proc/cpuinfo");
    strcpy(before,"model name");
    strcpy(after,"");
    parseFile(fp,path,before,after,&buff);
    strcpy(datos->cpu,buff+2);

    strcpy(path,"/proc/version");
    strcpy(before,"version ");
    strcpy(after,"(");
    parseFile(fp,path,before,after,&buff);
    strcpy(datos->kernel,buff);

    strcpy(path,"/proc/uptime");
    strcpy(before,"");
    strcpy(after," ");
    parseFile(fp,path,before,after,&buff);
    datos->uptime = atol(buff);
}

void calculateInterval(struct Interval *interval){

    strcpy(path,"/proc/meminfo");
    strcpy(before,"MemTotal:");
    strcpy(after,"");
    parseFile(fp,path,before,after,&buff);
    
    interval->memTot = atoi(buff);

    strcpy(before,"MemAvailable:");
    parseFile(fp,path,before,after,&buff);
    
    interval->memDisp = atoi(buff);

    strcpy(path,"/proc/loadavg");
    strcpy(before,"");
    strcpy(after,"");
    parseFile(fp,path,before,after,&buff);
    if(sscanf( buff, "%s %s %s", interval->load1Min, interval->load5Min, interval->load15Min) != 3){
        printf("Error sscanf\n");
        exit(EXIT_FAILURE);
    }
}

    /**
    * @brief Parsea un archivo en busca de una cadena de caracteres.
    *
    * Recibido un path, una cadena a buscar y delimitadores, busca en el archivo
    * y guarda la cadena en buff. Si no puede abrir el archivo termina el programa.
    * Busca iterativamente hasta encontrar la primer ocurrencia del string buscado
    * y la recorta en base a los delimitadores.
    * @param fp File Descriptor, usado para abrir un archivo como lectura
    * @param path[] Cadena de caracteres con la ruta en la que se buscara el archivo
    * @param before[] String con el delimitador que se encuentra antes de la cadena deseada
    * @param after[] String con el delimitador que se encuentra despues de la cadena deseada
    * @param buff Puntero a char donde se almacena el resultado
    */
int parseFile(FILE* fp, char path[], char before[], char after[], char** buff){

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
    fp = fopen(path,"r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        if(strstr(line,before)!=NULL){
            *buff = strstr(line,before) + strlen(before);
            *buff = strtok(*buff,after);
            break;
        }
    }
    fclose(fp);
    return 0;
}