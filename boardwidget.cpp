#include "boardwidget.h"
#include <QPainter>
#include <QBrush>
#include <QFont>
#include <QColor>
#include <QString>

BoardWidget::BoardWidget(QWidget *parent)
    : QWidget(parent), board(nullptr)
{
    setMinimumSize(320, 320);
}

void BoardWidget::setBoard(const Board* bd) {
    board = bd;
    update();
}

void BoardWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QRect rect = this->rect();
    int side = std::min(rect.width(), rect.height());
    int cellSide = side / 4;
    QFont font = painter.font();
    font.setPointSize(cellSide / 4);
    font.setBold(true);
    painter.setFont(font);

    painter.setRenderHint(QPainter::Antialiasing, true);

    // 绘制背景
    painter.setBrush(QColor(187, 173, 160));
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, side, side);

    if (!board) return;

    int margin = cellSide / 12;
    auto grid = board->getGrid();
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            int x = j * cellSide;
            int y = i * cellSide;
            int value = grid[i][j];

            // 颜色
            QColor tileColor;
            switch (value) {
            case 0: tileColor = QColor(205, 193, 180); break;
            case 2: tileColor = QColor(238, 228, 218); break;
            case 4: tileColor = QColor(237, 224, 200); break;
            case 8: tileColor = QColor(242, 177, 121); break;
            case 16: tileColor = QColor(245, 149, 99); break;
            case 32: tileColor = QColor(246, 124, 95); break;
            case 64: tileColor = QColor(246, 94, 59); break;
            case 128: tileColor = QColor(237, 207, 114); break;
            case 256: tileColor = QColor(237, 204, 97); break;
            case 512: tileColor = QColor(237, 200, 80); break;
            case 1024: tileColor = QColor(237, 197, 63); break;
            case 2048: tileColor = QColor(237, 194, 46); break;
            default: tileColor = QColor(60, 58, 50); break;
            }
            painter.setBrush(tileColor);
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(x + margin, y + margin, cellSide - 2 * margin, cellSide - 2 * margin, 10, 10);

            // 数字
            if (value) {
                painter.setPen(value <= 4 ? QColor(119, 110, 101) : QColor(249, 246, 242));
                painter.drawText(x, y, cellSide, cellSide, Qt::AlignCenter, QString::number(value));
            }
        }
}