#ifndef ONLINEGAMEWINDOW_H
#define ONLINEGAMEWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QFile>
#include <QPushButton>
#include <QDebug>

#include "powerpellet.h"
#include "pacman.h"
#include "blinky.h"
#include "pinky.h"
#include "inky.h"
#include "clyde.h"
#include "server.h"
#include "client.h"

namespace Ui {
class onlineGameWindow;
}

class onlineGameWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit onlineGameWindow(QWidget *parent = nullptr);
    ~onlineGameWindow();
    QPoint point[29][26];
    int isPoint[29][26];
    int emptyPoint[29][26];
    int score;
    static QGraphicsScene *scene;

    static Pacman *pacman;
    static Blinky *blinky;
    static Pinky *pinky;
    static Inky *inky;
    static Clyde *clyde;

    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_pushButton_clicked();

private:
    Ui::onlineGameWindow *ui;
};

#endif // ONLINEGAMEWINDOW_H
