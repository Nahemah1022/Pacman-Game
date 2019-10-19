#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Pacman;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QString ipAddress = "127.0.0.1" ,unsigned short ipPort = 1234, QObject *parent = nullptr);
    void writeOut(int direction);
    Pacman *player;
    bool isInitialized = false;

signals:
    void dataRecieved(int);

public slots:
    void readyRead();

private:
    QTcpSocket *socket;
};

#endif // CLIENT_H
