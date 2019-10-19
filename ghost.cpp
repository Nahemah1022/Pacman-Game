#include "ghost.h"
#include "mainwindow.h"

Ghost::Ghost(QPixmap pix, QPoint initialPoint, QWidget *parent) : MovableItem (parent)
{
    setPixmap(pix);
    setScale(scale());
    setOffset(-pix.width()/2,-pix.height()/2);
    setPos(initialPoint);

    moveSpeed = 10;
    movingTimer = new QTimer();
    movingTimer->setInterval(100);
    connect(movingTimer, SIGNAL(timeout()), this, SLOT(movement()));
    movingTimer->start();

    coorChangeTimer = new QTimer();
    coorChangeTimer->setInterval(100);
    connect(coorChangeTimer, SIGNAL(timeout()), this, SLOT(coorChanging()));
    coorChangeTimer->start();

    setDirection(1);
    frightened = new QPixmap(":/img2/src/frightenedGhost.png");
    dyingPix = new QPixmap(":/img2/src/dyingGhost_1.png");
}

Ghost::~Ghost()
{

}

void Ghost::movement()
{
    if(coor > 753 || coor<0 || isStop){
        return ;
    }

    if(getNearPointIndex(pos().toPoint()) == 338){
        setPos(MainWindow::point[13][24]);
    }
    if(getNearPointIndex(pos().toPoint()) == 363){
        setPos(MainWindow::point[13][1]);
    }

    if(MainWindow::isPoint[coor/26][coor%26] == 2){
        BreadthFirstSearch();
        QPoint targetPoint = MainWindow::point[getNearPointIndex(pos().toPoint())/26][getNearPointIndex(pos().toPoint())%26];
        if(directionAfterBFS ==1 && getDirection() != 1 && MainWindow::isPoint[getCoor()/26][getCoor()%26+1]){
            if(isDying){
                setPixmap(*dyingPix);
            }
            else if(isFrightened){
                setPixmap(*frightened);
            }
            else{
                setPixmap(*right);
            }
            setDirection(1);
            setPos(targetPoint);
        }
        else if(directionAfterBFS == 2 && getDirection() != 2 && MainWindow::isPoint[getCoor()/26+1][getCoor()%26]){
            if(isDying){
                setPixmap(*dyingPix);
            }
            else if(isFrightened){
                setPixmap(*frightened);
            }
            else{
                setPixmap(*down);
            }
            setDirection(2);
            setPos(targetPoint);
        }
        else if(directionAfterBFS == 3 && getDirection() != 3 && MainWindow::isPoint[getCoor()/26][getCoor()%26-1]){
            if(isDying){
                setPixmap(*dyingPix);
            }
            else if(isFrightened){
                setPixmap(*frightened);
            }
            else{
                setPixmap(*left);
            }
            setDirection(3);
            setPos(targetPoint);
        }
        else if(directionAfterBFS == 4 && getDirection() != 4 && MainWindow::isPoint[getCoor()/26-1][getCoor()%26]){
            if(isDying){
                setPixmap(*dyingPix);
            }
            else if(isFrightened){
                setPixmap(*frightened);
            }
            else{
                setPixmap(*up);
            }
            setDirection(4);
            setPos(targetPoint);
        }
    }
/*
    if(isFrightened){
        direction = (direction+2)%4;
    }
*/
    switch (direction) {
    case 1:
        if(x()<795){
            if(!MainWindow::isPoint[coor/26][coor%26+1] && MainWindow::isPoint[coor/26][coor%26] == 2){
                setPos(MainWindow::point[coor/26][coor%26]);
                return ;
            }
            moveBy(moveSpeed, 0);
        }
        break;
    case 2:
        if(y()<937){
            if(!MainWindow::isPoint[coor/26+1][coor%26] && MainWindow::isPoint[coor/26][coor%26] == 2){
                setPos(MainWindow::point[coor/26][coor%26]);
                return ;
            }
            moveBy(0, moveSpeed);
        }
        break;
    case 3:
        if(x()>45){
            if(!MainWindow::isPoint[coor/26][coor%26-1] && MainWindow::isPoint[coor/26][coor%26] == 2){
                setPos(MainWindow::point[coor/26][coor%26]);
                return ;
            }
            moveBy(-moveSpeed, 0);

        }
        break;
    case 4:
        if(y()>125){
            if(!MainWindow::isPoint[coor/26-1][coor%26] && MainWindow::isPoint[coor/26][coor%26] == 2){
                setPos(MainWindow::point[coor/26][coor%26]);
                return ;
            }
            moveBy(0, -moveSpeed);

        }
        break;
    default:
        setDirection(0);
        return ;
    }

}

