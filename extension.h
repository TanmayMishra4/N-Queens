#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "neillsdl2.h"

#define MAX_QUEUE_SIZE 1000000
#define MAX_BOARD_SIZE 10
#define MAX_SOLUTION_SIZE 800
#define VALID_FLAG "-verbose"
#define EMPTY '0'
#define QUEEN '1'
#define NULLCHAR '\0'
#define RECTSIZE 40
#define BLACK 0
#define WHITE 255
#define CELLSIZE 80
#define QUEENSIZE 60
#define QUEENCOLOUR_R 255
#define QUEENCOLOUR_G 255
#define QUEENCOLOUR_B 0
#define OFFSET 40
#define RES_COL_OFFSET 1

typedef struct Board {
    int arr[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    int size;
    int nextRow;
    int colMask;
    int diagonal1Mask;
    int diagonal2Mask;
} Board;

typedef struct Node {
    Board b;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* start;
    int size;
    Node* end;
} Queue;

typedef struct Result {
    int arr[MAX_SOLUTION_SIZE][MAX_BOARD_SIZE];
    int nextIndex;
} Result;
