#include <stdio.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int pixel_a_gris(unsigned char r, unsigned char g, unsigned char b)
{
    return 0.299 * r + 0.587 * g + 0.114 * b;
}

char gris_a_ascii(int gris)
{
    char caracteres[] = " .:-=+*#%@";
    int indice = gris * 10 / 256;
    return caracteres[indice];
}

void imagen_a_ascii(unsigned char *imagen, int ancho, int alto, int canales)
{
    int nuevo_ancho = 120;
    int nuevo_alto = 25;

    for (int y = 0; y < nuevo_alto; y++)
    {
        for (int x = 0; x < nuevo_ancho; x++)
        {
            int suma = 0;
            int cantidad = 0;

            int x_inicio = x * ancho / nuevo_ancho;
            int x_fin = (x + 1) * ancho / nuevo_ancho;
            int y_inicio = y * alto / nuevo_alto;
            int y_fin = (y + 1) * alto / nuevo_alto;

            for (int py = y_inicio; py < y_fin; py++)
            {
                for (int px = x_inicio; px < x_fin; px++)
                {
                    int indice = (py * ancho + px) * canales;

                    unsigned char r = imagen[indice];
                    unsigned char g = imagen[indice + 1];
                    unsigned char b = imagen[indice + 2];

                    int gris = pixel_a_gris(r, g, b);

                    suma += gris;
                    cantidad++;
                }
            }
            int gris_medio = suma / cantidad;
            char caracter = gris_a_ascii(gris_medio);
            printf("%c", caracter);
        }
        printf("\n");
    }
}

int main(void)
{
    for (int numero = 1; numero <= 20; numero++)
    {
        char nombre[100];

        snprintf(nombre, sizeof(nombre), "frames_prueba/frame_%03d.png", numero);

        int ancho, alto, canales;

        unsigned char *imagen = stbi_load(
            nombre,
            &ancho,
            &alto,
            &canales,
            0
        );

        if (imagen == NULL)
        {
            printf("Error al cargar %s\n", nombre);
            continue;
        }

        printf("\033[H\033[J");

        imagen_a_ascii(
            imagen,
            ancho,
            alto,
            canales
        );

        stbi_image_free(imagen);

        usleep(500000);
    }

    return 0;
}

/*

int main(void)
{
    for (int numero = 1; numero <= 4; numero++)
    {
        char nombre[100];
        snprintf(nombre, sizeof(nombre), "frames_prueba/frame_%02d.png", numero);

        printf("procesando imagen: %s\n", nombre);

        int ancho, alto, canales;
    
        unsigned char *imagen = stbi_load(
            nombre,
            &ancho,
            &alto,
            &canales,
            0
        );

        if (imagen == NULL)
        {
            printf("Error al cargar la imagen\n");
            continue;
        }

        imagen_a_ascii(imagen, ancho, alto, canales);
        stbi_image_free(imagen);
    }

    return 0;
}

*/