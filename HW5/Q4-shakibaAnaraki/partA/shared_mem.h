#pragma once

#define IN_LEN 10
#define C1 10
#define C2 5
#define C3 2
#define FINISH -999
#define PROC_NO 4
#define SIZE 1024
#define NAME "my_shr_mem"
#define SEM_NAME "my_sem1"
#define empty_name "empty"
#define full_name "full"

#define MAX_STACK_SIZE 5 

typedef struct {
    int data[MAX_STACK_SIZE];
    int top;
} SharedStack;
