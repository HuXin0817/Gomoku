#pragma once

#include "common.h"

#include <fstream>
#include <thread>
#include <unordered_set>

class Board
{
public:
  Board() : chessMap(Config::CHESS_NUMBER, std::vector<ChessPlayer>(Config::CHESS_NUMBER)) {}

  void addPiece(int x, int y);

  [[nodiscard]] bool judgeIsPos(int x, int y) const;

  [[nodiscard]] auto getNowPlayer() const { return nowPlayer; }

  [[nodiscard]] auto isGameOver() const { return gameOver; }

  [[nodiscard]] std::vector<Point> winPieces() const;

  [[nodiscard]] const auto &getMoveRecords() const { return moveRecords; }

  Point undo();

  void restart();

private:
  bool                                  gameOver  = false;
  ChessPlayer                           nowPlayer = ChessPlayer::BLACK;
  std::vector<std::vector<ChessPlayer>> chessMap;
  std::vector<Point>                    moveRecords;

  [[nodiscard]] bool checkWin(int x, int y) const;

  [[nodiscard]] bool checkLine(int x, int y, int dx, int dy) const;

  static bool checkInBoard(int x, int y);

  bool findOne(int x, int y, int dx, int dy, std::vector<Point> &pos) const;
};
