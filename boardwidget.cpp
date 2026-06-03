#include "boardwidget.h"
#include "ElementData.h"
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
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRect rect = this->rect();
    int side = std::min(rect.width(), rect.height());
    int cellSide = side / 4;
    int margin = cellSide / 16;          // 更细的间距，让格子呼吸
    int borderRadius = 8;

    // ── 棋盘背景 ──
    painter.setBrush(QColor(187, 173, 160));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(0, 0, side, side, 6, 6);

    if (!board) return;

    auto grid = board->getGrid();
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            int x = j * cellSide;
            int y = i * cellSide;
            int atomicNumber = grid[i][j];

            // ── 格子颜色（来自共享元素表） ──
            QColor tileColor = (atomicNumber >= 0 && atomicNumber < kElementCount)
                ? kElementDetails[atomicNumber].tileColor
                : QColor(60, 50, 40);

            painter.setBrush(tileColor);
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(
                x + margin, y + margin,
                cellSide - 2 * margin, cellSide - 2 * margin,
                borderRadius, borderRadius
            );

            // ── 文字 ──
            if (atomicNumber) {
                // 元素符号 (大号)
                QFont font("Microsoft YaHei", cellSide / 3);
                font.setBold(true);
                painter.setFont(font);

                QString elSymbol = QString::fromUtf8(Board::elementSymbol(atomicNumber));

                // 浅色格子用深字，深色格子用白字
                painter.setPen(atomicNumber <= 2
                    ? QColor(60, 58, 50)
                    : QColor(249, 246, 242));

                int textH = cellSide / 2;
                painter.drawText(x, y + cellSide / 6, cellSide, textH,
                                 Qt::AlignHCenter | Qt::AlignVCenter, elSymbol);

                // 原子质量 (小号)
                QFont massFont("Microsoft YaHei", cellSide / 8);
                massFont.setBold(false);
                painter.setFont(massFont);

                double mass = Board::elementMass(atomicNumber);
                QString massTxt = QString::number(mass, 'f', 2);

                painter.setPen(atomicNumber <= 2
                    ? QColor(120, 110, 100)
                    : QColor(249, 246, 242, 200));

                painter.drawText(x, y + cellSide * 3 / 5, cellSide, cellSide / 3,
                                 Qt::AlignHCenter | Qt::AlignTop, massTxt);
            }
        }
}
