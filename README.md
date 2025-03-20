Proyecto de Raycasting en C con SDL2

Descripción

Este proyecto es una implementación de un motor de raycasting en C utilizando la biblioteca SDL2. Permite la exploración de un mapa en primera persona, renderizando paredes y un objetivo utilizando la técnica de raycasting.

Características

Motor de raycasting para la representación 3D de un entorno 2D.

Movimiento del jugador con colisión.

Rotación de la cámara para simular una vista en primera persona.

Mini mapa para visualización del entorno.

Detección de meta para finalizar el nivel.

Requisitos

Para compilar y ejecutar el proyecto, necesitas:

GCC (o cualquier compilador de C compatible)

SDL2

Make

Instalación de dependencias en Linux (Ubuntu/Debian)

Compilación y Ejecución

Compilar el proyecto

Ejecutar el juego

Controles

W - Mover hacia adelante

S - Mover hacia atrás

A - Rotar a la izquierda

D - Rotar a la derecha

ESC - Salir del juego

Estructura del Proyecto

Formato del Archivo de Mapa

El mapa se representa en un archivo de texto (maps/map1.txt), donde:

0 - Espacio vacío (se puede caminar sobre él)

1 - Pared (bloquea el paso)

2 - Meta (objetivo a alcanzar)

Ejemplo de map1.txt:
