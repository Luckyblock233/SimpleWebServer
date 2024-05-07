#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QFile>

void MainWindow::init_Exp() {
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    //    qDebug()<<ipAddressesList;
    foreach (QHostAddress address, ipAddressesList) {
        if(address.protocol() == QAbstractSocket::IPv4Protocol) {
            ui->comboBox_localIp->addItem(address.toString());
        }
    }

    myServer = new MyServer(this,ui->comboBox_localIp->currentText(),ui->lineEdit_port->text().toInt(),ui->lineEdit_fileName->text());

    connect(myServer,&MyServer::sendError,this,&MainWindow::myServerError);
    connect(myServer,&MyServer::sendData,this,&MainWindow::recvServerData);
}

void MainWindow::on_pushButton_start_clicked() {
    if(ui->pushButton_start->text() == "启动服务器") {
        myServer->localIp = ui->comboBox_localIp->currentText();
        myServer->port = ui->lineEdit_port->text().toInt();
        myServer->fileName = ui->lineEdit_fileName->text();

        if(myServer->startServer()) {
            ui->textBrowser_debug->setText("");
            ui->pushButton_start->setText("关闭服务器");
            ui->comboBox_localIp->setEnabled(false);
            ui->lineEdit_port->setEnabled(false);
            ui->lineEdit_fileName->setEnabled(false);

            ui->textBrowser_debug->setText(QString("http://%1:%2/").arg(myServer->localIp).arg(myServer->port));
        }
    } else if(ui->pushButton_start->text() == "关闭服务器") {
        myServer->close();
        ui->pushButton_start->setText("启动服务器");
        ui->comboBox_localIp->setEnabled(true);
        ui->lineEdit_port->setEnabled(true);
        ui->lineEdit_fileName->setEnabled(true);
    }
}


void MainWindow::myServerError(QString error) {
     QMessageBox::critical(this,"错误",error);
}

void MainWindow::recvServerData(QString data) {
    ui->textBrowser_debug->append("==============================================================");
    ui->textBrowser_debug->append(data);
}
