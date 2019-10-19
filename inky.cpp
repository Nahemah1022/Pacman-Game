#include "inky.h"
#include "mainwindow.h"

Inky::Inky(QPixmap pix, QPoint initialPoint, QWidget *parent) : Ghost (pix, initialPoint, parent)
{
    right = new QPixmap(":/img2/src/inky_right.png");
    down = new QPixmap(":/img2/src/inky_down.png");
    left = new QPixmap(":/img2/src/inky_left.png");
    up = new QPixmap(":/img2/src/inky_up.png");

    bfsTimer = new QTimer();
    bfsTimer->setInterval(bfsSearchInterval);
    connect(bfsTimer, SIGNAL(timeout()), this, SLOT(getTargetAndSearch()));
    bfsTimer->start();

    rebornTimer = new QTimer();
    rebornTimer->setInterval(5000);
    connect(rebornTimer, SIGNAL(timeout()), this, SLOT(reborn()));

}

void Inky::getTargetAndSearch()
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
    if(getDistanceFromOther(*MainWindow::pacman) < 10000){
        trackingTargetIndex = pacmanIndex;
    }
    else {
        int dx = trackingTargetIndex%26-MainWindow::blinky->getCoor()%26;
        int dy = trackingTargetIndex/26-MainWindow::blinky->getCoor()/26;
        int x, y;
        if(trackingTargetIndex%26+dx > 25){
            x = 25;
        }
        else if(trackingTargetIndex%26+dx < 0){
            x = 0;
        }
        else{
            x = trackingTargetIndex%26+dx;
        }

        if(trackingTargetIndex/26+dy > 28){
            y = 28;
        }
        else if(trackingTargetIndex/26+dy < 0){
            y = 0;
        }
        else {
            y = trackingTargetIndex/26+dy;
        }
        trackingTargetIndex = y*26+x;
    }
    if(rebornTimer->isActive()){
        trackingTargetIndex = 14*26+15;
        moveSpeed = 25;
    }
}

void Inky::reborn()
{
    MainWindow::scene->addItem(MainWindow::inky = new Inky(QPixmap(":/img2/src/inky_left.png"), MainWindow::point[14][15]));
    disconnect(rebornTimer, SIGNAL(timeout()), this, SLOT(reborn()));
    this->~Inky();
}
