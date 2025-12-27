#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

struct MessagePayload {
    long type;
    int size;
    int data[50];
};

// Function to swap 
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to sort
void sortArray(int arr[], int size) {
    int i, j;
    for(i = 0; i < size; i++) {
        for(j = i + 1; j < size; j++) {
            if(arr[i] > arr[j]) {
                swap(&arr[i], &arr[j]);
            }
        }
    }
}

int main() {
    key_t key = ftok("msgfile", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if(fork() == 0) {
        struct MessagePayload msg;
        msgrcv(msgid, &msg, sizeof(msg), 1, 0);
        sortArray(msg.data, msg.size);
        msg.type = 2;
        msgsnd(msgid, &msg, sizeof(msg), 0);
    }
    else {
        struct MessagePayload msg;
        msg.type = 1;
        printf("Enter number of elements: ");
        scanf("%d", &msg.size);
        int i;
        for(i = 0; i < msg.size; i++) {
            scanf("%d", &msg.data[i]);
        }
        msgsnd(msgid, &msg, sizeof(msg), 0);
        msgrcv(msgid, &msg, sizeof(msg), 2, 0);
        printf("Sorted Array:\n");
        for(i = 0; i < msg.size; i++) {
            printf("%d ", msg.data[i]);
        }
        msgctl(msgid, IPC_RMID, NULL);
    }
    return 0;
}
