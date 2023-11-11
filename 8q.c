#include "8q.h"

void addToQueue(Queue* queue, Board board);
Board removeFromQueue(Queue* queue);
bool isQueueEmpty(Queue* queue);
bool isValidFlag(char* flag);
void parseArgs(int* boardSize, int argc, char** argv);
void solveBoards(Result* result, int boardSize);
void initializeBoard(Board* res, int boardSize);
void addToResult(Board* currentBoard, Result* result);
Board generateNextBoard(Board* board, int row, int col);
void initializeResult(Result* result);
void fillValueInResult(Result* res, int it, int val);
bool isValidPosition(Board* currentBoard, int row, int col);
void copyBoard(Board* copy, Board* original);
void displayResult(Result* result, int boardSize);
void initializeQueue(Queue* queue);
bool isQueen(Board* board, int row, int col);
bool checkColumn(Board* b, int col);
bool checkRow(Board* b, int row);
void fillVal(Board* board, int row, int col, int type);
bool checkDiagonal1(Board* b, int row, int col);
bool checkDiagonal2(Board* b, int row, int col);
void initializeSet(Set* set);
void addToSet(Set* set, Board board);
bool isContained(Set* set, Board* board);
bool isEqual(Board* first, Board* second);
bool isSolution(Board* board);
int generateHash(Board* board);

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
    
    if(argc == 3) {
        displayResult(&result, boardSize);
    }
    printf("%i solutions\n", result.nextIndex);
    return 0;
}

bool isValidFlag(char* flag) {
    return (strcmp(flag, VALID_FLAG) == 0);
}

int generateHash(Board* board){
    int primes[MAX_BOARD_SIZE] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    long hash = 0;
    for(int row=0;row<board->size;row++){
        hash += (long)primes[row] * (long)board->arr[row];
    }
    return hash%BIG_PRIME;
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
        exit(EXIT_FAILURE);
    }
}

bool isQueueEmpty(Queue* queue) {
    return queue->end == queue->start;
}

void addToQueue(Queue* queue, Board board) {
    int end = queue->end;
    queue->arr[end] = board;
    queue->end++;
}

Board removeFromQueue(Queue* queue) {
    int start = queue->start;
    Board next = queue->arr[start];
    queue->start++;
    return next;
}

void initializeBoard(Board* res, int boardSize) {
    res->size = boardSize;
    res->colMask = 0;
    res->rowMask = 0;
    res->numQueens = 0;
    res->diagonal1Mask = 0;
    res->diagonal2Mask = 0;
    res->nextRow = 0;
    for (int i = 0; i < boardSize; i++) {
        res->arr[i] = EMPTY;
    }
}

void initializeSet(Set* set){
    static Board arr[MAX_QUEUE_SIZE];
    set->size = 0;
    set->arr = arr;
}

bool isEqual(Board* first, Board* second){
    int size = first->size;
    int size2 = second->size;
    if(size != size2) return false;

    for(int row=0;row<size;row++){
        if(first->arr[row] != second->arr[row]){
            return false;
        }
    }
    return true;
}

bool isSolution(Board* board){
    if(board->numQueens == board->size){
        return true;
    }
    return false;
}

bool isContained(Set* set, Board* board){
    // printf("isContained\n");
    // int hash = generateHash(board);
    // if(isEqual(&(set->arr[hash]), board)){
    //     return true;
    // }
    // return false;
    int size = set->size;
    for(int i=size-1;i>=0;i--){
        if(isEqual(&set->arr[i], board)){
            return true;
        }
    }
    return false;
}

void addToSet(Set* set, Board board){
    // if(isContained(set, &board)){
    //     return;
    // }
    // printf("addToSet\n");
    // int hash = generateHash(&board);
    int size = set->size;
    set->arr[size] = board;
    set->size++;
    // set->arr[hash] = board;
    // set->size++;
}

