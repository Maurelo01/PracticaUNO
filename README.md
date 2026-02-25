# PracticaUNO

Implementación del juego de cartas UNO en consola, utilizando Estructuras de Datos dinámicas como: Pilas, Listas Dobles y Listas Circulares.

# Requisitos

Para compilar y ejecutar este proyecto, asegúrate de contar con lo siguiente en tu sistema:
Compilador de C++ (`g++` en Linux o `mingw-w64` para compilación en Windows).
Herramienta `make` o `cmake` (Versión mínima sugerida: 4.0).
Importante: Una terminal con soporte para colores ANSI (PowerShell/Windows Terminal en Windows; cualquier terminal nativa en Linux) para la correcta visualización de las cartas.

# Instrucciones de Compilación

Este proyecto ofrece dos métodos para compilar: utilizando el archivo `Makefile` provisto o mediante `CMake`. Elige el método que mejor se adapte a tu entorno (Que no tengas el makefile).

# Opción 1: Compilar usando Makefile

1. Abre una terminal y navega hasta la carpeta raíz del proyecto (donde se encuentra el archivo `Makefile` normalmente dentro de build).
2. Para jugar en Linux:
   - Escribe el comando:
     make
   - Una vez finalizado, ejecuta el juego con:
     ./UNO_Flip
3. Para generar un ejecutable de Windows (`.exe`):
   - (Requiere tener instalado el compilador cruzado `x86_64-w64-mingw32-g++`)-.
   - Escribe el comando:
     make exe
   - Se generará el archivo `UNO_Flip.exe`, el cual puede ser ejecutado en Windows (preferiblemente desde PowerShell para soportar los colores).
4. Para limpiar los archivos compilados:
   make clean
