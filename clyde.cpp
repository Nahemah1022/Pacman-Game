#include "clyde.h"
#include "mainwindow.h"

Clyde::Clyde(QPixmap pix, QPoint initialPoint, QWidget *parent) : Ghost (pix, initialPoint, parent)
{
    right = new QPixmap(":/img2/src/clyde_right.png");
    down = new QPixmap(":/img2/src/clyde_down.png");
    left = new QPixmap(":/img2/src/clyde_left.png");
    up = new QPixmap(":/img2/src/clyde_up.png");

    bfsTimer = new QTimer();
    bfsTimer->setInterval(bfsSearchInterval);
    connect(bfsTimer, SIGNAL(timeout()), this, SLOT(getTargetAndSearch()));
    bfsTimer->start();

    rebornTimer = new QTimer();
    rebornTimer->setInterval(5000);
    connect(rebornTimer, SIGNAL(timeout()), this, SLOT(reborn()));

}

void Clyde::getTargetAndSearch()
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

    if(getDistanceFromOther(*MainWindow::pacman) > 200*200){
        bfsTimer->setInterval(100);
        trackingTargetIndex = MainWindow::pacman->getCoor();
    }
    else{
        bfsTimer->setInterval(2000);
        trackingTargetIndex = qrand()%753;
    }
    if(rebornTimer->isActive()){
        trackingTargetIndex = 14*26+10;
        moveSpeed = 25;
    }

}

void Clyde::reborn()
{
    MainWindow::scene->addItem(MainWindow::clyde = new Clyde(QPixmap(":/img2/src/clyde_left.png"), MainWindow::point[14][10]));
    disconnect(rebornTimer, SIGNAL(timeout()), this, SLOT(reborn()));
    this->~Clyde();
}
