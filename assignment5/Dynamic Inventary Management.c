#include <stdio.h>
#include <stdlib.h>
#define nameLength 50

// Structure to store product details
typedef struct {
    int id;
    char name[nameLength];
    float price;
    int quantity;
} Product;

// Function to check if the productID is unique or not
int isUnique(Product *products, int count, int id) {
    for (int i = 0; i < count; i++) {
        if (products[i].id == id) { 
            return 0;
        }
    }
    return 1;
}

// Function to get input from the user
void inputProduct(Product *p, Product *products, int current_count) {
    int tempId;
    while (1) {
        printf("\nProduct ID: ");
        scanf("%d", &tempId);
        if (isUnique(products, current_count, tempId)) {
            p->id = tempId;
            break;
        } 
        else {
            printf("ProductID %d already exists.\n", tempId);
        }
    }
    printf("Product Name: ");
    scanf("%s", p->name);          
    scanf("%f", &p->price);
    printf("Product Quantity: ");
    scanf("%d", &p->quantity);
}

// Function to add products
void addProduct(Product **products, int *size) {
    int oldSize = *size;
    (*size)++;
    *products = realloc(*products, (*size) * sizeof(Product));
    if (*products == NULL) {
        printf("Memory allocation failed\n");
        exit(0);
    }
    inputProduct(&((*products)[*size - 1]), *products, oldSize);
    printf("Product added successfully\n");
}

// Function to view products 
void viewProducts(Product *products, int size) {
    if (size == 0) {
        printf("No products available\n");
        return;
    }
    printf("\nProduct ID\tProduct Name\t\tPrice\t\tQuantity\n");
    for (int i = 0; i < size; i++) {
        printf("%-10d\t%-20s\t%-10.2f\t%d\n",
               products[i].id, products[i].name, products[i].price, products[i].quantity);
    }
}

// Function to update the quantity of the product
void updateQuantity(Product *products, int size) {
    int id, newQuantity, found = 0;
    printf("\nEnter Product ID to update quantity: ");
    scanf("%d", &id);
    for (int i = 0; i < size; i++) {
        if (products[i].id == id) {
            printf("Enter new Quantity: ");
            scanf("%d", &newQuantity);
            products[i].quantity = newQuantity;
            printf("Quantity updated successfully\n");
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Product not found\n");
    }
}

// Function to search the product by id
void searchByID(Product *products, int size) {
    int id, found = 0;
    printf("\nEnter Product ID to search: ");
    scanf("%d", &id);
    for (int i = 0; i < size; i++) {
        if (products[i].id == id) {
            printf("Product Found:\n");
            printf("Product ID\tProduct Name\t\tPrice\t\tQuantity\n");
            printf("%-10d\t%-20s\t%-10.2f\t%d\n",
                   products[i].id, products[i].name, products[i].price, products[i].quantity);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Product not found\n");
    }
}

// Function to search a product by name
void searchByName(Product *products, int size) {
    if (size == 0) {   
        printf("No products available to search\n");
        return;
    }
    char keyword[nameLength];
    int found = 0;
    printf("\nEnter name to search: ");
    scanf("%s", keyword);  
    for (int i = 0; i < size; i++) {
        int nameIndex = 0;
        int match = 0;
        for (nameIndex = 0; products[i].name[nameIndex] != '\0'; nameIndex++) {
            int k = 0;
            while (keyword[k] != '\0' && products[i].name[nameIndex + k] != '\0'
                   && products[i].name[nameIndex + k] == keyword[k]) {
                k++;
            }
            if (keyword[k] == '\0') {  
                match = 1;
                break;
            }
        }
        if (match) {
            if (found == 0) {  
                printf("\nProducts Found:\n");
                printf("Product ID\tProduct Name\t\tPrice\t\tQuantity\n");
            }
            printf("%-10d\t%-20s\t%-10.2f\t%d\n",
                   products[i].id, products[i].name, products[i].price, products[i].quantity);
            found = 1;
        }
    }
    if (!found) {
        printf("Product not found\n");
    }
}


// Function to search the product by price
void searchByPrice(Product *products, int size) {
    if (size == 0) {   
        printf("\nNo products available to search\n");
        return;
    }
    float min, max;
    int found = 0;
    printf("Enter minimum price: ");
    scanf("%f", &min);
    printf("Enter maximum price: ");
    scanf("%f", &max);
    for (int i = 0; i < size; i++) {
        if (products[i].price >= min && products[i].price <= max) {
            if (found == 0) {  
                printf("\nProducts in price range:\n");
                printf("Product ID\tProduct Name\t\tPrice\t\tQuantity\n");
            }
            printf("%-10d\t%-20s\t%-10.2f\t%d\n",
                   products[i].id, products[i].name, products[i].price, products[i].quantity);
            found = 1;
        }
    }
    if (!found) {
        printf("Product not found\n");
    }
}

// Function to delete a product
void deleteProduct(Product **products, int *size) {
    int id, index = -1;
    printf("\nEnter Product ID to delete: ");
    scanf("%d", &id);
    for (int i = 0; i < *size; i++) {
        if ((*products)[i].id == id) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("Product not found\n");
        return;
    }
    for (int i = index; i < *size - 1; i++) {
        (*products)[i] = (*products)[i + 1];
    }
    (*size)--;
    *products = realloc(*products, (*size) * sizeof(Product));
    if (*products == NULL && *size > 0) {
        printf("Memory reallocation failed\n");
        exit(0);
    }
    printf("Product deleted successfully\n");
}

int main() {
    int size, choice;
    Product *products = NULL;
    printf("Enter initial number of products: ");
    scanf("%d", &size);
    if (size < 0) {
        size = 0;
    }
    products = (Product *)calloc(size, sizeof(Product));
    if (products == NULL && size > 0) {
        printf("Memory allocation failed!\n");
        return 1;
    }
    for (int i = 0; i < size; i++) {
        printf("\nEnter details for product %d:\n", i + 1);
        inputProduct(&products[i], products, i);
    }
    do {
        printf("\n1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n");
        printf("5. Search Product by Name\n");
        printf("6. Search Product by Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                addProduct(&products, &size);
                break;
            case 2:
                viewProducts(products, size);
                break;
            case 3:
                updateQuantity(products, size);
                break;
            case 4:
                searchByID(products, size);
                break;
            case 5:
                searchByName(products, size);
                break;
            case 6:
                searchByPrice(products, size);
                break;
            case 7:
                deleteProduct(&products, &size);
                break;
            case 8:
                free(products);
                printf("Program executed successfully\n");
                break;
            default:
                printf("Invalid choice entered\n");
        }
    } while (choice != 8);
    return 0;
}
