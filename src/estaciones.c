#include "../include/funciones_estaciones.h"

int 
main( int argc, char *argv[] ) {
    printf("Content-Type: text/html\n\n");
    struct Estacion stationArray[10];
    FILE* stream;

    stream = fopen("../datos_meteorologicos.CSV", "r");
    // stream = fopen("datos_meteorologicos.CSV", "r");
    if (stream == NULL) {
      perror("Error opening CSV table");
      exit(EXIT_FAILURE);
    }

    leer_archivo(stationArray, stream);

    // char data[50];
    char *data = getenv("QUERY_STRING");
    char query[50];
    char query_arg[50];
    // strcpy(data, "q=descargar&a=30099");
    // printf("DATA: '%s'\n", data);

    if(parseString(data, "a=", "", query_arg) == -1){
        printf("Error parseString\n");
        exit(EXIT_FAILURE);
    }

    if(parseString(data, "q=", "&", query) == -1){
        printf("Error parseString\n");
        exit(EXIT_FAILURE);
    }

    // printf("'%s' '%s'\n",query, query_arg );

    if(strcmp(query,"promedio") == 0){
        float promedios[NRO_ESTACIONES];
        int indiceSensor;
        if(promediar(stationArray, query_arg, promedios, &indiceSensor) == -1){
            printf("Error promediar\n");
        }

        char titulo[50] = "Promedio - ";
        char unidades[10];
        char sensor[50];
        traducir_nombre_sensor(query_arg, sensor, unidades);
        strcat(titulo, sensor);
        print_page_header(titulo);
        print_resto_pagina(stationArray, query, query_arg, promedios, NRO_ESTACIONES, NULL, unidades);
    }

    else if(strcmp(query,"diario_precip") == 0){
        float precipitaciones[31];
        int index_dias[31];
        diarioPrecipitacion(stationArray, atoi(query_arg), precipitaciones, index_dias);
        int i;

        int num = check_estacion_existente(stationArray, atoi(query_arg));

        for (i = 0; i < 31; ++i)
        {
            if(precipitaciones[i] == -1){
                break;
            }
            // printf("%s %.1f, %d\n",stationArray[num].dato[index_dias[i]].dia, precipitaciones[i], index_dias[i]);
        }
        char titulo[100];
        snprintf(titulo, 100, "Precipitación Diaria - Estación %i %s", stationArray[num].numero, stationArray[num].nombre);
        print_page_header(titulo);
        print_resto_pagina(stationArray, query, query_arg, precipitaciones, i, index_dias, NULL);
    }

    else if(strcmp(query,"mensual_precip") == 0){
        float precipitaciones[1];
        mensual_precip(stationArray, atoi(query_arg), precipitaciones);
        // printf("%i %.1f\n",atoi(query_arg), precipitaciones[0]);
        int num = check_estacion_existente(stationArray, atoi(query_arg));
        char titulo[100];
        snprintf(titulo, 100, "Precipitación Mensual - Estación %i %s", stationArray[num].numero, stationArray[num].nombre);
        print_page_header(titulo);
        print_resto_pagina(stationArray, query, query_arg, precipitaciones, 1, NULL, NULL);
    }
    else if(strcmp(query,"descargar") == 0){
        char filename[50];
        descargar_estacion(atoi(query_arg), stationArray, stream, filename);
        int num = check_estacion_existente(stationArray, atoi(query_arg));
        char titulo[100];
        snprintf(titulo, 100, "Archivo generado - Estación %i %s", stationArray[num].numero, stationArray[num].nombre);
        print_page_header(titulo);
        print_resto_pagina(stationArray, query, query_arg, NULL, 1, NULL, filename);
    }

    return 0; 
}

