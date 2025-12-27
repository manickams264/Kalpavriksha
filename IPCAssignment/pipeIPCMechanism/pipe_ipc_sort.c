#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// Function to swap values 
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to sort array 
void sortArray(int arr[], int size){
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
    int pipeParentToChild[2], pipeChildToParent[2];
    pipe(pipeParentToChild);
    pipe(pipeChildToParent);
    if(fork() == 0) {
        int size, arr[50];
        read(pipeParentToChild[0], &size, sizeof(size));
        read(pipeParentToChild[0], arr, sizeof(int) * size);
        sortArray(arr, size);
        write(pipeChildToParent[1], arr, sizeof(int) * size);
    }
    else {
        int size, i, arr[50];
        printf("Enter number of elements: ");
        scanf("%d", &size);
        for(i = 0; i < size; i++) {
            scanf("%d", &arr[i]);
        }
        write(pipeParentToChild[1], &size, sizeof(size));
        write(pipeParentToChild[1], arr, sizeof(int) * size);
        read(pipeChildToParent[0], arr, sizeof(int) * size);
        printf("Sorted Array:\n");
        for(i = 0; i < size; i++) {
            printf("%d ", arr[i]);
        }
    }
    return 0;
}
