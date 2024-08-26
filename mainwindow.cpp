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
    auto *addWindowSizeShortcut = new QShortcut(QKeySequence("Ctrl+="), this);
    auto *reduceWindowSizeShortcut = new QShortcut(QKeySequence("Ctrl+-"), this);

    connect(restartShortcut, &QShortcut::activated, this, &MainWindow::restart);
    connect(undoShortcut, &QShortcut::activated, this, &MainWindow::undo);
    connect(addBoardSizeShortcut, &QShortcut::activated, this, &MainWindow::addBoardSize);
    connect(reduceBoardSizeShortcut, &QShortcut::activated, this, &MainWindow::reduceBoardSize);
    connect(addChessNumberShortcut, &QShortcut::activated, this, &MainWindow::addChessNumber);
    connect(reduceChessNumberShortcut, &QShortcut::activated, this, &MainWindow::reduceChessNumber);
    connect(addWindowSizeShortcut, &QShortcut::activated, this, &MainWindow::addWindowSize);
    connect(reduceWindowSizeShortcut, &QShortcut::activated, this, &MainWindow::reduceWindowSize);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QSize newSize = size();
    int midX = newSize.width() / 2;
    int midY = newSize.height() / 2;
    QMainWindow::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), BackGroundColor);
    painter.setPen(QPen(LineColor, Config::BOARD_LINE_WIDTH()));
    for (int i = 0; i < Config::CHESS_NUMBER; i++)
    {
        for (int j = 0; j < Config::CHESS_NUMBER; j++)
        {
            double x = midX + (i - Config::CHESS_NUMBER / 2) * Config::BOARD_PIECE_SPACING;
            double y = midY + (j - Config::CHESS_NUMBER / 2) * Config::BOARD_PIECE_SPACING;
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
    for (int i : starPositions)
    {
        for (int j : starPositions)
        {
            double x = midX + (i - Config::CHESS_NUMBER / 2) * Config::BOARD_PIECE_SPACING;
            double y = midY + (j - Config::CHESS_NUMBER / 2) * Config::BOARD_PIECE_SPACING;
            QPointF point(x, y);
            painter.drawEllipse(point, Config::BOARD_STAR_POINT_WIDTH(), Config::BOARD_STAR_POINT_WIDTH());
        }
    }
}

void MainWindow::restart() { reload({}); }

void MainWindow::undo()
{
    auto moveRecord = board->getMoveRecords();
    if (moveRecord.empty())
    {
        return;
    }
    moveRecord.pop_back();
    reload(moveRecord);
}

void MainWindow::addBoardSize()
{
    Config::BOARD_PIECE_SPACING *= 1.1;
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
    reload(board->getMoveRecords());
}

void MainWindow::reduceChessNumber()
{
    if (Config::CHESS_NUMBER - 2 > 0)
    {
        Config::CHESS_NUMBER -= 2;
        reload(board->getMoveRecords());
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

void MainWindow::addWindowSize()
{
    int width = size().width() * 1.1;
    int height = size().height() * 1.1;
    resize(width, height);
}

void MainWindow::reduceWindowSize()
{
    int width = size().width() / 1.1;
    int height = size().height() / 1.1;
    resize(width, height);
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
    show();
}

void MainWindow::moveWidgets()
{
    int midX = size().width() / 2;
    int midY = size().height() / 2;
    for (int i = 0; i < Config::CHESS_NUMBER; i++)
    {
        for (int j = 0; j < Config::CHESS_NUMBER; j++)
        {
            int x = midX + (i - Config::CHESS_NUMBER / 2) * Config::BOARD_PIECE_SPACING - Config::BOARD_PIECE_SPACING / 2;
            int y = midY + (j - Config::CHESS_NUMBER / 2) * Config::BOARD_PIECE_SPACING - Config::BOARD_PIECE_SPACING / 2;
            widgets[i][j]->move(x, y);
            int space = Config::BOARD_PIECE_SPACING;
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
