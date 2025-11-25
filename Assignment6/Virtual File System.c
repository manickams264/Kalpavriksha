#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define NAME_LEN 64
#define CMD_LEN 1024
#define PATH_SEGMENTS 128

typedef struct FreeBlock {
    int index;
    struct FreeBlock *prev;
    struct FreeBlock *next;
} FreeBlock;

typedef struct FileNode {
    char name[NAME_LEN];
    int isDirectory;              
    int blockPointers[NUM_BLOCKS];
    int blockCount;
    struct FileNode *next;       
    struct FileNode *parent;      
    struct FileNode *child;       
} FileNode;

unsigned char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];
FreeBlock *freeListHead = NULL;
FileNode *root = NULL;
FileNode *cwd = NULL;

// Function to copy the string
void my_strcpy(char *d, const char *s) {
    while (*s) { 
        *d++ = *s++; 
    }
    *d = '\0';
}

// Function to check if the strings are equal
int my_streq(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) {
            return 0;
        }
        a++; 
        b++;
    }
    return (*a == '\0' && *b == '\0');
}

// Function to calculate the length of the string
int my_strlen(const char *s) {
    int n = 0; 
    while (*s++) {
        n++;
    } 
    return n;
}

// Function to initialize all blocks as free and creates doubly linked list
void initFreeList() {
    freeListHead = NULL;
    for (int i = 0; i < NUM_BLOCKS; ++i) {
        FreeBlock *f = (FreeBlock*)malloc(sizeof(FreeBlock));
        f->index = i;
        f->prev = f->next = NULL;
        if (!freeListHead) {
            freeListHead = f;
        }
        else {
            FreeBlock *t = freeListHead;
            while (t->next) t = t->next;
            t->next = f;
            f->prev = t;
        }
    }
}

// Function to remove the first free block from the list
FreeBlock* popFreeHead() {
    if (!freeListHead) {
        return NULL;
    }
    FreeBlock *f = freeListHead;
    freeListHead = f->next;
    if (freeListHead) {
        freeListHead->prev = NULL;
    }
    f->next = f->prev = NULL;
    return f;
}

// Function to add a freed block to the end
void pushFreeTail(int index) {
    FreeBlock *f = (FreeBlock*)malloc(sizeof(FreeBlock));
    f->index = index;
    f->next = f->prev = NULL;
    if (!freeListHead) {
        freeListHead = f;
        return;
    }
    FreeBlock *t = freeListHead;
    while (t->next) {
        t = t->next;
    }
    t->next = f;
    f->prev = t;
}

// Function to create a new FileNode
FileNode* newNode(const char *name, int isDir, FileNode *parent) {
    FileNode *n = (FileNode*)malloc(sizeof(FileNode));
    my_strcpy(n->name, name);
    n->isDirectory = isDir;
    n->child = NULL;
    n->next = NULL;
    n->parent = parent;
    n->blockCount = 0;
    for (int i=0;i<NUM_BLOCKS;i++) {
        n->blockPointers[i] = -1;
    }
    return n;
}

// Function to insert a new node into parents circular child list
void insertChild(FileNode *parent, FileNode *node) {
    if (!parent->child) {
        parent->child = node;
        node->next = node;
    } else {
        FileNode *t = parent->child;
        while (t->next != parent->child) t = t->next;
        t->next = node;
        node->next = parent->child;
    }
}

// Function to find a child node 
FileNode* findChild(FileNode *dir, const char *name) {
    if (!dir->child) {
        return NULL;
    }
    FileNode *t = dir->child;
    do {
        if (my_streq(t->name, name)) {
            return t;
        }
        t = t->next;
    } while (t != dir->child);
    return NULL;
}

// Function to remove a child node from its parent’s circular list
int removeChildFromParent(FileNode *parent, FileNode *node) {
    if (!parent->child) {
        return 0;
    }
    FileNode *cur = parent->child, *prev = NULL;
    do {
        if (cur == node) break;
        prev = cur;
        cur = cur->next;
    } while (cur != parent->child);
    if (cur != node) {
        return 0;
    } 
    if (cur->next == cur) {
        parent->child = NULL;
    } else {
        if (parent->child == cur) parent->child = cur->next;
        prev->next = cur->next;
    }
    cur->next = NULL;
    return 1;
}

// Function to create directory
void cmd_mkdir(const char *name) {
    if (findChild(cwd, name)) {
        printf("Directory already exists.\n");
        return;
    }
    FileNode *d = newNode(name, 1, cwd);
    insertChild(cwd, d);
    printf("Directory '%s' created successfully.\n\n", name);
}

// Function to create file
void cmd_create(const char *name) {
    if (findChild(cwd, name)) {
        printf("File already exists.\n");
        return;
    }
    FileNode *f = newNode(name, 0, cwd);
    insertChild(cwd, f);
    printf("File '%s' created successfully.\n\n", name);
}

