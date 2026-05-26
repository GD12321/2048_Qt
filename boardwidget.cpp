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
            int atomicNumber = grid[i][j];

            // 颜色
            QColor tileColor;
            switch (atomicNumber) {
            case 0: tileColor = QColor(205, 193, 180); break;
            case 1: tileColor = QColor(238, 228, 218); break;
            case 2: tileColor = QColor(237, 224, 200); break;
            default:
                tileColor = QColor(246 - std::min(atomicNumber * 5, 180), 177, 121); break;
            }
            painter.setBrush(tileColor);
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(x + margin, y + margin, cellSide - 2 * margin, cellSide - 2 * margin, 10, 10);

            // 字体调整
            if (atomicNumber) {
                // 大号字体显示元素符号
                QFont font = painter.font();
                font.setPointSize(cellSide / 3);
                font.setBold(true);
                painter.setFont(font);

                QString elSymbol = QString::fromUtf8(Board::elementSymbol(atomicNumber));
                painter.setPen(QColor(60, 58, 50));
                int textH = cellSide / 2; // 顶部高度

                painter.drawText(x, y + cellSide/6, cellSide, textH,
                                 Qt::AlignHCenter | Qt::AlignVCenter, elSymbol);

                // 小号字体显示质量（小数点两位）
                font.setPointSize(cellSide / 7);
                font.setBold(false);
                painter.setFont(font);

                double mass = Board::elementMass(atomicNumber);
                QString massTxt = QString::number(mass, 'f', 2);

                painter.setPen(QColor(120, 110, 100));
                painter.drawText(x, y + cellSide * 3 / 5, cellSide, cellSide/3,
                                 Qt::AlignHCenter | Qt::AlignTop, massTxt);
            }
        }
}