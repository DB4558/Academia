#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUF_SIZE 1024
int clientSocketfd;

void reset_str(char *str, size_t size) {
    for (size_t i = 0; i < size; i++) {
        str[i] = '\0';
    }
}

void *readFromServer(void *arg) {
    char buffer[BUF_SIZE];
    while (1) {
        reset_str(buffer, sizeof(buffer));
        ssize_t bytesRead = read(clientSocketfd, buffer, sizeof(buffer));
        if (bytesRead == -1) {
            perror("Error reading data from server");
            break;
        }

        if (bytesRead == 0) {
            printf("Server closed the connection.\n");
            break;
        }

        printf("%s", buffer);
    }
    return NULL;
}

void *cinfromuser(void *arg) {
    char userInput[1024];
    while (1) {
        reset_str(userInput, sizeof(userInput));
        if (fgets(userInput, sizeof(userInput), stdin) == NULL) {
            perror("Error reading user input");
            break;
        }
        userInput[strcspn(userInput, "\n")] = '\0'; // Remove the newline character

        if (strcmp(userInput, "quit") == 0) {
            break;
        }

        ssize_t bytesSent = write(clientSocketfd, userInput, strlen(userInput));
        if (bytesSent == -1) {
            perror("Error sending data to server");
            break;
        }
    }
    return NULL;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
        return 1;
    }

    const char *serverIp = argv[1];
    int serverPort = atoi(argv[2]);

    clientSocketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocketfd == -1) {
        perror("Error creating socket");
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = inet_addr(serverIp);

    if (connect(clientSocketfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error connecting to server");
        return 1;
    }

    pthread_t threadIDread, threadIDcin;
    pthread_create(&threadIDread, NULL, readFromServer, NULL);
    pthread_create(&threadIDcin, NULL, cinfromuser, NULL);

    pthread_join(threadIDread, NULL);
    pthread_join(threadIDcin, NULL);
    close(clientSocketfd);

    return 0;
}
