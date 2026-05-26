#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include "boardwidget.h"
#include "Game2048.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
protected:
    void keyPressEvent(QKeyEvent *event) override;
private slots:
    void restartGame();
    void menuNewGame();
private:
    BoardWidget *boardWidget;
    QLabel *scoreLabel;
    QPushButton *restartButton;
    Game2048 game;

    // menu bar related
    QMenuBar* menuBar_;
    QMenu* gameMenu_;
    QAction* newGameAction_;

    void updateUI();
    void handleMove(Direction dir);
    void showGameOver();
};