#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *pipe = popen(
        "ffprobe -v error -select_streams v:0 "
        "-show_entries stream=width,height "
        "-of default=noprint_wrappers=1:nokey=1 "
        "videoplayback.mp4",
        "r"
    );

    if (pipe == NULL)
    {
        printf("Error ejecutando ffprobe\n");
        return 1;
    }

    char linea[100];

    if (fgets(linea, sizeof(linea), pipe) == NULL)
    {
        printf("Error leyendo el ancho\n");
        pclose(pipe);
        return 1;
    }

    int ancho = atoi(linea);

    if (fgets(linea, sizeof(linea), pipe) == NULL)
    {
        printf("Error leyendo el alto\n");
        pclose(pipe);
        return 1;
    }

    int alto = atoi(linea);

    pclose(pipe);

    printf("Ancho detectado: %d\n", ancho);
    printf("Alto detectado: %d\n", alto);

    return 0;
}