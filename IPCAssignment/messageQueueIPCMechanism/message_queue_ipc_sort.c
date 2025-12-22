#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

struct messageBuffer {
    long messageType;
    int size;
    int array[100];
};

// Function to sort array using message queue data 
void sortArray(int array[], int size) {
    int i, j, temp;
    for(i = 0; i < size - 1; i++) {
        for(j = i + 1; j < size; j++) {
            if(array[i] > array[j]) {
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
}

int main() {
    int messageQueueId;
    struct messageBuffer message;
    int i;
    messageQueueId = msgget(1234, 0666 | IPC_CREAT);
    if(fork() == 0) {
        msgrcv(messageQueueId, &message, sizeof(message) - sizeof(long), 1, 0);
        sortArray(message.array, message.size);
        message.messageType = 2;
        msgsnd(messageQueueId, &message, sizeof(message) - sizeof(long), 0);
    } 
    else {
        printf("Enter number of elements: ");
        scanf("%d", &message.size);
        printf("Enter elements:\n");
        for(i = 0; i < message.size; i++) {
            scanf("%d", &message.array[i]);
        }
        printf("Before sorting:\n");
        for(i = 0; i < message.size; i++) {
            printf("%d ", message.array[i]);
        }
        printf("\n");
        message.messageType = 1;
        msgsnd(messageQueueId, &message, sizeof(message) - sizeof(long), 0);
        msgrcv(messageQueueId, &message, sizeof(message) - sizeof(long), 2, 0);
        printf("After sorting:\n");
        for(i = 0; i < message.size; i++) {
            printf("%d ", message.array[i]);
        }
        printf("\n");
        msgctl(messageQueueId, IPC_RMID, NULL);
    }
    return 0;
}
