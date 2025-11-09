#pragma once

#include <QtGui/qpalette.h>

#include <QApplication>
#include <unordered_map>

enum class ChessPlayer {
  None,
  Black,
  White,
};

struct Point {
  int x;
  int y;

  Point(int x, int y) : x(x), y(y) {
  }

  auto
  operator<=>(const Point&) const = default;
};

template <>
struct std::hash<Point> {
  std::size_t
  operator()(const Point& p) const noexcept {
    return (static_cast<std::size_t>(p.x) << (sizeof(std::size_t) / 2)) + p.y;
  }
};

struct Config {
  static inline int WIN_PIECE_NUMBER = 5;
  static inline double ChessNumber = 15;
  static inline double BoardPieceSpacing = 36.0;

  static auto
  BoardLineWidth() {
    return BoardPieceSpacing / 20.0;
  }

  static auto
  BoardLineWidth(double BoardPieceSpacing) {
    return BoardPieceSpacing / 20.0;
  }

  static auto
  BoardStarPointWidth() {
    return BoardLineWidth() * 2;
  }

  static auto
  BoardPieceWidth(double BoardPieceSpacing) {
    return BoardLineWidth(BoardPieceSpacing) * 8;
  }

  static auto
  BoardMargin() {
    return BoardPieceSpacing / 4 * 5;
  }

  static auto
  BoardSize() {
    return BoardMargin() * 2 + BoardPieceSpacing * (ChessNumber - 1);
  }

  static std::vector<double>
  StarPositions() {
    double starPos = static_cast<int>(ChessNumber / 5);
    if (starPos < 2) {
      return {(ChessNumber - 1) / 2};
    } else {
      return {starPos, (ChessNumber - 1) / 2, ChessNumber - starPos - 1};
    }
  }
};

static constexpr QColor LineColor = {128, 128, 128};

inline bool
IsDarkTheme() {
  return qApp->palette().color(QPalette::Window).lightness() < 128;
}

inline QColor
BackGroundColor() {
  return IsDarkTheme() ? QColor(60, 60, 60) : QColor(205, 205, 205);
}

inline QColor
BlackFringePieceColor() {
  return IsDarkTheme() ? QColor(30, 30, 30) : QColor(40, 40, 40);
}

inline QColor
BlackMidPieceColor() {
  int rgb = BlackFringePieceColor().red() + 30;
  return {rgb, rgb, rgb};
}

inline QColor
WriteFringePieceColor() {
  return IsDarkTheme() ? QColor(165, 165, 165) : QColor(205, 205, 205);
}

inline QColor
WriteMidPieceColor() {
  int rgb = WriteFringePieceColor().red() + 30;
  return {rgb, rgb, rgb};
}

inline QColor
WriteEdgePieceColor() {
  int rgb = WriteFringePieceColor().red() - 20;
  return {rgb, rgb, rgb};
}
