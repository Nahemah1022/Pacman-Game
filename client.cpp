#include "client.h"
#include "mainwindow.h"
#include "onlinegamewindow.h"
#include "pacman.h"

Client::Client(QString ipAddress, unsigned short ipPort, QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    socket->connectToHost(ipAddress, ipPort);
    player = new Pacman();
    onlineGameWindow::scene->addItem(player);
}

void Client::readyRead()
{
    emit dataRecieved(socket->readAll().toInt());
}

void Client::writeOut(int direction)
{
    QByteArray data;
    data.setNum(direction);

    socket->write(data);
}
