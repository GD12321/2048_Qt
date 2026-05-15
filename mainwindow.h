#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
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
private:
    BoardWidget *boardWidget;
    QLabel *scoreLabel;
    QPushButton *restartButton;
    Game2048 game;
    void updateUI();
    void handleMove(Direction dir);
    void showGameOver();
};