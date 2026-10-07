#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main(void)
{
    struct winsize terminal;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal) == -1)
    {
        perror("Error al obtener el tamaño del terminal");
        return 1;
    }

    printf("Columnas: %d\n", terminal.ws_col);
    printf("Filas: %d\n", terminal.ws_row);

    return 0;
}