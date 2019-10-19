#include "pacman.h"
#include "mainwindow.h"

Pacman::Pacman(QWidget *parent) : MovableItem (parent)
{
    pix1 = new QPixmap(":/img2/src/pacman_fianl.png");
    pix2 = new QPixmap(":/img2/src/pacman_half.png");
    pix3 = new QPixmap(":/img2/src/pacman_full.png");
    setPixmap(*pix1);
    setScale(1);
    setOffset(-pix1->width()/2,-pix1->height()/2);
    setPos(MainWindow::point[0][0]);

    movingTimer = new QTimer();
    movingTimer->setInterval(100);
    connect(movingTimer, SIGNAL(timeout()), this, SLOT(movement()));
    movingTimer->start();

    coorChangeTimer = new QTimer();
    coorChangeTimer->setInterval(100);
    connect(coorChangeTimer, SIGNAL(timeout()), this, SLOT(coorChanging()));
    coorChangeTimer->start();

    pixTimer = new QTimer();
    pixTimer->setInterval(70);
    connect(pixTimer, SIGNAL(timeout()), this, SLOT(changePix()));
    pixTimer->start();

    powerProgressBar = new QProgressBar();
    powerProgressBar->setRange(1, 70);
    powerProgressBar->setValue(70);
    powerProgressBar->setTextVisible(false);
    powerProgressBar->setGeometry(880, 650, 120, 25);
    powerTimerForBar = new QTimer(this);
    connect(powerTimerForBar, &QTimer::timeout, [=](){
        powerProgressBar->setValue(powerProgressBar->value()-1);
    });
    //powerProgressBar->show();

    powerTimer = new QTimer(this);
    connect(powerTimer, &QTimer::timeout, [=](){
        isPowered = false;
        emit frightenSignal(false);
    });


    direction = 4;
    direction = 0;

    moveSpeed = 15;
}

Pacman::Pacman(unsigned short ipPort, QWidget *parent) : MovableItem(ipPort, parent)
{
    pix1 = new QPixmap(":/img2/src/pacman_fianl.png");
    pix2 = new QPixmap(":/img2/src/pacman_half.png");
    pix3 = new QPixmap(":/img2/src/pacman_full.png");
    setPixmap(*pix1);
    setScale(1);
    setOffset(-pix1->width()/2,-pix1->height()/2);

    movingTimer = new QTimer();
    movingTimer->setInterval(100);
    connect(movingTimer, SIGNAL(timeout()), this, SLOT(movement()));
    movingTimer->start();

    coorChangeTimer = new QTimer();
    coorChangeTimer->setInterval(100);
    connect(coorChangeTimer, SIGNAL(timeout()), this, SLOT(coorChanging()));
    coorChangeTimer->start();

    pixTimer = new QTimer();
    pixTimer->setInterval(70);
    connect(pixTimer, SIGNAL(timeout()), this, SLOT(changePix()));
    pixTimer->start();

    powerProgressBar = new QProgressBar();
    powerProgressBar->setRange(1, 70);
    powerProgressBar->setValue(70);
    powerProgressBar->setTextVisible(false);
    powerProgressBar->setGeometry(880, 650, 120, 25);
    powerTimerForBar = new QTimer(this);
    connect(powerTimerForBar, &QTimer::timeout, [=](){
        powerProgressBar->setValue(powerProgressBar->value()-1);
    });
    //powerProgressBar->show();

    powerTimer = new QTimer(this);
    connect(powerTimer, &QTimer::timeout, [=](){
        isPowered = false;
        emit frightenSignal(false);
    });

    direction = 4;
    direction = 0;

    moveSpeed = 15;
}

