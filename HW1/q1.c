#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct participant {
    char name[30];
    int age;
    int score;
};

struct group {
    struct participant p1;
    struct participant p2;
    double avgScore;
};

int compareParticipants(const void *a, const void *b) {
    return ((struct participant *)a)->score - ((struct participant *)b)->score;
}

int main() {
    int participantCount;
    scanf("%d", &participantCount);

    struct participant *participantArray = (struct participant *)malloc(participantCount * sizeof(struct participant));
    for (int i = 0; i < participantCount; i++) {
        scanf("%s %d %d", participantArray[i].name, &participantArray[i].age, &participantArray[i].score);
    }
    
    if (participantCount % 2 == 1) {
        int minScore = participantArray[0].score;
        int minIndex = 0;
        for (int i = 1; i < participantCount; i++) {
            if (participantArray[i].score < minScore) {
                minScore = participantArray[i].score;
                minIndex = i;
            }
        }
        // Remove the participant with the least score by shifting elements
        for (int i = minIndex; i < participantCount - 1; i++) {
            participantArray[i] = participantArray[i + 1];
        }

        participantCount--;
    }

    struct group *groupArray = (struct group *)malloc(participantCount / 2 * sizeof(struct group));
    for (int i = 0, j = 0; i < participantCount/2; i++, j++) {
        groupArray[j].p1 = participantArray[i];
        groupArray[j].p2 = participantArray[participantCount - i - 1];
        groupArray[j].avgScore = (participantArray[i].score + participantArray[participantCount - i - 1].score) / 2.0;
    }

    // Sort the groups in descending order by average score
    struct group temp;
    for (int i = 0; i < participantCount / 2 - 1; i++) {
        for (int j = i + 1; j < participantCount / 2; j++) {
            if (groupArray[i].avgScore < groupArray[j].avgScore) {
                temp = groupArray[i];
                groupArray[i] = groupArray[j];
                groupArray[j] = temp;
            }
        }
    }

    for (int i = 0; i < participantCount / 2; i++) {
        printf("%s %d %d, %s %d %d\n", groupArray[i].p1.name, groupArray[i].p1.age, groupArray[i].p1.score, groupArray[i].p2.name, groupArray[i].p2.age, groupArray[i].p2.score);
    }

    free(participantArray);
    free(groupArray);
    return 0;
}
