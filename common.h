#pragma once

#include <QColor>
#include <QApplication>
#include <QtGui/qpalette.h>

struct point
{
    int x;
    int y;

    point(int x, int y) : x(x), y(y) {}

    auto operator<=>(const point &other) const = default;
};

template <>
struct std::hash<point>
{
    std::size_t operator()(const point &p) const;
};

struct Config
{
    static double CHESS_NUMBER;

    static double BOARD_PIECE_SPACING;

    static int WIN_PIECE_NUMBER;

    static auto BOARD_LINE_WIDTH() { return BOARD_PIECE_SPACING / 20.0; }

    static auto BOARD_LINE_WIDTH(double BOARD_PIECE_SPACING) { return BOARD_PIECE_SPACING / 20.0; }

    static auto BOARD_STAR_POINT_WIDTH() { return BOARD_LINE_WIDTH() * 2; }

    static auto BOARD_PIECE_WIDTH(double BOARD_PIECE_SPACING) { return BOARD_LINE_WIDTH(BOARD_PIECE_SPACING) * 8; }

    static auto BOARD_MARGIN() { return BOARD_PIECE_SPACING / 4 * 5; }

    static auto BOARD_SIZE() { return BOARD_MARGIN() * 2 + BOARD_PIECE_SPACING * (CHESS_NUMBER - 1); }

    static std::vector<double> StarPositions();
};

static constexpr auto LineColor = QColor(128, 128, 128);

static bool IS_DARK_THEME()
{
    return qApp->palette().color(QPalette::Window).lightness() < 128;
}

static QColor BackGroundColor()
{
    return IS_DARK_THEME() ? QColor(60, 60, 60) : QColor(205, 205, 205);
}

static QColor BlackFringePieceColor()
{
    return IS_DARK_THEME() ? QColor(30, 30, 30) : QColor(40, 40, 40);
}

static QColor BlackMidPieceColor()
{
    int rgb = BlackFringePieceColor().red() + 30;
    return {rgb, rgb, rgb};
}

static QColor WriteFringePieceColor()
{
    return IS_DARK_THEME() ? QColor(165, 165, 165) : QColor(205, 205, 205);
}

static QColor WriteMidPieceColor()
{
    int rgb = WriteFringePieceColor().red() + 30;
    return {rgb, rgb, rgb};
}

static QColor WriteEdgePieceColor()
{
    int rgb = WriteFringePieceColor().red() - 20;
    return {rgb, rgb, rgb};
}

enum class ChessPlayer
{
    NONE,
    BLACK,
    WRITE
};
