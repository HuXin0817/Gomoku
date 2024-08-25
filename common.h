#pragma once

#include <QColor>

struct Config
{
    static int CHESS_NUMBER;

    static double BOARD_PIECE_SPACING;

    static int WIN_PIECE_NUMBER;

    static double BOARD_LINE_WIDTH() { return BOARD_PIECE_SPACING / 20.0; }

    static double BOARD_STAR_POINT_WIDTH() { return BOARD_LINE_WIDTH() * 2; }

    static double BOARD_PIECE_WIDTH() { return BOARD_LINE_WIDTH() * 8; }

    static double BOARD_MARGIN() { return BOARD_PIECE_SPACING / 4 * 5; }

    static double BOARD_SIZE() { return BOARD_MARGIN() * 2 + BOARD_PIECE_SPACING * (CHESS_NUMBER - 1); }

    static std::vector<int> StarPositions();
};

static constexpr QColor BackGroundColor = QColor(205, 205, 205);
static constexpr QColor LineColor = QColor(128, 128, 128);
static constexpr QColor BlackMidPieceColor = QColor(70, 70, 70);
static constexpr QColor BlackFringePieceColor = QColor(40, 40, 40);
static constexpr QColor WriteMidPieceColor = QColor(235, 235, 235);
static constexpr QColor WriteFringePieceColor = QColor(205, 205, 205);
static constexpr QColor WriteEdgePieceColor = QColor(180, 180, 180);

enum class ChessPlayer
{
    NONE,
    BLACK,
    WRITE
};