int
traducir_nombre_sensor(char sensor[], char traduccion[], char unidades[]){
    if(strcmp(sensor, "temp") == 0){
        strcpy(traduccion, "Temperatura");
        strcpy(unidades, "°C");
        return 0;
    }
    else if(strcmp(sensor, "humedad") == 0){
        strcpy(traduccion, "Humedad");
        strcpy(unidades, "%HR");
        return 0;
    }
    else if(strcmp(sensor, "ptoRocio") == 0){
        strcpy(traduccion, "Punto de Rocío");
        strcpy(unidades, "°C");
        return 0;
    }
    else if(strcmp(sensor, "precip") == 0){
        strcpy(traduccion, "Precipitaciones");
        strcpy(unidades, "mm");
        return 0;
    }
    else if(strcmp(sensor, "velocViento") == 0){
        strcpy(traduccion, "Velocidad del Viento");
        strcpy(unidades, "Km/h");
        return 0;
    }
    else if(strcmp(sensor, "rafagaMax") == 0){
        strcpy(traduccion, "Ráfaga Máxima");
        strcpy(unidades, "Km/h");
        return 0;
    }
    else if(strcmp(sensor, "presion") == 0){
        strcpy(traduccion, "Presión");
        strcpy(unidades, "hPa");
        return 0;
    }
    else if(strcmp(sensor, "radiacion") == 0){
        strcpy(traduccion, "Radiación");
        strcpy(unidades, "W/m2");
        return 0;
    }
    else if(strcmp(sensor, "tempSuelo1") == 0){
        strcpy(traduccion, "Temperatura de Suelo 1");
        strcpy(unidades, "°C");
        return 0;
    }
    else if(strcmp(sensor, "tempSuelo2") == 0){
        strcpy(traduccion, "Temperatura de Suelo 2");
        strcpy(unidades, "°C");
        return 0;
    }
    else if(strcmp(sensor, "tempSuelo3") == 0){
        strcpy(traduccion, "Temperatura de Suelo 3");
        strcpy(unidades, "°C");
        return 0;
    }
    else if(strcmp(sensor, "humedadSuelo1") == 0){
        strcpy(traduccion, "Humedad de Suelo 1");
        strcpy(unidades, "%%grav");
        return 0;
    }
    else if(strcmp(sensor, "humedadSuelo2") == 0){
        strcpy(traduccion, "Humedad de Suelo 2");
        strcpy(unidades, "°%%grav");
        return 0;
    }
    else if(strcmp(sensor, "humedadSuelo3") == 0){
        strcpy(traduccion, "Humedad de Suelo 3");
        strcpy(unidades, "%%grav");
        return 0;
    }
    else if(strcmp(sensor, "humedadHoja") == 0){
        strcpy(traduccion, "Humedad de Hoja");
        strcpy(unidades, "%%");
        return 0;
    }
    //Si no es ningun sensor valido
    strcpy(traduccion, "Sensor inexistente");
    return -1;
}

