#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mymodbus = new MyModbus();
    connect(mymodbus,&MyModbus::signal_data,this,&MainWindow::slot_recData);
    ui->lEdit_addr->setText("0");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::slot_recData(QString s)
{
    ui->lEdit_value->setText(s);
}

void MainWindow::on_btnRead_clicked()
{
    mymodbus->slot_readReg(1, ui->lEdit_addr->text().toInt(), 2);
}

void MainWindow::on_btnWrite_clicked()
{
    QVector<quint16> buff;
    buff << 5 << 24;
    mymodbus->slot_writeReg(1, ui->lEdit_addr->text().toInt(), buff);
}

