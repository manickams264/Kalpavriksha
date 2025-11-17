#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Players_data.h"   

// Function to find the length of the string
int str_len(const char *s) {
    int i = 0;
    if (!s) {
        return 0;
    }
    while (s[i] != '\0') {
        i++;
    }
    return i;
}

// Function to compare two strings
int str_cmp(const char *a, const char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        unsigned char ca = (unsigned char)a[i];
        unsigned char cb = (unsigned char)b[i];
        if (ca != cb) {
            return (int)(ca - cb);
        }
        i++;
    }
    return (int)((unsigned char)a[i] - (unsigned char)b[i]);
}

// Function to copy a string
void str_cpy(char *dest, const char *src) {
    int i = 0;
    if (!dest) {
        return;
    }
    if (!src) { 
        dest[0] = '\0'; return; 
    }
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

// Function to copy src into dest with max length n
void str_ncpy(char *dest, const char *src, int n) {
    int i;
    if (n <= 0) {
        return;
    }
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

// Function to remove trailing newline in buffer if present 
void trim_newline(char *s) {
    int i = 0;
    if (!s) {
        return;
    }
    while (s[i] != '\0') {
        i++;
    }
    if (i == 0) {
        return;
    }
    if (s[i - 1] == '\n') {
        s[i - 1] = '\0';
    }
}

// Function to lowercase copy of string into dest 
void str_tolower_copy(char *dest, const char *src, int destSize) {
    int i = 0;
    if (!dest || !src) {
        return;
    }
    while (i < destSize - 1 && src[i] != '\0') {
        char c = src[i];
        if (c >= 'A' && c <= 'Z') {
            dest[i] = c - 'A' + 'a';
        }
        else {
            dest[i] = c;
        }
        i++;
    }
    dest[i] = '\0';
}

typedef struct PlayerNode {
    int id;
    char name[64];
    char team[64];
    char role[24];
    int runs;
    float avg;
    float sr;
    int wickets;
    float er;
    float perfIndex;
    struct PlayerNode *next;
} PlayerNode;

typedef struct {
    int teamId;
    char name[64];
    int totalPlayers;
    float avgBattingSR; 
    PlayerNode *head;
} Team;

Team Teams[10];
float calc_performance(PlayerNode *p);
void insert_player_to_team_list(Team *t, PlayerNode *p);

// Function to initialize the system
void initialize_system() {
    int i;
    for (i = 0; i < teamCount && i < 10; i++) {
        Teams[i].teamId = i + 1;
        str_ncpy(Teams[i].name, teams[i], sizeof(Teams[i].name));
        Teams[i].totalPlayers = 0;
        Teams[i].avgBattingSR = 0.0f;
        Teams[i].head = NULL;
    }
    for (i = 0; i < playerCount; i++) {
        PlayerNode *pn = (PlayerNode*)malloc(sizeof(PlayerNode));
        if (!pn) { 
            printf("Memory allocation failed\n"); exit(1); 
        }
        pn->id = players[i].id;
        str_ncpy(pn->name, players[i].name, sizeof(pn->name));
        str_ncpy(pn->team, players[i].team, sizeof(pn->team));
        str_ncpy(pn->role, players[i].role, sizeof(pn->role));
        pn->runs = players[i].totalRuns;
        pn->avg = players[i].battingAverage;
        pn->sr = players[i].strikeRate;
        pn->wickets = players[i].wickets;
        pn->er = players[i].economyRate;
        pn->perfIndex = calc_performance(pn);
        pn->next = NULL;
        int tindex = -1;
        int j;
        for (j = 0; j < teamCount && j < 10; j++) {
            if (str_cmp(Teams[j].name, pn->team) == 0) { tindex = j; break; }
        }
        if (tindex == -1) {
            free(pn);
        } else {
            insert_player_to_team_list(&Teams[tindex], pn);
        }
    }
    for (i = 0; i < teamCount && i < 10; i++) {
        PlayerNode *cur = Teams[i].head;
        int cnt = 0;
        float sumSR = 0.0f;
        while (cur) {
            if (str_cmp(cur->role, "Batsman") == 0 || str_cmp(cur->role, "All-rounder") == 0) {
                sumSR += cur->sr;
                cnt++;
            }
            cur = cur->next;
        }
        if (cnt > 0) Teams[i].avgBattingSR = sumSR / cnt;
        else Teams[i].avgBattingSR = 0.0f;
    }
}

// Function to calculate performance
float calc_performance(PlayerNode *p) {
    if (!p) {
        return 0.0f;
    }
    if (str_cmp(p->role, "Batsman") == 0) {
        return (p->avg * p->sr) / 100.0f;
    } 
    else if (str_cmp(p->role, "Bowler") == 0) {
        return (p->wickets * 2.0f) + (100.0f - p->er);
    } 
    else {
        return ((p->avg * p->sr) / 100.0f) + (p->wickets * 2.0f);
    }
}

// Function to insert a player to team list
void insert_player_to_team_list(Team *t, PlayerNode *p) {
    if (!t || !p) {
        return;
    }
    p->next = t->head;
    t->head = p;
    t->totalPlayers++;
}


// Function to Read a line into buffer using scanf 
void read_line_input(char *buf, int maxlen) {
    int c;
    while ((c = getchar()) != EOF) {
        if (c == '\n') continue;
        ungetc(c, stdin);
        break;
    }
    if (fgets(buf, maxlen, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    trim_newline(buf);
}

// Function to add a player to a team
void add_player_to_team() {
    int teamId;
    printf("Enter Team ID to add player: ");
    if (scanf("%d", &teamId) != 1) { 
        while (getchar() != '\n'); return; 
    }
    if (teamId < 1 || teamId > teamCount) {
        printf("Invalid Team ID\n");
        return;
    }
    while (getchar() != '\n');
    PlayerNode *pn = (PlayerNode*)malloc(sizeof(PlayerNode));
    if (!pn) { 
        printf("Memory error\n"); return; 
    }
    printf("Enter Player ID: ");
    scanf("%d", &pn->id); 
    while (getchar() != '\n');
    printf("Enter Player Name: ");
    read_line_input(pn->name, sizeof(pn->name));
    int roleChoice;
    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &roleChoice); 
    while (getchar() != '\n');
    if (roleChoice == 1) {
        str_cpy(pn->role, "Batsman");
    }
    else if (roleChoice == 2) {
        str_cpy(pn->role, "Bowler");
    }
    else {
        str_cpy(pn->role, "All-rounder");
    }
    str_cpy(pn->team, Teams[teamId - 1].name);
    printf("Total Runs: ");
    scanf("%d", &pn->runs); while (getchar() != '\n');
    printf("Batting Average: ");
    scanf("%f", &pn->avg); while (getchar() != '\n');
    printf("Strike Rate: ");
    scanf("%f", &pn->sr); while (getchar() != '\n');
    printf("Wickets: ");
    scanf("%d", &pn->wickets); while (getchar() != '\n');
    printf("Economy Rate: ");
    scanf("%f", &pn->er); while (getchar() != '\n');
    pn->perfIndex = calc_performance(pn);
    pn->next = NULL;
    insert_player_to_team_list(&Teams[teamId - 1], pn);
    PlayerNode *cur = Teams[teamId - 1].head;
    int cnt = 0;
    float sumSR = 0.0f;
    while (cur) {
        if (str_cmp(cur->role, "Batsman") == 0 || str_cmp(cur->role, "All-rounder") == 0) {
            sumSR += cur->sr;
            cnt++;
        }
        cur = cur->next;
    }
    if (cnt > 0) {
        Teams[teamId - 1].avgBattingSR = sumSR / cnt;
    }
    else {
        Teams[teamId - 1].avgBattingSR = 0.0f;
    }
    printf("Player added successfully to Team %s!\n", Teams[teamId - 1].name);
}

// Function to display players of the team
void display_players_of_team() {
    int teamId;
    printf("Enter Team ID: ");
    if (scanf("%d", &teamId) != 1) { 
        while (getchar() != '\n'); return; 
    }
    if (teamId < 1 || teamId > teamCount) {
        printf("Invalid Team ID\n");
        return;
    }
    while (getchar() != '\n');
    Team *t = &Teams[teamId - 1];
    printf("\nPlayers of Team %s:\n", t->name);
    printf("====================================================================================\n");
    printf("ID   Name                       Role          Runs   Avg    SR    Wkts  ER   PerfIndex\n");
    printf("====================================================================================\n");
    PlayerNode *cur = t->head;
    while (cur) {
        printf("%4d %-25s %-12s %6d %6.2f %6.2f %6d %5.2f %10.2f\n",
               cur->id, cur->name, cur->role, cur->runs, cur->avg, cur->sr,
               cur->wickets, cur->er, cur->perfIndex);
        cur = cur->next;
    }
    printf("====================================================================================\n");
    int cntPlayers = t->totalPlayers;
    int cntSR = 0;
    float sumSR = 0.0f;
    cur = t->head;
    while (cur) {
        if (str_cmp(cur->role, "Batsman") == 0 || str_cmp(cur->role, "All-rounder") == 0) {
            sumSR += cur->sr;
            cntSR++;
        }
        cur = cur->next;
    }
    float avgSR = (cntSR > 0) ? (sumSR / cntSR) : 0.0f;
    printf("Total Players: %d\n", cntPlayers);
    printf("Average Batting Strike Rate: %.2f\n", avgSR);
    t->avgBattingSR = avgSR;
}

// Function to swap teams
void swap_teams(Team *a, Team *b) {
    Team tmp = *a;
    *a = *b;
    *b = tmp;
}

// Function to display teams sorted by avgBattingSR descending 
void display_teams_by_avgSR() {
    Team arr[10];
    int i;
    for (i = 0; i < teamCount && i < 10; i++) {
        arr[i] = Teams[i];
    }
    int n = teamCount;
    for (i = 0; i < n - 1; i++) {
        int maxidx = i;
        int j;
        for (j = i + 1; j < n; j++) {
            if (arr[j].avgBattingSR > arr[maxidx].avgBattingSR) {
                maxidx = j;
            }
        }
        if (maxidx != i) {
            Team tmp = arr[i];
            arr[i] = arr[maxidx];
            arr[maxidx] = tmp;
        }
    }
    printf("Teams Sorted by Average Batting Strike Rate\n");
    printf("=========================================================\n");
    printf("ID  Team Name                 Avg Bat SR   Total Players\n");
    printf("=========================================================\n");
    for (i = 0; i < n; i++) {
        printf("%2d  %-25s %10.2f   %5d\n",
               arr[i].teamId, arr[i].name, arr[i].avgBattingSR, arr[i].totalPlayers);
    }
    printf("=========================================================\n");
}

// Function to display top K players of a team by role
void display_top_k_team_role() {
    int teamId, roleChoice, K;
    printf("Enter Team ID: ");
    if (scanf("%d", &teamId) != 1) { 
        while (getchar() != '\n'); 
        return; 
    }
    if (teamId < 1 || teamId > teamCount) { 
        printf("Invalid Team ID\n"); 
        return; 
    }
    while (getchar() != '\n');
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    if (scanf("%d", &roleChoice) != 1) { 
        while (getchar() != '\n'); 
        return; 
    }
    while (getchar() != '\n');
    printf("Enter number of players : ");
    if (scanf("%d", &K) != 1) { 
        while (getchar() != '\n'); 
        return; 
    }
    while (getchar() != '\n');
    char roleName[24];
    if (roleChoice == 1) {
        str_cpy(roleName, "Batsman");
    }
    else if (roleChoice == 2) {
        str_cpy(roleName, "Bowler");
    }
    else {
        str_cpy(roleName, "All-rounder");
    }
    PlayerNode *arr[200];
    int count = 0;
    PlayerNode *cur = Teams[teamId - 1].head;
    while (cur) {
        if (str_cmp(cur->role, roleName) == 0) {
            arr[count++] = cur;
        }
        cur = cur->next;
    }
    int i, j;
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            if (arr[j]->perfIndex < arr[j + 1]->perfIndex) {
                PlayerNode *tmp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = tmp;
            }
        }
    }
    printf("Top %d %s of Team %s:\n", K, roleName, Teams[teamId - 1].name);
    printf("====================================================================================\n");
    printf("ID   Name                       Role          Runs   Avg    SR    Wkts  ER   PerfIndex\n");
    printf("====================================================================================\n");
    for (i = 0; i < K && i < count; i++) {
        PlayerNode *p = arr[i];
        printf("%4d %-25s %-12s %6d %6.2f %6.2f %6d %5.2f %10.2f\n",
               p->id, p->name, p->role, p->runs, p->avg, p->sr, p->wickets, p->er, p->perfIndex);
    }
    printf("====================================================================================\n");
}

// Function to display all players of a role across teams 
void display_all_players_by_role() {
    int roleChoice;
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    if (scanf("%d", &roleChoice) != 1) { 
        while (getchar() != '\n'); 
        return; 
    }
    while (getchar() != '\n');
    char roleName[24];
    if (roleChoice == 1) {
        str_cpy(roleName, "Batsman");
    }
    else if (roleChoice == 2) {
        str_cpy(roleName, "Bowler");
    }
    else {
        str_cpy(roleName, "All-rounder");
    }
    PlayerNode *arr[1000];
    int count = 0;
    int i;
    for (i = 0; i < teamCount; i++) {
        PlayerNode *cur = Teams[i].head;
        while (cur) {
            if (str_cmp(cur->role, roleName) == 0) {
                arr[count++] = cur;
            }
            cur = cur->next;
        }
    }
    int a, b;
    for (a = 0; a < count - 1; a++) {
        for (b = 0; b < count - a - 1; b++) {
            if (arr[b]->perfIndex < arr[b + 1]->perfIndex) {
                PlayerNode *tmp = arr[b]; arr[b] = arr[b + 1]; arr[b + 1] = tmp;
            }
        }
    }
    printf("All %s across all teams:\n", roleName);
    printf("======================================================================================\n");
    printf("ID   Name                       Team                  Role          Runs   Avg    SR    Wkts  ER   PerfIndex\n");
    printf("======================================================================================\n");
    for (i = 0; i < count; i++) {
        PlayerNode *p = arr[i];
        printf("%4d %-25s %-20s %-12s %6d %6.2f %6.2f %6d %5.2f %10.2f\n",
               p->id, p->name, p->team, p->role, p->runs, p->avg, p->sr, p->wickets, p->er, p->perfIndex);
    }
    printf("======================================================================================\n");
}

int main() {
    initialize_system();
    while (1) {
        printf("\n===============================================================================\n");
        printf("ICC ODI PLAYER PERFORMANCE ANALYZER\n");
        printf("===============================================================================\n");
        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display All Players of a Specific Role Across All Teams\n");
        printf("6. Exit\n");
        printf("===============================================================================\n");

        printf("Enter your choice: ");
        int ch;
        if (scanf("%d", &ch) != 1) { 
            while (getchar() != '\n'); 
            continue; 
        }
        while (getchar() != '\n');
        switch (ch) {
            case 1: 
                add_player_to_team(); 
                break;
            case 2: 
                display_players_of_team(); 
                break;
            case 3: 
                display_teams_by_avgSR(); 
                break;
            case 4: 
                display_top_k_team_role(); 
                break;
            case 5: 
                display_all_players_by_role(); 
                break;
            case 6: 
                printf("Exiting...\n"); 
                return 0;
            default: 
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}

