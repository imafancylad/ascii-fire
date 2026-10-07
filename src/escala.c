#include <stdio.h>

int main(void)
{
    int ancho_video = 640;
    int alto_video = 360;
    int ancho_terminal = 120;
    int alto_terminal = 56;

    double proporcion = (double)ancho_video / alto_video;
    double factor_caracter = 0.5; // Ajusta este valor según la proporción de los caracteres en tu terminal

    int nuevo_ancho = ancho_terminal;
    int nuevo_alto = nuevo_ancho * factor_caracter / proporcion;

    if (nuevo_alto > alto_terminal)
    {
        nuevo_alto = alto_terminal;
        nuevo_ancho = nuevo_alto * proporcion / factor_caracter;
    }

    printf("Video: %dx%d\n", ancho_video, alto_video);
    printf("Terminal: %dx%d\n", ancho_terminal, alto_terminal);
    printf("ASCII: %dx%d\n", nuevo_ancho, nuevo_alto);

    return 0;
}