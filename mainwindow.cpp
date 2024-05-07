#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("A Simple Web Server");
    this->setWindowIcon(QIcon(":/image/photo.jpg"));

    init_Exp();
}

MainWindow::~MainWindow() {
    delete ui;
}
