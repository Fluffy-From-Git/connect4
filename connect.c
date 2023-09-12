#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <math.h>
#include <time.h>

#define ROWS 6
#define COLS 7
#define BLUE 0x0001F535
#define RED 0x0001F534

typedef struct boardCell {
    wchar_t chip;
    int player;
} BoardCell;

enum Player {
    NONE = 0,
    PLAYER1 = 1,
    PLAYER2 = 2,
    BOT = 3,
    PLAYER = 4
};
typedef struct gameState {
    BoardCell **board;
    enum Player player1;
    enum Player player2;
    enum Player turn;
    int chipCount;
} GameState;

BoardCell **generateBoard(void) {
    BoardCell **board = malloc(ROWS * sizeof(BoardCell*));
    for (int i = 0; i < ROWS; i++) {
        board[i] = malloc(COLS * sizeof(BoardCell));
        for (int j = 0; j < COLS; j++) {
            board[i][j].player = 0;
        }
    }
    return board;
}

void printBoard(BoardCell **board) {
    printf("  0   1   2   3   4   5   6\n");
    setlocale(LC_CTYPE, "");
    for (int i = 0; i < ROWS; i++) {
        printf(" ___________________________\n");
        printf("|");
        for (int j = 0; j < COLS; j++) {
            if (board[i][j].player == 0) {
                printf("   |");
            } else {
                wchar_t chip = (board[i][j].player == 1) ? BLUE : RED;
                wprintf(L" %lc|", chip);
            }
        }
        printf("\n");
    }
    printf(" ___________________________\n");
}

bool checkRowWin(BoardCell **board, int row, int col, int player) {
    int count = 0;
    for (int i = col; i < COLS; i++) {
        if (board[row][i].player == player) {
            count++;
        } else {
            break;
        }
    }
    for (int i = col - 1; i >= 0; i--) {
        if (board[row][i].player == player) {
            count++;
        } else {
            break;
        }
    }
    return count >= 4;
}
bool checkColWin(BoardCell **board, int row, int col, int player) {
    int count = 0;
    for (int i = row; i < ROWS; i++) {
        if (board[i][col].player == player) {
            count++;
        } else {
            break;
        }
    }
    for (int i = row - 1; i >= 0; i--) {
        if (board[i][col].player == player) {
            count++;
        } else {
            break;
        }
    }
    return count >= 4;
}
bool checkDiagWin(BoardCell **board, int row, int col, int player) {
    int count1 = 0;
    int count2 = 0;
    for (int i = row, j = col; i < ROWS && j < COLS; i++, j++) {
        if (board[i][j].player == player) {
            count1++;
        } else {
            break;
        }
    }
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j].player == player) {
            count1++;
        } else {
            break;
        }
    }
    for (int i = row, j = col; i < ROWS && j >= 0; i++, j--) {
        if (board[i][j].player == player) {
            count2++;
        } else {
            break;
        }
    }
    for (int i = row - 1, j = col + 1; i >= 0 && j < COLS; i--, j++) {
        if (board[i][j].player == player) {
            count2++;
        } else {
            break;
        }
    }
    return count1 >= 4 || count2 >= 4;
}

bool checkWin(GameState *game, int row, int col, enum Player player) {
    int playerposition = 0;
    if (player == game->player1) {
        playerposition = 1;
    } else if (player == game->player2) {
        playerposition = 2;
    } else {
        printf("Invalid player\n");
        return false;
    }
    return checkRowWin(game->board, row, col, playerposition) ||
           checkColWin(game->board, row, col, playerposition) ||
           checkDiagWin(game->board, row, col, playerposition);
}


int makeMove(GameState *game, enum Player player, int col) {
    int playerposition = 0;
    if (player == game->player1) {
        playerposition = 1;
    } else if (player == game->player2) {
        playerposition = 2;
    } else {
        printf("Invalid player\n");
        return -1;
    }
    if (col >= COLS || col < 0){
        return -1;
    }
    for (int i = ROWS - 1; i >= 0; i--) {
        if(game->board[i][col].player == 0) {
            game->board[i][col].player = playerposition;
            game->board[i][col].chip = (player == 1) ? BLUE : RED;
            game->chipCount++;
            printBoard(game->board);
            return i;
        }
    }
    return -1;
}

GameState *createGameState(void) {
    GameState *game = malloc(sizeof(GameState));
    game->board = generateBoard();
    game->chipCount = 0;
    printf("Play against bot? (Y/N): ");
    char c;
    while(scanf(" %c", &c) != EOF) {
        if (c == 'Y' || c == 'y') {
            printf("Would you like to go first? (Y/N): ");
            while(scanf(" %c", &c) != EOF) {
                if (c == 'Y' || c == 'y') {
                    game->player1 = PLAYER;
                    game->player2 = BOT;
                    break;
                } else if (c == 'N' || c == 'n') {
                    game->player1 = BOT;
                    game->player2 = PLAYER;
                    break;
                } else {
                    printf("Invalid input\n");
                    printf("Would you like to go first? (Y/N): ");
                }
            }
            break;
        } else if (c == 'N' || c == 'n') {
            game->player1 = PLAYER1;
            game->player2 = PLAYER2;
            break;
        } else {
            printf("Invalid input\n");
            printf("Play against bot? (Y/N): ");
        }
    }
    game->turn = game->player1;
    return game;
}

bool playGame(GameState **game) {
    if((*game)->turn == BOT) {
        printf("Bots turn!\n");
        int col = (((time(NULL)+rand())/1)% COLS);
        int row = makeMove(*game, (*game)->turn, col);
        while(row == -1) {
            col = (((time(NULL)+rand())/1)% COLS);
            row = makeMove(*game, (*game)->turn, col);
        };
        if((*game)->chipCount >= 7 && checkWin(*game, row, col, (*game)->turn)) {
            printf("Bot wins!\n");
            return false;
        }
        if((*game)->chipCount == ROWS * COLS) {
            printf("Draw!\n");
            return false;
        }
        (*game)->turn = PLAYER;
    } else if ((*game)->turn == PLAYER) {
        printf("Player turn!\n"
            "Enter col: ");
        int col;
        while(scanf(" %d", &col) != EOF) {
            int row = makeMove(*game, (*game)->turn, col);
            if (row != -1) {
                if((*game)->chipCount >= 7 && checkWin((*game), row, col, (*game)->turn)) {
                    printf("Player wins!\n");
                    return false;
                }
                if((*game)->chipCount == ROWS * COLS) {
                    printf("Draw!\n");
                    return false;
                }
                (*game)->turn = BOT;
                break;
            } else {
                printf("Invalid move\nEnter col: ");
            }
        }
    } else {
        printf("Player %d turn!"
            "\nEnter col: ", (*game)->turn);
        int col;
        while(scanf(" %d", &col) != EOF) {
            int row = makeMove((*game), (*game)->turn, col);
            if (row != -1) {
                if((*game)->chipCount >= 7 && checkWin((*game), row, col, (*game)->turn)) {
                    printf("Player %d wins!\n", (*game)->turn);
                    return false;
                }
                if((*game)->chipCount == ROWS * COLS) {
                    printf("Draw!\n");
                    return false;
                }
                (*game)->turn = ((*game)->turn == (*game)->player1) ? (*game)->player2 : (*game)->player1;
                break;
            } else {
                printf("Invalid move\nEnter col: ");
            }
        }
    }
    return true;
}
int main(void) {
    GameState *game = createGameState();
    while (playGame(&game)) {
        // playGame(game);
    }
    return 0;
}