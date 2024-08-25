#include "common.h"

int Config::WIN_PIECE_NUMBER = 5;
int Config::CHESS_NUMBER = 15;
double Config::BOARD_PIECE_SPACING = 36.0;

std::vector<int> Config::StarPositions()
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
