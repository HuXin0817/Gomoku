#pragma once

#include <QApplication>
#include <QtGui/qpalette.h>
#include <unordered_map>

enum class ChessPlayer
{
    NONE,
    BLACK,
    WHITE
};

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
    std::size_t operator()(const point &p) const
    {
        return reinterpret_cast<std::size_t>(&p);
    }
};

struct Config
{
    static inline int WIN_PIECE_NUMBER = 5;
    static inline double CHESS_NUMBER = 15;
    static inline double BOARD_PIECE_SPACING = 36.0;

    static auto BOARD_LINE_WIDTH() { return BOARD_PIECE_SPACING / 20.0; }

    static auto BOARD_LINE_WIDTH(double BOARD_PIECE_SPACING) { return BOARD_PIECE_SPACING / 20.0; }

    static auto BOARD_STAR_POINT_WIDTH() { return BOARD_LINE_WIDTH() * 2; }

    static auto BOARD_PIECE_WIDTH(double BOARD_PIECE_SPACING) { return BOARD_LINE_WIDTH(BOARD_PIECE_SPACING) * 8; }

    static auto BOARD_MARGIN() { return BOARD_PIECE_SPACING / 4 * 5; }

    static auto BOARD_SIZE() { return BOARD_MARGIN() * 2 + BOARD_PIECE_SPACING * (CHESS_NUMBER - 1); }

    static std::vector<double> StarPositions()
    {
        double starPos = int(CHESS_NUMBER / 5);
        if (starPos < 2)
        {
            return {(CHESS_NUMBER - 1) / 2};
        }
        else
        {
            return {starPos, (CHESS_NUMBER - 1) / 2, CHESS_NUMBER - starPos - 1};
        }
    }

    static inline std::unordered_map<ChessPlayer, bool> AI_CHESS_PLAYER;
};

static constexpr QColor LineColor = {128, 128, 128};

inline bool IS_DARK_THEME()
{
    return qApp->palette().color(QPalette::Window).lightness() < 128;
}

inline QColor BackGroundColor()
{
    return IS_DARK_THEME() ? QColor(60, 60, 60) : QColor(205, 205, 205);
}

inline QColor BlackFringePieceColor()
{
    return IS_DARK_THEME() ? QColor(30, 30, 30) : QColor(40, 40, 40);
}

inline QColor BlackMidPieceColor()
{
    int rgb = BlackFringePieceColor().red() + 30;
    return {rgb, rgb, rgb};
}

inline QColor WriteFringePieceColor()
{
    return IS_DARK_THEME() ? QColor(165, 165, 165) : QColor(205, 205, 205);
}

inline QColor WriteMidPieceColor()
{
    int rgb = WriteFringePieceColor().red() + 30;
    return {rgb, rgb, rgb};
}

inline QColor WriteEdgePieceColor()
{
    int rgb = WriteFringePieceColor().red() - 20;
    return {rgb, rgb, rgb};
}
