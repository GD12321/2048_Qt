#pragma once
#include "Board.h"

class Game2048 {
public:
    Game2048();
    void init();
    bool move(Direction dir);  // 返回是否有变化
    bool isOver() const;
    int getScore() const;
    Board& getBoard();
    void setScore(int s) { score = s; }
private:
    Board board;
    int score;
};