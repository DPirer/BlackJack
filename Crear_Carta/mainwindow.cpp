#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <time.h>
#include <vector>
#include <algorithm>
#include "carta.h"


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


void MainWindow::on_boton_clicked()
{
    // Creamos la matriz
    int matriz[4][10] = {{1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10}};

    while (1){
        srand(time(0));                         // Escogemos una fila y columna aleatorias
        int valor = 1 + rand() % 10;
        int palos = 1 + rand() % 4;

        if (matriz[valor][palos] != 0){         // Si esos valores escogidos no son cero, la carta no ha sido elegida antes
            Carta carta1(valor,palos);          // Creamos el objeto de tipo Carta
            matriz[valor][palos] = 0;           // Ponemos esa posición a 0 para marcar la carta

            // Mostramos la carta
            ui -> marco -> setText(QString::number(carta1.valor));
            ui -> marcos -> setText(carta1.palo);
            break;
        }
    }
}

