#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QObject>
#include <QTcpSocket>

class MySocket : public QObject {
    Q_OBJECT
public:
    explicit MySocket(QTcpSocket* socket);
    QTcpSocket* socket;
    void close();

signals:
    void socketReadyRead(QTcpSocket* socket,QString data);

public slots:
    void readyRead();
};

#endif // MYSOCKET_H
