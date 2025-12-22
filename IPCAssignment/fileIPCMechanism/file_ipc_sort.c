#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

// Function to sort an integer array using bubble sort 
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
    int size, array[100], i;
    FILE *filePointer;
    if(fork() == 0) {
        sleep(1);
        filePointer = fopen("data.txt", "r");
        fscanf(filePointer, "%d", &size);
        for(i = 0; i < size; i++) {
            fscanf(filePointer, "%d", &array[i]);
        }
        fclose(filePointer);
        sortArray(array, size);
        filePointer = fopen("data.txt", "w");
        fprintf(filePointer, "%d\n", size);
        for(i = 0; i < size; i++) {
            fprintf(filePointer, "%d ", array[i]);
        }
        fclose(filePointer);
    } 
    else {
        filePointer = fopen("data.txt", "w");
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
        fprintf(filePointer, "%d\n", size);
        for(i = 0; i < size; i++) {
            fprintf(filePointer, "%d ", array[i]);
        }
        fclose(filePointer);
        wait(NULL);
        filePointer = fopen("data.txt", "r");
        fscanf(filePointer, "%d", &size);
        for(i = 0; i < size; i++) {
            fscanf(filePointer, "%d", &array[i]);
        }
        fclose(filePointer);
        printf("After sorting:\n");
        for(i = 0; i < size; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");
    }
    return 0;
}
