# TC2038 Análisis y diseño de algoritmos avanzados (Gpo 602) Acts

Oliver Moreno Ibarra - A01742930

## Requisitos

- **GNU Compiler Collection (gcc)**
- **Make** para compilación automatizada

En caso de no tener alguno de los 2 anteriores:

- **Scoop** para instalar paqueterías en Windows

## Instalación

Para instalar GCC y Make necesitarás [Scoop](https://scoop.sh/), que permite instalar paqueterías desde línea de comandos.

### Scoop

1. Abre **PowerShell** como administrador y pega las siguientes líneas:

```
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser

Invoke-RestMethod -Uri https://get.scoop.sh | Invoke-Expression
```

2. Para verificar la instalación pega lo siguiente:

```
scoop --version
```

Scoop podría instalar paquetes extra que vienen de un "main bucket" que no importan para este proyecto.

Ahora podemos instalar los paquetes que realmente necesitamos:

### GNU Compiler Collection

Instala y verifica con las siguientes 2 líneas:

```
scoop install gcc

g++ --version
```

### Make

Instala y verifica con las siguientes 2 líneas:

```
scoop install make

make --version
```

## SETUP Compilación - MAKE

**Para compilar** entra a Makefile y cambia el directorio en Source "SRC" que se ve de la siguiente manera:

```
SOURCES = carpeta/archivo.cpp
```

En caso de necesitar compilar más de un archivo como librería, sería de la siguiente manera:

```
SOURCES = carpeta/archivo.cpp carpeta/libreria1.cpp carpeta/libreria2.cpp
```

Cada vez que se compile generará el archivo executable en `output/main`

## Compilación y Ejecución - MAKE

**Finalmente** pega la siguiente linea en el `cmd`:

- Para **solo** compilar:

```
make
```

- Para compilar y ejecutar:

```
make run
```

- Para **solo** ejecutar:

```
./output/main
```
