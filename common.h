#pragma once

#include <QColor>

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

static constexpr auto BackGroundColor = QColor(205, 205, 205);
static constexpr auto LineColor = QColor(128, 128, 128);
static constexpr auto BlackMidPieceColor = QColor(70, 70, 70);
static constexpr auto BlackFringePieceColor = QColor(40, 40, 40);
static constexpr auto WriteMidPieceColor = QColor(235, 235, 235);
static constexpr auto WriteFringePieceColor = QColor(205, 205, 205);
static constexpr auto WriteEdgePieceColor = QColor(180, 180, 180);

enum class ChessPlayer
{
    NONE,
    BLACK,
    WRITE
};
