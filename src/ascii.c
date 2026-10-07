#include <stdio.h>

int main(void)
{
    unsigned char imagen[5][5] = {
        {  0,  0,  50,   0,   0 },
        {  0, 80, 100,  80,   0 },
        { 50,100, 255, 100,  50 },
        {  0, 80, 100,  80,   0 },
        {  0,  0,  50,   0,   0 }
    };

    char caracteres[] = " .:-=+*#%@";

    for (int y = 0; y < 5; y++)
    {
        for (int x = 0; x < 5; x++)
        {
            int indice = imagen[y][x] * 10 / 256;

            printf("%c", caracteres[indice]);
        }

        printf("\n");
    }

    return 0;
}