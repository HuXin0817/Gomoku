#pragma once

#include "common.h"

class Board
{
public:
    void addPiece(int x, int y);

    bool judgeIsPos(int x, int y) const;

    ChessPlayer getNowPlayer() const { return nowPlayer; }

    bool isGameOver() const { return gameOver; }

    std::vector<std::pair<int, int>> winPieces() const;

private:
    bool gameOver = false;
    ChessPlayer nowPlayer = ChessPlayer::BLACK;
    ChessPlayer chessMap[CHESS_NUMBER][CHESS_NUMBER]{};

    bool checkWin(int x, int y) const;

    bool checkLine(int x, int y, int dx, int dy) const;

    static bool checkInBoard(int x, int y) { return x >= 0 && x < CHESS_NUMBER && y >= 0 && y < CHESS_NUMBER; }

    bool findOne(int x, int y, int dx, int dy, std::vector<std::pair<int, int>> &pos) const;
};
