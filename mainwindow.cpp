#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMessageBox>
#include <QFont>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *topLayout = new QHBoxLayout;
    boardWidget = new BoardWidget(this);
    scoreLabel = new QLabel("Score: 0", this);

    QFont font = scoreLabel->font();
    font.setPointSize(16);
    scoreLabel->setFont(font);

    restartButton = new QPushButton("Restart", this);

    topLayout->addWidget(scoreLabel);
    topLayout->addStretch();
    topLayout->addWidget(restartButton);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(boardWidget);

    central->setLayout(mainLayout);

    resize(450, 550);
    setWindowTitle("2048 Qt");

    connect(restartButton, &QPushButton::clicked, this, &MainWindow::restartGame);

    game.init();
    boardWidget->setBoard(&game.getBoard());
    updateUI();
}

void MainWindow::restartGame() {
    game.init();
    updateUI();
}

void MainWindow::updateUI() {
    boardWidget->setBoard(&game.getBoard());
    scoreLabel->setText(QString("Score: %1").arg(game.getScore()));
}

void MainWindow::handleMove(Direction dir) {
    if (game.isOver())
        return;
    bool moved = game.move(dir);
    if (moved)
        updateUI();
    if (game.isOver())
        showGameOver();
}

void MainWindow::showGameOver() {
    QMessageBox::information(this, "Game Over", QString("Game over!\nFinal score: %1").arg(game.getScore()));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_A:
        handleMove(Direction::Left);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        handleMove(Direction::Right);
        break;
    case Qt::Key_Up:
    case Qt::Key_W:
        handleMove(Direction::Up);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        handleMove(Direction::Down);
        break;
    default:
        QMainWindow::keyPressEvent(event);
        break;
    }
}
