# ASCII-FIRE

Proyecto en C para reproducir un vídeo .mp4 como **arte ASCII directamente en la terminal**.

## Estado actual

* Funciona usando ubuntu 26.04 + GCC + FFmpeg.
* Vídeo de prueba: `640x360`, `24 FPS`, ~1 hora.
* FFmpeg envía los frames en `RGB24` mediante un `PIPE`.
* C recibe los frames con `fread()`.
* RGB → escala de grises → caracteres ASCII.
* Se calcula el promedio de cada zona de la imagen para obtener un carácter.
* El programa detecta automáticamente:

  * Resolución del vídeo.
  * FPS.
  * Tamaño de la terminal.
* El ASCII se adapta al tamaño de la terminal y corrige aproximadamente la proporción de los caracteres.
* La reproducción intenta respetar los `24 FPS`.

## Compilar

```bash
gcc raw.c -o ascii-fire
```

## Ejecutar

```bash
./ascii-fire video.mp4
```

## Mejoras futuras

* Mejorar la calidad visual.
* Eliminar el ruido del fondo oscuro.
* Mejorar contraste y brillo.
* Centrar la imagen.
* Manejar correctamente `Ctrl+C` y el redimensionamiento de la terminal.
* Optimizar el rendimiento.
* Añadir colores ANSI y audio de 8-bit.

## Objetivo

Convertir el video en una reproducción ASCII fluida desde la terminal.
