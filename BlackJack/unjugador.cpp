#include "unjugador.h"
#include "ui_unjugador.h"
#include <Windows.h>
#include "mainwindow.h"
#include <vector>

using namespace std;

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
    ui -> boton_continuar -> hide();
    ui -> label_valor -> hide();
    ui -> label_palo -> hide();
    ui -> label_retirada -> hide();
    ui -> label -> hide();
    ui -> label_3 -> hide();
    ui -> label_valor_banca -> hide();
    ui -> pantalla -> hide();
    ui -> pantalla_2 -> hide();
    ui -> pantalla_3 -> hide();

}

UnJugador::~UnJugador()
{
    delete ui;
}



/* VARIABLES GLOBALES ***********************************************************************************************************************/

int matriz[4][13] = {{1,2,3,4,5,6,7,8,9,10,11,12,13}, {1,2,3,4,5,6,7,8,9,10,11,12,13},{1,2,3,4,5,6,7,8,9,10,11,12,13},{1,2,3,4,5,6,7,8,9,10,11,12,13}};

// isDoblar e isDividir se crean debido a que solo se puede usar doblar y dividir una vez
bool isDividir, BJ = false;

// Varaible para comprobar si estamos en la primera mano
int contadorManos = 0;

//Se inicia el juego y se asigna un saldo de 2000
int saldo = 2000, valorJugador = 0, valorJugador2 = 0, valorBanca = 0, apuesta;

// Inicializo un vector de la clase Mano cuyo tamaño es el máximo posible para las cartas más bajas
Mano ManoJugador[11] {{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}};
Mano ManoJugador2[11] {{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}};
Mano ManoBanca[11] {{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}};


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

void UnJugador::crearMano(){
    // Creamos la mano del jugador
    ManoJugador[contadorManos] = UnJugador::repartirCarta();

    // Actualizamos el valor total de las cartas del jugador y las mostramos por pantalla
    valorJugador = valorJugador + ManoJugador[contadorManos].numero;
    ui -> pantalla_2 -> display(valorJugador);

    // Mostramos la carta. Es decir, en el programa de momento sólo se ve la segunda carta y a partir de ahí. Ahí es donde entra
    // Alberto para que vayan apareciendo las imágenes de las cartas
    ui -> label_valor -> setText(QString::number(ManoJugador[contadorManos].numero));
    ui -> label_palo -> setText(ManoJugador[contadorManos].palo);

    // Creamos la mano de la banca si la banca no ha llegado a 21
    if (valorBanca < 21){
        ManoBanca[contadorManos] = UnJugador::repartirCarta();
        valorBanca = valorBanca + ManoBanca[contadorManos].numero;
    }

    // Creamos una tercera mano para el caso en el que hayamos dividido cartas
    if (isDividir == true) {
        ManoJugador2[contadorManos] = UnJugador::repartirCarta();
        valorJugador = valorJugador2 + ManoJugador2[contadorManos].numero;
    }

    // Finalmente, sumamos uno al contador de manos para pasar a la sigieinte mano
    contadorManos++;
}

void UnJugador::resetear(){
    // Si reinicializo la matriz de forma normal me da error excess elements of scalar initializer, asi que uso un bucle
    for (int i = 0; i<4 ; i++){
        for (int j = 0; j<13; j++){
            matriz[i][j] = j;
        }
    }

    // Reseteamos las variables
    valorJugador = 0, valorBanca = 0, valorJugador2 = 0, contadorManos = 0, isDividir = false, BJ = false;

    // Ahora ocultamos y mostramos los elementos necesarios
    ui -> label_retirada -> hide();
    ui -> label -> hide();
    ui -> label_3 -> hide();
    ui -> label_valor_banca -> hide();
    ui -> pantalla -> hide();
    ui -> pantalla_2 -> hide();
    ui -> pantalla_3 -> hide();

    ui -> boton_empezar -> show();
    ui -> label_2 -> show();
    ui -> selector_apuesta -> show();
}

