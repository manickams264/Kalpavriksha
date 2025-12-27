#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    int choice, amount, result;
    printf("1.Withdraw\n2.Deposit\n3.Display Balance\n4.Exit\n");
    scanf("%d", &choice);
    write(sock, &choice, sizeof(choice));
    if(choice == 1 || choice == 2) {
        printf("Enter amount: ");
        scanf("%d", &amount);
        write(sock, &amount, sizeof(amount));
        read(sock, &result, sizeof(result));
        if(result == -1) {
            printf("Insufficient Balance\n");
        }
        else {
            printf("Updated Balance: %d\n", result);
        }
    }
    if(choice == 3) {
        read(sock, &result, sizeof(result));
        printf("Balance: %d\n", result);
    }
    close(sock);
    return 0;
}