void solveBoards(Result* result, int boardSize) {
    Queue queue;
    initializeQueue(&queue);
    Board board;
    initializeBoard(&board, boardSize);
    Set set;
    initializeSet(&set);

    addToQueue(&queue, board);
    addToSet(&set, board);
    while (!isQueueEmpty(&queue)) {
        Board currentBoard = removeFromQueue(&queue);

        for(int row=0;row<boardSize;row++){
            for(int col=0;col<boardSize;col++){
                if(!isQueen(&currentBoard, row, col)){
                    if(isValidPosition(&currentBoard, row, col)){
                        Board next = generateNextBoard(&currentBoard, row, col);
                        if(!isContained(&set, &next)){
                            addToSet(&set, next);
                            if(isSolution(&next)){
                                addToSet(&set, next);
                                addToResult(&next, result);
                            }
                            else{
                                addToQueue(&queue, next);
                            }
                        }
                    }
                }
            }
        }
    }
}

void fillVal(Board* board, int row, int col, int type){
    if(type == QUEEN){
        board->arr[row] = (board->arr[row] | (1 << col));
    }
    else if(type == EMPTY){
        board->arr[row] = (board->arr[row] ^ (1 << col));
    }
}

Board generateNextBoard(Board* board, int rowFill, int colFill) {
    Board nextBoard;
    copyBoard(&nextBoard, board);
    nextBoard.numQueens = board->numQueens + 1;
    fillVal(&nextBoard, rowFill, colFill, QUEEN);

    int colMask = nextBoard.colMask;
    int rowMask = nextBoard.rowMask;
    int diagonal1Mask = nextBoard.diagonal1Mask;
    int diagonal2Mask = nextBoard.diagonal2Mask;
    int diag1Val = rowFill - colFill + MAX_BOARD_SIZE;
    int diag2Val = rowFill + colFill;

    nextBoard.colMask = (colMask | (1 << colFill));
    nextBoard.rowMask = (rowMask | (1 << rowFill));
    nextBoard.diagonal2Mask = (diagonal2Mask | (1 << diag2Val));
    nextBoard.diagonal1Mask = (diagonal1Mask | (1 << diag1Val));
    return nextBoard;
}

void copyBoard(Board* copy, Board* original) {
    copy->size = original->size;
    copy->colMask = original->colMask;
    copy->diagonal1Mask = original->diagonal1Mask;
    copy->diagonal2Mask = original->diagonal2Mask;
    for (int row = 0; row < copy->size; row++) {
        copy->arr[row] = original->arr[row];
    }
}

void displayResult(Result* result, int boardSize) {
    int size = result->nextIndex;
    for (int i = 0; i < size; i++) {
        for(int it=0;it<boardSize;it++){
            printf("%i", result->arr[i][it]);
        }
        printf("\n");
    }
}

void addToResult(Board* currentBoard, Result* result) {
    int size = currentBoard->size;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (isQueen(currentBoard, row, col)) {
                fillValueInResult(result, row, col + 1);
            }
        }
    }
    result->nextIndex++;
}

void initializeResult(Result* result) {
    result->nextIndex = 0;
    for (int row = 0; row < MAX_SOLUTION_SIZE; row++) {
        for (int col = 0; col < MAX_BOARD_SIZE + 4; col++) {
            result->arr[row][col] = 0;
        }
    }
}

void fillValueInResult(Result* res, int it, int val) {
    int nextIndex = res->nextIndex;
    res->arr[nextIndex][it] = val;
}

// optimize to use bitmasks
bool isValidPosition(Board* currentBoard, int row, int col) {
    bool isValidRow = checkRow(currentBoard, row);
    if(!isValidRow){
        return false;
    }
    bool isValidCol = checkColumn(currentBoard, col);
    if(!isValidCol){
        return false;
    }
    bool isValidDiag1 = checkDiagonal1(currentBoard, row, col);
    if(!isValidDiag1){
        return false;
    }
    bool isValidDiag2 = checkDiagonal2(currentBoard, row, col);
    if(!isValidDiag2){
        return false;
    }
    return true;
}

void initializeQueue(Queue* queue) {
    static Board arr[MAX_QUEUE_SIZE];
    queue->arr = arr;
    queue->start = 0;
    queue->end = 0;
}

bool checkColumn(Board* b, int col){
    // for(int row=0;row<b->size;row++){
    //     if(isQueen(b, row, col)){
    //         return false;
    //     }
    // }
    // return true;
    int colMask = b->colMask;
    return ((colMask & (1 << col)) == 0);
}

