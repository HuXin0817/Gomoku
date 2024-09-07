#include "board.h"

bool Board::checkLine(int x, int y, int dx, int dy) const
{
    int count = 0;
    int cx = x, cy = y;
    while (checkInBoard(cx, cy) && chessMap[cx][cy] == chessMap[x][y])
    {
        cx -= dx;
        cy -= dy;
        count++;
    }
    cx = x, cy = y;
    while (checkInBoard(cx, cy) && chessMap[cx][cy] == chessMap[x][y])
    {
        cx += dx;
        cy += dy;
        count++;
    }
    return count > Config::WIN_PIECE_NUMBER;
}

bool Board::checkWin(int x, int y) const
{
    if (checkLine(x, y, 1, 0))
    {
        return true;
    }
    if (checkLine(x, y, 0, 1))
    {
        return true;
    }
    if (checkLine(x, y, 1, 1))
    {
        return true;
    }
    if (checkLine(x, y, 1, -1))
    {
        return true;
    }
    return false;
}

void Board::addPiece(int x, int y)
{
    if (!judgeIsPos(x, y))
    {
        return;
    }
    chessMap[x][y] = nowPlayer;
    moveRecords.emplace_back(x, y);
    nowPlayer = nowPlayer == ChessPlayer::BLACK ? ChessPlayer::WHITE : ChessPlayer::BLACK;
    gameOver = checkWin(x, y);
}

bool Board::judgeIsPos(int x, int y) const
{
    if (gameOver)
    {
        return false;
    }
    if (x < 0 || x >= Config::CHESS_NUMBER)
    {
        return false;
    }
    if (y < 0 || y >= Config::CHESS_NUMBER)
    {
        return false;
    }
    if (chessMap[x][y] != ChessPlayer::NONE)
    {
        return false;
    }
    return true;
}

std::vector<point> Board::winPieces() const
{
    std::vector<point> pos;
    for (int i = 0; i < Config::CHESS_NUMBER; i++)
    {
        for (int j = 0; j < Config::CHESS_NUMBER; j++)
        {
            if (findOne(i, j, 1, 0, pos))
            {
                return pos;
            }
            if (findOne(i, j, 1, 1, pos))
            {
                return pos;
            }
            if (findOne(i, j, 1, -1, pos))
            {
                return pos;
            }
            if (findOne(i, j, 0, 1, pos))
            {
                return pos;
            }
        }
    }
    return {};
}

bool Board::findOne(int x, int y, int dx, int dy, std::vector<point> &pos) const
{
    pos.clear();
    int mx = x, my = y;
    if (chessMap[x][y] == ChessPlayer::NONE)
    {
        return false;
    }
    while (checkInBoard(mx, my) && chessMap[mx][my] == chessMap[x][y])
    {
        pos.emplace_back(mx, my);
        mx += dx;
        my += dy;
    }
    return pos.size() >= Config::WIN_PIECE_NUMBER;
}

bool Board::checkInBoard(int x, int y)
{
    return x >= 0 && x < Config::CHESS_NUMBER && y >= 0 && y < Config::CHESS_NUMBER;
}

point Board::undo()
{
    auto back = moveRecords.back();
    moveRecords.pop_back();
    chessMap[back.x][back.y] = ChessPlayer::NONE;
    gameOver = false;
    if (nowPlayer == ChessPlayer::WHITE)
    {
        nowPlayer = ChessPlayer::BLACK;
    }
    else
    {
        nowPlayer = ChessPlayer::WHITE;
    }
    return back;
}

void Board::restart()
{
    gameOver = false;
    nowPlayer = ChessPlayer::BLACK;
    chessMap = {size_t(Config::CHESS_NUMBER), std::vector<ChessPlayer>(Config::CHESS_NUMBER)};
    moveRecords.clear();
}

point Board::getBestPoint() const
{
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return getRandomFromNearPoints();
}

std::unordered_set<point> Board::getNearPoints() const
{
    std::unordered_set<point> nearPoints;
    for (auto [x, y] : moveRecords.get())
    {
        for (int i = -2; i <= 2; i++)
        {
            for (int j = -2; j <= 2; j++)
            {
                if (!checkInBoard(x + i, y + j))
                {
                    continue;
                }
                if (chessMap[x + i][y + j] == ChessPlayer::NONE)
                {
                    nearPoints.emplace(x + i, y + j);
                }
            }
        }
    }
    return nearPoints;
}

point Board::getRandomFromNearPoints() const
{
    auto nearPiecesSet = getNearPoints();
    std::vector<point> nearPieces(nearPiecesSet.begin(), nearPiecesSet.end());
    point point(Config::CHESS_NUMBER / 2, Config::CHESS_NUMBER / 2);
    if (!nearPieces.empty())
    {
        size_t idx = random() % nearPieces.size();
        point = nearPieces[idx];
    }
    return point;
}
