#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Guardamos la imagen de la mesa de blackjack proporcionandoles la ruta
    QPixmap MesaInicio("C:/Program Files (x86)/BlackJack/Imagenes/Mesa_blackjack3.jpg");
    //Obtenemos la altura y anchura de la etiqueta para porder colocar bien la imagen
    int w = ui -> Mesa -> width();
    int h = ui -> Mesa -> height();
    //Imprimimos la imagen en la etiqueta
    ui -> Mesa -> setPixmap(MesaInicio.scaled(w,h,Qt::KeepAspectRatio));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Variable para saber el numero de jugadores
int NumeroJ = 0;


void MainWindow::on_boton_jugador1_clicked()
{
    this -> hide();
    NumeroJ = 1;
    jugador_uno -> show();
}

void MainWindow::on_boton_jugador2_clicked()
{
    this -> hide();
    NumeroJ = 2;
    jugador_uno -> show();
}

void MainWindow::on_boton_salir_clicked()
{
    this -> close();
}