void 
print_page_header(char titulo[]){
  printf(
      "<html>"
      "<title>Stations</title>"
      "<meta charset=\"UTF-8\">"
      "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
      "<link rel=\"stylesheet\" href=\"https://www.w3schools.com/w3css/4/w3.css\">"
      "<link rel=\"stylesheet\" href=\"https://fonts.googleapis.com/css?family=Lato\">"
      "<link rel=\"stylesheet\" href=\"https://fonts.googleapis.com/css?family=Montserrat\">"
      "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">"
      "<style>"
      "  body,h1,h2,h3,h4,h5,h6 {font-family: \"Lato\", sans-serif}"
      "  .w3-bar,h1,button {font-family: \"Montserrat\", sans-serif}"
      "  .fa-anchor,.fa-coffee {font-size:200px}"
      "</style>"
      "<body>"
      );

    // <!-- Navbar -->
  printf(
      "<div>"
      "  <div class=\"w3-bar w3-red w3-card-2 w3-left-align w3-large\">"
      "    <a href=\"../index.html\" class=\"w3-bar-item w3-button w3-padding-large w3-white\">Home</a>"
      "    <a href=\"ksamp.cgi\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">System Info</a>"
      "    <a href=\"../stations.html\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">Stations</a>"
      "    <a href=\"#\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">Drivers and Modules</a>"
      "    <a href=\"#\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">Documentation</a>"
      "  </div>"
      "</div>"
  // "<!-- Header -->"
      "<header class=\"w3-container w3-red w3-left-align\" style=\"padding:0px 16px\">"
      "  <h1 class=\"w3-padding-8 w3-xxlarge w3-third\">Estaciones</h1>"
      "  <h1 class=\"w3-padding-8 w3-margin-top w3-xlarge w3-twothird\">%s</h1>"
      "</header>"
      ,titulo);

  printf(
      "<div class=\"w3-third w3-container w3-padding\">"
      "  <div class=\"w3-bar w3-light-grey\">"
      "  <a class=\"w3-bar-item \">Promedio</a>"
      "    <div class=\"w3-dropdown-hover w3-right\">"
      "      <button class=\"w3-button\" onclick=\"displaySensores()\">"
      "        Sensor <i class=\"fa fa-caret-down\"></i>"
      "      </button>"
      "      <div id=\"Sensores\" class=\"w3-dropdown-content w3-bar-block w3-card-2\">"
      "        <a href=\"estaciones.cgi?q=promedio&a=temp\" class=\"w3-bar-item w3-button\">Temperatura</a>"
      "        <a href=\"estaciones.cgi?q=promedio&a=humedad\" class=\"w3-bar-item w3-button\">Humedad</a>"
      "        <a href=\"estaciones.cgi?q=promedio&a=ptoRocio\" class=\"w3-bar-item w3-button\">Punto de Rocío</a>"
      "        <a href=\"estaciones.cgi?q=promedio&a=precip\" class=\"w3-bar-item w3-button\">Precipitación</a>"
      "        <a href=\"estaciones.cgi?q=promedio&a=velocViento\" class=\"w3-bar-item w3-button\">Velocidad Viento</a>"
      "        <a href=\"estaciones.cgi?q=promedio&a=rafagaMax\" class=\"w3-bar-item w3-button\">Ráfaga Máxima</a>"
      "        <a href=\"estaciones.cgi?q=promedio&a=presion\" class=\"w3-bar-item w3-button\">Presión</a>"
      "        <a href=\"estaciones.cgi?q=promedio&a=radiacion\" class=\"w3-bar-item w3-button\">Radiación Solar</a>"
      "        <a href=\"estaciones.cgi?q=promedio&a=tempSuelo1\" class=\"w3-bar-item w3-button\">Temperatura Suelo 1</a>"
      "        <a href=\"estaciones.cgi?q=promedio&a=tempSuelo2\" class=\"w3-bar-item w3-button\">Temperatura Suelo 2</a>"
      "        <a href=\"estaciones.cgi?q=promedio&a=tempSuelo3\" class=\"w3-bar-item w3-button\">Temperatura Suelo 3</a>"
      "        <a href=\"estaciones.cgi?q=promedio&a=humedadSuelo1\" class=\"w3-bar-item w3-button\">Humedad Suelo 1</a>"
      "        <a href=\"estaciones.cgi?q=promedio&a=humedadSuelo2\" class=\"w3-bar-item w3-button\">Humedad Suelo 2</a>"
      "        <a href=\"estaciones.cgi?q=promedio&a=humedadSuelo3\" class=\"w3-bar-item w3-button\">Humedad Suelo 3</a>"
      "        <a href=\"estaciones.cgi?q=promedio&a=humedadHoja\" class=\"w3-bar-item w3-button\">Humedad Hoja</a>"
      "      </div>"
      "    </div>"
      "  </div>"
      );

  printf(""
      "  <div class=\"w3-bar w3-light-grey\">"
      "    <a class=\"w3-bar-item \">Precipitaciones Diarias</a>"
      "    <div class=\"w3-dropdown-hover w3-right\">"
      "      <button class=\"w3-button\" onclick=\"displayPrecipDiarias()\">"
      "        Estación <i class=\"fa fa-caret-down\"></i>"
      "      </button>"
      "      <div id=\"precipDiarias\" class=\"w3-dropdown-content w3-bar-block w3-card-2\">"
      "        <a href=\"estaciones.cgi?q=diario_precip&a=30135\" class=\"w3-bar-item w3-button\">30135 - Yacanto Norte</a>"
      "        <a href=\"estaciones.cgi?q=diario_precip&a=30057\" class=\"w3-bar-item w3-button\">30057 - Magyacba 60 cuadras</a>"
      "        <a href=\"estaciones.cgi?q=diario_precip&a=30061\" class=\"w3-bar-item w3-button\">30061 - Magyacba La Cumbrecita </a>"
      "        <a href=\"estaciones.cgi?q=diario_precip&a=30099\" class=\"w3-bar-item w3-button\">30099 - Cerro Obero </a>"
      "        <a href=\"estaciones.cgi?q=diario_precip&a=30069\" class=\"w3-bar-item w3-button\">30069 - Magyacba Oliva </a>"
      "      </div>"
      "    </div>"
      "  </div>"

      "<div class=\"w3-bar w3-light-grey\">"
      "  <a class=\"w3-bar-item \">Precipitaciones Mensuales</a>"
      "  <div class=\"w3-dropdown-hover w3-right\">"
      "    <button class=\"w3-button\" onclick=\"displayPrecipMensuales()\">"
      "      Estación <i class=\"fa fa-caret-down\"></i>"
      "    </button>"
      "    <div id=\"precipMensuales\" class=\"w3-dropdown-content w3-bar-block w3-card-2\">"
      "      <a href=\"estaciones.cgi?q=mensual_precip&a=30135\" class=\"w3-bar-item w3-button\">30135 - Yacanto Norte</a>"
      "      <a href=\"estaciones.cgi?q=mensual_precip&a=30057\" class=\"w3-bar-item w3-button\">30057 - Magyacba 60 cuadras</a>"
      "      <a href=\"estaciones.cgi?q=mensual_precip&a=30061\" class=\"w3-bar-item w3-button\">30061 - Magyacba La Cumbrecita </a>"
      "      <a href=\"estaciones.cgi?q=mensual_precip&a=30099\" class=\"w3-bar-item w3-button\">30099 - Cerro Obero </a>"
      "      <a href=\"estaciones.cgi?q=mensual_precip&a=30069\" class=\"w3-bar-item w3-button\">30069 - Magyacba Oliva </a>"
      "    </div>"
      "  </div>"
      "</div>"

      "<div class=\"w3-bar w3-light-grey\">"
      "  <a class=\"w3-bar-item \">Descargar</a>"
      "  <div class=\"w3-dropdown-hover w3-right\">"
      "    <button class=\"w3-button\" onclick=\"displayDescarga()\">"
      "      Estación <i class=\"fa fa-caret-down\"></i>"
      "    </button>"
      "    <div id=\"descarga\" class=\"w3-dropdown-content w3-bar-block w3-card-2\">"
      "      <a href=\"#\" class=\"w3-bar-item w3-button\">30135 - Yacanto Norte</a>"
      "      <a href=\"#\" class=\"w3-bar-item w3-button\">30057 - Magyacba 60 cuadras</a>"
      "      <a href=\"#\" class=\"w3-bar-item w3-button\">30061 - Magyacba La Cumbrecita </a>"
      "      <a href=\"#\" class=\"w3-bar-item w3-button\">30099 - Cerro Obero </a>"
      "      <a href=\"#\" class=\"w3-bar-item w3-button\">30069 - Magyacba Oliva </a>"
      "    </div>"
      "  </div>"
      "</div>"
      "</div>"
      );
}