// Function for writing data to file
void cmd_write(const char *name, const char *content) {
    FileNode *f = findChild(cwd, name);
    if (!f || f->isDirectory) {
        printf("File not found.\n\n");
        return;
    }
    int len = my_strlen(content);
    int blocksNeeded = (len + BLOCK_SIZE - 1) / BLOCK_SIZE;
    for (int i=0;i<f->blockCount;i++) {
        if (f->blockPointers[i] >= 0) pushFreeTail(f->blockPointers[i]);
        f->blockPointers[i] = -1;
    }
    f->blockCount = 0;
    for (int b=0;b<blocksNeeded;b++) {
        FreeBlock *blk = popFreeHead();
        if (!blk) {
            for (int j=0;j<f->blockCount;j++) {
                pushFreeTail(f->blockPointers[j]);
            }
            f->blockCount = 0;
            printf("Disk full!\n\n");
            return;
        }
        int idx = blk->index;
        free(blk);
        f->blockPointers[f->blockCount++] = idx;
        for (int j=0;j<BLOCK_SIZE;j++) {
            virtualDisk[idx][j] = 0;
        }
        for (int j=0;j<BLOCK_SIZE;j++) {
            int pos = b*BLOCK_SIZE + j;
            if (pos >= len) break;
            virtualDisk[idx][j] = (unsigned char)content[pos];
        }
    }
    printf("Data written successfully (size=%d bytes).\n\n", len);
}

// Function to read file content
void cmd_read(const char *name) {
    FileNode *f = findChild(cwd, name);
    if (!f || f->isDirectory) {
        printf("File not found.\n\n");
        return;
    }
    if (f->blockCount == 0) {
        printf("(empty)\n\n");
        return;
    }
    for (int b=0;b<f->blockCount;b++) {
        int idx = f->blockPointers[b];
        for (int j=0;j<BLOCK_SIZE;j++) {
            unsigned char ch = virtualDisk[idx][j];
            if (ch == 0) break;
            putchar(ch);
        }
    }
    printf("\n\n");
}

// Function to list directory contents
void cmd_ls() {
    if (!cwd->child) {
        printf("(empty)\n\n");
        return;
    }
    FileNode *t = cwd->child;
    do {
        if (t->isDirectory) printf("%s/\n", t->name);
        else printf("%s\n", t->name);
        t = t->next;
    } while (t != cwd->child);
    printf("\n");
}

// Function to change directory
void cmd_cd(const char *name) {
    if (my_streq(name, "..")) {
        if (cwd->parent) cwd = cwd->parent;
        if (cwd == root) printf("Moved to /\n\n");
        else printf("Moved to /%s\n\n", cwd->name);
        return;
    }
    FileNode *d = findChild(cwd, name);
    if (!d || !d->isDirectory) {
        printf("Directory not found.\n\n");
        return;
    }
    cwd = d;
    printf("Moved to /%s\n\n", cwd == root ? "" : cwd->name);
}

// Function to delete file
void cmd_delete(const char *name) {
    FileNode *f = findChild(cwd, name);
    if (!f || f->isDirectory) {
        printf("File not found.\n\n");
        return;
    }
    for (int i=0;i<f->blockCount;i++) {
        if (f->blockPointers[i] >= 0) pushFreeTail(f->blockPointers[i]);
    }
    removeChildFromParent(cwd, f);
    free(f);
    printf("File deleted successfully.\n\n");
}

// Function to remove empty directory
void cmd_rmdir(const char *name) {
    FileNode *d = findChild(cwd, name);
    if (!d || !d->isDirectory) {
        printf("Directory not found.\n\n");
        return;
    }
    if (d->child) {
        printf("Directory not empty. rmdir failed.\n\n");
        return;
    }
    removeChildFromParent(cwd, d);
    free(d);
    printf("Directory removed successfully.\n\n");
}

// Function to print working directory
void cmd_pwd() {
    char parts[PATH_SEGMENTS][NAME_LEN];
    int count = 0;
    FileNode *t = cwd;
    while (t && t != root) {
        my_strcpy(parts[count++], t->name);
        t = t->parent;
    }
    if (count == 0) {
        printf("/\n\n");
        return;
    }
    for (int i = count-1; i >= 0; --i) {
        putchar('/');
        printf("%s", parts[i]);
    }
    putchar('\n');
    printf("\n");
}

// Function for disk usage
void cmd_df() {
    int freeCount = 0;
    FreeBlock *t = freeListHead;
    while (t) {
        freeCount++; t = t->next; 
    }
    int used = NUM_BLOCKS - freeCount;
    double percent = (NUM_BLOCKS == 0) ? 0.0 : ((double)used / NUM_BLOCKS) * 100.0;
    printf("Total Blocks: %d\n", NUM_BLOCKS);
    printf("Used Blocks: %d\n", used);
    printf("Free Blocks: %d\n", freeCount);
    printf("Disk Usage: %.2f%%\n\n", percent);
}

