# TP3-OS2-2017 

### Trabajo Práctico 3 - Sistemas Embebidos
![Icono](https://cdn0.iconfinder.com/data/icons/seo-flat-long-shadow/512/Cloud_Server-256.png)
___
|Autor                               |
|------------------------------------|
|Maero Facundo - 38479441              |

## 1. Descripción del trabajo
---
Este proyecto permite instalar un webserver liviano, para poder acceder al mismo y realizar consultas de distintos tipos. 
Es capaz de mostrar información sobre el sistema (CPU, Kernel, RAM, etc), módulos del Kernel instalados, la posibilidad de subir un módulo propio e instalarlo o eliminarlo, y mostrar la información que se desee sobre un conjunto de estaciones hidrometeorológicas (precipitaciones, humedad, temperatura, etc.), con la posibilidad de descargar esta información en un archivo de texto plano.
La interfaz proporcionada es una página web con diferentes pestañas, una para cada función descrita previamente.

La interactividad con el servidor se realiza mediante el protocolo CGI (Common Gateway Interface) 

![CGI Logo](http://blog.honeybadger.io/images/2015/12/prismx300.gif)

El servidor seleccionado para el proyecto es Lighttpd

![Lighttpd logo](https://www.lighttpd.net/light_logo.png)

## 2. Instalación y Ejecución
---
Para instalar el proyecto deben seguirse los siguientes pasos:
En el sistema que funcionará como webserver (debe tener instalado una versión de Linux) debe ejecutarse:

```$ sudo apt update ``` para actualizar la información sobre los paquetes disponibles para instalar.

``` $ sudo apt install lighttpd``` instala el webserver liviano Lighttpd.

Luego debe abrirse una terminal en el directorio donde se descargó el repositorio, y ejecutar ```$ make ``` para compilar el proyecto y generar la carpeta **html/**, que contiene todos los archivos que el servidor necesita para funcionar.

Finalmente debe ejecutar el script de configuración con ```$ sudo ./configure.sh ```

Este script modifica permisos en las carpetas del servidor y mueve el directorio **html/** a su ruta correcta en el sistema. Además modifica el acceso del usuario **www-data** a los comandos **insmod** y **rmmod**, para poder ejecutarlos sin necesidad de la contraseña de superusuario. Finalmente sobreescribe la configuración de Lighttpd por la correcta para ejecutar los scripts CGI.

El servidor se ejecutará cada vez que el servidor se encienda, automáticamente. 
Para acceder a la página del proyecto, ingrese a la URL ```0.0.0.0/index.html ``` desde el servidor, o desde un cliente, reemplace la IP por la que tenga configurada en ese momento el servidor.

Para mostrar el funcionamiento de la instalación y eliminación de módulos del Kernel, se provee un driver básico en la carpeta **ksrc/**. Este es compilado al correr el comando ```make```.
Para instalarlo, diríjase a la pestaña correspondiente, y al elegir el archivo a subir, seleccione ```helloWorld.ko ```. 
Es importante destacar que para que el módulo pueda ser instalado, en un principio la versión de kernel del sistema que lo compila, y la que esté instalada en el servidor deben ser las mismas.

Una vez instalado el módulo, puede verse que imprime un mensaje de bienvenida, y si es borrado, uno de despedida, en los logs del kernel, con el comando 
```$ tail -n 10 /var/log/kern.log ```

## 3. CppCheck
--- 
Al compilar y linkear, se genera un archivo donde se guardan los posibles errores y advertencias que encuentre el programa CppCheck al realizar el análisis estático del código. Este archivo se encuentra en:
```
TP3-OS2-2017/err.txt
```
Si desea más información, remítase a la documentación proporcionada, que se encuentra en la ruta ```doc/html/index.html```
