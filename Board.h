#pragma once

#include <QObject>

enum class Direction { Up, Down, Left, Right };

class Board {
public:
    Board();
    void reset();
    bool canMove() const;
    bool move(Direction dir); // 返回是否有变动
    void addRandomTile();
    int getMaxTile() const;
    const int (*getGrid() const)[4];
private:
    int grid[4][4];
    bool slideAndMerge(int line[4]);
};