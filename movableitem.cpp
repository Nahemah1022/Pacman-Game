#include "movableitem.h"
#include "mainwindow.h"

MovableItem::MovableItem(QWidget *parent)
{
    this->parent = parent;
}

MovableItem::MovableItem(unsigned short ipPort, QWidget *parent)
{
    this->parent = parent;
    itemServer = new myServer(ipPort, this);
    connect(itemServer, SIGNAL(dataRecieved(int)), this, SLOT(dataRecieve(int)));
}

MovableItem::MovableItem(QString ipAddress, unsigned short ipPort, QWidget *parent)
{
    this->parent = parent;
    itemClient = new Client(ipAddress, ipPort, this);
    connect(itemClient, SIGNAL(dataRecieved(int)), this, SLOT(dataRecieve(int)));
}

QPoint MovableItem::getNearPoint(QPoint currentPosition)
{
    for(int i=0; i<29; ++i){
        for(int j=0; j<26; ++j){
            if(currentPosition == MainWindow::point[i][j]){
                return currentPosition;
            }
        }
    }
    return currentPosition;
}

int MovableItem::getNearPointIndex(QPoint currentPoint)
{   
    xIndex = (currentPoint.x()-45)/30;
    yIndex = (currentPoint.y()-125)/29;

    if(direction == 4){
        yIndex = (currentPoint.y()-100)/29;
    }

    if(direction == 3){
        xIndex = (currentPoint.x()-20)/30;
    }

    return yIndex*26 + xIndex;
}

qreal MovableItem::getDistanceFromOther(const MovableItem &other)
{
    return (x() - other.x()) * (x() - other.x()) + (y() - other.y()) * (y() - other.y());
}

double MovableItem::getDistanceFromNearPoint()
{
    if(direction == 1 || direction == 3){
        return abs(getNearPoint(pos().toPoint()).x() - pos().x());
    }
    else{
        return abs(getNearPoint(pos().toPoint()).y() - pos().y());
    }
}

bool MovableItem::isAtPoint(QPoint currentPoint)
{
    for(int i=0; i<20; ++i){
        for(int j=0; j<17; ++j){
            if(currentPoint.x() == MainWindow::point[i][j].x() && currentPoint.y() == MainWindow::point[i][j].y()){
                return true;
            }
        }
    }
    return false;
}

int MovableItem::getDirection()
{
    return direction;
}

int MovableItem::getCoor()
{
    return coor;
}

int MovableItem::getMoveSpeed()
{
    return moveSpeed;
}

void MovableItem::stop()
{
    isStop = true;
    direction = 0;
}

void MovableItem::restart()
{
    isStop = false;
}

MovableItem::~MovableItem()
{

}

void MovableItem::dataRecieve(int data)
{
    if(itemServer != nullptr){
        itemServer->player->setDirection(data/1000);
        int currentIndex = data%1000;
        itemServer->player->setPos(MainWindow::point[currentIndex/26][currentIndex%26]);
    }
    else {
        itemClient->player->setDirection(data/1000);
        int currentIndex = data%1000;
        itemClient->player->setPos(MainWindow::point[currentIndex/26][currentIndex%26]);
    }
}

void MovableItem::setDirection(int direction)
{
    this->direction = direction;

    if(itemClient != nullptr){
        itemClient->writeOut(direction*1000 + getNearPointIndex(pos().toPoint()));
    }
    else if(itemServer != nullptr){
        itemServer->writeOut(direction*1000 + getNearPointIndex(pos().toPoint()));
    }
}
