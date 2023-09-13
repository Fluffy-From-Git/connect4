#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>

#include "c4Create.h"

void freeValidPositions(GameState *game) {
    for (int i = 0; i < ROWS; i++) {
        ValidPosition *position = game->validPositions[i];
        while (position != NULL) {
            ValidPosition *temp = position;
            position = position->next;
            free(temp);
        }
    }
    free(game->validPositions);
}

void freeBoard(BoardCell **board) {
    for (int i = 0; i < ROWS; i++) {
        free(board[i]);
    }
    free(board);
}

void freeGameState(GameState *game) {
    freeValidPositions(game);
    freeBoard(game->board);
    free(game);
}

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

ValidPosition *newPosition(int col) {
    ValidPosition *position = malloc(sizeof(ValidPosition));
    position->col = col;
    position->next = NULL;
    return position;
}

void insertPosition(GameState *game, int row, int col) {
    if (game->validPositions[row] == NULL) {
        game->validPositions[row] = newPosition(col);
        return;
    }
    ValidPosition *position = newPosition(col);
    position->next = game->validPositions[row];
    game->validPositions[row] = position;
}


ValidPosition **generateValidPositions(GameState *game) {
    ValidPosition **validPositions = malloc(ROWS * sizeof(ValidPosition));
    for (int i = 0; i < ROWS; i++) {
        validPositions[i] = NULL;
        for(int j = COLS - 1; j >= 0; j--) {
            insertPosition(game, i, j);
        }
    }
    return validPositions;
}

void botSetUp(GameState *game) {
    char c;
    printf("Would you like to go first? (Y/N): ");
    while(scanf(" %c", &c) != EOF) {
        if (c == 'Y' || c == 'y') {
            game->player1 = PLAYER;
            game->player2 = BOT;
            return;
        } else if (c == 'N' || c == 'n') {
            game->player1 = BOT;
            game->player2 = PLAYER;
            return;
        } else {
            printf("Invalid input\n");
            printf("Would you like to go first? (Y/N): ");
        }
    }
    return;
}

GameState *createGameState(void) {
    GameState *game = malloc(sizeof(GameState));
    game->board = generateBoard();
    game->chipCount = 0;
    game->validPositions = generateValidPositions(game);
    printf("Play against bot? (Y/N): ");
    char c;
    while(scanf(" %c", &c) != EOF) {
        if (c == 'Y' || c == 'y') {
            botSetUp(game);
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