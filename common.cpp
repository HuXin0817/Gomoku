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

bool point::operator==(const point &other) const
{
    return x == other.x && y == other.y;
}

bool point::operator<(const point &other) const
{
    if (x != other.x)
    {
        return x < other.x;
    }
    return y < other.y;
}

std::size_t std::hash<point>::operator()(const point &p) const
{
    return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
}
