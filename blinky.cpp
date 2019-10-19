#include "blinky.h"
#include "mainwindow.h"
#include "onlinegamewindow.h"

Blinky::Blinky(QPixmap pix, QPoint initialPoint, QWidget *parent) : Ghost (pix, initialPoint, parent)
{
    right = new QPixmap(":/img2/src/blinky_right.png");
    down = new QPixmap(":/img2/src/blinky_down.png");
    left = new QPixmap(":/img2/src/blinky_left.png");
    up = new QPixmap(":/img2/src/blinky_up.png");

    bfsTimer = new QTimer();
    bfsTimer->setInterval(bfsSearchInterval);
    connect(bfsTimer, SIGNAL(timeout()), this, SLOT(getTargetAndSearch()));
    bfsTimer->start();

    rebornTimer = new QTimer();
    connect(rebornTimer, SIGNAL(timeout()), this, SLOT(reborn()));
}

Blinky::~Blinky()
{

}

void Blinky::getTargetAndSearch()
{
    QList<QGraphicsItem *> list = collidingItems();
    foreach(QGraphicsItem *i , list){
        Pacman *pacman = dynamic_cast<Pacman *>(i);
        if(pacman && pacman->isPowered){
            setPixmap(*dyingPix);
            isDying = true;
            right = dyingPix;
            down = dyingPix;
            left = dyingPix;
            up = dyingPix;
            if(!rebornTimer->isActive()){
                rebornTimer->start(5000);
            }
        }
        else if(pacman){
            emit pacman->gameoverSignal();
            pacman->~Pacman();
        }
    }

        trackingTargetIndex = MainWindow::pacman->getCoor();
    if(rebornTimer->isActive()){
        trackingTargetIndex = 12*26+15;
        moveSpeed = 25;
    }
}

void Blinky::reborn()
{
    MainWindow::scene->addItem(MainWindow::blinky = new Blinky(QPixmap(":/img2/src/blinky_left.png"), MainWindow::point[12][15]));
    disconnect(rebornTimer, SIGNAL(timeout()), this, SLOT(reborn()));
    this->~Blinky();
}
