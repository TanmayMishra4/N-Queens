#include "extension.h"

void addToQueue(Queue* queue, Board board);
void removeFromQueue(Queue* queue, Board* board);
bool isValidFlag(char* flag);
void parseArgs(int* boardSize, int argc, char** argv);
void solveBoards(Result* result, int boardSize);
void initializeBoard(Board* res, int boardSize);
bool isQueueEmpty(Queue* queue);
void addToResult(Board* currentBoard, Result* result);
Board generateNextBoard(Board* board, int row, int col);
void initializeResult(Result* result);
void fillValueInResult(Result* res, int it, int val);
bool isValidPosition(Board* currentBoard, int row, int col);
void copyBoard(Board* copy, Board* original);
void displayResult(Result* result, int boardSize);
void displayUsingSDL(Result* result, int boardSize);
Queue* initializeQueue(void);
bool checkColumn(Board* b, int col);
bool checkDiagonal1(Board* b, int row, int col);
bool checkDiagonal2(Board* b, int row, int col);
void freeQueue(Queue* queue);
void printUsage(void);
void printWrongArgs(void);
void printSizeOutOfBounds(void);
void printWrongFlag(void);
void drawChessBoard(SDL_Simplewin* sw, int boardSize);
void drawQueens(SDL_Simplewin* sw, Result* result, int boardSize, int ind);

void test(void);
void testValidFlag(void);
void testInitializeQueue(void);
void testInitializeBoard(void);
void testIsQueueEmpty(void);
void testAddAndRemove(void);
void testCheckCol(void);
void testCheckDiagonal1(void);
void testCheckDiagonal2(void);
void testIsValidPosition(void);
void testGenerateNextBoard(void);
void testCopyBoard(void);
void testInitializeResult(void);
void testAddToResult(void);
void testSolveBoards(void);
void testFillValInResult(void);

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

Queue* initializeQueue(void) {
    Queue* queue = (Queue*)calloc(1, sizeof(Queue));
    queue->start = 0;
    queue->end = 0;
    queue->size = 0;
    return queue;
}

void freeQueue(Queue* queue){
    if(queue != NULL){
        Node* cur = queue->start;
        while(cur != NULL){
            Node* next = cur->next;
            free(cur);
            cur = next;
        }
        free(queue);
    }
}

void freeQueue(Queue* queue){
    if(queue != NULL){
        Node* cur = queue->start;
        while(cur != NULL){
            Node* next = cur->next;
            free(cur);
            cur = next;
        }
        free(queue);
    }
}

bool isQueueEmpty(Queue* queue) {
    return (queue->size == 0);
}

void addToQueue(Queue* queue, Board board) {
    Node* temp;
    temp = (Node*)calloc(1, sizeof(Node));
    temp->b = board;
    temp->next = NULL;
    if(queue->size == 0){
        queue->end = temp;
        queue->start = temp;
    }
    else{
        queue->end->next = temp;
        queue->end = temp;
    }
    queue->size = queue->size+1;
}

void removeFromQueue(Queue* queue, Board* board) {
    Node* f = queue->start->next;
    *board = queue->start->b;
    free(queue->start);
    queue->start = f;
    queue->size = queue->size-1;
}

void initializeBoard(Board* res, int boardSize) {
    res->size = boardSize;
    res->colMask = 0;
    res->diagonal1Mask = 0;
    res->diagonal2Mask = 0;
    res->nextRow = 0;
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            res->arr[row][col] = EMPTY;
        }
    }
}

void solveBoards(Result* result, int boardSize) {
    Queue* queue = initializeQueue();
    Board board;
    initializeBoard(&board, boardSize);

    addToQueue(queue, board);
    while (!isQueueEmpty(queue)) {
        Board currentBoard;
        removeFromQueue(queue, &currentBoard);

        int nextRow = currentBoard.nextRow;
        if (nextRow < boardSize) {
            for (int col = 0; col < boardSize; col++) {
                if (isValidPosition(&currentBoard, nextRow, col)) {
                    Board nextBoard = generateNextBoard(&currentBoard, nextRow, col);
                    addToQueue(queue, nextBoard);
                }
            }
        }
        else{
            addToResult(&currentBoard, result);
        }
    }
    freeQueue(queue);
}

