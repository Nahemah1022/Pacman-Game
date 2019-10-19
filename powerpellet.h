#ifndef POWERPELLET_H
#define POWERPELLET_H

#include <QObject>
#include <QGraphicsItem>
#include <QBrush>

class PowerPellet : public QGraphicsEllipseItem, public QObject
{
    //Q_OBJECT
public:
    PowerPellet(QPoint initialPoint, int width, int height);

signals:

public slots:
};

#endif // POWERPELLET_H
