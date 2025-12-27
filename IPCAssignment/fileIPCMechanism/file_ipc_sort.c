#include <stdio.h>
#include <stdlib.h>

// Function to swap two integers 
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to sort array 
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

// Function to read array from file 
void readFromFile(int arr[], int size) {
    FILE *file = fopen("data.txt", "r");
    if(file == NULL) {
        perror("File open failed");
        exit(1);
    }
    int i;
    for(i = 0; i < size; i++) {
        fscanf(file, "%d", &arr[i]);
    }
    fclose(file);
}

// Function to write array to file 
void writeToFile(int arr[], int size) {
    FILE *file = fopen("data.txt", "w");
    if(file == NULL) {
        perror("File open failed");
        exit(1);
    }
    int i;
    for(i = 0; i < size; i++) {
        fprintf(file, "%d ", arr[i]);
    }
    fclose(file);
}

int main() {
    int size, i;
    printf("Enter number of elements: ");
    scanf("%d", &size);
    int arr[size];
    printf("Enter elements:\n");
    for(i = 0; i < size; i++) {
        scanf("%d", &arr[i]);
    }
    writeToFile(arr, size);
    readFromFile(arr, size);
    printf("Before Sorting:\n");
    for(i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    sortArray(arr, size);
    writeToFile(arr, size);
    readFromFile(arr, size);
    printf("\nAfter Sorting:\n");
    for(i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    return 0;
}
