#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

class Pacman;

class myServer : public QObject
{
    Q_OBJECT
public:
    explicit myServer(unsigned short ipPort = 1234, QObject *parent = nullptr);
    unsigned short ipPort;
    QTcpServer *server;
    void writeOut(int);
    Pacman *player;

signals:
    void dataRecieved(int);

public slots:
    void onnewConnection();
    void readyRead();

private:
    QTcpSocket *socket;

};

#endif // SERVER_H
