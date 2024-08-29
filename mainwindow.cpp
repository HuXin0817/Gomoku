#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    board = std::make_unique<Board>();
    reloadSize();

    auto *restartShortcut = new QShortcut(QKeySequence("Ctrl+R"), this);
    auto *undoShortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);
    auto *addBoardSizeShortcut = new QShortcut(QKeySequence("Ctrl+Up"), this);
    auto *reduceBoardSizeShortcut = new QShortcut(QKeySequence("Ctrl+Down"), this);
    auto *addChessNumberShortcut = new QShortcut(QKeySequence("Ctrl+Right"), this);
    auto *reduceChessNumberShortcut = new QShortcut(QKeySequence("Ctrl+Left"), this);

    connect(restartShortcut, &QShortcut::activated, this, &MainWindow::restart);
    connect(undoShortcut, &QShortcut::activated, this, &MainWindow::undo);
    connect(addBoardSizeShortcut, &QShortcut::activated, this, &MainWindow::addBoardSize);
    connect(reduceBoardSizeShortcut, &QShortcut::activated, this, &MainWindow::reduceBoardSize);
    connect(addChessNumberShortcut, &QShortcut::activated, this, &MainWindow::addChessNumber);
    connect(reduceChessNumberShortcut, &QShortcut::activated, this, &MainWindow::reduceChessNumber);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    QPainter painter(this);
    painter.fillRect(rect(), BackGroundColor());
}

void MainWindow::undo()
{
    if (board->getMoveRecords().empty())
    {
        return;
    }
    if (board->isGameOver())
    {
        auto pieces = board->winPieces();
        for (auto [x, y] : pieces)
        {
            widgets[x][y]->stopFlashing();
        }
    }
    auto back = board->undo();
    widgets[back.x][back.y]->clear();
}

void MainWindow::addBoardSize()
{
    auto OLD_BOARD_PIECE_SPACING = Config::BOARD_PIECE_SPACING;
    Config::BOARD_PIECE_SPACING *= 1.1;
    if (Config::BOARD_SIZE() > getMinWindowSize())
    {
        Config::BOARD_PIECE_SPACING = OLD_BOARD_PIECE_SPACING;
        return;
    }
    reloadSize();
}

void MainWindow::reduceBoardSize()
{
    if (Config::BOARD_PIECE_SPACING / 1.1 > 25)
    {
        Config::BOARD_PIECE_SPACING /= 1.1;
        reloadSize();
    }
    else if (Config::BOARD_PIECE_SPACING / 1.1 < 25)
    {
        Config::BOARD_PIECE_SPACING = 25;
        reloadSize();
    }
}

void MainWindow::addChessNumber()
{
    Config::CHESS_NUMBER += 2;
    if (Config::BOARD_SIZE() > getMinWindowSize())
    {
        Config::CHESS_NUMBER -= 2;
        return;
    }
    reload(centerPieces());
}

void MainWindow::reduceChessNumber()
{
    if (Config::CHESS_NUMBER - 2 > 0)
    {
        Config::CHESS_NUMBER -= 2;
        auto moves = centerPieces();
        for (auto [x, y] : moves)
        {
            if (x >= Config::CHESS_NUMBER || y >= Config::CHESS_NUMBER)
            {
                Config::CHESS_NUMBER += 2;
                return;
            }
        }
        reload(moves);
    }
}

void MainWindow::reload(const std::vector<point> &moveRecord)
{
    for (auto [x, y] : board->getMoveRecords())
    {
        widgets[x][y]->clear();
    }
    board->restart();
    reloadSize();
    for (auto [x, y] : moveRecord)
    {
        if (x < Config::CHESS_NUMBER && y < Config::CHESS_NUMBER)
        {
            widgets[x][y]->press();
        }
    }
    update();
}

void MainWindow::reloadSize()
{
    while (widgets.size() > Config::CHESS_NUMBER)
    {
        widgets.pop_back();
    }
    while (widgets.size() < Config::CHESS_NUMBER)
    {
        widgets.emplace_back();
    }
    for (int i = 0; i < Config::CHESS_NUMBER; i++)
    {
        while (widgets[i].size() > Config::CHESS_NUMBER)
        {
            widgets[i].pop_back();
        }
        while (widgets[i].size() < Config::CHESS_NUMBER)
        {
            int j = widgets[i].size();
            widgets[i].emplace_back(std::make_unique<Sensor>(this, board.get(), i, j, &widgets));
            double x, y;
            if (i == 0 && j == 0)
            {
                x = Config::BOARD_MARGIN() / 2;
                y = Config::BOARD_MARGIN() / 2;
            }
            else if (j == 0)
            {
                x = widgets[i - 1][j]->pos().x() + Config::BOARD_PIECE_SPACING;
                y = widgets[i - 1][j]->pos().y();
            }
            else
            {
                x = widgets[i][j - 1]->pos().x();
                y = widgets[i][j - 1]->pos().y() + Config::BOARD_PIECE_SPACING;
            }
            widgets[i][j]->move(x, y);
        }
    }
    fixSize();
    auto minBoardSize = Config::BOARD_SIZE();
    setMinimumSize(minBoardSize, minBoardSize);
    if (!isFullScreen())
    {
        resize(minBoardSize, minBoardSize);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    fixSize();
}

std::vector<point> MainWindow::centerPieces() const
{
    int maxX = 0;
    int maxY = 0;
    int minX = Config::CHESS_NUMBER;
    int minY = Config::CHESS_NUMBER;
    auto moveRecords = board->getMoveRecords();
    for (auto [x, y] : moveRecords)
    {
        maxX = std::max(x, maxX);
        maxY = std::max(y, maxY);
        minX = std::min(x, minX);
        minY = std::min(y, minY);
    }
    for (auto &[x, y] : moveRecords)
    {
        x = x + (Config::CHESS_NUMBER - maxX - minX) / 2;
        y = y + (Config::CHESS_NUMBER - maxY - minY) / 2;
    }
    return moveRecords;
}

double MainWindow::getMinWindowSize()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    auto screenWidth = screenGeometry.width();
    auto screenHeight = screenGeometry.height();
    auto minSize = std::min(screenHeight, screenWidth);
    return minSize - 50;
}

void MainWindow::fixSize()
{
    double width = size().width();
    double height = size().height();
    auto midX = width / 2;
    auto midY = height / 2;
    for (double i = 0; i < Config::CHESS_NUMBER; i++)
    {
        for (double j = 0; j < Config::CHESS_NUMBER; j++)
        {
            auto x = midX + (i - Config::CHESS_NUMBER / 2) * Config::BOARD_PIECE_SPACING - 0.5;
            auto y = midY + (j - Config::CHESS_NUMBER / 2) * Config::BOARD_PIECE_SPACING - 0.5;
            auto space = Config::BOARD_PIECE_SPACING + 1;

            auto *animation = new QPropertyAnimation(widgets[i][j].get(), "pos");
            animation->setDuration(200);
            animation->setStartValue(widgets[i][j]->pos());
            animation->setEndValue(QPoint(x, y));
            animation->start(QAbstractAnimation::DeleteWhenStopped);

            widgets[i][j]->setFixedSize(space, space);
            widgets[i][j]->show();
        }
    }
}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::ApplicationPaletteChange)
    {
        update();
    }
    return QMainWindow::event(event);
}
