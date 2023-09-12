#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

#define ROWS 6
#define COLS 7
#define BLUE 0x0001F535
#define RED 0x0001F534

typedef struct boardCell {
    wchar_t chip;
    int player;
} BoardCell;

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

bool checkWin(BoardCell **board, int row, int col, int player) {
    return checkRowWin(board, row, col, player) ||
           checkColWin(board, row, col, player) ||
           checkDiagWin(board, row, col, player);
}


int makeMove(BoardCell **board, int player, int col) {
    if (col >= COLS) {
        printf("Invalid move\n");
        return -1;
    }
    for (int i = ROWS - 1; i >= 0; i--) {
        if(board[i][col].player == 0) {
            board[i][col].player = player;
            printBoard(board);
            return i;
        }
    }
    printf("Invalid move\n");
    return -1;
}


int main(void) {
    BoardCell **board = generateBoard();
    int col;
    int player = 1;
    int chipCount = 0;
    printf("Player %d turn!"
           "\nEnter row: ", player);
    while(scanf("%d", &col) != EOF) {
        int row = makeMove(board, player, col);
        if (row != -1) {
            chipCount++;
            if(chipCount >= 7 && checkWin(board, row, col, player)) {
                printf("Player %d wins!\n", player);
                return 0;
            }
            if(chipCount == ROWS * COLS) {
                printf("Draw!\n");
                return 0;
            }
            player = (player == 1) ? 2 : 1;
        }
        printf("Player %d turn!"
               "\nEnter row: ", player);
    }
    return 0;
}