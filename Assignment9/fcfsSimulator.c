#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pcb {
    char name[50];
    int pid;
    int cpu;
    int ioStart;
    int ioDuration;
    int executed;
    int ioRemaining;
    int startedIo;
    int state;
    int completionTime;
    struct Pcb* next;
} Pcb;

typedef struct Node {
    Pcb* pcb;
    struct Node* next;
} Node;

Node* readyQueue = NULL;
Node* waitQueue = NULL;
Node* termQueue = NULL;

typedef struct {
    int pid;
    int time;
} KillEvent;

KillEvent killEvents[50];
int killCount = 0;

// Function to add a PCB to a queue 
void enqueue(Node** queue, Pcb* pcb) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->pcb = pcb;
    node->next = NULL;

    if (*queue == NULL) {
        *queue = node;
    } 
    else {
        Node* temp = *queue;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = node;
    }
}

// Function to remove and return a PCB from a queue 
Pcb* dequeue(Node** queue) {
    if (*queue == NULL) {
        return NULL;
    }
    Node* temp = *queue;
    Pcb* pcb = temp->pcb;
    *queue = (*queue)->next;
    free(temp);
    return pcb;
}

// Function to check if a queue is empty 
int isEmpty(Node* queue) {
    if (queue == NULL) {
        return 1;
    }
    return 0;
}

// Function to remove a PCB from ready queue by PID
void removeFromReadyQueue(int pid) {
    Node* current = readyQueue;
    Node* previous = NULL;
    while (current != NULL) {
        if (current->pcb->pid == pid) {
            if (previous == NULL) {
                readyQueue = current->next;
            } 
            else {
                previous->next = current->next;
            }
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

// Function to load all processes 
void loadProcesses(int count) {
    for (int i = 0; i < count; i++) {
        Pcb* pcb = (Pcb*)malloc(sizeof(Pcb));
        char io1[10], io2[10];
        scanf("%s %d %d %s %s", pcb->name, &pcb->pid, &pcb->cpu, io1, io2);
        if (strcmp(io1, "-") == 0 && strcmp(io2, "-") == 0) {
            pcb->ioStart = -1;
            pcb->ioDuration = 0;
        } 
        else {
            pcb->ioStart = atoi(io1);
            pcb->ioDuration = atoi(io2);
        }
        pcb->executed = 0;
        pcb->ioRemaining = pcb->ioDuration;
        pcb->startedIo = 0;
        pcb->state = 0;
        enqueue(&readyQueue, pcb);
    }
}

// Function to loads kill events 
void loadKillEvents(int count) {
    killCount = count;
    for (int i = 0; i < count; i++) {
        char temp[10];
        scanf("%s %d %d", temp, &killEvents[i].pid, &killEvents[i].time);
    }
}

// Function to return kill PID at a specific time 
int getKillPid(int time) {
    for (int i = 0; i < killCount; i++) {
        if (killEvents[i].time == time) {
            return killEvents[i].pid;
        }
    }
    return -1;
}

// Function to update the IO queue 
void updateIoQueue() {
    Node* current = waitQueue;
    while (current != NULL) {
        if (current->pcb->ioRemaining > 0) {
            current->pcb->ioRemaining--;
        }
        current = current->next;
    }
    Node* node = waitQueue;
    Node* previous = NULL;
    while (node != NULL) {
        Pcb* pcb = node->pcb;
        if (pcb->ioRemaining == 0) {
            pcb->state = 0;
            pcb->startedIo = 0;
            enqueue(&readyQueue, pcb);
            if (previous == NULL) {
                waitQueue = node->next;
                free(node);
                node = waitQueue;
            } else {
                previous->next = node->next;
                free(node);
                node = previous->next;
            }
            continue;
        }
        previous = node;
        node = node->next;
    }
}

// Function to run the FCFS simulation 
void runSimulation() {
    int time = 0;
    Pcb* running = NULL;
    while (!isEmpty(readyQueue) || !isEmpty(waitQueue) || running != NULL) {
        int killPid = getKillPid(time);
        if (killPid != -1) {
            Node* cur = readyQueue;
            while (cur != NULL) {
                if (cur->pcb->pid == killPid) {
                    cur->pcb->state = 4;
                    cur->pcb->completionTime = time;
                    enqueue(&termQueue, cur->pcb);
                    removeFromReadyQueue(killPid);
                }
                cur = cur->next;
            }
            if (running != NULL) {
                if (running->pid == killPid) {
                    running->state = 4;
                    running->completionTime = time;
                    enqueue(&termQueue, running);
                    running = NULL;
                }
            }
        }
        if (running == NULL) {
            if (!isEmpty(readyQueue)) {
                running = dequeue(&readyQueue);
                running->state = 1;
            }
        }
        updateIoQueue();
        if (running != NULL) {
            running->executed++;
            if (!running->startedIo &&
                running->ioStart != -1 &&
                running->executed == running->ioStart) {
                running->startedIo = 1;
                running->state = 2;
                running->ioRemaining = running->ioDuration;
                enqueue(&waitQueue, running);
                running = NULL;
            } 
            else {
                if (running->executed == running->cpu) {
                    running->state = 3;
                    running->completionTime = time + 1;
                    enqueue(&termQueue, running);
                    running = NULL;
                }
            }
        }

        time++;
    }
}

// Function to print final result
void printResults() {
    printf("\nPID Name CPU IO Turnaround Waiting\n");
    Node* cur = termQueue;
    while (cur != NULL) {
        Pcb* pcb = cur->pcb;
        if (pcb->state == 4) {
            printf("%d %s %d %d KILLED at %d - -\n", pcb->pid, pcb->name, pcb->cpu, pcb->ioDuration, pcb->completionTime);
        } 
        else {
            int tat = pcb->completionTime;
            int wt = tat - pcb->cpu - pcb->ioDuration;
            if (wt < 0) {
                wt = 0;
            }
            printf("%d %s %d %d %d %d\n", pcb->pid, pcb->name, pcb->cpu, pcb->ioDuration, tat, wt);
        }
        cur = cur->next;
    }
}

int main() {
    int size, killEvents;
    scanf("%d", &size);
    loadProcesses(size);
    scanf("%d", &killEvents);
    loadKillEvents(killEvents);
    runSimulation();
    printResults();
    return 0;
}
