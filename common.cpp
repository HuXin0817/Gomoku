#include "common.h"

int Config::WIN_PIECE_NUMBER = 5;
double Config::CHESS_NUMBER = 15;
double Config::BOARD_PIECE_SPACING = 36.0;

std::vector<double> Config::StarPositions()
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

std::size_t std::hash<point>::operator()(const point &p) const
{
    return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
}
