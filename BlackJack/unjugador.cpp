#include "unjugador.h"
#include "ui_unjugador.h"

UnJugador::UnJugador(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UnJugador)
{
    ui->setupUi(this);

    // Empezamos con todos los botones ocultos para que el usuario haga la apuesta sin joder nada
    ui -> boton_retirarse -> hide();
    ui -> boton_doblar -> hide();
    ui -> boton_dividir -> hide();
    ui -> boton_pedir_carta -> hide();
}

UnJugador::~UnJugador()
{
    delete ui;
}



/* VARIABLES GLOBALES ***********************************************************************************************************************/

int matriz[4][13]={{1,2,3,4,5,6,7,8,9,10,11,12,13}, {1,2,3,4,5,6,7,8,9,10,11,12,13},{1,2,3,4,5,6,7,8,9,10,11,12,13},{1,2,3,4,5,6,7,8,9,10,11,12,13}};

// isDoblar e isDividir se crean debido a que solo se puede usar doblar y dividir una vez
bool isDoblar, isDividir;

//Se inicia el juego y se asigna un saldo de 2000
int saldo = 2000, valorJugador = 0, valorBanca = 0, apuesta;


/* FUNCIONES *******************************************************************************************************************************/

Mano UnJugador::repartirCarta(){
    int num, palo;
    do{
       srand(time(0));
       num = 1 + rand() % 12;
       palo = 1 + rand() % 3;
    } while(matriz[palo][num] == 0);

    // Ponemos esa posición a 0 para marcarla
    matriz[palo][num] = 0;

    // Creamos el objeto carta de tipo mano
    Mano carta(num, palo);

    // Y lo retornamos para que sea la mano o de la banca o del jugador
    return carta;
}


/* BOTONES *********************************************************************************************************************************/


void UnJugador::on_boton_empezar_clicked()
{

    // Una vez presionamos el botón empezar, cogemos la apuesta del usuario
    apuesta = ui -> selector_apuesta -> value();

    // Actualizamos el valor del saldo y lo mostramos
    saldo = saldo - apuesta;
    ui -> pantalla -> display(saldo);

    // Y ocultamos y mostramos los botones que necesitamos
    ui -> boton_retirarse -> show();
    ui -> boton_doblar -> show();
    ui -> boton_dividir -> show();
    ui -> boton_pedir_carta -> show();

    ui -> boton_empezar -> hide();
    ui -> label_2 -> hide();
    ui -> selector_apuesta -> hide();

    // Creamos la mano del jugador
    Mano ManoJugador = UnJugador::repartirCarta();
    valorJugador = valorJugador + ManoJugador.numero;
    ui -> pantalla_2 -> display(valorJugador);

    // Creamos la mano de la banca
    Mano ManoBanca = UnJugador::repartirCarta();
    valorBanca = valorBanca + ManoBanca.numero;


    // Mostramos la carta para comprobar que está todo correcto
    ui -> label_valor -> setText(QString::number(ManoJugador.numero));
    ui -> label_palo -> setText(ManoJugador.palo);
}



void UnJugador::on_boton_pedir_carta_clicked()
{

    // Creamos la mano del jugador
    Mano ManoJugador = UnJugador::repartirCarta();
    valorJugador = valorJugador + ManoJugador.numero;
    ui -> pantalla_2 -> display(valorJugador);

    // Creamos la mano de la banca
    Mano ManoBanca = UnJugador::repartirCarta();
    valorBanca = valorBanca + ManoBanca.numero;

    // Mostramos la carta para comprobar que está todo correcto
    ui -> label_valor -> setText(QString::number(ManoJugador.numero));
    ui -> label_palo -> setText(ManoJugador.palo);
}



void UnJugador::on_boton_retirarse_clicked()
{
    // Si el jugador no llega a 16 no se puede retirar.
        if (valorJugador < 16){
            ui -> label_retirada -> setText("No te puedes retirar, subnormal");
        }
        else if (valorJugador > 21){
            ui -> label_retirada -> setText("Has perdido, subnormal");
           }
        else {
            // mostrarMano(pJugador); Aquí es donde va el código con las imágenes

            // Si la banca tiene menor valor que tú, intentará subirlo
            while (valorBanca < valorJugador){
                Mano ManoBanca = UnJugador::repartirCarta();
                valorBanca = valorBanca + ManoBanca.numero;
            }

            // Mostramos el valor de la banca y su mano
            ui -> pantalla_3 -> display(valorBanca);
            //mostrarMano(pBanca);

            //Se incluyen todas las posibilidades para ganar.
            if ((valorBanca > 21) || (valorBanca < valorJugador)){ // Aquí faltan mas opciones. Mirar luego
                ui -> label_retirada ->  setText("Enhorabuena subnormal");
                saldo = saldo * apuesta * 2;
                ui -> pantalla -> display(saldo);
            }
            //else if ((blackjack(puntero)==true && blackjack(pBanca)==true) || valorMano(puntero)==valorMano(pBanca)){
              ////saldo = saldo + apuesta;
            //}
            else {
                ui -> label_retirada -> setText("Has perdido, subnormal");
             }
        }
    }

