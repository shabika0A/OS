#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct picture {
    int size;
    int **pixels; // 2D array of size x size
};

void initializePicture(struct picture *p) {
    scanf("%d", &p->size);

    // Dynamically allocate memory for the 2D array
    p->pixels = (int **)malloc(p->size * sizeof(int *));
    for (int i = 0; i < p->size; i++) {
        p->pixels[i] = (int *)malloc(p->size * sizeof(int));
    }

    for (int i = 0; i < p->size; i++) {
        for (int j = 0; j < p->size; j++) {
            scanf("%d", &p->pixels[i][j]);
        }
    }
}

void displayPicture(const struct picture *p) {
    for (int i = 0; i < p->size; i++) {
        for (int j = 0; j < p->size; j++) {
            printf("%d ", p->pixels[i][j]);
        }
        printf("\n");
    }
}

void rotate90DegreesClockwise(int **matrix, int size) {
    for (int layer = 0; layer < size / 2; layer++) {
        int first = layer;
        int last = size - 1 - layer;

        for (int i = first; i < last; i++) {
            int offset = i - first;
            int top = matrix[first][i];

            // Right -> Top
            matrix[first][i] = matrix[last - offset][first];
            // Bottom -> Right
            matrix[last - offset][first] = matrix[last][last - offset];
            // Left -> Bottom
            matrix[last][last - offset] = matrix[i][last];
            // Top -> Left
            matrix[i][last] = top;
        }
    }
}

int gatRotationNumber(int **matrix , int size){
    if(matrix[0][0]==0){
        return 0;
    }else if(matrix[size-1][0]==0){
        return 1;
    }else if(matrix[size-1][size-1]==0){
        return 2;
    }else if(matrix[0][size-1]==0){
        return 3;
    }
}


int main() {
    struct picture myPicture;
    initializePicture(&myPicture);
    int rotationCount= gatRotationNumber(myPicture.pixels ,  myPicture.size);

    for (int i=0;i<rotationCount;i++){
        rotate90DegreesClockwise(myPicture.pixels,myPicture.size);
    }

    displayPicture(&myPicture);

    return 0;
}
