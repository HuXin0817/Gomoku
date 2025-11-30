#pragma once

#include <fstream>
#include <thread>

#include "common.h"

class Board {
  public:
  Board()
      : chessMap(static_cast<size_t>(Config::ChessNumber),
                 std::vector<ChessPlayer>(static_cast<size_t>(Config::ChessNumber))) {
  }

  void
  addPiece(int x, int y);

  bool
  judgeIsPos(int x, int y) const;

  auto
  getNowPlayer() const {
    return nowPlayer;
  }

  auto
  isGameOver() const {
    return gameOver;
  }

  std::vector<Point>
  winPieces() const;

  const auto&
  getMoveRecords() const {
    return moveRecords;
  }

  Point
  undo();

  void
  restart();

  private:
  bool gameOver = false;
  ChessPlayer nowPlayer = ChessPlayer::Black;
  std::vector<std::vector<ChessPlayer>> chessMap;
  std::vector<Point> moveRecords;

  bool
  checkWin(int x, int y) const;

  bool
  checkLine(int x, int y, int dx, int dy) const;

  static bool
  checkInBoard(int x, int y);

  std::vector<Point>
  findOne(int x, int y, int dx, int dy) const;
};
