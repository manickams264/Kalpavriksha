#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>

// Swap function 
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Sort function
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
    int shmid = shmget(IPC_PRIVATE, 1024, 0666 | IPC_CREAT);
    int *shared = (int *)shmat(shmid, NULL, 0);
    if(fork() == 0) {
        sortArray(shared + 1, shared[0]);
    }
    else {
        int i;
        printf("Enter number of elements: ");
        scanf("%d", &shared[0]);
        for(i = 1; i <= shared[0]; i++) {
            scanf("%d", &shared[i]);
        }
        wait(NULL);
        printf("Sorted Array:\n");
        for(i = 1; i <= shared[0]; i++) {
            printf("%d ", shared[i]);
        }
        shmdt(shared);
        shmctl(shmid, IPC_RMID, NULL);
    }
    return 0;
}
