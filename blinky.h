#ifndef BLINKY_H
#define BLINKY_H

#include "ghost.h"

class Blinky : public Ghost
{
    Q_OBJECT
public:
    Blinky(QPixmap pix, QPoint initialPoint, QWidget *parent = nullptr);
    ~Blinky();

private slots:
    void getTargetAndSearch();
    void reborn();
};

#endif // BLINKY_H
