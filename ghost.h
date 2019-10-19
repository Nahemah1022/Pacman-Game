#ifndef GHOST_H
#define GHOST_H

#include "movableitem.h"
#include <QRandomGenerator>

class Ghost : public MovableItem
{
    Q_OBJECT
public:
    Ghost(QPixmap pix, QPoint initialPoint, QWidget *parent = nullptr);
    ~Ghost();
    void movement();
    void coorChanging();
    void restart();
    void foolSearch();
    void setFrightened(bool isFrightened);

    bool foolTracking = false;
    bool randomTracking = false;
    bool bfsTracking = false;

protected:
    bool isFrightened = false;
    bool isDying = false;
    int trackingTargetIndex;
    int directionAfterBFS;
    int bfsSearchInterval = 400;
    QTimer *bfsTimer, *rebornTimer;
    QPixmap *right, *down, *left, *up, *frightened, *dyingPix;

protected slots:
    void frighten();
    void BreadthFirstSearch();

private:
    QTimer *randomTimer;
    int getBFSPathValue(int currentPosIndex, int pathValue, int currentMap[29][26]);
    int bfsArray[29][26];

private slots:
    void gotRandomDirection();
    void randomCall();

};

#endif // GHOST_H
