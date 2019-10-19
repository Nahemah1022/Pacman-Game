#include "onlinegamewindow.h"
#include "ui_onlinegamewindow.h"
#include "mainwindow.h"

QGraphicsScene *onlineGameWindow::scene;
Pacman *onlineGameWindow::pacman;
Blinky *onlineGameWindow::blinky;
Pinky *onlineGameWindow::pinky;
Inky *onlineGameWindow::inky;
Clyde *onlineGameWindow::clyde;

onlineGameWindow::onlineGameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::onlineGameWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,ui->graphicsView->width()-2,ui->graphicsView->height()-2);
    ui->graphicsView->setScene(scene);
    QPixmap map(":/img2/src/Level.png");
    scene->addPixmap(map);
    scene->addPixmap(map.scaled(ui->graphicsView->width()-2, ui->graphicsView->height()-2));

    QFile mapFile(":/file/src/isPoint.txt");
    if(!mapFile.open(QIODevice::ReadOnly)){
        qDebug() << "no file exist";
    }
    QTextStream data(&mapFile);
    for(int i=0; i<29; ++i){
        for(int j=0; j<26; ++j){
            data >> isPoint[i][j];
            MainWindow::isPoint[i][j] = isPoint[i][j];
        }
    }
    QFile mapFile2(":/file/src/emptyPoint.txt");
    if(!mapFile2.open(QIODevice::ReadOnly)){
        qDebug() << "no file exist";
    }
    QTextStream data2(&mapFile2);
    for(int i=0; i<29; ++i){
        for(int j=0; j<26; ++j){
            data2 >> emptyPoint[i][j];
        }
    }

    int dx = 30, dy = 29;
    for(int i=0; i<29; ++i){
        for(int j=0; j<26; ++j){
            point[i][j].setX(45 + j*dx);
            point[i][j].setY(125 + i*dy);
            MainWindow::point[i][j].setX(45 + j*dx);
            MainWindow::point[i][j].setY(125 + i*dy);
            if((i==2 || i==22) && (j==0 || j==25)){
                PowerPellet *p = new PowerPellet(point[i][j], 20, 20);
                scene->addItem(p);
                p->setPos(p->pos().x()-5, p->pos().y()-5);
            }
            else if(isPoint[i][j] && !emptyPoint[i][j]){
                QGraphicsEllipseItem *e = new QGraphicsEllipseItem(point[i][j].x(), point[i][j].y(), 10, 10);
                e->setPos(e->pos().x()-5, e->pos().y()-5);
                scene->addItem(e);
                e->setBrush(Qt::white);
            }
        }
    }

    //pacman = new Pacman(1234);
    pacman = new Pacman("127.0.0.1", 1234);
    scene->addItem(pacman);
    //pacman->setPos(point[22][13]);

    MainWindow::pacman = pacman;

}

onlineGameWindow::~onlineGameWindow()
{
    delete ui;
}

void onlineGameWindow::keyPressEvent(QKeyEvent *event)
{
    //pacman->restart();
    QPoint targetPoint = point[pacman->getNearPointIndex(pacman->pos().toPoint())/26][pacman->getNearPointIndex(pacman->pos().toPoint())%26];

    if(event->key() == Qt::Key_W && pacman->getDirection() != 4 && isPoint[pacman->getCoor()/26-1][pacman->getCoor()%26]){
        if(pacman->getDirection() != 2){
            pacman->setPos(targetPoint);
        }
        pacman->setDirection(4);
    }
    else if (event->key() == Qt::Key_S && pacman->getDirection() != 2 && isPoint[pacman->getCoor()/26+1][pacman->getCoor()%26]) {
        if(pacman->getDirection() != 4){
            pacman->setPos(targetPoint);
        }
        pacman->setDirection(2);
    }
    else if (event->key() == Qt::Key_A && pacman->getDirection() != 3 && isPoint[pacman->getCoor()/26][pacman->getCoor()%26-1]) {
        if(pacman->getDirection() != 1){
            pacman->setPos(targetPoint);
        }
        pacman->setDirection(3);
    }
    else if (event->key() == Qt::Key_D && pacman->getDirection() != 1 && isPoint[pacman->getCoor()/26][pacman->getCoor()%26+1]) {
        if(pacman->getDirection() != 3){
            pacman->setPos(targetPoint);
        }
        pacman->setDirection(1);
    }
}

void onlineGameWindow::on_pushButton_clicked()
{
    scene->addItem(blinky = new Blinky(QPixmap(":/img2/src/blinky_left.png"), point[12][15], this));
    MainWindow::blinky = blinky;
    scene->addItem(pinky = new Pinky(QPixmap(":/img2/src/pinky_left.png"), point[12][10], this));
    MainWindow::pinky = pinky;
    scene->addItem(inky = new Inky(QPixmap(":/img2/src/inky_left.png"), point[14][15], this));
    MainWindow::inky = inky;
    scene->addItem(clyde = new Clyde(QPixmap(":/img2/src/clyde_left.png"), point[14][10], this));
    MainWindow::clyde =clyde;

    setFocus();
}
