#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <netinet/in.h>
#include <unistd.h>

pthread_mutex_t mutex;

// Function to read balance 
int readBalance() {
    FILE *file = fopen("resource/accountDB.txt", "r");
    if(file == NULL) {
        return 0;
    }
    int balance;
    fscanf(file, "%d", &balance);
    fclose(file);
    return balance;
}

// Function to write balance 
void writeBalance(int balance) {
    FILE *file = fopen("resource/accountDB.txt", "w");
    if(file != NULL){
        fprintf(file, "%d", balance);
        fclose(file);
    }
}

/* Client handler */
void *handleClient(void *socketDesc) {
    int clientSocket = *(int *)socketDesc;
    int choice, amount;
    pthread_mutex_lock(&mutex);
    read(clientSocket, &choice, sizeof(choice));
    if(choice == 1) {
        read(clientSocket, &amount, sizeof(amount));
        int balance = readBalance();
        if(amount <= balance) {
            balance = balance - amount;
            writeBalance(balance);
            write(clientSocket, &balance, sizeof(balance));
        }
        else {
            int fail = -1;
            write(clientSocket, &fail, sizeof(fail));
        }
    }
    if(choice == 2) {
        read(clientSocket, &amount, sizeof(amount));
        int balance = readBalance();
        balance = balance + amount;
        writeBalance(balance);
        write(clientSocket, &balance, sizeof(balance));
    }
    if(choice == 3) {
        int balance = readBalance();
        write(clientSocket, &balance, sizeof(balance));
    }
    pthread_mutex_unlock(&mutex);
    close(clientSocket);
    return NULL;
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);
    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);
    pthread_mutex_init(&mutex, NULL);
    while(1) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        pthread_t thread;
        pthread_create(&thread, NULL, handleClient, &clientSocket);
    }
    return 0;
}
