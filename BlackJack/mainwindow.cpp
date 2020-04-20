#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_boton_jugador1_clicked()
{
    this -> hide();
    jugador_uno -> show();
}

void MainWindow::on_boton_salir_clicked()
{
    this -> close();
}
