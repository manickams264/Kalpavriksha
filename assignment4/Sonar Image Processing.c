#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to display matrix using pointer arithmetic
void displayMatrix(int *matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d ", *(matrix + i * n + j));
        }
        printf("\n");
    }
}

// Function to rotate elements in clockwise order
void rotate(int *top, int *left, int *bottom, int *right) {
    int temp = *top;
    *top = *left;
    *left = *bottom;
    *bottom = *right;
    *right = temp;
}

// Function to rotate matrix using pointers
void rotateMatrix(int *matrix, int n) {
    for (int layer = 0; layer < n / 2; layer++) {
        int first = layer;
        int last = n - layer - 1;
        for (int i = first; i < last; i++) {
            int offset = i - first;
            int *top = matrix + first * n + i;
            int *left = matrix + (last - offset) * n + first;
            int *bottom = matrix + last * n + (last - offset);
            int *right = matrix + i * n + last;
            int temp = *top;
            
            rotate(top, left, bottom, right);
        }
    }
}

// Function to traverse neighbour elements
void traverseNeighbours(int *temp, int size, int row, int col, int *totalSum, int *neighbourCount) {
    int sum = 0;
    int count = 0;
    for (int drow = -1; drow <= 1; drow++) {
        for (int dcol = -1; dcol <= 1; dcol++) {
            int nrow = row + drow; 
            int ncol = col + dcol;
            if (nrow >= 0 && nrow < size && ncol >= 0 && ncol < size) {
                sum += *(temp + nrow * size + ncol);
                count++;
            }
        }
    }
    *totalSum = sum;
    *neighbourCount = count;
}

// function to apply smoothing filter using  pointers
void smoothMatrix(int *matrix, int size) {
    int *temp = (int *)malloc(size * size * sizeof(int));
    if (!temp) {
        printf("Memory allocation failed \n");
        return;
    }
    for (int i = 0; i < size * size; i++) {
        temp[i] = *(matrix + i);
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int sum = 0, count = 0;
            traverseNeighbours(temp, size, i, j, &sum, &count);
            if (count > 0)
                *(matrix + i * size + j) = sum / count;
        }
    }
    free(temp);
}


int main() {
    int size;
    printf("Enter matrix size: ");
    scanf("%d", &size);
    if (size < 2 || size > 10) {
        printf("Invalid size!\n");
        return 0;
    }
    int *matrix = (int *)malloc(size * size * sizeof(int));
    if (!matrix) {
        printf("Memory allocation failed \n");
        return 0;
    }
    srand(time(0));
    for (int i = 0; i < size * size; i++) {
        *(matrix + i) = rand() % 256;
    }
    printf("\nOriginal:\n");
    displayMatrix(matrix, size);

    rotateMatrix(matrix, size);
    printf("\nRotated:\n");
    displayMatrix(matrix, size);

    smoothMatrix(matrix, size);
    printf("\nFinal Output:\n");
    displayMatrix(matrix, size);
    free(matrix);
    return 0;
}