void UnJugador::comprobarBlackjack(){
    if ((ManoJugador[0].numero == 1 || ManoJugador[1].numero == 1) && ((ManoJugador[0].numero == 11) || (ManoJugador[1].numero == 11) || (ManoJugador[0].numero == 12) || (ManoJugador[1].numero == 12) || (ManoJugador[0].numero == 13) || (ManoJugador[1].numero == 13))){

        // Si tenemos un As y una figura automáticamente tenemos BlackJack, que son 21 puntos
        valorJugador = 21;
        ui -> pantalla_2 -> display(valorJugador);
    }

    // Pero también tenemos que comprobar si lo tiene la banca
    if ((ManoBanca[0].numero == 1 || ManoBanca[1].numero == 1) && ((ManoBanca[0].numero == 11) || (ManoBanca[1].numero == 11) || (ManoBanca[0].numero == 12) || (ManoBanca[1].numero == 12) || (ManoBanca[0].numero == 13) || (ManoBanca[1].numero == 13))){

        // Si tenemos un As y una figura automáticamente tenemos BlackJack, que son 21 puntos
        valorBanca = 21;
    }
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
    ui -> label_valor -> show();
    ui -> label_palo -> show();
    ui -> label -> show();
    ui -> label_3 -> show();
    ui -> label_valor_banca -> show();
    ui -> pantalla -> show();

    ui -> pantalla_2 -> show();
    ui -> pantalla_2 -> display(valorJugador);
    ui -> pantalla_3 -> show();
    ui -> pantalla_3 -> display(valorBanca);

    ui -> boton_empezar -> hide();
    ui -> label_2 -> hide();
    ui -> selector_apuesta -> hide();

    // Llamamos a la función crear carta dos veces, que me creará las manos del jugador y de la banca
    // Lo metemos en una funcón para no repetir código porque en la función pedir carta hay que hacer lo mismo
    UnJugador::crearMano();
    UnJugador::crearMano();

    // Llamamos a la función para comprobar si tenemos BlackJack
    UnJugador::comprobarBlackjack();
}



void UnJugador::on_boton_pedir_carta_clicked()
{
    UnJugador::crearMano();
}



void UnJugador::on_boton_retirarse_clicked()
{
    ui -> label_retirada -> show();

    if (isDividir == false){

        if (valorJugador < 16){
            ui -> label_retirada -> setText("No te puedes retirar, subnormal");
            //Sleep(1000);
            //ui -> label_retirada -> hide();
        }
        else if (valorJugador > 21){
            ui -> label_retirada -> setText("Has perdido, subnormal");
            ui -> pantalla_3 -> display(valorBanca);

            ui -> boton_retirarse -> hide();
            ui -> boton_doblar -> hide();
            ui -> boton_dividir -> hide();
            ui -> boton_pedir_carta -> hide();
            ui -> label_valor -> hide();
            ui -> label_palo -> hide();

            ui -> boton_continuar -> show();
            }
        else {
            // Si la banca tiene menor valor que tú, intentará subirlo
            while (valorBanca < valorJugador){
                ManoBanca[contadorManos] = UnJugador::repartirCarta();
                valorBanca = valorBanca + ManoBanca[contadorManos].numero;
                contadorManos++;
            }

            // Mostramos el valor de la banca y su mano
            ui -> pantalla_3 -> display(valorBanca);

            // Se incluyen todas las posibilidades para ganar.
            if ((valorBanca > 21) || (valorBanca < valorJugador)){      // Aquí faltan mas opciones. Mirar luego
                ui -> label_retirada ->  setText("Enhorabuena subnormal");

                saldo = saldo + apuesta * 2;
                ui -> pantalla -> display(saldo);

                ui -> boton_retirarse -> hide();
                ui -> boton_doblar -> hide();
                ui -> boton_dividir -> hide();
                ui -> boton_pedir_carta -> hide();
                ui -> label_valor -> hide();
                ui -> label_palo -> hide();

                ui -> boton_continuar -> show();
            }
            else {
                ui -> label_retirada -> setText("Has perdido, subnormal");

                ui -> boton_retirarse -> hide();
                ui -> boton_doblar -> hide();
                ui -> boton_dividir -> hide();
                ui -> boton_pedir_carta -> hide();
                ui -> label_valor -> hide();
                ui -> label_palo -> hide();

                ui -> boton_continuar -> show();
            }
        }
    }
    else {

        if ((valorJugador < 16) || (valorJugador2 < 16)){
            ui -> label_retirada -> setText("No te puedes retirar, subnormal");
            //Sleep(1000);
            //ui -> label_retirada -> hide();
        }
        else if (valorJugador > 21 && valorJugador2 > 21){
            ui -> label_retirada -> setText("Has perdido, subnormal");
            ui -> pantalla_3 -> display(valorBanca);

            // Tenemos que restar la apuesta del saldo porque hemos perdido con las dos manos como subnormales
            saldo = saldo - apuesta;
            ui -> pantalla -> display(saldo);

            ui -> boton_retirarse -> hide();
            ui -> boton_doblar -> hide();
            ui -> boton_dividir -> hide();
            ui -> boton_pedir_carta -> hide();
            ui -> label_valor -> hide();
            ui -> label_palo -> hide();

            ui -> boton_continuar -> show();
            }
        else {

            // Si la banca tiene menor valor que tú, intentará subirlo
            while ((valorBanca < valorJugador) || (valorBanca < valorJugador2)){
                ManoBanca[contadorManos] = UnJugador::repartirCarta();
                valorBanca = valorBanca + ManoBanca[contadorManos].numero;
                contadorManos++;
            }

            // Mostramos el valor de la banca y su mano
            ui -> pantalla_3 -> display(valorBanca);

            // Se incluyen todas las posibilidades para ganar.
            if (valorBanca > 21) {      // Aquí faltan mas opciones. Mirar luego
                ui -> label_retirada ->  setText("Enhorabuena subnormal");

                saldo = saldo + apuesta * 2;
                ui -> pantalla -> display(saldo);

                ui -> boton_retirarse -> hide();
                ui -> boton_doblar -> hide();
                ui -> boton_dividir -> hide();
                ui -> boton_pedir_carta -> hide();
                ui -> label_valor -> hide();
                ui -> label_palo -> hide();

                ui -> boton_continuar -> show();
            }
            else if((valorBanca < 21) && (valorBanca > valorJugador) && (valorBanca > valorJugador2) ){

                ui -> label_retirada -> setText("Has perdido, subnormal");

                // Tenemos que restar la apuesta del saldo porque hemos perdido con las dos manos como subnormales
                saldo = saldo - apuesta;
                ui -> pantalla -> display(saldo);

                ui -> boton_retirarse -> hide();
                ui -> boton_doblar -> hide();
                ui -> boton_dividir -> hide();
                ui -> boton_pedir_carta -> hide();
                ui -> label_valor -> hide();
                ui -> label_palo -> hide();

                ui -> boton_continuar -> show();
            }
            else if ((valorBanca < 21) && (valorBanca > valorJugador) && (valorBanca < valorJugador2)){
                ui -> label_retirada ->  setText("Te quedas como estabas subnormal");

                ui -> boton_retirarse -> hide();
                ui -> boton_doblar -> hide();
                ui -> boton_dividir -> hide();
                ui -> boton_pedir_carta -> hide();
                ui -> label_valor -> hide();
                ui -> label_palo -> hide();

                ui -> boton_continuar -> show();
            }
            else if ((valorBanca < 21) && (valorBanca < valorJugador) && (valorBanca > valorJugador2)){
                ui -> label_retirada ->  setText("Te quedas como estabas subnormal");

                ui -> boton_retirarse -> hide();
                ui -> boton_doblar -> hide();
                ui -> boton_dividir -> hide();
                ui -> boton_pedir_carta -> hide();
                ui -> label_valor -> hide();
                ui -> label_palo -> hide();

                ui -> boton_continuar -> show();
                }
            else {
                ui -> label_retirada -> setText("Has perdido, subnormal");

                ui -> boton_retirarse -> hide();
                ui -> boton_doblar -> hide();
                ui -> boton_dividir -> hide();
                ui -> boton_pedir_carta -> hide();
                ui -> label_valor -> hide();
                ui -> label_palo -> hide();

                ui -> boton_continuar -> show();
            }
        }
    }
}


