#include "pinky.h"
#include "mainwindow.h"

Pinky::Pinky(QPixmap pix, QPoint initialPoint, QWidget *parent) : Ghost (pix, initialPoint, parent)
{
    right = new QPixmap(":/img2/src/pinky_right.png");
    down = new QPixmap(":/img2/src/pinky_down.png");
    left = new QPixmap(":/img2/src/pinky_left.png");
    up = new QPixmap(":/img2/src/pinky_up.png");

    bfsTimer = new QTimer();
    bfsTimer->setInterval(bfsSearchInterval);
    connect(bfsTimer, SIGNAL(timeout()), this, SLOT(getTargetAndSearch()));
    bfsTimer->start();

    rebornTimer = new QTimer();
    rebornTimer->setInterval(5000);
    connect(rebornTimer, SIGNAL(timeout()), this, SLOT(reborn()));
}

void Pinky::getTargetAndSearch()
{
    QList<QGraphicsItem *> list = collidingItems();
    foreach(QGraphicsItem *i , list){
        Pacman *pacman = dynamic_cast<Pacman *>(i);
        if(pacman && pacman->isPowered){
            isDying = true;
            setPixmap(*dyingPix);
            right = dyingPix;
            down = dyingPix;
            left = dyingPix;
            up = dyingPix;
            if(!rebornTimer->isActive()){
                rebornTimer->start(5000);
            }
        }
        else if(pacman){
            pacman->~Pacman();
            emit pacman->gameoverSignal();
        }
    }

    int pacmanIndex = MainWindow::pacman->getCoor();
    if(getDistanceFromOther(*MainWindow::pacman) < 10000){
        trackingTargetIndex = pacmanIndex;
    }
    else if(MainWindow::pacman->getDirection() == 1 && MainWindow::pacman->getCoor()%26<26-TrackingOffset){
        trackingTargetIndex = pacmanIndex+TrackingOffset;
    }
    else if(MainWindow::pacman->getDirection() == 1 && MainWindow::pacman->getCoor()%26>25-TrackingOffset){
        trackingTargetIndex = (pacmanIndex/26)*26+25;
    }
    else if(MainWindow::pacman->getDirection() == 2 && MainWindow::pacman->getCoor()/26<29-TrackingOffset){
        trackingTargetIndex = pacmanIndex+TrackingOffset*26;
    }
    else if(MainWindow::pacman->getDirection() == 2 && MainWindow::pacman->getCoor()/26>28-TrackingOffset){
        trackingTargetIndex = 28*26+pacmanIndex%26;
    }
    else if(MainWindow::pacman->getDirection() == 3 && MainWindow::pacman->getCoor()%26>TrackingOffset-1){
        trackingTargetIndex = pacmanIndex -= TrackingOffset;
    }
    else if(MainWindow::pacman->getDirection() == 3 && MainWindow::pacman->getCoor()%26<TrackingOffset){
        trackingTargetIndex = (pacmanIndex/26)*26;
    }
    else if(MainWindow::pacman->getDirection() == 4 && MainWindow::pacman->getCoor()/26>TrackingOffset-1){
        trackingTargetIndex = pacmanIndex-TrackingOffset*26;
    }
    else if(MainWindow::pacman->getDirection() == 4 && MainWindow::pacman->getCoor()/26<TrackingOffset){
        trackingTargetIndex = pacmanIndex%26;
    }
    else{
        trackingTargetIndex = 0;
    }
    if(rebornTimer->isActive()){
        trackingTargetIndex = 12*26+10;
        moveSpeed = 25;
    }
}

void Pinky::reborn()
{
    MainWindow::scene->addItem(MainWindow::pinky = new Pinky(QPixmap(":/img2/src/pinky_left.png"), MainWindow::point[12][10]));
    disconnect(rebornTimer, SIGNAL(timeout()), this, SLOT(reborn()));
    this->~Pinky();
}