Board generateNextBoard(Board* board, int rowFill, int colFill) {
    Board nextBoard;
    copyBoard(&nextBoard, board);
    nextBoard.nextRow = rowFill + 1;
    nextBoard.arr[rowFill][colFill] = QUEEN;
    // return nextBoard;
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
    copy->colMask = original->colMask;
    copy->diagonal1Mask = original->diagonal1Mask;
    copy->diagonal2Mask = original->diagonal2Mask;
    for (int row = 0; row < copy->size; row++) {
        for (int col = 0; col < copy->size; col++) {
            copy->arr[row][col] = original->arr[row][col];
        }
    }
}

void displayResult(Result* result, int boardSize) {
    int size = result->nextIndex;
    for (int row = 0; row < size; row++) {
        for(int col=0;col<boardSize;col++){
            printf("%i", result->arr[row][col]);
        }
        printf("\n");
    }
    displayUsingSDL(result, boardSize);
}

void addToResult(Board* currentBoard, Result* result) {
    int size = currentBoard->size;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (currentBoard->arr[row][col] == QUEEN) {
                fillValueInResult(result, row, col + 1);
            }
        }
        }
    result->nextIndex++;
}
void initializeResult(Result* result){
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
    bool columns = checkColumn(currentBoard, col);
    bool diagonal1 = checkDiagonal1(currentBoard, row, col);
    bool diagonal2 = checkDiagonal2(currentBoard, row, col);
    if(columns && diagonal1 && diagonal2){
        return true;
    }
    return false;
}

bool checkColumn(Board* b, int col){
    int colMask = b->colMask;
    return ((colMask & (1 << col)) == 0);
}

bool checkDiagonal1(Board* b, int row, int col){
    int diagonal1Mask = b->diagonal1Mask;
    int diag1Val = row - col + MAX_BOARD_SIZE;
    if ((diagonal1Mask & (1 << diag1Val)) > 0) {
        return false;
    }
    return true;
}

bool checkDiagonal2(Board* b, int row, int col){
    int diag2Val = row + col;
    int diagonal2Mask = b->diagonal2Mask;
    if ((diagonal2Mask & (1 << diag2Val)) > 0) {
        return false;
    }
    return true;
}

void drawQueens(SDL_Simplewin* sw, Result* result, int boardSize, int ind){
    SDL_Rect rectangle;
    Neill_SDL_SetDrawColour(sw, QUEENCOLOUR_R%SDL_8BITCOLOUR,
                                QUEENCOLOUR_G%SDL_8BITCOLOUR,
                                QUEENCOLOUR_B%SDL_8BITCOLOUR);

    for(int row=0;row<boardSize;row++){
        int col = result->arr[ind][row] - RES_COL_OFFSET;
        rectangle.w = QUEENSIZE;
        rectangle.h = QUEENSIZE;
        int x = OFFSET + (col*CELLSIZE) + (CELLSIZE - QUEENSIZE)/2;
        int y = OFFSET + (row*CELLSIZE) + (CELLSIZE - QUEENSIZE)/2;
        rectangle.y = y%(WWIDTH-RECTSIZE);
        rectangle.x = x%(WHEIGHT-RECTSIZE);
        printf("x = %i, y = %i\n", rectangle.x, rectangle.y);
        SDL_RenderFillRect(sw->renderer, &rectangle);
        SDL_RenderDrawRect(sw->renderer, &rectangle);
    }

    Neill_SDL_UpdateScreen(sw);
    Neill_SDL_Events(sw);
    SDL_Delay(1000);
    if(sw->finished){
        SDL_Quit();
        atexit(SDL_Quit);
    }
}

void drawChessBoard(SDL_Simplewin* sw, int boardSize){
    SDL_Rect rectangle;
    int colour = BLACK;
    int cellNumber = 0;
    for(int row=0;row<boardSize;row++){
        for(int col=0;col<boardSize;col++){
            if((row+col)%2 == 0){
                colour = BLACK;
            }
            else{
                colour = WHITE;
            }
            Neill_SDL_SetDrawColour(sw, colour%SDL_8BITCOLOUR,
                                    colour%SDL_8BITCOLOUR,
                                    colour%SDL_8BITCOLOUR);
            SDL_Rect rectangle;
            rectangle.w = CELLSIZE;
            rectangle.h = CELLSIZE;
            rectangle.x = (OFFSET + row*CELLSIZE)%(WWIDTH-CELLSIZE);
            rectangle.y = (OFFSET + col*CELLSIZE)%(WHEIGHT-CELLSIZE);
            SDL_RenderFillRect(sw->renderer, &rectangle);
            SDL_RenderDrawRect(sw->renderer, &rectangle);
        }
    }
    Neill_SDL_UpdateScreen(sw);
    Neill_SDL_Events(sw);
    if(sw->finished){
        SDL_Quit();
        atexit(SDL_Quit);
    }
}

