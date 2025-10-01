#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#define FILE_NAME "Users.txt"

typedef struct {
    int id;
    char name[25];
    int age;
} User;

void Create_User() {
    FILE *file = fopen(FILE_NAME, "a");
    if (!file) {
        printf("Error in opening file\n");
        return;
    }
    User user;
    printf("Enter user ID: ");
    scanf("%d", &user.id);
    printf("Enter user name: ");
    scanf("%s", user.name);
    printf("Enter user age: ");
    scanf("%d", &user.age);
    fprintf(file, "%d %s %d\n", user.id, user.name, user.age);
    fclose(file);
    printf("User added successfully\n");
}

void Read_Users() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        printf("No records found\n");
        return;
    }
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        printf("No records found\n");
        fclose(file);
        return;
    }
    rewind(file);
    User user;
    printf("ID\tName\tAge\n");
    while (fscanf(file, "%d %s %d", &user.id, user.name, &user.age) != EOF) {
        printf("%d\t%s\t%d\n", user.id, user.name, user.age);
    }
    fclose(file);
}

void Update_User() {
    FILE *file = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!file || !temp) {
        printf("Error in opening file\n");
        return;
    }
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        printf("No records found\n");
        fclose(file);
        return;
    }
    rewind(file);
    int id;
    int found = 0;
    User user;
    printf("Enter user ID to update: ");
    scanf("%d", &id);
    while (fscanf(file, "%d %s %d", &user.id, user.name, &user.age) != EOF) {
        if (user.id == id) {
            found = 1;
            printf("Enter new name: ");
            scanf("%s", user.name);
            printf("Enter new age: ");
            scanf("%d", &user.age);
        }
        fprintf(temp, "%d %s %d\n", user.id, user.name, user.age);
    }
    fclose(file);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);
    if (found) {
        printf("User updated successfully\n");
    }
    else {
        printf("User with ID %d not found", id);
    }
}

void Delete_User() {
    FILE *file = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!file || !temp) {
        printf("Error in opening file\n");
        return;
    }
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        printf("No records found\n");
        fclose(file);
        return;
    }
    rewind(file);
    int id;
    int found = 0;
    User user;
    printf("Enter ID to delete: ");
    scanf("%d", &id);
    while (fscanf(file, "%d %s %d", &user.id, user.name, &user.age) != EOF) {
        if (user.id == id) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d %s %d\n", user.id, user.name, user.age);
    }
    fclose(file);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);
    if (found) {
        printf("User deleted successfully\n");
    }
    else {
        printf("User with ID %d not found\n", id);
    }
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
                Create_User();
                break;
            case 2:
                Read_Users();
                break;
            case 3:
                Update_User();
                break;
            case 4:
                Delete_User();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice entered\n");
        }
    }
    return 0;
}