void
print_resto_pagina(struct Estacion stationArray[], char query[], 
    char query_arg[], float datos[], int len, int index_dias[], char extra[]){

    printf(""
    "<div class='w3-twothird w3-container'>");

    if(strcmp(query, "promedio") == 0){

        printf(""
        "<table class=\"w3-table w3-striped w3-bordered\">"
        "  <tr>"
        "    <th>Estación</th>"
        "    <th>Promedio</th>"
        "  </tr>");

        for (int i = 0; i < NRO_ESTACIONES; ++i)
        {
            if(datos[i] != -1000){
                printf(""
                  "<tr>"
                  "  <td>%d - %s</td>"
                  "  <td>%.2f %s</td>"
                  "</tr>            "
                  ,stationArray[i].numero, stationArray[i].nombre,
                  datos[i], extra
                  );
            }
            else{
                printf(""
                  "<tr>"
                  "  <td>%d - %s</td>"
                  "  <td>Sensor no disponible</td>"
                  "</tr>"
                  ,stationArray[i].numero, stationArray[i].nombre
                  );
            }
        }
        
        printf(
        "</table>"
        "</body>"
        "</html>"
        );
    }

    else if(strcmp(query, "diario_precip") == 0){

        int est = check_estacion_existente(stationArray, atoi(query_arg));

        printf(""
        "<table class=\"w3-table w3-striped w3-bordered\">"
        "  <tr>"
        "    <th>Día</th>"
        "    <th>Precipitaciones</th>"
        "  </tr>");

        for (int i = 0; i < len; ++i)
        {
            printf(""
          "<tr>"
          "  <td>%s</td>"
          "  <td>%.1f mm</td>"
          "</tr>            "
          ,stationArray[est].dato[index_dias[i]].dia,
          datos[i]
          );
        }
        
        printf(
        "</table>"
        );
    }

    else if(strcmp(query, "mensual_precip") == 0){

        printf(""
        "<table class=\"w3-table w3-striped w3-bordered\">"
        "  <tr>"
        "    <th>Mes</th>"
        "    <th>Precipitaciones</th>"
        "  </tr>");

        printf(""
        "<tr>"
        "  <td>Febrero</td>"
        "  <td>%.1f</td>"
        "</tr>            "
        ,datos[0]
        );
        
        printf(
        "</table>"
        );
    }

    else if(strcmp(query, "descargar") == 0){
        char filename[50];
        parseString(extra, "../files/", "", filename);
        
        printf(""
        "<div class='w3-twothird w3-container'>"
        "<p><a class=\"w3-button w3-border w3-xlarge\" href=\"download.pl?file=%s\">Descargar</a></p>"
        "</div>",
        filename
        );
    }

    printf(
    "</div>"
    "</body>"
    "</html>"
    );
}

