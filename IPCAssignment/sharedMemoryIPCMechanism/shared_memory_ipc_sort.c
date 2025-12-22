#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

// Function to sort shared memory data 
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
    int sharedMemoryId;
    int *sharedData;
    int i;
    sharedMemoryId = shmget(1234, 1024, 0666 | IPC_CREAT);
    sharedData = (int *)shmat(sharedMemoryId, NULL, 0);
    if(fork() == 0) {
        sleep(1);
        sortArray(&sharedData[1], sharedData[0]);
        shmdt(sharedData);
    } 
    else {
        printf("Enter number of elements: ");
        scanf("%d", &sharedData[0]);
        printf("Enter elements:\n");
        for(i = 1; i <= sharedData[0]; i++) {
            scanf("%d", &sharedData[i]);
        }
        printf("Before sorting:\n");
        for(i = 1; i <= sharedData[0]; i++) {
            printf("%d ", sharedData[i]);
        }
        printf("\n");
        wait(NULL);
        printf("After sorting:\n");
        for(i = 1; i <= sharedData[0]; i++) {
            printf("%d ", sharedData[i]);
        }
        printf("\n");
        shmdt(sharedData);
        shmctl(sharedMemoryId, IPC_RMID, NULL);
    }
    return 0;
}