void displayUsingSDL(Result* result, int boardSize){
    SDL_Simplewin sw;
    SDL_Rect rectangle;
    rectangle.w = RECTSIZE;
    rectangle.h = RECTSIZE;
    Neill_SDL_Init(&sw);
    // do{
    for(int ind=0;ind<result->nextIndex;ind++){
        drawChessBoard(&sw, boardSize);

        drawQueens(&sw, result, boardSize, ind);
    }

    Neill_SDL_UpdateScreen(&sw);
    Neill_SDL_Events(&sw);

    SDL_Quit();
   atexit(SDL_Quit);
}

void test(void) {
    testValidFlag();
    testInitializeQueue();
    testInitializeBoard();
    // // testFillVal();
    testCopyBoard();
    testIsQueueEmpty();
    testAddAndRemove();
    testGenerateNextBoard();
    testCheckCol();
    testCheckDiagonal1();
    testCheckDiagonal2();
    testIsValidPosition();
    testInitializeResult();
    testAddToResult();
    testFillValInResult();
    testSolveBoards();
}

void testValidFlag(void){
    assert(isValidFlag("-verbose"));
    assert(!isValidFlag("--verbose"));
    assert(!isValidFlag(" "));
    assert(!isValidFlag("6"));
}

void testInitializeQueue(void){
    Queue* queue = initializeQueue();
    assert(queue->end == NULL && queue->start == NULL);
    assert(queue->size == 0);
    free(queue);
    Queue* q = initializeQueue();
    assert(q->end == NULL && q->start == NULL);
    assert(q->size == 0);
    free(q);
}

void testIsQueueEmpty(void){
    Queue* queue = initializeQueue();
    assert(isQueueEmpty(queue));
    free(queue);
}

void testAddAndRemove(void){
    Queue* queue = initializeQueue();
    Board a;
    initializeBoard(&a, 3);
    addToQueue(queue, a);
    assert(queue->size == 1);
    assert(queue->end != NULL);
    assert(queue->start != NULL);

    Board b;
    removeFromQueue(queue, &b);
    assert(a.nextRow == b.nextRow);
    assert(a.colMask == b.colMask);
    assert(a.diagonal1Mask == b.diagonal1Mask);
    assert(a.diagonal2Mask == b.diagonal2Mask);

    for(int row=0;row<3;row++){
        for(int col=0;col<3;col++){
            assert(a.arr[row][col] == b.arr[row][col]);
        }
    }
    free(queue);
}

void testInitializeBoard(void){
    Board b;
    int size = 3;
    initializeBoard(&b, size);
    assert(b.colMask == 0);
    assert(b.diagonal1Mask == 0 && b.diagonal2Mask == 0);
    assert(b.size == size);
    for(int row=0;row<size;row++){
        for(int col=0;col<size;col++){
            assert(b.arr[row][col] == EMPTY);
        }
    }
}

void testCopyBoard(void){
    Board a;
    initializeBoard(&a, 3);
    Board b;
    a = generateNextBoard(&a, 0, 2);
    copyBoard(&b, &a);
    assert(b.colMask == a.colMask);
    assert(b.diagonal1Mask == a.diagonal1Mask && b.diagonal2Mask == a.diagonal2Mask);
    assert(b.size == a.size);
    for(int row=0;row<a.size;row++){
        for(int col=0;col<a.size;col++){
            assert(b.arr[row][col] == a.arr[row][col]);
        }
    }
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

void testInitializeResult(void){
    Result res;
    initializeResult(&res);
    // assert(res.nextIndex == 0);
    // assert(res.arr != NULL);
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

void testGenerateNextBoard(void){
    Board a;
    initializeBoard(&a, 4);
    Board b = generateNextBoard(&a, 0, 0);
    assert(b.arr[0][0] == QUEEN);
    assert(b.colMask == 1);
    assert(b.diagonal2Mask == 1);

    b = generateNextBoard(&b, 2, 2);
    assert(b.arr[2][2] == QUEEN);
    assert(b.colMask == 5);
    assert(b.diagonal2Mask == 17);
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
