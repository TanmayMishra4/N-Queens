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
void printUsage(void);
void printWrongArgs(void);
void printSizeOutOfBounds(void);
void printWrongFlag(void);
void checkNextBoards(Board* currentBoard, Result* result, 
                int row, int col, Set* set, Queue* queue);

void test(void);
void testValidFlag(void);
void testInitializeQueue(void);
void testInitializeBoard(void);
void testCopyBoard(void);
void testIsEqual(void);
void testFillVal(void);
void testIsQueen(void);
void testIsQueueEmpty(void);
void testAddAndRemove(void);
void testInitializeSet(void);
void testAddToSet(void);
void testIsContained(void);
void testCheckRow(void);
void testCheckCol(void);
void testCheckDiagonal1(void);
void testCheckDiagonal2(void);
void testGenerateNextBoard(void);
void testIsSolution(void);
void testIsValidPosition(void);
void testInitializeResult(void);
void testAddToResult(void);
void testFillValInResult(void);
void testSolveBoards(void);
void testCheckNextBoards(void);

int main(int argc, char** argv) {
    test();
    int boardSize = 0;
    Result result;
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

void printUsage(void){
    fprintf(stderr, "usage : ./8q [-verbose](optional) [size of board]\n");
}
void printWrongArgs(void){
    fprintf(stderr, "Wrong number of args passed\n");
    printUsage();
    exit(EXIT_FAILURE);
}
void printSizeOutOfBounds(void){
    fprintf(stderr, 
    "Invalid Board Size, Board size needs to be between 1 and 10 inclusive\n");
    printUsage();
    exit(EXIT_FAILURE);
}
void printWrongFlag(void){
    fprintf(stderr, "Wrong flag used, only -verbose allowed\n");
    printUsage();
    exit(EXIT_FAILURE);
    
}

void parseArgs(int* boardSize, int argc, char** argv) {
    char* flag = NULL;
    if (argc != 2 && argc != 3) {
        printWrongArgs();
    }
    if (argc == 2) {
        *boardSize = atoi(argv[1]);
    }
    else if (argc == 3) {
        *boardSize = atoi(argv[2]);
        flag = argv[1];

        if (!isValidFlag(flag)) {
            printWrongFlag();
        }
    }
    if (*boardSize > 10 || *boardSize <= 0) {
        printSizeOutOfBounds();
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
    if(res == NULL){
        return;
    }
    res->size = boardSize;
    res->colMask = 0;
    res->numQueens = 0;
    res->diagonal1Mask = 0;
    res->diagonal2Mask = 0;
    res->nextRow = 0;
    for (int i = 0; i < boardSize; i++) {
        res->arr[i] = EMPTY;
    }
}

void initializeSet(Set* set){
    if(set == NULL){
        return;
    }
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
    int size = set->size;
    for(int i=size-1;i>=0;i--){
        int numQueens = set->arr[i].numQueens;
        if(numQueens < board->numQueens){
            return false;
        }
        if(isEqual(&set->arr[i], board)){
            return true;
        }
    }
    return false;
}

void addToSet(Set* set, Board board){
    int size = set->size;
    set->arr[size] = board;
    set->size++;
}

void checkNextBoards(Board* currentBoard, Result* result, int row, 
                    int col, Set* set, Queue* queue){
    if(!isQueen(currentBoard, row, col)){
        if(isValidPosition(currentBoard, row, col)){
            Board next = generateNextBoard(currentBoard, row, col);
            if(!isContained(set, &next)){
                addToSet(set, next);
                if(isSolution(&next)){
                    // addToSet(&set, next);
                    addToResult(&next, result);
                }
                else{
                    addToQueue(queue, next);
                }
            }
        }
    }
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
                checkNextBoards(&currentBoard, result, row, col, &set, &queue);
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
    int diagonal1Mask = nextBoard.diagonal1Mask;
    int diagonal2Mask = nextBoard.diagonal2Mask;
    int diag1Val = rowFill - colFill + MAX_BOARD_SIZE;
    int diag2Val = rowFill + colFill;

    nextBoard.colMask = (colMask | (1 << colFill));
    nextBoard.diagonal2Mask = (diagonal2Mask | (1 << diag2Val));
    nextBoard.diagonal1Mask = (diagonal1Mask | (1 << diag1Val));
    return nextBoard;
}

void copyBoard(Board* copy, Board* original) {
    copy->size = original->size;
    copy->numQueens = original->numQueens;
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
    if(result == NULL){
        return;
    }
    result->nextIndex = 0;
    for (int row = 0; row < MAX_SOLUTION_SIZE; row++) {
        for (int col = 0; col < MAX_BOARD_SIZE; col++) {
            result->arr[row][col] = 0;
        }
    }
}

void fillValueInResult(Result* res, int it, int val) {
    int nextIndex = res->nextIndex;
    res->arr[nextIndex][it] = val;
}

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
    if(queue == NULL){
        return;
    }
    static Board arr[MAX_QUEUE_SIZE];
    queue->arr = arr;
    queue->start = 0;
    queue->end = 0;
}

bool checkColumn(Board* b, int col){
    int colMask = b->colMask;
    return ((colMask & (1 << col)) == 0);
}

bool checkRow(Board* b, int row){
    if(b->arr[row] > 0){
        return false;
    }
    return true;
}

bool checkDiagonal1(Board* b, int row, int col){
    int diag1Mask = b->diagonal1Mask;
    int diag1Val = row - col + MAX_BOARD_SIZE;
    return ((diag1Mask & (1 << diag1Val)) == 0);
}

bool checkDiagonal2(Board* b, int row, int col){
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

// TESTING FUNCTIONS

void test(void) {
    testValidFlag();
    testInitializeQueue();
    testInitializeBoard();
    testIsEqual();
    testIsQueen();
    testFillVal();
    testCopyBoard();
    testIsQueueEmpty();
    testAddAndRemove();
    testInitializeSet();
    testAddToSet();
    testIsContained();
    testGenerateNextBoard();
    testCheckRow();
    testCheckCol();
    testCheckDiagonal1();
    testCheckDiagonal2();
    testIsSolution();
    testIsValidPosition();
    testInitializeResult();
    testAddToResult();
    testFillValInResult();
    testSolveBoards();
    testCheckNextBoards();
}

void testValidFlag(void){
    assert(isValidFlag("-verbose"));
    assert(!isValidFlag("--verbose"));
    assert(!isValidFlag(" "));
    assert(!isValidFlag("6"));
}

void testInitializeQueue(void){
    Queue queue;
    initializeQueue(&queue);
    assert(queue.end == 0 && queue.start == 0);
    assert(queue.arr != NULL);

    Queue* q = NULL;
    initializeQueue(q);
    assert(q == NULL);
}

void testIsQueueEmpty(void){
    Queue queue;
    Board a;
    initializeBoard(&a, 3);
    initializeQueue(&queue);
    assert(isQueueEmpty(&queue));
    addToQueue(&queue, a);
    assert(!isQueueEmpty(&queue));
    removeFromQueue(&queue);
    assert(isQueueEmpty(&queue));
}

void testAddAndRemove(void){
    Queue q;
    initializeQueue(&q);
    Board a;
    initializeBoard(&a, 3);
    fillVal(&a, 1, 2, QUEEN);
    addToQueue(&q, a);
    Board c = removeFromQueue(&q);
    assert(isEqual(&a, &c));

    initializeBoard(&a, 4);
    fillVal(&a, 0, 2, QUEEN);
    addToQueue(&q, a);
    c = removeFromQueue(&q);
    assert(isEqual(&a, &c));
}

void testInitializeBoard(void){
    Board b;
    int size = 3;
    initializeBoard(&b, size);
    assert(b.colMask == 0 && b.numQueens == 0);
    assert(b.diagonal1Mask == 0 && b.diagonal2Mask == 0);
    assert(b.size == 3);
    for(int row=0;row<size;row++){
        for(int col=0;col<size;col++){
            assert(b.arr[row] == EMPTY);
        }
    }
    Board* a = NULL;
    initializeBoard(a, 3);
    assert(a == NULL);
}

void testFillVal(void){
    Board a;
    initializeBoard(&a, 4);
    fillVal(&a, 1, 3, QUEEN);
    assert(isQueen(&a, 1, 3));
    assert(!isQueen(&a, 3, 1));
}

void testIsEqual(void){
    Board a;
    initializeBoard(&a, 5);
    Board b;
    initializeBoard(&b, 4);
    assert(!isEqual(&a, &b));
    initializeBoard(&b, 5);
    a.arr[3] = QUEEN;
    b.arr[3] = QUEEN;
    assert(isEqual(&a, &b));
}

void testCopyBoard(void){
    Board a;
    initializeBoard(&a, 3);
    Board b;
    copyBoard(&b, &a);
    assert(isEqual(&b, &a));
    assert(b.colMask == a.colMask);
    assert(b.diagonal1Mask == a.diagonal1Mask);
    assert(b.diagonal2Mask == a.diagonal1Mask);
    assert(b.numQueens == a.numQueens);
}

void testIsQueen(void){
    Board a;
    initializeBoard(&a, 4);
    fillVal(&a, 2, 3, QUEEN);
    fillVal(&a, 0, 0, QUEEN);
    assert(isQueen(&a, 2, 3));
    assert(isQueen(&a, 0, 0));
    assert(!isQueen(&a, 3, 2));
}

void testInitializeSet(void){
    Set s;
    initializeSet(&s);
    assert(s.size == 0);
    assert(s.arr != NULL);
    Set* st = NULL;
    initializeSet(st);
    assert(st == NULL);
}

void testAddToSet(void){
    Set s;
    initializeSet(&s);
    Board a;
    initializeBoard(&a, 4);
    fillVal(&a, 1, 2, QUEEN);
    addToSet(&s, a);
    assert(s.size == 1);
    assert(isEqual(&(s.arr[s.size-1]), &a));
}

void testIsContained(void){
    Set s;
    initializeSet(&s);
    Board a;
    initializeBoard(&a, 4);
    Board b;
    copyBoard(&b, &a);
    addToSet(&s, a);
    assert(isContained(&s, &b));
    fillVal(&b, 0, 0, QUEEN);
    assert(!isContained(&s, &b));
}

void testCheckRow(void){
    Board a;
    initializeBoard(&a, 4);
    fillVal(&a, 1, 2, QUEEN);
    assert(!checkRow(&a, 1));
    assert(checkRow(&a, 0));
    fillVal(&a, 3, 0, QUEEN);
    assert(!checkRow(&a, 3));
    assert(checkRow(&a, 2));
}

void testCheckCol(void){
    Board a;
    initializeBoard(&a, 4);
    Board next = generateNextBoard(&a, 1, 2);
    assert(!checkColumn(&next, 2));
    assert(checkColumn(&next, 0));
    next = generateNextBoard(&next, 3, 0);
    assert(!checkColumn(&next, 0));
    assert(checkColumn(&next, 3));
}

void testCheckDiagonal1(void){
    Board a;
    initializeBoard(&a, 4);
    Board next = generateNextBoard(&a, 1, 2);
    assert(!checkDiagonal1(&next, 0, 1));
    assert(checkDiagonal1(&next, 0, 0));
    next = generateNextBoard(&next, 3, 1);
    assert(!checkDiagonal1(&next, 2, 0));
    assert(checkDiagonal1(&next, 0, 0));
}

void testCheckDiagonal2(void){
    Board a;
    initializeBoard(&a, 4);
    Board next = generateNextBoard(&a, 1, 2);
    assert(!checkDiagonal2(&next, 0, 3));
    assert(checkDiagonal2(&next, 0, 0));
    next = generateNextBoard(&next, 2, 0);
    assert(!checkDiagonal2(&next, 0, 2));
    assert(checkDiagonal2(&next, 1, 3));
}

void testGenerateNextBoard(void){
    Board a;
    initializeBoard(&a, 4);
    Board b = generateNextBoard(&a, 0, 0);
    assert(isQueen(&b, 0, 0));
    assert(b.numQueens == 1);
    assert(b.colMask == 1);
    assert(b.diagonal2Mask == 1);

    b = generateNextBoard(&b, 2, 2);
    assert(isQueen(&b, 2, 2));
    assert(b.numQueens == 2);
    assert(b.colMask == 5);
    assert(b.diagonal2Mask == 17);
}

void testIsValidPosition(void){
    Board a;
    initializeBoard(&a, 3);
    assert(isValidPosition(&a, 0, 0));
    assert(isValidPosition(&a, 1, 2));
    a = generateNextBoard(&a, 0, 0);
    assert(isValidPosition(&a, 1, 2));
    assert(!isValidPosition(&a,  1, 0));
    assert(!isValidPosition(&a,  2, 0));
}

void testIsSolution(void){
    Board a;
    initializeBoard(&a, 4);
    assert(!isSolution(&a));
    a = generateNextBoard(&a, 0, 1);
    assert(!isSolution(&a));
    a = generateNextBoard(&a, 1, 3);
    a = generateNextBoard(&a, 2, 0);
    assert(!isSolution(&a));
    a = generateNextBoard(&a, 3, 2);
    assert(isSolution(&a));
}

void testInitializeResult(void){
    Result res;
    initializeResult(&res);
    assert(res.nextIndex == 0);
    assert(res.arr != NULL);
    Result* r = NULL;
    initializeResult(r);
    assert(r == NULL);
}

void testAddToResult(void){
    Result r;
    initializeResult(&r);
    Board a;
    initializeBoard(&a, 3);
    a = generateNextBoard(&a, 0, 0);
    addToResult(&a, &r);
    assert(r.nextIndex == 1);
    assert(r.arr[0][0] == 1);
    a = generateNextBoard(&a, 1, 2);
    addToResult(&a, &r);
    assert(r.nextIndex == 2);
    assert(r.arr[1][1] == 3);
}

void testFillValInResult(void){
    Result r;
    initializeResult(&r);
    fillValueInResult(&r, 0, 1);
    assert(r.arr[0][0] == 1);
    fillValueInResult(&r, 1, 3);
    assert(r.arr[0][1] == 3);
}

void testSolveBoards(void){
    Result r;
    initializeResult(&r);
    solveBoards(&r, 3);
    assert(r.nextIndex == 0);
    initializeResult(&r);
    solveBoards(&r, 4);
    assert(r.nextIndex == 2);
    assert(r.arr[0][0] == 2);
    assert(r.arr[0][1] == 4);
    assert(r.arr[0][2] == 1);
    assert(r.arr[1][2] == 4);

    initializeResult(&r);
    solveBoards(&r, 5);
    assert(r.nextIndex == 10);

    initializeResult(&r);
    solveBoards(&r, 1);
    assert(r.nextIndex == 1);
}

void testCheckNextBoards(void){
    Board b;
    initializeBoard(&b, 3);
    Set s;
    initializeSet(&s);
    Result r;
    initializeResult(&r);
    Queue q;
    initializeQueue(&q);
    checkNextBoards(&b, &r, 0, 0, &s, &q);
    assert(r.nextIndex == 0);
    assert(s.size == 1);
    assert(!isQueueEmpty(&q));
    assert(q.end == 1);
    assert(q.start == 0);

    initializeBoard(&b, 4);
    initializeSet(&s);
    initializeResult(&r);
    initializeQueue(&q);
    b = generateNextBoard(&b, 0, 1);
    b = generateNextBoard(&b, 1, 3);
    b = generateNextBoard(&b, 2, 0);

    checkNextBoards(&b, &r, 3, 2, &s, &q);

    assert(r.nextIndex == 1);
    assert(s.size == 1);
    assert(isQueueEmpty(&q));
    assert(q.end == 0);
    assert(q.start == 0);
}

