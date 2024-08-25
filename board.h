#pragma once

#include "common.h"

class Board
{
public:
    static int WIN_PIECE_NUMBER;

    Board() : chessMap(CHESS_NUMBER, std::vector<ChessPlayer>(CHESS_NUMBER)) {}

    void addPiece(int x, int y);

    bool judgeIsPos(int x, int y) const;

    ChessPlayer getNowPlayer() const { return nowPlayer; }

    bool isGameOver() const { return gameOver; }

    std::vector<std::pair<int, int>> winPieces() const;

    std::vector<std::pair<int, int>> &getMoveRecords() { return moveRecords; }

private:
    bool gameOver = false;
    ChessPlayer nowPlayer = ChessPlayer::BLACK;
    std::vector<std::vector<ChessPlayer>> chessMap;
    std::vector<std::pair<int, int>> moveRecords;

    bool checkWin(int x, int y) const;

    bool checkLine(int x, int y, int dx, int dy) const;

    static bool checkInBoard(int x, int y);

    bool findOne(int x, int y, int dx, int dy, std::vector<std::pair<int, int>> &pos) const;
};
