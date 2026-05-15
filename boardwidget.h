#pragma once

#include <QWidget>
#include "Board.h"

class BoardWidget : public QWidget {
    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = nullptr);
    void setBoard(const Board* board);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    const Board* board = nullptr;
};