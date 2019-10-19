#include "powerpellet.h"

PowerPellet::PowerPellet(QPoint initialPoint, int width, int height) : QGraphicsEllipseItem (initialPoint.x(), initialPoint.y(), width, height)
{
    setPos(pos().x()-rect().width()+15, pos().y()-rect().height()+15);
    setBrush(Qt::yellow);
}
