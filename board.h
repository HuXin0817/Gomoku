#pragma once

#include "moverecords.h"

#include <QStandardPaths>

#include <unordered_set>
#include <fstream>
#include <thread>

class Board
{
  public:
    Board() : chessMap(Config::CHESS_NUMBER, std::vector<ChessPlayer>(Config::CHESS_NUMBER)) {}

    void addPiece(int x, int y);

    [[nodiscard]] bool judgeIsPos(int x, int y) const;

    [[nodiscard]] const auto getNowPlayer() const { return nowPlayer; }

    [[nodiscard]] const auto isGameOver() const { return gameOver; }

    [[nodiscard]] std::vector<Point> winPieces() const;

    [[nodiscard]] const auto getMoveRecords() const { return moveRecords.get(); }

    Point undo();

    void restart();

    [[nodiscard]] Point getBestPoint() const;

  private:
    bool                                  gameOver  = false;
    ChessPlayer                           nowPlayer = ChessPlayer::BLACK;
    std::vector<std::vector<ChessPlayer>> chessMap;
    MoveRecords                           moveRecords;

    [[nodiscard]] bool checkWin(int x, int y) const;

    [[nodiscard]] bool checkLine(int x, int y, int dx, int dy) const;

    static bool checkInBoard(int x, int y);

    bool findOne(int x, int y, int dx, int dy, std::vector<Point> &pos) const;

    [[nodiscard]] std::unordered_set<Point> getNearPoints() const;

    [[nodiscard]] Point getRandomFromNearPoints() const;
};