void UnJugador::on_boton_continuar_clicked()
{
    UnJugador::resetear();
}

void UnJugador::on_boton_reiniciar_clicked()
{
    UnJugador::resetear();

    ui -> boton_retirarse -> hide();
    ui -> boton_doblar -> hide();
    ui -> boton_dividir -> hide();
    ui -> boton_pedir_carta -> hide();
    ui -> boton_continuar -> hide();
    ui -> label_valor -> hide();
    ui -> label_palo -> hide();

    saldo = 2000;
}

void UnJugador::on_boton_salir_clicked()
{
    this -> close();
    MainWindow *newmain = new MainWindow();
    newmain -> show();
}

void UnJugador::on_boton_doblar_clicked()
{
    if ((2 * apuesta) > saldo){
        ui -> label_retirada -> show();
        ui -> label_retirada -> setText("No tienes saldo suficiente, subnormal");
        }
    else { // Si se dispone de salgo suficiente se dobla la apuesta y se reparte una carta.
        UnJugador::crearMano();

        saldo = saldo - apuesta;
        ui -> pantalla -> display(saldo);

        apuesta = 2 * apuesta;
    }
}



void UnJugador::on_boton_dividir_clicked()
{
    // Comprobamos si las primeras dos cartas son iguales
    if (ManoJugador[0].numero == ManoJugador[1].numero){

        // Activamos el flag
        isDividir = true;

        // Creamos una nueva mano y le asingnamos la segunda carta y actualizamos el valor
        ManoJugador2[0] = ManoJugador[1];
        valorJugador = valorJugador2 + ManoJugador2[contadorManos].numero;

        // No aumentamos el contador de manos porque hemos perdido una carta al traspasarla. Repartimos una carta en la misma posición
        UnJugador::crearMano();

        // Ahora sí aumentamos el contador
        contadorManos++;

    } else {
         ui -> label_retirada -> setText("No se puede dividir la mano, subnormal");
    }
}
