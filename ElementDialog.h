#pragma once

#include <QDialog>

class ElementDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ElementDialog(int atomicNumber, QWidget *parent = nullptr);
};
