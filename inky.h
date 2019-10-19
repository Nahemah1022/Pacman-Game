#ifndef INKY_H
#define INKY_H

#include "ghost.h"

class Inky : public Ghost
{
    Q_OBJECT
public:
    Inky(QPixmap pix, QPoint initialPoint, QWidget *parent = nullptr);

private:
    int TrackingOffset = 4;

private slots:
    void getTargetAndSearch();
    void reborn();
};

#endif // INKY_H
