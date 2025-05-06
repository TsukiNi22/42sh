#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main() {
    int tty = open("/dev/tty", O_RDONLY);
    if (tty < 0) {
        perror("open");
        return 1;
    }

    struct termios oldt, newt;
    tcgetattr(tty, &oldt);          // Sauvegarder l'état courant
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Désactiver mode canonique et echo
    tcsetattr(tty, TCSANOW, &newt);  // Appliquer les changements

    char c;
    while (1) {
        read(tty, &c, 1);
        if (c == 'q') break;
        printf("Touche: %c (code %d)\n", c, c);
    }

    tcsetattr(tty, TCSANOW, &oldt); // Restaurer l’état d’origine
    close(tty);
    return 0;
}

