#pragma once

#include <QObject>

enum class Direction { Up, Down, Left, Right };

class Board {
public:
    Board();
    void reset();
    bool canMove() const;
    bool move(Direction dir, std::vector<int>* merged = nullptr);
    void addRandomTile();
    int getMaxAtomicNumber() const;
    const int (*getGrid() const)[4];
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