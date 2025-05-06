
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pty.h>

int main() {
    int master_fd, slave_fd;
    pid_t pid;
    char slave_name[100];

    // Créer un PTY
    if (openpty(&master_fd, &slave_fd, slave_name, NULL, NULL) == -1) {
        perror("openpty");
        exit(1);
    }

    printf("Slave terminal: %s\n", slave_name);  // Affiche le nom du terminal esclave

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {  // Processus enfant
        // Rediriger stdout et stderr vers le terminal esclave
        if (dup2(slave_fd, STDOUT_FILENO) == -1 || dup2(slave_fd, STDERR_FILENO) == -1) {
            perror("dup2");
            exit(1);
        }

        // Exécuter une commande (par exemple, 'ls')
        execlp("ls", "ls", "-l", NULL);
        exit(1);
    } else {  // Processus parent
        // Lire la sortie du terminal esclave et l'afficher dans le terminal principal
        char buffer[1024];
        int nbytes;
        close(slave_fd);  // Fermer le descripteur de fichier dans le parent

        // Lire depuis le PTY maître
        while ((nbytes = read(master_fd, buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, nbytes);  // Afficher dans le terminal principal
        }

        wait(NULL);  // Attendre la fin du processus enfant
    }

    close(master_fd);
    return 0;
}
