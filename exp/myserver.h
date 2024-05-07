#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QTcpServer>
#include "mysocket.h"

//GET / HTTP/1.1
//Host: 192.168.43.246:8088
//Connection: keep-alive
//Cache-Control: max-age=0
//Upgrade-Insecure-Requests: 1
//User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36
//Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
//Accept-Encoding: gzip, deflate
//Accept-Language: zh-CN,zh;q=0.9

//HTTP/1.1 200 OK
//Date: Sat, 31 Dec 2005 23:59:59 GMT
//Content-Type: text/html;charset=ISO-8859-1
//Content-Length: 122
//
//＜html＞
//...
//＜/html＞


class MyServer : public QObject {
    Q_OBJECT
public:
    explicit MyServer(QObject *parent,QString localIp,int port,QString fileName);

    bool startServer();
    void newConnection();
    void close();
    QString getHeaderString(bool state);

    QString localIp;
    int port;
    QString fileName;

    QList<MySocket*> mySocketList;

signals:
    void sendError(QString error);
    void sendData(QString data);

public slots:
    void socketReadyRead(QTcpSocket* socket,QString data);

private:
    QTcpServer * tcpServer;
};

#endif // MYSERVER_H
