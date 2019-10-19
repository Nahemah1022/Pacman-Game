#ifndef CLYDE_H
#define CLYDE_H

#include "ghost.h"

class Clyde : public Ghost
{
    Q_OBJECT
public:
    Clyde(QPixmap pix, QPoint initialPoint, QWidget *parent = nullptr);

private slots:
    void getTargetAndSearch();
    void reborn();
};

#endif // CLYDE_H
