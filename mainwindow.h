#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "exp/myserver.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    const QString DEFAULT_IP = QString("39.101.201.13");

signals:

private slots:

    void myServerError(QString error);

    void recvServerData(QString data);

    void on_pushButton_start_clicked();

    private:
    Ui::MainWindow *ui;
    MyServer* myServer;
    void init_Exp();
};
#endif // MAINWINDOW_H
