#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define MAX_QUEUE_SIZE 1000000
#define MAX_BOARD_SIZE 10
#define MAX_SOLUTION_SIZE 800
#define VALID_FLAG "-verbose"
#define EMPTY '.'
#define QUEEN 'Q'
#define NULLCHAR '\0'

typedef struct Board {
    char arr[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    int size;
    int nextRow;
    int colMask;
    int diagonal1Mask;
    int diagonal2Mask;
} Board;

typedef struct Queue {
    Board* arr[MAX_QUEUE_SIZE];
    int start;
    int end;
} Queue;

typedef struct Result {
    char arr[MAX_SOLUTION_SIZE][MAX_BOARD_SIZE + 4];
    int nextIndex;
} Result;
