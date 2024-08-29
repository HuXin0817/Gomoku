#pragma once

#include "common.h"

#include <unordered_set>

class Board
{
public:
    Board() : chessMap(Config::CHESS_NUMBER, std::vector<ChessPlayer>(Config::CHESS_NUMBER)) {}

    Board(const Board &) = default;

    void addPiece(int x, int y);

    bool judgeIsPos(int x, int y) const;

    ChessPlayer getNowPlayer() const { return nowPlayer; }

    bool isGameOver() const { return gameOver; }

    std::vector<point> winPieces() const;

    std::vector<point> getMoveRecords() const { return moveRecords; }

    point undo();

private:
    bool gameOver = false;
    ChessPlayer nowPlayer = ChessPlayer::BLACK;
    std::vector<std::vector<ChessPlayer>> chessMap;
    std::vector<point> moveRecords;

    bool checkWin(int x, int y) const;

    bool checkLine(int x, int y, int dx, int dy) const;

    static bool checkInBoard(int x, int y);

    bool findOne(int x, int y, int dx, int dy, std::vector<point> &pos) const;
};
