#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define MAX_QUEUE_SIZE 10000003
#define MAX_BOARD_SIZE 10
#define MAX_SOLUTION_SIZE 800
#define VALID_FLAG "-verbose"
#define EMPTY 0
#define QUEEN 1
#define BIG_PRIME 1000003
#define NULLCHAR '\0'

typedef struct Board {
    int arr[MAX_BOARD_SIZE];
    int size;
    int numQueens;
    int nextRow;
    int rowMask;
    int colMask;
    int diagonal1Mask;
    int diagonal2Mask;
} Board;

typedef struct Queue {
    Board* arr;
    int start;
    int end;
} Queue;

typedef struct Set{
    int size;
    Board* arr;
} Set;

typedef struct Result {
    int arr[MAX_SOLUTION_SIZE][MAX_BOARD_SIZE];
    int nextIndex;
} Result;
