#pragma once

#include "moverecords.h"

#include <QStandardPaths>

#include <unordered_set>
#include <fstream>
#include <thread>

class Board {
public:
    Board() : chessMap(Config::CHESS_NUMBER, std::vector<ChessPlayer>(Config::CHESS_NUMBER)) {}

    void addPiece(int x, int y);

    bool judgeIsPos(int x, int y) const;

    auto getNowPlayer() const { return nowPlayer; }

    auto isGameOver() const { return gameOver; }

    std::vector<point> winPieces() const;

    auto getMoveRecords() const { return moveRecords.get(); }

    point undo();

    void restart();

    point getBestPoint() const;

private:
    bool gameOver = false;
    ChessPlayer nowPlayer = ChessPlayer::BLACK;
    std::vector<std::vector<ChessPlayer>> chessMap;
    MoveRecords moveRecords;

    bool checkWin(int x, int y) const;

    bool checkLine(int x, int y, int dx, int dy) const;

    static bool checkInBoard(int x, int y);

    bool findOne(int x, int y, int dx, int dy, std::vector<point> &pos) const;

    std::unordered_set<point> getNearPoints() const;

    point getRandomFromNearPoints() const;
};
