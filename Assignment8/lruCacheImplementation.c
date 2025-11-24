#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 1009 

typedef struct Node {
    int key;
    char data[100];
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct HashEntry {
    int key;
    Node *node;
    struct HashEntry *next;
} HashEntry;

typedef struct LruCache {
    int capacity;
    int size;
    Node *head;   
    Node *tail;   
    HashEntry *map[HASH_SIZE];
} LruCache;

int getHash(int key) {
    return key % HASH_SIZE;
}

// Function to create new doubly linked list
Node *createNode(int key, char *value) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->key = key;
    strcpy(newNode->data, value);
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

// Function to create hash entry 
HashEntry *createHashEntry(int key, Node *node) {
    HashEntry *entry = (HashEntry *)malloc(sizeof(HashEntry));
    entry->key = key;
    entry->node = node;
    entry->next = NULL;
    return entry;
}

// -------------------- Initialize LRU Cache --------------------
LruCache *createCache(int capacity) {
    LruCache *cache = (LruCache *)malloc(sizeof(LruCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;
    for (int i = 0; i < HASH_SIZE; i++) {
        cache->map[i] = NULL;
    }
    return cache;
}

// Function to insert hashmap
void hashMapPut(LruCache *cache, int key, Node *node) {
    int hash = getHash(key);
    HashEntry *entry = createHashEntry(key, node);
    entry->next = cache->map[hash];
    cache->map[hash] = entry;
}

// Function to get hashmap
Node *hashMapGet(LruCache *cache, int key) {
    int hash = getHash(key);
    HashEntry *entry = cache->map[hash];
    while (entry != NULL) {
        if (entry->key == key) {
            return entry->node;
        }
        entry = entry->next;
    }
    return NULL;
}

// Function to remove hashmap
void hashMapDelete(LruCache *cache, int key) {
    int hash = getHash(key);
    HashEntry *entry = cache->map[hash];
    HashEntry *prev = NULL;
    while (entry != NULL) {
        if (entry->key == key) {
            if (prev == NULL) {
                cache->map[hash] = entry->next;
            } 
            else {
                prev->next = entry->next;
            }
            free(entry);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

// Function to move node to front 
void moveToFront(LruCache *cache, Node *node) {
    if (cache->head == node) {
        return;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    if (cache->tail == node) {
        cache->tail = node->prev;
    }
    node->prev = NULL;
    node->next = cache->head;
    if (cache->head != NULL) {
        cache->head->prev = node;
    }
    cache->head = node;
    if (cache->tail == NULL) {
        cache->tail = node;
    }
}

// Function to remove LRU node 
void removeTail(LruCache *cache) {
    if (cache->tail == NULL) {
        return;
    }
    Node *node = cache->tail;
    hashMapDelete(cache, node->key);
    if (node->prev != NULL) {
        node->prev->next = NULL;
    } 
    else {
        cache->head = NULL;
    }
    cache->tail = node->prev;
    free(node);
    cache->size--;
}

// Function for get Operation 
char *get(LruCache *cache, int key) {
    Node *node = hashMapGet(cache, key);
    if (node == NULL) {
        return NULL;
    }
    moveToFront(cache, node);
    return node->data;
}

// Function for put Operation
void put(LruCache *cache, int key, char *value) {
    Node *node = hashMapGet(cache, key);
    if (node != NULL) {
        strcpy(node->data, value);
        moveToFront(cache, node);
        return;
    }
    Node *newNode = createNode(key, value);
    hashMapPut(cache, key, newNode);
    newNode->next = cache->head;
    if (cache->head != NULL) {
        cache->head->prev = newNode;
    }
    cache->head = newNode;
    if (cache->tail == NULL) {
        cache->tail = newNode;
    }
    cache->size++;
    if (cache->size > cache->capacity) {
        removeTail(cache);
    }
}

int main() {
    LruCache *cache = NULL;
    char command[50];
    while (scanf("%s", command)) {
        if (strcmp(command, "createCache") == 0) {
            int size;
            scanf("%d", &size);
            cache = createCache(size);
        } 
        else if (strcmp(command, "put") == 0) {
            int key;
            char value[100];
            scanf("%d %s", &key, value);
            put(cache, key, value);
        } 
        else if (strcmp(command, "get") == 0) {
            int key;
            scanf("%d", &key);
            char *result = get(cache, key);

            if (result == NULL) {
                printf("NULL\n");
            } 
            else {
                printf("%s\n", result);
            }
        } 
        else if (strcmp(command, "exit") == 0) {
            break;
        }
    }
    return 0;
}
