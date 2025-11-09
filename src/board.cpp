#include "board.h"

bool
Board::checkLine(int x, int y, int dx, int dy) const {
  int count = 0;
  int cx = x, cy = y;
  while (checkInBoard(cx, cy) && chessMap[cx][cy] == chessMap[x][y]) {
    cx -= dx;
    cy -= dy;
    count++;
  }
  cx = x, cy = y;
  while (checkInBoard(cx, cy) && chessMap[cx][cy] == chessMap[x][y]) {
    cx += dx;
    cy += dy;
    count++;
  }
  return count > Config::WIN_PIECE_NUMBER;
}

bool
Board::checkWin(int x, int y) const {
  if (checkLine(x, y, 1, 0)) {
    return true;
  }
  if (checkLine(x, y, 0, 1)) {
    return true;
  }
  if (checkLine(x, y, 1, 1)) {
    return true;
  }
  if (checkLine(x, y, 1, -1)) {
    return true;
  }
  return false;
}

void
Board::addPiece(int x, int y) {
  if (!judgeIsPos(x, y)) {
    return;
  }
  chessMap[x][y] = nowPlayer;
  moveRecords.emplace_back(x, y);
  nowPlayer = nowPlayer == ChessPlayer::Black ? ChessPlayer::White : ChessPlayer::Black;
  gameOver = checkWin(x, y);
}

bool
Board::judgeIsPos(int x, int y) const {
  if (gameOver) {
    return false;
  }
  if (x < 0 || x >= Config::ChessNumber) {
    return false;
  }
  if (y < 0 || y >= Config::ChessNumber) {
    return false;
  }
  if (chessMap[x][y] != ChessPlayer::None) {
    return false;
  }
  return true;
}

std::vector<Point>
Board::winPieces() const {
  std::vector<Point> pos;
  for (int i = 0; i < Config::ChessNumber; i++) {
    for (int j = 0; j < Config::ChessNumber; j++) {
      auto find = findOne(i, j, 1, 0);
      if (find.size() >= Config::WIN_PIECE_NUMBER) {
        for (auto p : find) {
          pos.emplace_back(p);
        }
      }
      find = findOne(i, j, 1, 1);
      if (find.size() >= Config::WIN_PIECE_NUMBER) {
        for (auto p : find) {
          pos.emplace_back(p);
        }
      }
      find = findOne(i, j, 1, -1);
      if (find.size() >= Config::WIN_PIECE_NUMBER) {
        for (auto p : find) {
          pos.emplace_back(p);
        }
      }
      find = findOne(i, j, 0, 1);
      if (find.size() >= Config::WIN_PIECE_NUMBER) {
        for (auto p : find) {
          pos.emplace_back(p);
        }
      }
    }
  }
  return pos;
}

std::vector<Point>
Board::findOne(int x, int y, int dx, int dy) const {
  std::vector<Point> pos;
  pos.clear();
  int mx = x, my = y;
  if (chessMap[x][y] == ChessPlayer::None) {
    return {};
  }
  while (checkInBoard(mx, my) && chessMap[mx][my] == chessMap[x][y]) {
    pos.emplace_back(mx, my);
    mx += dx;
    my += dy;
  }
  return pos;
}

bool
Board::checkInBoard(int x, int y) {
  return x >= 0 && x < Config::ChessNumber && y >= 0 && y < Config::ChessNumber;
}

Point
Board::undo() {
  auto back = moveRecords.back();
  moveRecords.pop_back();
  chessMap[back.x][back.y] = ChessPlayer::None;
  gameOver = false;
  if (nowPlayer == ChessPlayer::White) {
    nowPlayer = ChessPlayer::Black;
  } else {
    nowPlayer = ChessPlayer::White;
  }
  return back;
}

void
Board::restart() {
  gameOver = false;
  nowPlayer = ChessPlayer::Black;
  chessMap = {
      static_cast<size_t>(Config::ChessNumber),
      std::vector<ChessPlayer>(static_cast<size_t>(Config::ChessNumber)),
  };
  moveRecords.clear();
}
