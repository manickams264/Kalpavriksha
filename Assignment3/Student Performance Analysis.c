#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// Enum for Grades
typedef enum {
    A, B, C, D, F
} Grade;

// Structure for storing rollnumber, name and marks
struct student {
    int rollNumber;
    char name[30];
    int marks[3];
};

// Function to convert enum Grade to character
char gradeToChar(Grade grade) {
    switch (grade) {
        case A: 
            return 'A';
        case B: 
            return 'B';
        case C: 
            return 'C';
        case D: 
            return 'D';
        case F: 
            return 'F';
    }
}

// Function for checking all the rollnumbers are unique
int isUniqueRollNumber(struct student students[], int count, int rollNumber) {
    for (int i = 0; i < count; i++) {
        if (students[i].rollNumber == rollNumber) {
            return 0;
        }
    }
    return 1;
}

// Function to calculate total for the marks scored
int getTotal(struct student *students) {
    if ((students -> marks[0] < 0 || students -> marks[0] > 100) || (students -> marks[1] < 0 || students -> marks[1] > 100) || (students -> marks[2] < 0 || students -> marks[3] > 100)) {
        printf("Invalid mark entered\n");
        exit(0);
    }
    int total = students -> marks[0] + students -> marks[1] +students -> marks[2];
    return total;
}

// Function to calculate average of marks
float getAverage(int total) {
    float average = total / 3.0;
    return average;
}

// Function to calculate grade for the marks scored
char calculateGrade(float average) {
    if (average >= 85) {
        return A;
    }
    else if (average >= 70) {
        return B;
    }
    else if (average >= 50) {
        return C;
    }
    else if (average >= 35) {
        return D;
    }
    else {
        return F;
    }
}

// Function to calculate performance based on the grades
char* calculatePerformance(Grade grade) {
    switch (grade) {
        case A:
            return "*****";
        case B:
            return "****";
        case C:
            return "***";
        case D:
            return "**";
        default:
            return "";
    }
    
}

// Function to sort the rollnumbers recursively
void RecursiveRollNumbersSorting(int array[], int n) {
    if (n == 1) {
        return;
    }
    for (int i = 0; i < n - 1; i++) {
        if (array[i] > array[i + 1]) {
            int temp = array[i];
            array[i] = array[i + 1];
            array[i + 1] = temp;
        }
    }
    RecursiveRollNumbersSorting(array, n-1);
}

// Function to print the rollnumbers recursively
void RecursiveRollNumberPrinting(int array[], int n, int index) {
    if (index == n) {
        return;
    }
    printf("%d ", array[index]);
    RecursiveRollNumberPrinting(array, n, index + 1);
}

// Function to get user input 
void inputStudents(struct student newStudents[], int rollNumbers[], int n) {
    for (int i = 0; i < n; i++) {
        scanf("%d %s %d %d %d", &newStudents[i].rollNumber, newStudents[i].name, &newStudents[i].marks[0], &newStudents[i].marks[1], &newStudents[i].marks[2]);
        if (!isUniqueRollNumber(newStudents, i, newStudents[i].rollNumber)) {
            printf("Roll number should be unique\n");
            exit(0);
        }
        rollNumbers[i] = newStudents[i].rollNumber;
    }
}

// Function to display student performance
void displayPerformance(struct student newStudents[], int n) {
    for (int i = 0; i < n; i++) {
        int total = getTotal(&newStudents[i]);
        float average = getAverage(total);
        Grade grade = calculateGrade(average);
        const char *performance = calculatePerformance(grade);

        printf("Roll No: %d\n", newStudents[i].rollNumber);
        printf("Name: %s\n", newStudents[i].name);
        printf("Total: %d\n", total);
        printf("Average: %.2f\n", average);
        printf("Grade: %c\n", gradeToChar(grade));
        if (grade != F) {
            printf("Performance: %s\n", performance);
        }
        printf("\n");
    }
}

// Function to print all roll numbers
void displayRollNumbers(int rollNumbers[], int n) {
    printf("List of Roll Numbers: ");
    if (n == 1) {
        RecursiveRollNumberPrinting(rollNumbers, n, 0);
    } else {
        RecursiveRollNumbersSorting(rollNumbers, n);
        RecursiveRollNumberPrinting(rollNumbers, n, 0);
    }
    printf("\n");
}

int main() {
    int n;
    // number of students
    scanf("%d", &n);
    if (n < 1) {
        printf("Invalid input\n");
        return 0;
    }

    struct student newStudents[n];
    int rollNumbers[n];

    inputStudents(newStudents, rollNumbers, n);
    displayPerformance(newStudents, n);
    displayRollNumbers(rollNumbers, n);

    return 0;
}
