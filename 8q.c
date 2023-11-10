#include "8q.h"

bool isQueueFull(Queue* queue);
void addToQueue(Queue* queue, Board* board);
Board* removeFromQueue(Queue* queue);
bool isValidFlag(char* flag);
void parseArgs(int* boardSize, int argc, char** argv);
void solveBoards(Result* result, int boardSize);
void initializeBoard(Board* res, int boardSize);
bool isQueueEmpty(Queue* queue);
void addToResult(Board* currentBoard, Result* result);
void generateNextBoard(Board* nextBoard, Board* board, int row, int col);
void initializeResult(Result* result);
void fillValueInResult(Result* res, int* it, int val);
bool isSolutionBoard(Board* board);
bool isValidPosition(Board* currentBoard, int row, int col);
void copyBoard(Board* copy, Board* original);
void displayResult(Result* result);
void initializeQueue(Queue* queue);
bool checkColumn(Board* b, int col);
bool checkDiagonal1(Board* b, int row, int col);
bool checkDiagonal2(Board* b, int row, int col);
void test(void);
void printBoard(Board* board);
void print2Board(Board* a, Board* b);

int main(int argc, char** argv) {
    int boardSize = 0;
    Result result;
    test();
    initializeResult(&result);
    parseArgs(&boardSize, argc, argv);

    solveBoards(&result, boardSize);
    if (argc == 2) {
        printf("%i solutions\n", result.nextIndex);
    }
    else {
        displayResult(&result);
    }
    return 0;
}

bool isValidFlag(char* flag) {
    return (strcmp(flag, VALID_FLAG) == 0);
}

void parseArgs(int* boardSize, int argc, char** argv) {
    char* flag = NULL;
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Wrong number of args passed, usage : ./8q [-verbose](optional) [size of board]\n");
        exit(EXIT_FAILURE);
    }
    if (argc == 2) {
        *boardSize = atoi(argv[1]);
    }
    else if (argc == 3) {
        *boardSize = atoi(argv[2]);
        flag = argv[1];

        if (!isValidFlag(flag)) {
            fprintf(stderr, "Wrong flag used, only -verbose allowed\n");
            exit(EXIT_FAILURE);
        }
    }
    if (*boardSize > 10 || *boardSize <= 0) {
        fprintf(stderr, "Invalid Board Size, Board size needs to be between 1 and 10 inclusive\n");
    }
}

bool isQueueFull(Queue* queue) {
    int end = queue->end;
    if (end == MAX_QUEUE_SIZE) {
        return true;
    }
    else {
        return false;
    }
}

bool isQueueEmpty(Queue* queue) {
    return queue->end == queue->start;
}

void addToQueue(Queue* queue, Board* board) {
    int end = queue->end;
    // copyBoard(queue->arr[end-1], board);
    queue->arr[end] = board;
    queue->end++;
}

Board* removeFromQueue(Queue* queue) {
    int start = queue->start;
    Board* res = queue->arr[start];
    queue->start++;
    return res;
}

void initializeBoard(Board* res, int boardSize) {
    res->size = boardSize;
    res->colMask = 0;
    res->diagonal1Mask = 0;
    res->diagonal2Mask = 0;
    res->nextRow = 0;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            res->arr[i][j] = EMPTY;
        }
    }
}

void solveBoards(Result* result, int boardSize) {
    Queue queue;
    initializeQueue(&queue);
    Board board;
    initializeBoard(&board, boardSize);

    addToQueue(&queue, &board);
    Board nextBoard;
    while (!isQueueEmpty(&queue)) {
        Board* currentBoard = removeFromQueue(&queue);
        printf("value of start, end after removing = %i, %i\n", queue.start, queue.end);
        // printBoard(currentBoard);
        
        int nextRow = currentBoard->nextRow;
        if (nextRow < boardSize) {
            for (int col = 0; col < boardSize; col++) {
                if (isValidPosition(currentBoard, nextRow, col)) {
                    // Board* nextBoard;
                    generateNextBoard(&nextBoard, currentBoard, nextRow, col);
                    printf("row = %i col = %i\n", nextRow, col);
                    print2Board(currentBoard, &nextBoard);
                    addToQueue(&queue, &nextBoard);
                }
            }
        }
        // else if (isSolutionBoard(currentBoard)) {
        else{
            addToResult(currentBoard, result);
        }
    }
}

void generateNextBoard(Board* nextBoard, Board* board, int rowFill, int colFill) {
    copyBoard(nextBoard, board);
    nextBoard->nextRow = rowFill + 1;
    nextBoard->arr[rowFill][colFill] = QUEEN;
    // int colMask = nextBoard->colMask;
    // int diagonal1Mask = nextBoard->diagonal1Mask;
    // int diagonal2Mask = nextBoard->diagonal2Mask;
    // int diag1Val = rowFill - colFill + MAX_BOARD_SIZE;
    // int diag2Val = rowFill + colFill;

    // nextBoard->colMask = (colMask | (1 << colFill));
    // nextBoard->diagonal2Mask = (diagonal2Mask | (1 << diag2Val));
    // nextBoard->diagonal1Mask = (diagonal1Mask | (1 << diag1Val));
}

void copyBoard(Board* copy, Board* original) {
    copy->size = original->size;
    for (int row = 0; row < copy->size; row++) {
        for (int col = 0; col < copy->size; col++) {
            copy->arr[row][col] = original->arr[row][col];
        }
    }
}

