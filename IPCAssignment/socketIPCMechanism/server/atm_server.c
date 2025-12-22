#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define DATABASE_PATH "../resource/accountDB.txt"

pthread_mutex_t fileMutex;

// Function to handle individual client ATM requests 
void *handleClientRequest(void *clientSocketPointer) {
    int clientSocket = *(int *)clientSocketPointer;
    int choice, amount, balance;
    FILE *filePointer;
    read(clientSocket, &choice, sizeof(int));
    pthread_mutex_lock(&fileMutex);
    filePointer = fopen(DATABASE_PATH, "r");
    fscanf(filePointer, "%d", &balance);
    fclose(filePointer);
    if(choice == 1) {
        read(clientSocket, &amount, sizeof(int));
        if(amount <= balance) {
            balance = balance - amount;
        } else {
            balance = -1;
        }
        write(clientSocket, &balance, sizeof(int));
    }
    if(choice == 2) {
        read(clientSocket, &amount, sizeof(int));
        balance = balance + amount;
        write(clientSocket, &balance, sizeof(int));
    }
    if(choice == 3) {
        write(clientSocket, &balance, sizeof(int));
    }
    if(balance != -1) {
        filePointer = fopen(DATABASE_PATH, "w");
        fprintf(filePointer, "%d", balance);
        fclose(filePointer);
    }
    pthread_mutex_unlock(&fileMutex);
    close(clientSocket);
    free(clientSocketPointer);
    return NULL;
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t addressLength;
    pthread_t threadId;
    pthread_mutex_init(&fileMutex, NULL);
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 5);
    printf("ATM Server running on port %d\n", PORT);
    while(1) {
        addressLength = sizeof(clientAddress);
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &addressLength);
        int *clientSocketPointer = malloc(sizeof(int));
        *clientSocketPointer = clientSocket;
        pthread_create(&threadId, NULL, handleClientRequest, clientSocketPointer);
        pthread_detach(threadId);
    }
    close(serverSocket);
    pthread_mutex_destroy(&fileMutex);
    return 0;
}
