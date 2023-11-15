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
// void testAddToQueue(void);
// void removeFromQueue(void);

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
    fprintf(stderr, "Invalid Board Size, Board size needs to be between 1 and 10 inclusive\n");
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
    do{
        for(int ind=0;ind<result->nextIndex;ind++){
            drawChessBoard(&sw, boardSize);

            drawQueens(&sw, result, boardSize, ind);
        }

        Neill_SDL_UpdateScreen(&sw);
        Neill_SDL_Events(&sw);

   }while(!sw.finished);

   SDL_Quit();
   atexit(SDL_Quit);
}

void test(void) {
    assert(sizeof(int) == 4);
    // Queue q;
    // initializeQueue(&q);
    // assert(q.start == 0 && q.end == 0);
    // Board b;
    // initializeBoard(&b, 8);
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

