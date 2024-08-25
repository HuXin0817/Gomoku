#pragma once

#include <QColor>

static auto CHESS_NUMBER = 15;

static double BOARD_PIECE_SPACING = 36.0;

static auto BOARD_LINE_WIDTH() { return BOARD_PIECE_SPACING / 20.0; }

static auto BOARD_STAR_POINT_WIDTH() { return BOARD_LINE_WIDTH() * 2; }

static auto BOARD_PIECE_WIDTH() { return BOARD_LINE_WIDTH() * 8; }

static auto BOARD_MARGIN() { return BOARD_PIECE_SPACING / 4 * 5; }

static auto BOARD_SIZE() { return BOARD_MARGIN() * 2 + BOARD_PIECE_SPACING * (CHESS_NUMBER - 1); }

static std::vector<int> StarPositions()
{
    if (CHESS_NUMBER > 11)
    {
        return {3, CHESS_NUMBER / 2, CHESS_NUMBER - 4};
    }
    else if (CHESS_NUMBER > 9)
    {
        return {2, CHESS_NUMBER / 2, CHESS_NUMBER - 3};
    }
    else
    {
        return {CHESS_NUMBER / 2};
    }
}

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
