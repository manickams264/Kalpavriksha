#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int clientSocket;
    struct sockaddr_in serverAddress;
    int choice, amount, response;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    printf("1. Withdraw\n2. Deposit\n3. Display Balance\n4. Exit\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    if(choice == 4) {
        close(clientSocket);
        return 0;
    }
    write(clientSocket, &choice, sizeof(int));
    if(choice == 1 || choice == 2) {
        printf("Enter amount: ");
        scanf("%d", &amount);
        write(clientSocket, &amount, sizeof(int));
    }
    read(clientSocket, &response, sizeof(int));
    if(choice == 1 && response == -1) {
        printf("Insufficient Balance\n");
    } 
    else {
        printf("Current Balance: %d\n", response);
    }
    close(clientSocket);
    return 0;
}