Pacman::Pacman(QString ipAddress, unsigned short ipPort, QWidget *parent) : MovableItem(ipAddress, ipPort, parent)
{
    pix1 = new QPixmap(":/img2/src/pacman_fianl.png");
    pix2 = new QPixmap(":/img2/src/pacman_half.png");
    pix3 = new QPixmap(":/img2/src/pacman_full.png");
    setPixmap(*pix1);
    setScale(1);
    setOffset(-pix1->width()/2,-pix1->height()/2);
    setPos(MainWindow::point[0][0]);

    movingTimer = new QTimer();
    movingTimer->setInterval(100);
    connect(movingTimer, SIGNAL(timeout()), this, SLOT(movement()));
    movingTimer->start();

    coorChangeTimer = new QTimer();
    coorChangeTimer->setInterval(100);
    connect(coorChangeTimer, SIGNAL(timeout()), this, SLOT(coorChanging()));
    coorChangeTimer->start();

    pixTimer = new QTimer();
    pixTimer->setInterval(70);
    connect(pixTimer, SIGNAL(timeout()), this, SLOT(changePix()));
    pixTimer->start();

    powerProgressBar = new QProgressBar();
    powerProgressBar->setRange(1, 70);
    powerProgressBar->setValue(70);
    powerProgressBar->setTextVisible(false);
    powerProgressBar->setGeometry(880, 650, 120, 25);
    powerTimerForBar = new QTimer(this);
    connect(powerTimerForBar, &QTimer::timeout, [=](){
        powerProgressBar->setValue(powerProgressBar->value()-1);
    });
    //powerProgressBar->show();

    powerTimer = new QTimer(this);
    connect(powerTimer, &QTimer::timeout, [=](){
        isPowered = false;
        emit frightenSignal(false);
    });


    direction = 4;
    direction = 0;

    moveSpeed = 15;
}

void Pacman::movement()
{
/*
    if(isStop && !pacClient && !pacServer){
        return ;
    }
*/
    if(getNearPointIndex(pos().toPoint()) == 338){
        setPos(MainWindow::point[13][24]);
    }
    if(getNearPointIndex(pos().toPoint()) == 363){
        setPos(MainWindow::point[13][1]);
    }

    QList<QGraphicsItem *> list = collidingItems();
    foreach(QGraphicsItem *i , list){
        QGraphicsEllipseItem *scorePoint= dynamic_cast<QGraphicsEllipseItem *>(i);
        PowerPellet *powerPellet = dynamic_cast<PowerPellet *>(i);
        if(scorePoint){
            scorePoint->hide();
            MainWindow::score+=10;
            emit scorePlus();
        }
        if(powerPellet){
            isPowered = true;
            powerPellet->~PowerPellet();
            powerTimer->start(7000);
            powerProgressBar->setValue(70);
            powerTimerForBar->start(100);
            emit frightenSignal(true);
        }
    }
    switch (getDirection()) {
    case 1:
        if(x()<795){
            if(!MainWindow::isPoint[coor/26][coor%26+1] && MainWindow::isPoint[coor/26][coor%26] == 2){
                setPos(MainWindow::point[coor/26][coor%26]);
                isStop = true;
                return ;
            }
            moveBy(moveSpeed, 0);

        }
        break;
    case 2:
        if(y()<937){
            if(!MainWindow::isPoint[coor/26+1][coor%26] && MainWindow::isPoint[coor/26][coor%26] == 2){
                setPos(MainWindow::point[coor/26][coor%26]);
                isStop = true;
                return ;
            }
            moveBy(0, moveSpeed);
        }
        break;
    case 3:
        if(x()>45){
            if(!MainWindow::isPoint[coor/26][coor%26-1] && MainWindow::isPoint[coor/26][coor%26] == 2){
                setPos(MainWindow::point[coor/26][coor%26]);
                isStop = true;
                return ;
            }
            moveBy(-moveSpeed, 0);
        }
        break;
    case 4:
        if(y()>130){
            if(!MainWindow::isPoint[coor/26-1][coor%26] && MainWindow::isPoint[coor/26][coor%26] == 2){
                //setPos(MainWindow::point[coor/26][coor%26]);
                isStop = true;
                return ;
            }
            moveBy(0, -moveSpeed);
        }
    }
}

void Pacman::changePix()
{
    if(pixStatus == 0){
        QPixmap pix(":/img2/src/pacman_fianl.png");
        transform = new QTransform();
        transform->rotate((getDirection()-1)*90);
        *pix1 = pix.transformed(*transform);
        setPixmap(*pix1);
        pixStatus = 1;
    }
    else if(pixStatus == 1){
        QPixmap pix(":/img2/src/pacman_half.png");
        transform = new QTransform();
        transform->rotate((getDirection()-1)*90);
        *pix2 = pix.transformed(*transform);
        setPixmap(*pix2);
        pixStatus = 2;
    }
    else {
        QPixmap pix(":/img2/src/pacman_full.png");
        transform = new QTransform();
        transform->rotate((getDirection()-1)*90);
        *pix3 = pix.transformed(*transform);
        setPixmap(*pix3);
        pixStatus = 0;
    }
}

void Pacman::coorChanging()
{
    coor = getNearPointIndex(pos().toPoint());
}
