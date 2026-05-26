#pragma once

#include <QObject>

enum class Direction { Up, Down, Left, Right };

// 静态元素表
struct ElementInfo {
    const char *symbol;
    double mass;
};

class Board {
public:
    Board();
    void reset();
    bool canMove() const;
    // 修改move，新增参数：结果合块的原子序序列
    bool move(Direction dir, std::vector<int>* merged = nullptr);
    void addRandomTile();
    int getMaxAtomicNumber() const;
    const int (*getGrid() const)[4];
    // 元素信息
    static const ElementInfo* getElementTable(int &count);
    static const char* elementSymbol(int atomicNumber);
    static double elementMass(int atomicNumber);
    void setGrid(const int inGrid[4][4]) {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                grid[i][j] = inGrid[i][j];
    }
private:
    int grid[4][4];
    bool slideAndMerge(int line[4], std::vector<int>* merged = nullptr);
};