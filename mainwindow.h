#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>

#include "pacman.h"
#include "ghost.h"
#include "powerpellet.h"
#include "blinky.h"
#include "pinky.h"
#include "inky.h"
#include "clyde.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static QPoint point[29][26];
    static int isPoint[29][26];
    int emptyPoint[29][26];
    static int score;
    static Pacman *pacman;
    static Blinky *blinky;
    static Pinky *pinky;
    static Inky *inky;
    static Clyde *clyde;
    static QGraphicsScene *scene;
    void keyPressEvent(QKeyEvent *event);

public slots:
    void scorePlus();
    void gameOver();
    void ghostFrighten(bool isFrightened);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    bool timeStop = false;
};

#endif // MAINWINDOW_H
