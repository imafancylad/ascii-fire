#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>

int obtener_tamano_terminal(int *columnas, int *filas)
{
    struct winsize terminal;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal) == -1)
    {
        return 0;
    }

    *columnas = terminal.ws_col;
    *filas = terminal.ws_row;

    return 1;
}

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

void imagen_a_ascii(unsigned char *imagen, int ancho, int alto, int canales, int nuevo_ancho, int nuevo_alto)
{
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

int obtener_resolucion(const char *nombre_video,int *ancho, int *alto)
{
    char comando[500];

    snprintf(comando, sizeof(comando),
        "ffprobe -v error -select_streams v:0 "
        "-show_entries stream=width,height "
        "-of default=noprint_wrappers=1:nokey=1 "
        "%s",
        nombre_video
    );

    FILE *pipe = popen(comando, "r");

    if (pipe == NULL)
    {
        return 0;
    }

    char linea[100];

    if (fgets(linea, sizeof(linea), pipe) == NULL)
    {
        pclose(pipe);
        return 0;
    }

    *ancho = atoi(linea);

    if (fgets(linea, sizeof(linea), pipe) == NULL)
    {
        pclose(pipe);
        return 0;
    }

    *alto = atoi(linea);

    pclose(pipe);

    return 1;
}

double obtener_fps(const char *nombre_video)
{
    char comando[500];

    snprintf(comando, sizeof(comando),
        "ffprobe -v error -select_streams v:0 "
        "-show_entries stream=r_frame_rate "
        "-of default=noprint_wrappers=1:nokey=1 "
        "%s",
        nombre_video
    );

    FILE *pipe = popen(comando, "r");

    if (pipe == NULL)
    {
        return 0;
    }

    char linea[100];

    if (fgets(linea, sizeof(linea), pipe) == NULL)
    {
        pclose(pipe);
        return 0;
    }

    pclose(pipe);
    int numerador, denominador;

    if (sscanf(linea, "%d/%d", &numerador, &denominador) != 2)
    {
        return 0;
    }

    if (denominador == 0)
    {
        return 0;
    }

    return (double)numerador / denominador;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Uso: %s video.mp4\n", argv[0]);
        return 1;
    }

    const char *nombre_video = argv[1];

    int ancho, alto;
    int canales = 3;

    if (!obtener_resolucion(nombre_video, &ancho, &alto))
    {
        printf("Error obteniendo resolución\n");
        return 1;
    }

    double fps = obtener_fps(nombre_video);

    if (fps <= 0)
    {
        printf("Error obteniendo FPS\n");
        return 1;
    }

    int columnas, filas;

    if (!obtener_tamano_terminal(&columnas, &filas))
    {
        printf("Error obteniendo tamaño de terminal\n");
        return 1;
    }

    double proporcion_video = (double)ancho / alto;
    double factor_caracter = 0.5;
    int nuevo_ancho = columnas;
    int nuevo_alto = nuevo_ancho * factor_caracter / proporcion_video;

    if (nuevo_alto > filas)
    {
        nuevo_alto = filas;
        nuevo_ancho = nuevo_alto * proporcion_video / factor_caracter;
    }

    printf("Resolución detectada: %d x %d\n", ancho, alto);
    printf("FPS detectado: %.2f\n", fps);
    printf("Terminal: %dx%d\n", columnas, filas);
    printf("ASCII: %dx%d\n", nuevo_ancho, nuevo_alto);

    int tamano = ancho * alto * canales;

    unsigned char *imagen = malloc(tamano);

    if (imagen == NULL)
    {
        printf("Error reservando memoria\n");
        return 1;
    }

    char comando[500];
    snprintf(comando, sizeof(comando),
        "ffmpeg -i %s -f rawvideo -pix_fmt rgb24 -loglevel error -",
        nombre_video
    );

    FILE *pipe = popen(comando, "r");

    if (pipe == NULL)
    {
        printf("Error abriendo FFmpeg\n");
        free(imagen);
        return 1;
    }

    // printf("Bytes leídos: %zu\n", leidos);
    while(1)
    {
        struct timespec inicio;
        struct timespec fin;
        clock_gettime(CLOCK_MONOTONIC, &inicio);

        size_t leidos = fread(imagen, 1, tamano, pipe);

        if (leidos != tamano)
        {
            break;
        }

        printf("\033[H\033[J");

        // printf("Frame leído correctamente\n");
        imagen_a_ascii(imagen, ancho, alto, canales, nuevo_ancho, nuevo_alto);

        clock_gettime(CLOCK_MONOTONIC, &fin);
        long segundos = fin.tv_sec - inicio.tv_sec;
        long nanosegundos = fin.tv_nsec - inicio.tv_nsec;
        long tiempo_ms = (segundos * 1000) + (nanosegundos / 1000000);
        printf("Tiempo del frame: %ld ms\n", tiempo_ms);

        double objetivo_ms = 1000.0 / fps;
        long espera_ms = objetivo_ms - tiempo_ms;

        if (espera_ms > 0)
        {
            usleep(espera_ms * 1700);
        }
    }

    // printf("Frame leído correctamente\n");
    pclose(pipe);
    free(imagen);

    return 0;
}