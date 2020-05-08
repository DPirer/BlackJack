#include "unjugador.h"
#include "ui_unjugador.h"
#include <Windows.h>
#include "mainwindow.h"
#include <vector>
#include <string.h>

using namespace std;


UnJugador::UnJugador(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UnJugador)
{
    ui->setupUi(this);

    // Guardamos la imagen de la mesa de blackjack proporcionandoles la ruta
    QPixmap MesaInicio("C:/Program Files (x86)/BlackJack/Imagenes/Mesa_blackjack3.jpg");
    QPixmap Flecha("C:/Program Files (x86)/BlackJack/Imagenes/flecha.png");
    // Obtenemos la altura y anchura de la etiqueta para porder colocar bien la imagen
    int w = ui -> Mesa1 -> width();
    int h = ui -> Mesa1 -> height();
    int wi = ui -> Flecha1 -> width();
    int he = ui -> Flecha1 -> height();

    // Imprimimos la imagen en la etiqueta
    ui -> Mesa1 -> setPixmap(MesaInicio.scaled(w,h,Qt::KeepAspectRatio));
    ui -> Flecha1 -> setPixmap(Flecha.scaled(wi,he,Qt::KeepAspectRatio));
    ui -> Flecha2 -> setPixmap(Flecha.scaled(wi,he,Qt::KeepAspectRatio));
    ui -> Flecha3 -> setPixmap(Flecha.scaled(wi,he,Qt::KeepAspectRatio));
    ui -> Flecha4 -> setPixmap(Flecha.scaled(wi,he,Qt::KeepAspectRatio));

    // Empezamos con todos los botones ocultos para que el usuario haga la apuesta sin joder nada
    ui -> Flecha1 -> hide();
    ui -> Flecha2 -> hide();
    ui -> Flecha3 -> hide();
    ui -> Flecha4 -> hide();
    ui -> boton_retirarse -> hide();
    ui -> boton_doblar -> hide();
    ui -> boton_dividir -> hide();
    ui -> boton_pedir_carta -> hide();
    ui -> boton_continuar -> hide();
    ui -> boton_pedir_carta2 -> hide();
    ui -> boton_doblar2 -> hide();
    ui -> label_3 -> hide();
    ui -> label_4 -> hide();
    ui -> label_valor_banca -> hide();
    ui -> pantalla_2 -> hide();
    ui -> pantalla_3 -> hide();
    ui -> pantalla_4 -> hide();
    ui -> label_final2 -> hide();
    ui -> label_final3 -> hide();
    ui -> label_final4 -> hide();

    ui -> label -> show();
    ui -> pantalla -> show();
    ui -> saldo2 -> hide();
    ui -> saldo3 -> hide();
    ui -> saldo4 -> hide();
    ui -> label_5 -> show();
    ui -> label_6 -> hide();
    ui -> label_7 -> hide();
    ui -> label_8 -> hide();

    // Mostramos quien empieza introduciendo la apuesta
    ui -> label_retirada -> setText("Turno Jugador 1");
    // Iniciamos la semilla de los numeros alatorios de las cartas
    srand(time(NULL));

}

UnJugador::~UnJugador()
{
    delete ui;
}


/* BOTONES *********************************************************************************************************************************/


void UnJugador::on_boton_empezar_clicked()
{
    BotonEmpezar();
}

void UnJugador::on_boton_pedir_carta_clicked()
{
    BotonPedirCarta();
}

void UnJugador::on_boton_retirarse_clicked()
{
    BotonRetirarse();
}

void UnJugador::on_boton_continuar_clicked()
{
    BotonContinuar();
}

void UnJugador::on_boton_reiniciar_clicked()
{
    BotonReiniciar();
}

void UnJugador::on_boton_salir_clicked()
{
    BotonSalir();
}

void UnJugador::on_boton_doblar_clicked()
{
    BotonDoblar();
}

void UnJugador::on_boton_dividir_clicked()
{
    BotonDividir();
}

void UnJugador::on_boton_pedir_carta2_clicked()
{
    BotonPedirCarta2();
}

void UnJugador::on_boton_doblar2_clicked()
{
    BotonDoblar2();
}
