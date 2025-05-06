#include "pas_client.h"
#include "../shared/utils_v3.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUF_SIZE 1024

// Variables globales pour faciliter le nettoyage des ressources
static int sock_fd = -1;      // Descripteur de la socket vers le serveur
static int pipefd[2] = {-1, -1}; // Descripteurs du pipe (lecture/écriture)
static pid_t ipl_pid = -1;    // PID du processus pas-cman-ipl

// Fonction de nettoyage pour fermer tous les descripteurs et tuer le fils si besoin
void cleanup(void) {
    if (sock_fd != -1) sclose(sock_fd);         // Ferme la socket si ouverte
    if (pipefd[0] != -1) sclose(pipefd[0]);     // Ferme l'extrémité lecture du pipe si ouverte
    if (pipefd[1] != -1) sclose(pipefd[1]);     // Ferme l'extrémité écriture du pipe si ouverte
    if (ipl_pid > 0) skill(ipl_pid, SIGTERM);   // Envoie SIGTERM au fils si encore vivant
}

// Gestionnaire de signaux pour terminer proprement le programme
void signal_handler(int signum) {
    printError("Signal reçu, fermeture du client...");
    cleanup();
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    // 1. Vérification des arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server_port = atoi(argv[1]);
    if (server_port <= 0 || server_port > 65535) {
        printError("Port invalide");
        exit(EXIT_FAILURE);
    }

    // 2. Installation des gestionnaires de signaux pour une terminaison propre
    ssigaction(SIGINT, signal_handler);   // Ctrl+C
    ssigaction(SIGTERM, signal_handler);  // kill ou arrêt système

    // 3. Connexion au serveur (fonction sécurisée fournie)
    sock_fd = connect_to_server(server_port);

    // 4. Création du pipe (unidirectionnel)
    //    - pipefd[1] : écriture (sera redirigée sur stdout de pas-cman-ipl)
    //    - pipefd[0] : lecture (utilisée par pas_client)
    spipe(pipefd);

    // 5. Fork pour lancer pas-cman-ipl dans un processus fils
    ipl_pid = sfork();
    if (ipl_pid == 0) {

        // === Fils ===
        // On ne garde que l'extrémité écriture du pipe
        sclose(pipefd[0]);

        // Redirige stdout du fils vers l'écriture du pipe
        sdup2(pipefd[1], STDOUT_FILENO);
        // Redirige stdin du pas_cman_ipl vers la sortie du socket
        sdup2(sock_fd, STDIN_FILENO);

        // On peut fermer l'original après duplication
        sclose(pipefd[1]); 

        // Lance pas-cman-ipl (il héritera de la socket si besoin)
        sexecl("./src/ui/student_kit/target/release/pas-cman-ipl", "pas-cman-ipl", NULL);
    }

    // === Parent ===
    // On ne garde que l'extrémité lecture du pipe
    sclose(pipefd[1]);

    // 6. Boucle principale : lit le pipe, écrit sur la socket
    // On prépare un buffer pour stocker les données lues depuis le pipe
    char buf[BUF_SIZE];

    // Boucle infinie sauf fin du pipe ou erreur
    while (1) {
        // On lit jusqu'à BUF_SIZE octets depuis pipefd[0].
        // Cette lecture est bloquante : le programme attend qu'il y ait des données à lire 
        ssize_t n = sread(pipefd[0], buf, BUF_SIZE);
        if (n == 0) {
            // Fin du pipe : pas-cman-ipl a terminé ou fermé stdout
            printOk("Fin du pipe (pas-cman-ipl terminé)");
            break;
        }
        if (n < 0) {
            printError("Erreur lecture pipe");
            break;
        }
        // Pour relayer la commande reçue vers le serveur
        // On envoie les n octets lus du pipe vers le serveur via la socket.
        // sock_fd : destinnation, buf : pointeur vers les données
        // n : nombre d'octets, 0 : pas de flags
        // return : le nombre d'octets envoyés
        // (équivalent au write mais pour les sockets)
        ssize_t sent = send(sock_fd, buf, n, 0);
        // Si l'envoi échoue
        if (sent < 0) {
            // Afficher une erreur et on sort de la boucle.
            printError("Erreur envoi serveur");
            break;
        }
    }

    // 7. Nettoyage et attente de la fin du fils
    cleanup();
    
    int status;
    swaitpid(ipl_pid, &status, 0); // Attend la fin du processus pas-cman-ipl
    return EXIT_SUCCESS;
}
