#pragma once
#include "Board.h"
#include <QSettings>

class Game2048 {
public:
    Game2048();
    void init();
    bool move(Direction dir);  // 返回是否有变化
    bool isOver() const;
    int getScore() const;
    int getHighScore() const;
    Board& getBoard();
    void setScore(int s) { score = s; }

    bool canUndo() const;
    void undo();               // 撤销上一步

private:
    Board board;
    int score;
    int highScore;

    // 撤销用
    bool hasUndoState = false;
    int  undoGrid[4][4];
    int  undoScore;

    void saveUndoState();
    void loadHighScore();
    void saveHighScore();
};