#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#define USER_FILE "Users.txt"

// Structure to hold user information
typedef struct {
    int userId;
    char userName[25];
    int userAge;
} User;

// Function to check if a file is empty
int isFileEmpty(FILE *file) {
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        fclose(file);
        printf("No user records found\n");
        return 1;
    }
    rewind(file);
    return 0;
}

// Function to check if a given user ID already exists in the file
int isUserIdExists(int idToCheck) {
    FILE *file = fopen(USER_FILE, "r");
    if (!file) return 0;
    User user;
    while (fscanf(file, "%d %s %d", &user.userId, user.userName, &user.userAge) != EOF) {
        if (user.userId == idToCheck) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

// Function to rewrite user data common for update and delete
void rewriteUserData(int targetId, int isUpdate) { 
    FILE *file = fopen(USER_FILE, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (!file || !tempFile) {
        printf("Error opening files.\n");
        return;
    }
    if (isFileEmpty(file)) {
        return;
    }
    User user;
    int found = 0;
    while (fscanf(file, "%d %s %d", &user.userId, user.userName, &user.userAge) != EOF) {
        if (user.userId == targetId) {
            found = 1;
            if (isUpdate) {
                printf("Enter new name: ");
                scanf("%s", user.userName);
                printf("Enter new age: ");
                scanf("%d", &user.userAge);

                // Validate age before updating
                if (user.userAge <= 0) {
                    printf("Invalid age\n");
                    fclose(file);
                    fclose(tempFile);
                    remove("temp.txt");
                    return;
                }
                fprintf(tempFile, "%d %s %d\n", user.userId, user.userName, user.userAge);
            }
        } else {
            // Write unchanged records to temp file
            fprintf(tempFile, "%d %s %d\n", user.userId, user.userName, user.userAge);
        }
    }
    fclose(file);
    fclose(tempFile);

    // Replace original file with updated temp file
    remove(USER_FILE);
    rename("temp.txt", USER_FILE);
    if (found) {
        if (isUpdate)
            printf("User updated successfully\n");
        else
            printf("User deleted successfully\n");
    } else {
        printf("User with ID %d not found\n", targetId);
    }
}

// Function to create user
void createUser() {
    FILE *file = fopen(USER_FILE, "a");
    if (!file) {
        printf("Error in opening file\n");
        return;
    }
    User newUser;

    // Get and validate unique ID
    printf("Enter user ID: ");
    scanf("%d", &newUser.userId);
    if (isUserIdExists(newUser.userId)) {
        printf("User ID already exists\n");
        fclose(file);
        return;
    }
    printf("Enter user name: ");
    scanf("%s", newUser.userName);
    printf("Enter user age: ");
    scanf("%d", &newUser.userAge);

    // Validate non-negative age
    if (newUser.userAge <= 0) {
        printf("Invalid age\n");
        fclose(file);
        return;
    }
    fprintf(file, "%d %s %d\n", newUser.userId, newUser.userName, newUser.userAge);
    fclose(file);
    printf("User added successfully\n");
}

// Function to display all the user records
void readUsers() {
    FILE *file = fopen(USER_FILE, "r");
    if (!file) {
        printf("No records found\n");
        return;
    }
    if (isFileEmpty(file)) {
        return;
    }
    User user;
    printf("ID\tName\tAge\n");
    while (fscanf(file, "%d %s %d", &user.userId, user.userName, &user.userAge) != EOF) {
        printf("%d\t%s\t%d\n", user.userId, user.userName, user.userAge);
    }
    fclose(file);
}

// Function to update an existing user's name and age
void updateUser() {
    int targetId;
    printf("Enter user ID to update: ");
    scanf("%d", &targetId);
    rewriteUserData(targetId, 1);
}

// Function to delete a user record by ID
void deleteUser() {
    int targetId;
    printf("Enter user ID to delete: ");
    scanf("%d", &targetId);
    rewriteUserData(targetId, 0);
}

int main() {
    int choice;
    while (1) {
        printf("Enter 1 for create user\n");
        printf("Enter 2 for read users\n");
        printf("Enter 3 for update user\n");
        printf("Enter 4 for delete user\n");
        printf("Enter 5 for exit\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                createUser();
                break;
            case 2:
                readUsers();
                break;
            case 3:
                updateUser();
                break;
            case 4:
                deleteUser();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice entered\n");
        }
    }
    return 0;
}
