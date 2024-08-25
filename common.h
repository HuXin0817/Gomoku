#pragma once

#include <QColor>

static constexpr auto WIN_PIECE_NUMBER = 5;
static constexpr auto CHESS_NUMBER = 15;
static constexpr auto BOARD_PIECE_SPACING = 35;
static constexpr auto BOARD_LINE_WIDTH = static_cast<float>(BOARD_PIECE_SPACING) / 20.0;
static constexpr auto BOARD_STAR_POINT_WIDTH = static_cast<int>(BOARD_LINE_WIDTH) * 3;
static constexpr auto BOARD_PIECE_WIDTH = static_cast<int>(BOARD_LINE_WIDTH * 8);
static constexpr auto BOARD_MARGIN = BOARD_PIECE_SPACING / 4 * 5;
static constexpr auto BOARD_SIZE = BOARD_MARGIN * 2 + BOARD_PIECE_SPACING * (CHESS_NUMBER - 1);
static constexpr int StarPositions[] = {3, CHESS_NUMBER / 2, CHESS_NUMBER - 4};

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
