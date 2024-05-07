#include "myserver.h"
#include <QHostAddress>
#include <QNetworkInterface>
#include <QFile>
#include <QDir>
#include <QDateTime>

MyServer::MyServer(QObject *parent,QString localIp,int port,QString fileName) : QObject(parent) {
    this->localIp = localIp;
    this->port = port;
    this->fileName = fileName;

    tcpServer = new QTcpServer;

    connect(tcpServer,&QTcpServer::newConnection,this,&MyServer::newConnection);
}

bool MyServer::startServer() {
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    //    qDebug()<<ipAddressesList;
    foreach (QHostAddress address, ipAddressesList) {
        if(address.toString() == localIp) {
            if(!tcpServer->listen(address,port)) {
                emit this->sendError("Listen Fail");
                return false;
            }
            return true;
        }
    }
    return false;
}

void MyServer::newConnection() {
    MySocket* mySocket = new MySocket(tcpServer->nextPendingConnection());
    connect(mySocket,&MySocket::socketReadyRead,this,&MyServer::socketReadyRead);

    mySocketList.push_back(mySocket);
}

void MyServer::socketReadyRead(QTcpSocket *socket, QString data) {
    emit this->sendData(data);
    QString path =  data.split(" ").at(1);
    if (path == "/") path += fileName;
    bool result = true;

    qDebug() << data << "\n" << path << "\n";
    QFile file(QDir::currentPath() + "/resource" + path);
    if (!file.exists()) {
        result = false;
        file.setFileName(QDir::currentPath() + "/resource/404.html");
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit this->sendError(QDir::currentPath()+"/" + fileName +" 打开失败");
        socket->write(getHeaderString(false).toUtf8());
        return;
    }

    QTextStream in(&file);
    QString fileData = in.readAll();
    socket->write((getHeaderString(true).arg(fileData.size()) + fileData +QString("\r\n")).toUtf8());
    if (result) {
        emit this->sendData("Server Response:\r\n"+ getHeaderString(true).arg(fileData.size()) + fileData +"\r\n");
    } else {
        emit this->sendData("Server Response:\r\n"+ getHeaderString(false));
    }

    socket->flush();
}

void MyServer::close() {
    foreach (MySocket* mySocket, mySocketList) {
        mySocket->close();
    }

    mySocketList.clear();
    tcpServer->close();
}

QString MyServer::getHeaderString(bool state) {
    //HTTP/1.1 200 OK
    //Date: Sat, 31 Dec 2005 23:59:59 GMT
    //Content-Type: text/html;charset=ISO-8859-1
    //Content-Length: %1
    QString header = "";
    if(state == true) {
        header = header + "HTTP/1.1 200 OK" + "\r\n";
        header = header + "Date: ";
        header = header + QDateTime::currentDateTime().toString("ddd, d MMM yyyy hh:mm:ss") + " GMT\r\n";
        header = header + "Content-Type: text/html;charset=UTF-8" + "\r\n";
        header = header + "Content-Length: %1" + "\r\n";
    }
    else if(state == false) {
        header = header + "HTTP/1.1 404 Not Found" + "\r\n";
        header = header + "Date: ";
        header = header + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + " GMT\r\n";
        header = header + "Content-Type: text/html;charset=UTF-8" + "\r\n";
        header = header + "Content-Length: 0" + "\r\n";
    }
    return header;
}