bool checkRow(Board* b, int row){
    for(int col=0;col<b->size;col++){
        if(isQueen(b, row, col)){
            return false;
        }
    }
    return true;
    // int rowMask = b->rowMask;
    // return ((rowMask & (1 << row)) == 0);
}

bool checkDiagonal1(Board* b, int row, int col){
    // int size = b->size;
    // int r = row, c = col;
    // while(r < size && c < size){
    //     if(isQueen(b, r, c)){
    //         return false;
    //     }
    //     r++;
    //     c++;
    // }
    // r = row-1;
    // c = col-1;
    // while(r >= 0 && c >= 0){
    //     if(isQueen(b, r, c)){
    //         return false;
    //     }
    //     r--;
    //     c--;
    // }
    // return true;
    int diag1Mask = b->diagonal1Mask;
    int diag1Val = row - col + MAX_BOARD_SIZE;
    return ((diag1Mask & (1 << diag1Val)) == 0);
}

bool checkDiagonal2(Board* b, int row, int col){
    // int size = b->size;
    // int r = row, c = col;
    // while(r >= 0 && c < size){
    //     if(isQueen(b, r, c)){
    //         return false;
    //     }
    //     r--;
    //     c++;
    // }
    // r = row+1;
    // c = col-1;
    // while(r < size && c >= 0){
    //     if(isQueen(b, r, c)){
    //         return false;
    //     }
    //     r++;
    //     c--;
    // }
    // return true;
    int diag2Mask = b->diagonal2Mask;
    int diag2Val = row + col;
    return ((diag2Mask & (1 << diag2Val)) == 0);
}

bool isQueen(Board* board, int row, int col){
    if((board->arr[row] & (1 << col)) > 0){
        return true;
    }
    return false;
}


void test(void) {
    assert(sizeof(int) == 4);
    // Queue q;
    // initializeQueue(&q);
    // assert(q.start == 0 && q.end == 0);
    Board b;
    initializeBoard(&b, 8);
    Board next = generateNextBoard(&b, 0, 0);
    assert(isValidPosition(&next, 3, 2));
    assert(!isValidPosition(&next, 0, 1));
    assert(b.rowMask == 0);
    assert(next.rowMask ==  1);
    assert(!checkRow(&next, 0));
    assert(next.numQueens == 1);
    // assert(b.size == 8 && b.nextRow == 0 && b.colMask == 0 && b.diagonal1Mask == 0 && b.diagonal2Mask == 0);
    // addToQueue(&q, &b);
    // assert(!isQueueEmpty(&q));
    // // printf("%i %i\n", q.start, q.end);
    // assert(q.end == 1 && q.start == 0);
    // Board* r = removeFromQueue(&q);
    // assert(q.start == 1 && q.end == 1);
    // assert(isQueueEmpty(&q));
    // // assert(r != NULL);
    // Result res;
    // initializeResult(&res);
    // b.arr[2][0] = QUEEN;
    // Board next = generateNextBoard(&b, 0, 0);
    // printf("printing test board\n");
    // printBoard(&next);
    // printf("done printing test board\n");

    // assert(next.arr[0][0] == QUEEN && next.arr[2][0] == QUEEN);
    // addToResult(&b, &res);
    // assert(res.nextIndex == 1);
    // printf("here\n");
}

void printBoard(Board* board){
    for(int i=0;i<board->size;i++){
        for(int j=0;j<board->size;j++){
            if(isQueen(board, i, j)){
                printf("Q ");
            }
            else{
                printf(". ");
            }
        }
        printf("\n");
    }
}
void print2Board(Board* a, Board* b){
    printf("first board and next board\n");
    for(int i=0;i<a->size;i++){
        for(int j=0;j<a->size;j++){
            if(isQueen(a, i, j)){
                printf("Q ");
            }
            else{
                printf(". ");
            }
        }
        printf("\t");
        for(int j=0;j<b->size;j++){
            if(isQueen(b, i, j)){
                printf("Q ");
            }
            else{
                printf(". ");
            }
        }
        printf("\n");
    }
}
