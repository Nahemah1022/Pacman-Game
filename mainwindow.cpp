#include "mainwindow.h"
#include "ui_mainwindow.h"

QPoint MainWindow::point[29][26];
int MainWindow::isPoint[29][26];
int MainWindow::score = 0;
Pacman *MainWindow::pacman;
Blinky *MainWindow::blinky;
Pinky *MainWindow::pinky;
Inky *MainWindow::inky;
Clyde *MainWindow::clyde;
QGraphicsScene *MainWindow::scene;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFocus();

    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,ui->graphicsView->width()-2,ui->graphicsView->height()-2);
    ui->graphicsView->setScene(scene);
    QPixmap map(":/img2/src/Level.png");
    scene->addPixmap(map);
    scene->addPixmap(map.scaled(ui->graphicsView->width()-2, ui->graphicsView->height()-2));

    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    ui->scoreLabel->setPalette(pe);

    QFile mapFile(":/file/src/isPoint.txt");
    if(!mapFile.open(QIODevice::ReadOnly)){
        qDebug() << "no file exist";
    }
    QTextStream data(&mapFile);
    for(int i=0; i<29; ++i){
        for(int j=0; j<26; ++j){
            data >> isPoint[i][j];
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
    pacman = new Pacman(this);
    connect(pacman, SIGNAL(scorePlus()), this, SLOT(scorePlus()));
    connect(pacman, SIGNAL(gameoverSignal()), this, SLOT(gameOver()));
    connect(pacman, &Pacman::frightenSignal, this, &MainWindow::ghostFrighten);

    scene->addItem(pacman);
    pacman->setPos(point[22][13]);
    pacman->setPos(pacman->pos().x()-15, pacman->pos().y());
    pacman->setDirection(0);

    scene->addItem(blinky = new Blinky(QPixmap(":/img2/src/blinky_left.png"), point[12][15], this));
    scene->addItem(pinky = new Pinky(QPixmap(":/img2/src/pinky_left.png"), point[12][10], this));
    scene->addItem(inky = new Inky(QPixmap(":/img2/src/inky_left.png"), point[14][15], this));
    scene->addItem(clyde = new Clyde(QPixmap(":/img2/src/clyde_left.png"), point[14][10], this));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    pacman->restart();
    QPoint targetPoint = point[pacman->getNearPointIndex(pacman->pos().toPoint())/26][pacman->getNearPointIndex(pacman->pos().toPoint())%26];

    if(event->key() == Qt::Key_Up && pacman->getDirection() != 4 && MainWindow::isPoint[pacman->getCoor()/26-1][pacman->getCoor()%26]){
        if(pacman->getDirection() != 2){
            pacman->setPos(targetPoint);
        }
        pacman->setDirection(4);
    }
    else if (event->key() == Qt::Key_Down && pacman->getDirection() != 2 && MainWindow::isPoint[pacman->getCoor()/26+1][pacman->getCoor()%26]) {
        if(pacman->getDirection() != 4){
            pacman->setPos(targetPoint);
        }
        pacman->setDirection(2);
    }
    else if (event->key() == Qt::Key_Left && pacman->getDirection() != 3 && MainWindow::isPoint[pacman->getCoor()/26][pacman->getCoor()%26-1]) {
        if(pacman->getDirection() != 1){
            pacman->setPos(targetPoint);
        }
        pacman->setDirection(3);
    }
    else if (event->key() == Qt::Key_Right && pacman->getDirection() != 1 && MainWindow::isPoint[pacman->getCoor()/26][pacman->getCoor()%26+1]) {
        if(pacman->getDirection() != 3){
            pacman->setPos(targetPoint);
        }
        pacman->setDirection(1);
    }

    if(event->key() == Qt::Key_Space){
        if(!timeStop){
            disconnect(pacman->movingTimer, SIGNAL(timeout()), pacman, SLOT(movement()));
            disconnect(blinky->movingTimer, SIGNAL(timeout()), blinky, SLOT(movement()));
            disconnect(pinky->movingTimer, SIGNAL(timeout()), pinky, SLOT(movement()));
            disconnect(inky->movingTimer, SIGNAL(timeout()), inky, SLOT(movement()));
            disconnect(clyde->movingTimer, SIGNAL(timeout()), clyde, SLOT(movement()));

            timeStop = true;
        }
        else{
            connect(pacman->movingTimer, SIGNAL(timeout()), pacman, SLOT(movement()));
            connect(blinky->movingTimer, SIGNAL(timeout()), blinky, SLOT(movement()));
            connect(pinky->movingTimer, SIGNAL(timeout()), pinky, SLOT(movement()));
            connect(inky->movingTimer, SIGNAL(timeout()), inky, SLOT(movement()));
            connect(clyde->movingTimer, SIGNAL(timeout()), clyde, SLOT(movement()));

            timeStop = false;
        }
    }

}

void MainWindow::scorePlus()
{
    ui->scoreLabel->setText(tr("SCORE\n") + QString::number(score));

    if(score == 2460){
        gameOver();
        ui->scoreLabel->setText("WIN!!");
    }
}

void MainWindow::gameOver()
{
    disconnect(pacman->movingTimer, SIGNAL(timeout()), pacman, SLOT(movement()));
    disconnect(blinky->movingTimer, SIGNAL(timeout()), blinky, SLOT(movement()));
    disconnect(pinky->movingTimer, SIGNAL(timeout()), pinky, SLOT(movement()));
    disconnect(inky->movingTimer, SIGNAL(timeout()), inky, SLOT(movement()));
    disconnect(clyde->movingTimer, SIGNAL(timeout()), clyde, SLOT(movement()));

    ui->scoreLabel->setText("LOSS");
}

void MainWindow::ghostFrighten(bool isFrightened)
{
    if(isFrightened){
        blinky->setPixmap(QPixmap(":/img2/src/frightenedGhost.png"));
        pinky->setPixmap(QPixmap(":/img2/src/frightenedGhost.png"));
        inky->setPixmap(QPixmap(":/img2/src/frightenedGhost.png"));
        clyde->setPixmap(QPixmap(":/img2/src/frightenedGhost.png"));
    }
    else{
        blinky->setPixmap(QPixmap(":/img2/src/blinky_right.png"));
        pinky->setPixmap(QPixmap(":/img2/src/pinky_right.png"));
        inky->setPixmap(QPixmap(":/img2/src/inky_right.png"));
        clyde->setPixmap(QPixmap(":/img2/src/clyde_right.png"));
    }
    blinky->setFrightened(isFrightened);
    pinky->setFrightened(isFrightened);
    inky->setFrightened(isFrightened);
    clyde->setFrightened(isFrightened);
}

void MainWindow::on_pushButton_clicked()
{
    setFocus();
}
