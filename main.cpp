#include <stdio.h>
#include <stdbool.h>

typedef enum {
    X_MOVE = 0,
    O_MOVE = 1,
    NONE = 2,
} State;

const char printChars[4] = {'X', 'O', '_'};

const int NUM_ROWS = 3;
const int NUM_COLS = 3;

State gameBoard[9] = {NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE};

//const int a = 1;

void printBoard(State *board) {
    printf("Board:\n");
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            printf("%c ", printChars[gameBoard[i * NUM_ROWS + j]]);
        }
        printf("\n");
    }
}

void getUserMove(State *board, State player) {
    int index = -1;
    while ((index < 0 || index >= 9) || board[index] != NONE) {
        printf("Enter move:\n");
        index = getchar() - '0' - 1;
        getchar();
    }
    board[index] = player;
}

State checkRow(const State *board) {
    return board[0] == board[1] && board[0] == board[2] ? board[0] : NONE;
}

State checkCol(const State *board) {
    return board[0] == board[NUM_COLS] && board[0] == board[NUM_COLS * 2] ? board[0] : NONE;
}

State checkDiag(State a, State b, State c) {
    return a == b && a == c ? a : NONE;
}

bool getWin(State *board, State &winner) {
    winner = NONE;
    for (int i = 0; i < NUM_ROWS && winner == NONE; i++)
        winner = checkRow(board + (i * NUM_COLS));

    for (int i = 0; i < NUM_ROWS && winner == NONE; i++)
        winner = checkCol(board + i);

    if (winner == NONE)
        winner = checkDiag(board[0], board[4], board[8]);

    if (winner == NONE)
        winner = checkDiag(board[2], board[4], board[6]);

    bool tie = true;
    for (int i = 0; i < 9; i++) {
        if (board[i] == NONE) {
            tie = false;
            break;
        }
    }

    return tie || winner != NONE;
}

typedef struct {
    bool ensureWin;
    bool ensureTie;
    int move;
} WinInfo;

WinInfo recurseHelper(State *board, const State player, bool isTurn) {
    WinInfo output{false, false, 9};

    // Check if the game has been won
    State winner;
    if (getWin(board, winner)) {
        if (winner == player) {
            output.ensureWin = true;
        } else if (winner == NONE)
            output.ensureTie = true;
        return output;
    }

    // Recurse through all options
    if (isTurn) {
        for (int i = 0; i < 9; i++) {
            if (board[i] == NONE) {
                board[i] = player;
                auto info = recurseHelper(board, player, false);
                board[i] = NONE;

                if (info.ensureWin) {
                    output.ensureWin = true;
                    output.ensureTie = true;
                    output.move = i;
                    return output;
                }
                if (info.ensureTie) {
                    output.ensureTie = true;
                    output.move = i;
                }
            }
        }
        return output;
    } else {
        output.ensureWin = true;
        output.ensureTie = true;
        for (int i = 0; i < 9; i++) {
            if (board[i] == NONE) {
                board[i] = (player == X_MOVE ? O_MOVE : X_MOVE);
                auto info = recurseHelper(board, player, true);
                board[i] = NONE;

                if (!info.ensureWin)
                    output.ensureWin = false;


                if (!info.ensureTie)
                    output.ensureTie = false;

            }
        }
        return output;
    }
}

void getComputerMove(State *board, const State player) {
    auto move = recurseHelper(board, player, true);
    board[move.move] = player;
//    printf("Win %d, tie %d, move: %d\n", move.ensureWin, move.ensureTie, move.move);
}

int main() {
    State winner;

    while (!getWin(gameBoard, winner)) {
        getUserMove(gameBoard, X_MOVE);
        printBoard(gameBoard);
        if (!getWin(gameBoard, winner)) {
            getComputerMove(gameBoard, O_MOVE);
            printBoard(gameBoard);
        }
    }
    printf("%c WON!", printChars[winner]);
    return 0;
}
