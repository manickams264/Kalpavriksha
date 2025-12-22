#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

// Function to sort an integer array 
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
    int pipeToChild[2], pipeToParent[2];
    int size, array[100], i;
    pipe(pipeToChild);
    pipe(pipeToParent);
    if(fork() == 0) {
        close(pipeToChild[1]);
        close(pipeToParent[0]);
        read(pipeToChild[0], &size, sizeof(int));
        read(pipeToChild[0], array, size * sizeof(int));
        sortArray(array, size);
        write(pipeToParent[1], array, size * sizeof(int));
    } 
    else {
        close(pipeToChild[0]);
        close(pipeToParent[1]);
        printf("Enter number of elements: ");
        scanf("%d", &size);
        printf("Enter elements:\n");
        for(i = 0; i < size; i++) {
            scanf("%d", &array[i]);
        }
        printf("Before sorting:\n");
        for(i = 0; i < size; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");
        write(pipeToChild[1], &size, sizeof(int));
        write(pipeToChild[1], array, size * sizeof(int));
        read(pipeToParent[0], array, size * sizeof(int));
        printf("After sorting:\n");
        for(i = 0; i < size; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");
        wait(NULL);
    }
    return 0;
}
