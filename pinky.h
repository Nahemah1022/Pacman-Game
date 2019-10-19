#ifndef PINKY_H
#define PINKY_H

#include "ghost.h"

class Pinky : public Ghost
{
    Q_OBJECT
public:
    Pinky(QPixmap pix, QPoint initialPoint, QWidget *parent = nullptr);

private:
    int TrackingOffset = 4;

private slots:
    void getTargetAndSearch();
    void reborn();
};


#endif // PINKY_H
