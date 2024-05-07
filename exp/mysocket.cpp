#include "mysocket.h"

MySocket::MySocket(QTcpSocket* socket) : QObject(socket) {
    this->socket = socket;
    connect(this->socket,&QTcpSocket::readyRead,this,&MySocket::readyRead);
}

void MySocket::readyRead() {
    emit this->socketReadyRead(this->socket,QString(this->socket->readAll()));
}

void MySocket::close() {
    this->socket->close();
}