// Function to free node recursively
void cleanup_nodes(FileNode *dir) {
    if (!dir) {
        return;
    }
    if (dir->child) {
        FileNode *start = dir->child;
        FileNode *t = start;
        do {
            FileNode *next = t->next;
            if (t->isDirectory) cleanup_nodes(t);
            for (int i=0;i<t->blockCount;i++) {
                if (t->blockPointers[i] >= 0) pushFreeTail(t->blockPointers[i]);
            }
            free(t);
            t = next;
        } while (t != start);
        dir->child = NULL;
    }
}

// Function to releases all allocated memory on exit
void cleanup_all() {
    if (root) {
        cleanup_nodes(root);
        free(root);
        root = cwd = NULL;
    }
    FreeBlock *t = freeListHead;
    while (t) {
        FreeBlock *n = t->next;
        free(t);
        t = n;
    }
    freeListHead = NULL;
}

// Function to displays the current directory prompt
void print_prompt() {
    if (cwd == root) {
        printf("/ > ");
    }
    else {
        printf("%s > ", cwd->name);
    }
}

int main() {
    initFreeList();
    root = newNode("/", 1, NULL);
    cwd = root;
    printf("Compact VFS - ready. Type 'exit' to quit.\n\n");
    char line[CMD_LEN];
    while (1) {
        print_prompt();
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }
        int pos = 0; while (line[pos] == ' ' || line[pos] == '\t') pos++;
        if (line[pos] == '\n' || line[pos] == '\0') { 
            continue; 
        }
        char cmd[64] = {0}, arg1[NAME_LEN] = {0}, rest[CMD_LEN] = {0};
        int i = 0;
        while (line[pos] && line[pos] != ' ' && line[pos] != '\t' && line[pos] != '\n') {
            if (i < (int)sizeof(cmd)-1) cmd[i++] = line[pos];
            pos++;
        }
        cmd[i] = '\0';
        while (line[pos] == ' ' || line[pos] == '\t') pos++;
        i = 0;
        while (line[pos] && line[pos] != ' ' && line[pos] != '\t' && line[pos] != '\n') {
            if (i < (int)sizeof(arg1)-1) arg1[i++] = line[pos];
            pos++;
        }
        arg1[i] = '\0';
        while (line[pos] == ' ' || line[pos] == '\t') pos++;
        i = 0;
        while (line[pos] && line[pos] != '\n') {
            if (i < (int)sizeof(rest)-1) rest[i++] = line[pos];
            pos++;
        }
        rest[i] = '\0';
        if (my_streq(cmd, "exit")) {
            printf("Memory released. Exiting program...\n");
            cleanup_all();
            break;
        } 
        else if (my_streq(cmd, "mkdir")) {
            if (arg1[0] == '\0') { 
                printf("Usage: mkdir <foldername>\n\n"); continue; 
            }
            cmd_mkdir(arg1);
        } 
        else if (my_streq(cmd, "ls")) {
            cmd_ls();
        } 
        else if (my_streq(cmd, "cd")) {
            if (arg1[0] == '\0') { 
                printf("Usage: cd <foldername>\n\n"); continue; 
            }
            cmd_cd(arg1);
        } 
        else if (my_streq(cmd, "create")) {
            if (arg1[0] == '\0') { 
                printf("Usage: create <filename>\n\n"); continue; 
            }
            cmd_create(arg1);
        } 
        else if (my_streq(cmd, "write")) {
            if (arg1[0] == '\0') { 
                printf("Usage: write <filename> <content>\n\n"); continue; 
            }
            char *content = rest;
            while (*content == ' ') {
                content++;
            }
            if (*content == '"' ) {
                content++;
                int len = my_strlen(content);
                if (len > 0 && content[len-1] == '"') {
                    content[len-1] = '\0';
                }
            }
            cmd_write(arg1, content);
        } 
        else if (my_streq(cmd, "read")) {
            if (arg1[0] == '\0') { 
                printf("Usage: read <filename>\n\n"); continue; 
            }
            cmd_read(arg1);
        } 
        else if (my_streq(cmd, "delete")) {
            if (arg1[0] == '\0') { 
                printf("Usage: delete <filename>\n\n"); continue; 
            }
            cmd_delete(arg1);
        } 
        else if (my_streq(cmd, "rmdir")) {
            if (arg1[0] == '\0') { 
                printf("Usage: rmdir <foldername>\n\n"); continue; 
            }
            cmd_rmdir(arg1);
        } 
        else if (my_streq(cmd, "pwd")) {
            cmd_pwd();
        } 
        else if (my_streq(cmd, "df")) {
            cmd_df();
        } 
        else {
            printf("Invalid command.\n\n");
        }
    }
    return 0;
}
