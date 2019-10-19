#ifndef MOVABLEITEM_H
#define MOVABLEITEM_H

#include <QKeyEvent>
#include <QWidget>
#include <QObject>
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QTcpServer>
#include <QTcpSocket>

#include "server.h"
#include "client.h"

class Pacman;

class MovableItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    MovableItem(QWidget *parent = nullptr);
    MovableItem(unsigned short ipPort, QWidget *parent = nullptr);
    MovableItem(QString ipAddress, unsigned short ipPort, QWidget *parent = nullptr);
    void setDirection(int direction);

    int getDirection();
    int getCoor();
    int getMoveSpeed();
    QPoint getNearPoint(QPoint currentPosition);
    int getNearPointIndex(QPoint currentPoint);
    qreal getDistanceFromOther(const MovableItem &other);
    double getDistanceFromNearPoint();

    bool isAtPoint(QPoint currentPoint);
    void stop();
    virtual void restart();

    QTimer *movingTimer = nullptr;
    virtual ~MovableItem();

    myServer *itemServer = nullptr;
    Client *itemClient = nullptr;

public slots:
    virtual void movement() = 0;
    virtual void coorChanging() = 0;

protected:
    int direction = 0;
    QTimer *coorChangeTimer = nullptr;
    int moveSpeed = 10;
    int coor;
    bool isStop = false;

private:
    int xIndex, yIndex;
    QWidget *parent;

private slots:
    void dataRecieve(int data);

signals:
    void scorePlus();
};

#endif // MOVABLEITEM_H
