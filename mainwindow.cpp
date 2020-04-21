#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <time.h>
#include <vector>
#include <algorithm>
#include "carta.h"
#include <string.h>
#include <QPixmap>


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

            //Creamos la direccion de la carta

            char direccion[]="C:/Users/Administrator/Desktop/Baraja_de_cartas/"; //Direccion de la carpeta
            char direccion_Final[200];
            strcpy(direccion_Final,direccion);
            int numero = 13*(palos-1)+valor; //Se calcula el numero de la carta
            char baraja[25];
            char jpg[]=".jpg";
            itoa(numero,baraja,10);
            strcat(baraja,jpg);
            strcat(direccion_Final,baraja);
            QPixmap pix(direccion_Final);  //

            // Mostramos la carta
            ui -> marco -> setText(QString::number(carta1.valor));
            ui -> marcos -> setText(carta1.palo);
            ui -> imagen -> setPixmap(pix);
            break;
        }
    }
}

