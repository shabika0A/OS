#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addName(char engineers[][80], int* engineerCount, const char* name, int index) {

    for (int i = *engineerCount; i > index; i--) {
        strcpy(engineers[i], engineers[i - 1]);
    }
    strcpy(engineers[index], name);
    (*engineerCount)++;
}

void deleteName(char engineers[][80], int* engineerCount, const char* name) {

     int index = -1;
    for (int i = 0; i < *engineerCount; i++) {
        if (strcmp(name, engineers[i]) == 0) {
            index = i;
            break;
        }
    }
   
    if (index != -1) {
        for (int j = index; j < *engineerCount - 1; j++) {
            strcpy(engineers[j], engineers[j + 1]);
        }
        (*engineerCount)--;
    }
    // for (int i = 0; i < *engineerCount; i++) {
    //     if (strcmp(engineers[i], name) == 0) {
    //         for (int j = i; j < *engineerCount - 1; j++) {
    //             strcpy(engineers[j], engineers[j + 1]);
    //         }
    //         (*engineerCount)--;
    //         break;
    //     }
    // }
}

void swapNames(char engineers[][80], int engineerCount, const char* name1, const char* name2) {
    int index1 = -1;
    int index2 = -1;
    for (int i = 0; i < engineerCount; i++) {
        if (strcmp(engineers[i], name1) == 0) {
            index1 = i;
        }
        if (strcmp(engineers[i], name2) == 0) {
            index2 = i;
        }
    }
    char temp[80];
    strcpy(temp, engineers[index1]);
    strcpy(engineers[index1], engineers[index2]);
    strcpy(engineers[index2], temp);
}

int main() {
    int engineerCount;
    scanf("%d", &engineerCount);

    // Assuming each name has a maximum length of 50 characters
    char engineers[engineerCount][80]; 
    
    for (int i = 0; i < engineerCount; i++) {
        scanf("%s", engineers[i]);
    }

    int numChanges;
    scanf("%d", &numChanges);

    while (numChanges > 0) {
        char changeType[10];
        scanf("%s", changeType);
        if (strcmp(changeType, "add") == 0) {
            char name[80];
            int index;
            scanf("%s", name);
            scanf("%d", &index);
            addName(engineers, &engineerCount, name, index);
        } else if (strcmp(changeType, "delete") == 0) {
            char name[80];
            scanf("%s", name);
            deleteName(engineers, &engineerCount, name);
        } else if (strcmp(changeType, "swap") == 0) {
            char name1[80];
            char name2[80];
            scanf("%s %s", name1, name2);
            swapNames(engineers, engineerCount, name1, name2);
        } else {
            printf("Invalid change type: %s\n", changeType);
        }

        numChanges--;
    }
     for (int i = 0; i < engineerCount; i++) {
        printf("%s ", engineers[i]);
    }

    return 0;
}