int 
parseString(char cadena[], char before[], char after[], char token[]){
    if(strstr(cadena,before)!=NULL){
        strcpy(token, strstr(cadena,before) + strlen(before));
        strcpy(token, strtok(token,after));
        return 0;
    }
    return -1;
}

void
leer_archivo(struct Estacion stationArray[10], FILE* stream){
  size_t characters = 0;
  size_t len = 0;
  char* line2 = NULL;
  char* nombreColumnas = NULL;

  const char s[2] = ",";
  char *token;
    // struct Estacion stationArray[10];
    int i=0;//fila del archivo
    int j=0;//numero de estacion
    int idEstacion;

    /*!< Salteo la primer linea y leo el nombre de las columnas */
    skip_lines(stream,INICIO_ESTACIONES-1);
    characters = getline(&nombreColumnas,&len,stream);

    /*!< Creo 16 sensores temporales (uno por cada columna del archivo) */
    struct sensor_disponible sensores_temp[16];

    /*!< Copio el nombre de la columna (el nombre del sensor) en la estruc
    tura de datos recien declarada */
    int cuenta=0;
    token = strtok(nombreColumnas, s);
    while( token != NULL ) {
        if(cuenta>3){
            strcpy(sensores_temp[cuenta-4].nombreSensor,token);
        }
        token = strtok(NULL, s);
        cuenta++;
    }

    /*!<  Bucle principal de adquisicion de datos.
    Leo todas las lineas del archivo una por una y las proceso.*/
    while((characters = getline(&line2,&len,stream)) != -1 ){     

        /*!< Busco el ID de la estacion */
        sscanf(line2,"%d",&idEstacion);
        if (i != 0){
            /*!< Si no es la primera linea de la estacion, comparo el ID
            con el ID del elemento anterior para ver si pase a una nueva
            estacion o si estoy en la misma.
            Esto podria hacerse contando la cantidad de muestras por dia, 
            pero hay un caso en el que una estacion estuvo apagada un tiempo. */
            if(idEstacion != stationArray[j].numero){
                /*!< Si el ID es diferente, paso a guardar datos de la estacion
                siguiente. Guardo el tamaño de la estacion (i). */
                stationArray[j].cantElem = i;
                j++;
                i=0;
                if(j==5){break;}
            }
        }

        /*!< Extraigo de la linea leida los datos, guardandolos en una variable
        de tipo dato_estacion, diferenciando valores numericos de alfanumericos */
        sscanf(line2, "%*d,%*[^','],%*d,%[^','],%f,%f,%f"
         ",%f,%f,%[^','],%f,%f,%f,%f,%f"
         ",%f,%f,%f,%f,%f",
         stationArray[j].dato[i].fecha,
         &stationArray[j].dato[i].temp,
         &stationArray[j].dato[i].humedad, 
         &stationArray[j].dato[i].ptoRocio,
         &stationArray[j].dato[i].precip,
         &stationArray[j].dato[i].velocViento,
         stationArray[j].dato[i].direcViento,
         &stationArray[j].dato[i].rafagaMax,
         &stationArray[j].dato[i].presion,
         &stationArray[j].dato[i].radiacion,
         &stationArray[j].dato[i].tempSuelo1,
         &stationArray[j].dato[i].tempSuelo2,
         &stationArray[j].dato[i].tempSuelo3,
         &stationArray[j].dato[i].humedadSuelo1,
         &stationArray[j].dato[i].humedadSuelo2,
         &stationArray[j].dato[i].humedadSuelo3,
         &stationArray[j].dato[i].humedadHoja);

        /*!< De la fecha completa extraigo solo el dia para uso futuro. */
        sscanf(stationArray[j].dato[i].fecha,"%s",stationArray[j].dato[i].dia);
        
        if(i==0){
            check_sensores(stationArray, j, line2,sensores_temp);
            /*!< Si estoy llenando la primer fila de la estacion, guardo
            su ID, nombre y ID de localidad (una vez por cada estacion). */
            sscanf(line2, "%d,%[^','],%d",
                &stationArray[j].numero,
                stationArray[j].nombre,
                &stationArray[j].idLocalidad);
        }
        i++;

    /*!< Guardo la cantidad de elementos de la ultima estacion*/
        stationArray[j].cantElem = i;
    }
    /*!< Fin de bucle de adquisicion de datos */
}


