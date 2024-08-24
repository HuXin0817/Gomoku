#pragma once

#include "common.h"

class Board {
public:
    void addPiece(int x, int y);

    ChessPlayer getMapInfo(int x, int y) { return chessMap[x][y]; }

private:
    bool isGameOver = false;
    ChessPlayer nowPlayer = ChessPlayer::BLACK;
    ChessPlayer chessMap[CHESS_NUMBER][CHESS_NUMBER]{};

    bool checkWin(int x, int y);

    bool checkLine(int x, int y, int dx, int dy);

    static bool checkInBoard(int x, int y) { return x >= 0 && x < CHESS_NUMBER && y >= 0 && y < CHESS_NUMBER; }
};
