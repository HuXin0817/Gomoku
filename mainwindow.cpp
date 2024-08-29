#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    reset();
    setWindowOpacity(0.95);

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
    double width = size().width();
    double height = size().height();
    double midX = width / 2;
    double midY = height / 2;
    QMainWindow::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), BackGroundColor);
    painter.setPen(QPen(LineColor, Config::BOARD_LINE_WIDTH()));
    for (double i = 0; i < Config::CHESS_NUMBER; i++)
    {
        for (double j = 0; j < Config::CHESS_NUMBER; j++)
        {
            double x = midX + (i - Config::CHESS_NUMBER / 2) * Config::BOARD_PIECE_SPACING + Config::BOARD_PIECE_SPACING / 2;
            double y = midY + (j - Config::CHESS_NUMBER / 2) * Config::BOARD_PIECE_SPACING + Config::BOARD_PIECE_SPACING / 2;
            QPointF point1(x, y);
            if (i + 1 < Config::CHESS_NUMBER)
            {
                QPointF point2(x + Config::BOARD_PIECE_SPACING, y);
                painter.drawLine(point1, point2);
            }
            if (j + 1 < Config::CHESS_NUMBER)
            {
                QPointF point2(x, y + Config::BOARD_PIECE_SPACING);
                painter.drawLine(point1, point2);
            }
        }
    }
    painter.setBrush(LineColor);
    auto starPositions = Config::StarPositions();
    for (double i : starPositions)
    {
        for (double j : starPositions)
        {
            double x = midX + (i - Config::CHESS_NUMBER / 2) * Config::BOARD_PIECE_SPACING + Config::BOARD_PIECE_SPACING / 2;
            double y = midY + (j - Config::CHESS_NUMBER / 2) * Config::BOARD_PIECE_SPACING + Config::BOARD_PIECE_SPACING / 2;
            QPointF point(x, y);
            painter.drawEllipse(point, Config::BOARD_STAR_POINT_WIDTH(), Config::BOARD_STAR_POINT_WIDTH());
        }
    }
}

void MainWindow::restart() { reload({}); }

void MainWindow::undo()
{
    if (board->getMoveRecords().empty())
    {
        return;
    }
    auto back = board->undo();
    widgets[back.x][back.y]->clear();
}

void MainWindow::addBoardSize()
{
    Config::BOARD_PIECE_SPACING *= 1.1;
    if (Config::BOARD_SIZE() > getMinWindowSize())
    {
        Config::BOARD_PIECE_SPACING /= 1.1;
        return;
    }
    reload(board->getMoveRecords());
}

void MainWindow::reduceBoardSize()
{
    if (Config::BOARD_PIECE_SPACING / 1.1 > 25)
    {
        Config::BOARD_PIECE_SPACING /= 1.1;
        reload(board->getMoveRecords());
    }
    else if (Config::BOARD_PIECE_SPACING / 1.1 < 25)
    {
        Config::BOARD_PIECE_SPACING = 25;
        reload(board->getMoveRecords());
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
    reset();
    for (auto [x, y] : moveRecord)
    {
        if (x < Config::CHESS_NUMBER && y < Config::CHESS_NUMBER)
        {
            widgets[x][y]->press();
        }
    }
    update();
}

void MainWindow::reset()
{
    widgets.clear();
    board = std::make_unique<Board>();
    widgets = std::vector<std::vector<std::unique_ptr<Sensor>>>(Config::CHESS_NUMBER);
    for (auto &w : widgets)
    {
        w = std::vector<std::unique_ptr<Sensor>>(Config::CHESS_NUMBER);
    }
    for (int i = 0; i < Config::CHESS_NUMBER; i++)
    {
        for (int j = 0; j < Config::CHESS_NUMBER; j++)
        {
            widgets[i][j] = std::make_unique<Sensor>(this, board.get(), i, j, &widgets);
        }
    }
    moveWidgets();
    int minBoardSize = Config::BOARD_SIZE();
    setMinimumSize(minBoardSize, minBoardSize);
    if (!isFullScreen())
    {
        resize(minBoardSize, minBoardSize);
    }
    show();
}

void MainWindow::moveWidgets()
{
    double width = size().width();
    double height = size().height();
    double midX = width / 2;
    double midY = height / 2;
    for (double i = 0; i < Config::CHESS_NUMBER; i++)
    {
        for (double j = 0; j < Config::CHESS_NUMBER; j++)
        {
            double x = midX + (i - Config::CHESS_NUMBER / 2) * Config::BOARD_PIECE_SPACING;
            double y = midY + (j - Config::CHESS_NUMBER / 2) * Config::BOARD_PIECE_SPACING;
            widgets[i][j]->move(x, y);
            double space = Config::BOARD_PIECE_SPACING;
            widgets[i][j]->setFixedSize(space, space);
            widgets[i][j]->show();
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    moveWidgets();
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
        minY = std::min(x, minY);
    }
    for (auto &[x, y] : moveRecords)
    {
        x = x + (Config::CHESS_NUMBER - maxX - minX) / 2;
        y = y + (Config::CHESS_NUMBER - maxY - minY) / 2;
    }
    return moveRecords;
}

int MainWindow::getMinWindowSize() const
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    int minSize = std::min(screenHeight, screenWidth);
    return minSize - 50;
}