/**
* @brief Calcula el offset de un elemento de una estructura dato_estacion.
*
* Permite obtener la distancia en bytes desde el inicio de la estructura
* dato_estacion a un elemento dado de la estructura, para poder iterar sobre
* el y calcular su promedio. 
* Para ello utiliza la macro offsetof(), que en Linux arroja resultados muy
* buenos.
* Se compara la cadena ingresada con una lista de comandos validos, y en funcion
* del resultado se calcula la distancia y se la envía.
* Si el comando es incorrecto se retorna -1.
*
* @param variable[] La cadena de caracteres con la variable a buscar.
* @param *indiceSensor Puntero a entero donde se guarda el indice del sensor solicitado.
* @return El offset solicitado desde el inicio de la estructura. 
*/
size_t
get_variable_offset(char variable[], int* indiceSensor){

    if(strcmp(variable, "temp") == 0){
        *indiceSensor = 0;
        return (offsetof(struct dato_estacion, temp));
    }
    else if(strcmp(variable, "humedad") == 0){
        *indiceSensor = 1;
        return (offsetof(struct dato_estacion, humedad));
    }
    else if(strcmp(variable, "ptoRocio") == 0){
        *indiceSensor = 2;
        return (offsetof(struct dato_estacion, ptoRocio));
    }
    else if(strcmp(variable, "precip") == 0){
        *indiceSensor = 3;
        return (offsetof(struct dato_estacion, precip));
    }
    else if(strcmp(variable, "velocViento") == 0){
        *indiceSensor = 4;
        return (offsetof(struct dato_estacion, velocViento));
    }
    else if(strcmp(variable, "rafagaMax") == 0){
        *indiceSensor = 6;
        return (offsetof(struct dato_estacion, rafagaMax));
    }
    else if(strcmp(variable, "presion") == 0){
        *indiceSensor = 7;
        return (offsetof(struct dato_estacion, presion));
    }
    else if(strcmp(variable, "radiacion") == 0){
        *indiceSensor = 8;
        return (offsetof(struct dato_estacion, radiacion));
    }
    else if(strcmp(variable, "tempSuelo1") == 0){
        *indiceSensor = 9;
        return (offsetof(struct dato_estacion, tempSuelo1));
    }
    else if(strcmp(variable, "tempSuelo2") == 0){
        *indiceSensor = 10;
        return (offsetof(struct dato_estacion, tempSuelo2));
    }
    else if(strcmp(variable, "tempSuelo3") == 0){
        *indiceSensor = 11;
        return (offsetof(struct dato_estacion, tempSuelo3));
    }
    else if(strcmp(variable, "humedadSuelo1") == 0){
        *indiceSensor = 12;
        return (offsetof(struct dato_estacion, humedadSuelo1));
    }
    else if(strcmp(variable, "humedadSuelo2") == 0){
        *indiceSensor = 13;
        return (offsetof(struct dato_estacion, humedadSuelo2));
    }
    else if(strcmp(variable, "humedadSuelo3") == 0){
        *indiceSensor = 14;
        return (offsetof(struct dato_estacion, humedadSuelo3));
    }
    else if(strcmp(variable, "humedadHoja") == 0){
        *indiceSensor = 15;
        return (offsetof(struct dato_estacion, humedadHoja));
    }
    else{
      *indiceSensor = -1;
        return -1;
    }
}

/**
* @brief Calcula el promedio de una variable dada en todas las estaciones disponibles.
*
* Calcula el promedio de la variable solicitada. Primero calcula con aritmetica
* de punteros la distancia entre el inicio de la estructura de datos y la variable
* en cuestión.
* Si la cadena no corresponde a un sensor devuelve error.
* Envía los resultados al cliente. Si el sensor no está disponible, lo notifica.
*
* @param stationArray[] Arreglo con todos los datos de todas las estaciones.
* @param variable[] Cadena de caracteres con la variable a promediar.
*/
int
promediar(struct Estacion stationArray[], char variable[], float promedios[], int* indiceSensor){
    size_t longitudElemento = offsetof(struct Estacion, dato[1]);
    longitudElemento = longitudElemento / 4;
    //lo convierto a unidades de 4 bytes (float)

    size_t offset;
    offset = get_variable_offset(variable, indiceSensor);
    if(offset > longitudElemento*4){
        printf("variable inexistente\n");
        return -1;
    }

    // printf("Promedios variable %s:\n", variable);

    for(int j=0; j < NRO_ESTACIONES; j++){
        if(stationArray[j].sensores[*indiceSensor].esta == 0){
            // printf("%d - %s: No se encuentran datos.\n",
            //     stationArray[j].numero, stationArray[j].nombre);
            promedios[j] = -1000;
            continue;
        }
        float* ptr = (float*)((char*)&stationArray[j] + offset);
        //apunto al primer elemento de la primer estacion, la medicion
        //del primer dia de la variable que quiero promediar

        float suma = 0;

        for (int i = 0; i < stationArray[j].cantElem; ++i)
        {
            suma += *(float*)(ptr+(i*longitudElemento));
        }
        
        suma = suma/stationArray[j].cantElem;
        // printf("%d - %s: %.1f\n", stationArray[j].numero, 
        //     stationArray[j].nombre,suma);
        promedios[j] = suma;
    }
    return 0; 
}

