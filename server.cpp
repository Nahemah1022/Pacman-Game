#include "server.h"
#include "pacman.h"
#include "onlinegamewindow.h"

myServer::myServer(unsigned short ipPort, QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(onnewConnection()));

    this->ipPort = ipPort;
    if(!server->listen(QHostAddress::Any, ipPort)){
        qDebug() << "Server could not start";
    }
    else {
        qDebug() << "Server start";
    }
}

void myServer::writeOut(int direction)
{
    QByteArray data;
    data.setNum(direction);

    socket->write(data);
}

void myServer::onnewConnection()
{
    socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    qDebug() << "new Connection!";
    player = new Pacman();
    onlineGameWindow::scene->addItem(player);

    socket->write(QByteArray().setNum(onlineGameWindow::pacman->getDirection()*1000+onlineGameWindow::pacman->getNearPointIndex(onlineGameWindow::pacman->pos().toPoint())));
}

void myServer::readyRead()
{
    emit dataRecieved(socket->readAll().toInt());
}
