#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 10

// Function to display matrix using pointer arithmetic
void displayMatrix(int *matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", *(matrix + i * n + j));
        }
        printf("\n");
    }
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
            *top = *left;
            *left = *bottom;
            *bottom = *right;
            *right = temp;
        }
    }
}

// function to apply smoothing filter using  pointers
void smoothMatrix(int *matrix, int n) {
    int temp[MAX * MAX];
    for (int i = 0; i < n * n; i++)
        temp[i] = *(matrix + i); 
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0, count = 0;
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    int ni = i + di;
                    int nj = j + dj;
                    if (ni >= 0 && ni < n && nj >= 0 && nj < n) {
                        sum += *(temp + ni * n + nj);
                        count++;
                    }
                }
            }
            *(matrix + i * n + j) = sum / count;
        }
    }
}

int main() {
    int n;
    printf("Enter matrix size: ");
    scanf("%d", &n);
    if (n < 2 || n > 10) {
        printf("Invalid size!\n");
        return 0;
    }
    int matrix[MAX][MAX];
    srand(time(0));
    int *ptr = &matrix[0][0];
    for (int i = 0; i < n * n; i++) {
        *(ptr + i) = rand() % 256;
    }
    printf("\nOriginal:\n");
    displayMatrix(ptr, n);
    rotateMatrix(ptr, n);
    printf("\nRotated:\n");
    displayMatrix(ptr, n);
    smoothMatrix(ptr, n);
    printf("\nFinal Output:\n");
    displayMatrix(ptr, n);
    return 0;
}