/**
* @brief Verifica si la estacion solicitada existe o no.
*
* Dado un ID de estacion, recorre el arreglo de estaciones y comprueba si se
* corresponde con una estacion existente. Si es así, devuelve el índice de la
* misma en el arreglo.
*
* @param estaciones[] Arreglo con todos los datos de todas las estaciones.
* @param *nro Puntero donde guardar el indice de la estacion en el arreglo.
* @return 1 si la estacion existe, 0 en caso contrario.
*/
int
check_estacion_existente(struct Estacion estaciones[], int nro){
    for (int i = 0; i < NRO_ESTACIONES; ++i)
    {
        if(estaciones[i].numero == nro){
            return i;
        }   
    } 
    return -1;
}

/**
* @brief Calcula la precipitacion diaria de la estacion dada
*
* Dado un ID de estacion, calcula las precipitaciones por día.
* Valida el ID de estacion, y si el valor es válido, recorre el arreglo de estaciones
* sumando el valor de precipitaciones y enviando el promedio de cada dia al cliente.
*
* @param estaciones[] Arreglo con todos los datos de todas las estaciones.
* @param nro ID de la estacion solicitada.
*/
void 
diarioPrecipitacion(struct Estacion estaciones[], int index, float precipitaciones[], int index_dias[]){
  int nro = check_estacion_existente(estaciones, index);
  if(nro == -1){
        printf("Estacion inexistente\n");
    return;
  }

    float precipAcumulada=0;
    int i, j;
    // printf("Precipitacion acumulada por dia.\n%d - Estacion %s:\n",
    //  estaciones[nro].numero, estaciones[nro].nombre);
    for (i = 0, j=0; i < estaciones[nro].cantElem; ++i)
    {
        if(i!=0 && (strcmp(estaciones[nro].dato[i].dia, estaciones[nro].dato[i-1].dia))){
            //termine de recorrer un dia. Sumo todo, muestro y reseteo contador
            // printf(" %s: %.1f mm, %i\n", estaciones[nro].dato[i-1].dia, precipAcumulada, i-1);
            precipitaciones[j] = precipAcumulada;
            index_dias[j] = i-1;
            j++;
            precipAcumulada = 0;
        }
        precipAcumulada+=estaciones[nro].dato[i].precip;
    }
    // printf(" %s: %.1f mm, %i\n", estaciones[nro].dato[i-1].dia, precipAcumulada, i-1);
    precipitaciones[j] = precipAcumulada;
    precipitaciones[j+1] = -1; //indica fin de arreglo
    index_dias[j] = i-1;
    index_dias[j+1] = -1; //indica fin de arreglo
}

/**
* @brief Calcula la precipitacion mensual de la estacion dada
*
* Dado un ID de estacion, calcula las precipitaciones del mes.
* Valida el ID de estacion, y si el valor es válido, recorre el arreglo de estaciones
* sumando el valor de precipitaciones y enviando el promedio del mes al cliente.
*
* @param estaciones[] Arreglo con todos los datos de todas las estaciones.
* @param nro ID de la estacion solicitada.
*/
void 
mensual_precip(struct Estacion estaciones[], int index, float precipitaciones[]){
  int nro = check_estacion_existente(estaciones, index);
  if(nro == -1){
    return;
  }
  float precipAcumulada=0;

    for (int i = 0; i < estaciones[nro].cantElem; ++i)
    {
        precipAcumulada+=estaciones[nro].dato[i].precip;
    }
    // printf("Precipitacion acumulada mensual.\n%d - Estacion %s: %.1f mm\n", 
    //     estaciones[nro].numero,estaciones[nro].nombre,precipAcumulada);
    precipitaciones[0] = precipAcumulada;
}

