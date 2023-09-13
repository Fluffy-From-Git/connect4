// structs for connect 4

#ifndef CONNECT4_H
#define CONNECT4_H
#define ROWS 6
#define COLS 7
#define BLUE 0x0001F535
#define RED 0x0001F534

typedef struct boardCell {
    wchar_t chip;
    int player;
} BoardCell;

enum Player {
    NONE,
    PLAYER1,
    PLAYER2,
    PLAYER,
    BOT,
};

typedef struct position {
    int col;
    struct position *next;
} ValidPosition;

typedef struct gameState {
    BoardCell **board;
    ValidPosition **validPositions;
    enum Player player1;
    enum Player player2;
    enum Player turn;
    int botDifficulty;
    int chipCount;
} GameState;


#endif