void displayResult(Result* result) {
    int size = result->nextIndex;
    for (int i = 0; i < size; i++) {
        puts(result->arr[i]);
    }
}

void addToResult(Board* currentBoard, Result* result) {
    int size = currentBoard->size;
    int it = 0;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (currentBoard->arr[row][col] == QUEEN) {
                fillValueInResult(result, &it, col + 1);
                // result->arr[nextIndex][it] = convertToChar(col + 1);
            }
        }
    }
    result->nextIndex++;
}

void initializeResult(Result* result) {
    result->nextIndex = 0;
    for (int row = 0; row < MAX_SOLUTION_SIZE; row++) {
        for (int col = 0; col < MAX_BOARD_SIZE + 4; col++) {
            result->arr[row][col] = NULLCHAR;
        }
    }
}

void fillValueInResult(Result* res, int* it, int val) {
    int nextIndex = res->nextIndex;
    if (val <= 9) {
        res->arr[nextIndex][*it++] = '0' + val;
    }
    else if (val == 10) {
        res->arr[nextIndex][*it++] = '0' + 1;
        res->arr[nextIndex][*it++] = '0' + 0;
    }
}

bool isSolutionBoard(Board* board) {
    int size = board->size;
    int numberQueens = 0;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board->arr[row][col] == QUEEN) {
                numberQueens++;
            }
        }
    }
    return numberQueens == size;
}
// optimize to use bitmasks
bool isValidPosition(Board* currentBoard, int row, int col) {
//     int colMask = currentBoard->colMask;
//     int diagonal1Mask = currentBoard->diagonal1Mask;
//     int diagonal2Mask = currentBoard->diagonal2Mask;

//     int diag1Val = row + col;
//     int diag2Val = row - col + MAX_BOARD_SIZE;
//     // checking if col already has queen
//     if ((colMask & (1 << col)) > 0) {
//         return false;
//     }
//     if ((diagonal1Mask & (1 << diag1Val)) > 0) {
//         return false;
//     }
//     if ((diagonal2Mask & (1 << diag2Val)) > 0) {
//         return false;
//     }
//     return true;
    bool columns = checkColumn(currentBoard, col);
    bool diagonal1 = checkDiagonal1(currentBoard, row, col);
    bool diagonal2 = checkDiagonal2(currentBoard, row, col);
    if(columns && diagonal1 && diagonal2){
        return true;
    }
    return false;
}

void initializeQueue(Queue* queue) {
    queue->start = 0;
    queue->end = 0;
}

bool checkColumn(Board* b, int col){
    int res = 0;
    for(int row=0;row<b->size;row++){
        if(res > 0){
            return false;
        }
        if(b->arr[row][col] == QUEEN){
            res++;
        }
    }
    return res == 0;
}

bool checkDiagonal1(Board* b, int row, int col){
    int size = b->size;
    int res = 0;
    int r = row, c = col;
    while(r < size && c < size){
        if(res > 0) return false;
        if(b->arr[r][c] == QUEEN){
            res++;
        }
        r++;
        c++;
    }
    r = row-1;
    c = col-1;
    while(r >= 0 && c >= 0){
        if(res > 0) return false;
        if(b->arr[r][c] == QUEEN){
            res++;
        }
        r--;
        c--;
    }
    return res == 0;
}

bool checkDiagonal2(Board* b, int row, int col){
    int size = b->size;
    int res = 0;
    int r = row, c = col;
    while(r >= 0 && c < size){
        if(res > 0) return false;
        if(b->arr[r][c] == QUEEN){
            res++;
        }
        r--;
        c++;
    }
    r = row+1;
    c = col-1;
    while(r < size && c >= 0){
        if(res > 0) return false;
        if(b->arr[r][c] == QUEEN){
            res++;
        }
        r++;
        c--;
    }
    return res == 0;
}


void test() {
    Queue q;
    initializeQueue(&q);
    assert(q.start == 0 && q.end == 0);
    Board b;
    initializeBoard(&b, 8);
    assert(b.size == 8 && b.nextRow == 0 && b.colMask == 0 && b.diagonal1Mask == 0 && b.diagonal2Mask == 0);
    addToQueue(&q, &b);
    assert(!isQueueEmpty(&q));
    // printf("%i %i\n", q.start, q.end);
    assert(q.end == 1 && q.start == 0);
    Board* r = removeFromQueue(&q);
    assert(q.start == 1 && q.end == 1);
    assert(isQueueEmpty(&q));
    assert(r != NULL);
    Result res;
    initializeResult(&res);
    b.arr[2][0] = QUEEN;
    Board next;
    generateNextBoard(&next, &b, 0, 0);
    printf("printing test board\n");
    printBoard(&next);
    printf("done printing test board\n");

    assert(next.arr[0][0] == QUEEN && next.arr[2][0] == QUEEN);
    addToResult(&b, &res);
    assert(res.nextIndex == 1);
    // printf("here\n");
}

void printBoard(Board* board){
    for(int i=0;i<board->size;i++){
        for(int j=0;j<board->size;j++){
            printf("%c ", board->arr[i][j]);
        }
        printf("\n");
    }
}
void print2Board(Board* a, Board* b){
    printf("first board and next board\n");
    for(int i=0;i<a->size;i++){
        for(int j=0;j<a->size;j++){
            printf("%c ", a->arr[i][j]);
        }
        printf("\t");
        for(int j=0;j<b->size;j++){
            printf("%c ", b->arr[i][j]);
        }
        printf("\n");
    }
}