void Ghost::coorChanging()
{
    coor = getNearPointIndex(pos().toPoint());
}

void Ghost::restart()
{
    isStop = false;
    BreadthFirstSearch();
}

void Ghost::BreadthFirstSearch()
{
    for(int i=0; i<29; ++i){
        for(int j=0; j<26; ++j){
            if(MainWindow::isPoint[i][j]){
                bfsArray[i][j] = 1;
            }
            else{
                bfsArray[i][j] = 0;
            }
        }
    }
    getBFSPathValue(coor, 0, bfsArray);
}

int Ghost::getBFSPathValue(int currentPosIndex, int pathValue, int currentMap[29][26])
{
    if(pathValue == 35){
        return pathValue;
    }
    currentMap[currentPosIndex/26][currentPosIndex%26] = 0;
    int tempMap[29][26];
    for(int i=0; i<29; ++i){
        for(int j=0; j<26; ++j){
            tempMap[i][j] = currentMap[i][j];
        }
    }

    if(currentPosIndex == trackingTargetIndex){
        return pathValue;
    }

    int targetDirection[4] = {0};

    //Right
    if(currentPosIndex%26 != 25 && currentMap[currentPosIndex/26][currentPosIndex%26+1]){
        targetDirection[0] = getBFSPathValue((currentPosIndex/26)*26+currentPosIndex%26+1, pathValue+1, currentMap);
    }

    for(int i=0; i<29; ++i){
        for(int j=0; j<26; ++j){
            currentMap[i][j] = tempMap[i][j];
        }
    }

    //Down
    if(currentPosIndex/26 != 28 && currentMap[currentPosIndex/26+1][currentPosIndex%26]){
        targetDirection[1] = getBFSPathValue((currentPosIndex/26+1)*26+currentPosIndex%26, pathValue+1, currentMap);
    }

    for(int i=0; i<29; ++i){
        for(int j=0; j<26; ++j){
            currentMap[i][j] = tempMap[i][j];
        }
    }

    //Left
    if(currentPosIndex%26 != 0 && currentMap[currentPosIndex/26][currentPosIndex%26-1]){
        targetDirection[2] = getBFSPathValue((currentPosIndex/26)*26+currentPosIndex%26-1, pathValue+1, currentMap);
    }

    for(int i=0; i<29; ++i){
        for(int j=0; j<26; ++j){
            currentMap[i][j] = tempMap[i][j];
        }
    }

    //Up
    if(currentPosIndex/26 != 0 && currentMap[currentPosIndex/26-1][currentPosIndex%26]){
        targetDirection[3] = getBFSPathValue((currentPosIndex/26-1)*26+currentPosIndex%26, pathValue+1, currentMap);
    }

    int minValue = 1000;
    for(int i=0; i<4; ++i){
        if(targetDirection[i]<minValue && targetDirection[i]>0){
            minValue = targetDirection[i];
            directionAfterBFS = i+1;
        }
    }
    return minValue;
}

void Ghost::foolSearch()
{
    int targetPointIndex = MainWindow::pacman->getNearPointIndex(MainWindow::pacman->pos().toPoint());
    if(abs(targetPointIndex/26-coor/26)>abs(targetPointIndex%26-coor%26)){
        if(targetPointIndex/26>coor/26 && MainWindow::isPoint[coor/26+1][coor%26]){
            setDirection(2);
        }
        else if(MainWindow::isPoint[coor/26-1][coor%26]){
            setDirection(4);
        }
    }
    else {
        if(targetPointIndex%26>coor%26 && MainWindow::isPoint[coor/26][coor%26+1]){
            setDirection(1);
        }
        else if(MainWindow::isPoint[coor/26][coor%26-1]){
            setDirection(3);
        }
    }
}

void Ghost::setFrightened(bool isFrightened)
{
    this->isFrightened = isFrightened;
}

void Ghost::frighten()
{
    isFrightened = true;
    setPixmap(*frightened);
}

void Ghost::randomCall()
{

}


void Ghost::gotRandomDirection()
{
    if(!randomTracking){
        return ;
    }

    int temp = 0;
    do{
        temp = qrand()%4+1;
    }while(temp == getDirection());

    setPos(MainWindow::point[getNearPointIndex(pos().toPoint())/26][getNearPointIndex(pos().toPoint())%26]);
    setDirection(temp);
}
