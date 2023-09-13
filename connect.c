#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>

#include "connect.h"
#include "c4Create.h"


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



void removeValidPosition(GameState *game, int row, int col) {
    ValidPosition *position = game->validPositions[row];
    if (position == NULL) {
        return;
    }
    if (position->col == col) {
        game->validPositions[row] = position->next;
        free(position);
        return;
    }
    while (position->next != NULL) {
        if (position->next->col == col) {
            ValidPosition *temp = position->next;
            position->next = position->next->next;
            free(temp);
            return;
        }
        position = position->next;
    }
    return;
}


bool playGame(GameState *game) {
    if(game->turn == BOT) {
        printf("Bots turn!\n");
        int col = (((time(NULL)+rand())/1)% COLS);
        int row = makeMove(game, game->turn, col);
        while(row == -1) {
            col = (((time(NULL)+rand())/1)% COLS);
            row = makeMove(game, game->turn, col);
        };
        if(game->chipCount >= 7 && checkWin(game, row, col, game->turn)) {
            printf("Bot wins!\n");
            return false;
        }
        if(game->chipCount == ROWS * COLS) {
            printf("Draw!\n");
            return false;
        }
        removeValidPosition(game, row, col);
        game->turn = PLAYER;
    } else if (game->turn == PLAYER) {
        printf("Player turn!\n"
            "Enter column: ");
        int col;
        while(scanf(" %d", &col) != EOF) {
            int row = makeMove(game, game->turn, col);
            if (row != -1) {
                if(game->chipCount >= 7 && checkWin(game, row, col, game->turn)) {
                    printf("Player wins!\n");
                    return false;
                }
                if(game->chipCount == ROWS * COLS) {
                    printf("Draw!\n");
                    return false;
                }
                removeValidPosition(game, row, col);
                game->turn = BOT;
                break;
            } else {
                printf("Invalid move\nEnter column: ");
            }
        }
    } else {
        printf("Player %d turn!"
            "\nEnter column: ", game->turn);
        int col;
        while(scanf(" %d", &col) != EOF) {
            int row = makeMove(game, (game)->turn, col);
            if (row != -1) {
                if(game->chipCount >= 7 && checkWin(game, row, col, game->turn)) {
                    printf("Player %d wins!\n", game->turn);
                    return false;
                }
                if(game->chipCount == ROWS * COLS) {
                    printf("Draw!\n");
                    return false;
                }
                game->turn = (game->turn == game->player1) ? game->player2 : game->player1;
                removeValidPosition(game, row, col);
                break;
            } else {
                printf("Invalid move\nEnter column: ");
            }
        }
    }
    return true;
}

int main(void) {
    GameState *game = createGameState();
    while (playGame(game)) {
        // playGame(game);
    }
    freeGameState(game);
    return 0;
}