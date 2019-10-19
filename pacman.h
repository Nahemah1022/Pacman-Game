#ifndef PACMAN_H
#define PACMAN_H

#include <QWidget>
#include "movableitem.h"
#include "ghost.h"
#include <QGraphicsItem>
#include <QProgressBar>
#include <QPushButton>

class Pacman : public MovableItem
{
    Q_OBJECT
public:
    Pacman(QWidget *parent = nullptr);
    Pacman(unsigned short ipPort, QWidget *parent = nullptr);
    Pacman(QString ipAddress, unsigned short ipPort, QWidget *parent = nullptr);
    myServer *pacServer = nullptr;
    Client *pacClient = nullptr;
    bool isPowered = false;

public slots:
    void movement();
    void changePix();
    void coorChanging();

private:
    int pixStatus = 0;
    int eatenPoint = 0;
    QPixmap *pix1;
    QPixmap *pix2;
    QPixmap *pix3;
    QTimer *pixTimer;
    QTransform *transform;
    QTimer *powerTimer;
    QTimer *powerTimerForBar;
    QProgressBar *powerProgressBar;

signals:
    void gameoverSignal();
    void frightenSignal(bool);
};

#endif // PACMAN_H