/**
* @brief Verifica si los sensores en una estacion estan funcionando o no.
*
* Primero copia el nombre de los sensores en cada estacion del arreglo.
* Luego tokeniza una linea cualquiera de datos de una estacion.
* Si encuentra en los datos la cadena "--" significa que el sensor no esta disponible.
* Dependiendo de este resultado, guarda un 1 o un 0 en el campo "esta" del arreglo
* de sensores de la estacion.
* 
*
* @param stationArray[] Arreglo con todos los datos de todas las estaciones.
* @param j Indice de la estacion actual en el arreglo
* @param *line2 Linea de datos de la estacion actual, donde se buscan los "--".
* @param sensores_temp[] Arreglo de sensores temporal con sus nombres.
*/
void 
check_sensores(struct Estacion stationArray[], int j, char* line2, 
    struct sensor_disponible sensores_temp[] ){

    char* tempstr = calloc(strlen(line2)+1, sizeof(char));
    strcpy(tempstr, line2);
    const char s[2] = ",";
    char *token;
    int cuenta=0, skip=0;

    for (int i = 0; i < NRO_SENSORES; ++i)
    {
        strcpy(stationArray[j].sensores[i].nombreSensor,sensores_temp[i].nombreSensor);
    }

    token = strtok(tempstr, s);
    while( token != NULL )
    {
        if(skip<4){
            skip++;
            token = strtok(NULL, s);
            continue;
        }

        if(!strcmp(token,"\n")){
            break;
        }

        if(!strcmp(token, "--")){
            stationArray[j].sensores[cuenta].esta = 0;
        }
        else{
            stationArray[j].sensores[cuenta].esta = 1;   
        }
        token = strtok(NULL, s);
        cuenta++;
    }
    free(tempstr);
}

/**
* @brief Envia la estacion solicitada al cliente por conexion no segura (UDP).
*
* Funcion principal de envio de archivos al cliente.
* Primero verifica si el ID proporcionado corresponde con una estación existente.
* Envia al cliente por TCP un flag de inicio de proceso UDP.
* El cliente deberá crear un servidor UDP. Se espera a que lo haga, leyendo por TCP
* y esperando el flag servidor UDP listo.
* Se inicializa el cliente UDP, y se utiliza este medio de comunicacion hasta
* el final de la subrutina.
* Se envia el nombre del archivo al cliente, y luego se lee de la estructura y
* se envian los datos solicitados. Luego de cada envio, se espera un
* mensaje de confirmacion, para solucionar el problema de que los mensajes pueden
* no llegar en orden.
* Una vez finalizado se envia al cliente un flag de fin de transmision, se espera
* la confirmación, y se cierra el socket UDP.
*
* @param numero ID de la estacion solicitada.
* @param stationArray[] Arreglo con todos los datos de todas las estaciones.
* @param newsockfd File Descriptor del socket para poder enviar los resultados al cliente.
* @param stream Descriptor del archivo .CSV para buscar la fecha y el nombre de las columnas.
*/
void
descargar_estacion(int index, struct Estacion stationArray[], FILE* stream, char filename[]){
    int numero = check_estacion_existente(stationArray, index);
    if(numero == -1){
      return;
    }

    char buffer[TAM];
    snprintf(filename, 50, "../files/estacion%d.txt", stationArray[numero].numero);
    // snprintf(filename, 30, "../files/log.txt");
    // printf("%s\n", filename);

    FILE *fd;
    fd = fopen(filename, "wb");

    //envio cabecera de archivo
    size_t len = 0;
    char* cabecera = NULL;
    rewind(stream);
    for (int i = 0; i < 3; ++i)
    {
        getline(&cabecera,&len,stream);
        fprintf(fd, "%s\n",cabecera);
    }

    //envio lineas mientras haya datos para enviar, y espero ack cada vez
    for(int i=0;i<stationArray[numero].cantElem;i++){
        snprintf(buffer, sizeof(buffer), 
        "%d,%s,%d,%s,%.1f,%.1f,%.1f,%.1f,%.1f,%s,%.1f,%.1f,%.1f,%.1f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f",
        stationArray[numero].numero,
        stationArray[numero].nombre,
        stationArray[numero].idLocalidad,
        stationArray[numero].dato[i].fecha,
        stationArray[numero].dato[i].temp,
        stationArray[numero].dato[i].humedad, 
        stationArray[numero].dato[i].ptoRocio,
        stationArray[numero].dato[i].precip,
        stationArray[numero].dato[i].velocViento,
        stationArray[numero].dato[i].direcViento,
        stationArray[numero].dato[i].rafagaMax,
        stationArray[numero].dato[i].presion,
        stationArray[numero].dato[i].radiacion,
        stationArray[numero].dato[i].tempSuelo1,
        stationArray[numero].dato[i].tempSuelo2,
        stationArray[numero].dato[i].tempSuelo3,
        stationArray[numero].dato[i].humedadSuelo1,
        stationArray[numero].dato[i].humedadSuelo2,
        stationArray[numero].dato[i].humedadSuelo3,
        stationArray[numero].dato[i].humedadHoja);

        fprintf(fd, "%s\n",buffer);
    }
    fclose(fd);
    return;
}

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