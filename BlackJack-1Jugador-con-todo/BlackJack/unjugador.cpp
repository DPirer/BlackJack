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
    QPixmap MesaInicio("C:/Users/Administrator/Desktop/Baraja_de_cartas/Mesa_blackjack3.jpg");
    // Obtenemos la altura y anchura de la etiqueta para porder colocar bien la imagen
    int w = ui -> Mesa1 -> width();
    int h = ui -> Mesa1 -> height();
    // Imprimimos la imagen en la etiqueta
    ui -> Mesa1 -> setPixmap(MesaInicio.scaled(w,h,Qt::KeepAspectRatio));

    // Empezamos con todos los botones ocultos para que el usuario haga la apuesta sin joder nada
    ui -> boton_retirarse -> hide();
    ui -> boton_doblar -> hide();
    ui -> boton_dividir -> hide();
    ui -> boton_pedir_carta -> hide();
    ui -> boton_continuar -> hide();
    ui -> boton_pedir_carta2 -> hide();
    ui -> boton_doblar2 -> hide();
    ui -> label_retirada -> hide();
    ui -> label -> hide();
    ui -> label_3 -> hide();
    ui -> label_4 -> hide();
    ui -> label_valor_banca -> hide();
    ui -> pantalla -> hide();
    ui -> pantalla_2 -> hide();
    ui -> pantalla_3 -> hide();
    ui -> pantalla_4 -> hide();

}

UnJugador::~UnJugador()
{
    delete ui;
}



/* VARIABLES GLOBALES ***********************************************************************************************************************/

int matriz[4][13] = {{1,2,3,4,5,6,7,8,9,10,11,12,13}, {1,2,3,4,5,6,7,8,9,10,11,12,13},{1,2,3,4,5,6,7,8,9,10,11,12,13},{1,2,3,4,5,6,7,8,9,10,11,12,13}};

// isDividir se crea debido a que solo se puede usar dividir una vez
// Tambien se le añade un flag de pedir al segundo jugador cuando pida carta
bool isDividir, BJB = false, BJ1 = false, BJ2 = false, flagPedir = false;

// isDoblar son variables para saber que el jugador a doblado su mano o sus manos
bool isDoblar = false, isDoblar2 = false;

// Varaible para comprobar si estamos en la primera mano
int contadorManos = 0, contadorManos2 = 0;
// Variable a contar las cartas de la banca
int contadorBanca = 0;

// Se inicia el juego y se asigna un saldo de 2000
int saldo = 2000, valorJugador = 0, valorJugador2 = 0, valorBanca = 0, apuesta;

// Inicializo un vector de la clase Mano cuyo tamaño es el máximo posible para las cartas más bajas
Mano ManoJugador[11] {{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}};
Mano ManoJugador2[11] {{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}};
Mano ManoBanca[11] {{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}};

// Variables para saber si el As a cambiado de valor
int AsB = 0, AsJ1 = 0, AsJ2 = 0;
// Y sus punteros
int *punB = &AsB, *punJ1 = &AsJ1, *punJ2 = &AsJ2;


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
    // Al jugador se le repartira carta al inicio, cuando se divida o cuando se halla pulsado
    // el boton de pedir carta correspondiente
    if ((flagPedir == 0)||(contadorManos<=1)){
        ManoJugador[contadorManos] = UnJugador::repartirCarta();

        valorJugador = ValorCarta(valorJugador,contadorManos,ManoJugador,punJ1);

        ui -> pantalla_2 -> display(valorJugador);

        // Creamos la direccion de la carta del jugador
        QString direccionJ = UnJugador::CrearDireccion(ManoJugador[contadorManos].numero,ManoJugador[contadorManos].palo);
        // Se muestra la carta del jugador
        int wi = ui -> Carta1J -> width();
        int he = ui -> Carta1J -> height();
        QPixmap pix(direccionJ);
        switch (contadorManos){
                        case 0:
                            ui -> Carta1J -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                            break;
                        case 1:
                            ui -> Carta2J -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                            break;
                        case 2:
                            ui -> Carta3J -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                            break;
                        case 3:
                            ui -> Carta4J -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                            break;
                        case 4:
                            ui -> Carta5J -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                            break;
        }
        // Si la primera mano el jugador se pasara de 21 se le impide coger mas cartas
        if (valorJugador > 21) {
            ui -> boton_pedir_carta -> hide();
        }
    }

    // Creamos la mano de la banca si la banca no ha llegado a 21
    if (contadorBanca <= 1){
        ManoBanca[contadorManos] = UnJugador::repartirCarta();
        valorBanca = ValorCarta(valorBanca,contadorBanca,ManoBanca,punB);

        // La banca unicamente enseñara la primera carta y el resto estaran boca abajo
        // Por lo que tendremos que distinguir entre la primera ronda y el resto
        // Usamos la funcion que nos indique donde se encuentra la carta a mostrar
        QString direccionB;

        if (contadorBanca == 0){
            direccionB = UnJugador::CrearDireccion(ManoBanca[contadorManos].numero,ManoBanca[contadorManos].palo);
        }
        else {
            direccionB = UnJugador::CrearDireccion(0,"Picas");
        }

        QPixmap pix(direccionB);
        int wi = ui -> Carta1J -> width();
        int he = ui -> Carta1J -> height();
        // Mostramos la carta resultate
        switch (contadorManos){
                        case 0:
                            ui -> Carta1B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                            break;
                        case 1:
                            ui -> Carta2B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                            break;
        }
        contadorBanca++;
    }

    // Creamos una tercera mano para el caso en el que hayamos dividido cartas
    if ((isDividir == true)&&(flagPedir == true)) {
        ManoJugador2[contadorManos2] = UnJugador::repartirCarta();
        valorJugador2 = ValorCarta(valorJugador2,contadorManos2,ManoJugador2,punJ2);
        ui -> pantalla_4 -> display(valorJugador2);
        // Creamos la direccion de la carta del jugador
        QString direccionJ = UnJugador::CrearDireccion(ManoJugador2[contadorManos2].numero,ManoJugador2[contadorManos2].palo);
        // Se muestra la carta del jugador
        int wi = ui -> Carta1J -> width();
        int he = ui -> Carta1J -> height();
        QPixmap pix(direccionJ);
        switch (contadorManos2){
                        case 1:
                            ui -> Carta2J2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                            break;
                        case 2:
                            ui -> Carta3J2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                            break;
                        case 3:
                            ui -> Carta4J2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                            break;
                        case 4:
                            ui -> Carta5J2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                            break;
        }
        contadorManos2++;
        // Si la segunda mano el jugador se pasara de 21 se le impide coger mas cartas
        if (valorJugador2 > 21){
            ui -> boton_pedir_carta2 -> hide();
        }
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
    valorJugador = 0, valorBanca = 0, valorJugador2 = 0;
    contadorManos = 0, contadorBanca = 0, contadorManos2 = 0;
    isDividir = false, isDoblar = false, isDoblar2 = false;
    BJB = false, BJ1 = false, BJ2 = false;
    AsB = 0, AsJ1 = 0, AsJ2 = 0;

    // Ahora ocultamos y mostramos los elementos necesarios
    ui -> label_retirada -> hide();
    ui -> label -> hide();
    ui -> label_3 -> hide();
    ui -> label_4 -> hide();
    ui -> label_valor_banca -> hide();
    ui -> pantalla -> hide();
    ui -> pantalla_2 -> hide();
    ui -> pantalla_3 -> hide();
    ui -> pantalla_4 -> hide();

    ui -> boton_empezar -> show();
    ui -> label_2 -> show();
    ui -> selector_apuesta -> show();

    // Eliminamos la imagenes de las cartas creadas
    ui -> Carta1J -> clear();
    ui -> Carta2J -> clear();
    ui -> Carta3J -> clear();
    ui -> Carta4J -> clear();
    ui -> Carta5J -> clear();

    ui -> Carta1J2 -> clear();
    ui -> Carta2J2 -> clear();
    ui -> Carta3J2 -> clear();
    ui -> Carta4J2 -> clear();
    ui -> Carta5J2 -> clear();

    ui -> Carta1B -> clear();
    ui -> Carta2B -> clear();
    ui -> Carta3B -> clear();
    ui -> Carta4B -> clear();
    ui -> Carta5B -> clear();
}

bool UnJugador::comprobarBlackjack(Mano JugadorX[11]){

    bool flag = false;
    if (((JugadorX[0].numero == 1) && (JugadorX[1].numero >= 10))||((JugadorX[1].numero == 1) && (JugadorX[0].numero >= 10))){

        flag = true;

    }
    return flag;
}


QString UnJugador::CrearDireccion(int numero, QString Palo){

    // Calculamos el valor de la carta segun su palo y valor de esta para buscar la imagen
    int palos;
    if (Palo == "Picas") {
        palos = 0;
    }
    else if (Palo == "Diamantes") {
        palos = 1;
    }
    else if (Palo == "Treboles") {
        palos = 2;
    }
    else {
        palos = 3;
    }
    int numeroCarta = 13 * palos + numero;


    // Rutas de las imagenes
    char direccion[] = "C:/Program Files (x86)/BlackJack/Imagenes/"; //Direccion de la carpeta
    char jpg[] = ".jpg";
    char direccion_Final[200];
    char baraja[25];
    strcpy(direccion_Final,direccion);
    itoa(numeroCarta,baraja,10);
    strcat(baraja,jpg);
    strcat(direccion_Final,baraja);
    return direccion_Final;

}

int UnJugador::ValorCarta(int ValorMano,int Contador,Mano JugadorX[11],int *puntero){
    // Actualizamos el valor total de las cartas del jugador y las mostramos por pantalla
    // Si la carta es una figura se le asigna el valor de 10 y si es un As 11
    if (JugadorX[Contador].numero > 10){
        ValorMano = ValorMano + 10;
    }
    else if (JugadorX[Contador].numero == 1){
        ValorMano = ValorMano + 11;
    }
    else {
        ValorMano = ValorMano + JugadorX[Contador].numero;
    }
    // El valor del As puede variar durante del recorrido de la partida
    // Por lo que hay que actualizarlo
    // El As puede valer 11 o 1, por lo que si el jugador se pasa de 21 el As pasa a valer 1.
    int contadorAs = 0;
    if (ValorMano>21){
        for (int i=0;i<=Contador;i++){
            if (JugadorX[i].numero == 1){
                contadorAs++;
            }
        }
        if ( contadorAs > *puntero){
            ValorMano = ValorMano - 10;
            *puntero = *puntero + 1;
        }
    }
    return ValorMano;
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

}



void UnJugador::on_boton_pedir_carta_clicked()
{
    UnJugador::crearMano();
    // Ya no se podra doblar ni dividir
    ui -> boton_doblar -> hide();
    ui -> boton_dividir -> hide();
}



void UnJugador::on_boton_retirarse_clicked()
{
    ui -> label_retirada -> show();
    ui -> boton_retirarse -> hide();
    ui -> boton_doblar -> hide();
    ui -> boton_dividir -> hide();
    ui -> boton_pedir_carta -> hide();

    ui -> boton_continuar -> show();
    int wi = ui -> Carta1B -> width();
    int he = ui -> Carta1B -> height();

    // Comprobamos si la banca o el jugador tiene BlackJack
    BJB = comprobarBlackjack(ManoBanca);
    BJ1 = comprobarBlackjack(ManoJugador);
    if (isDividir == true){
        BJ2 = comprobarBlackjack(ManoJugador2);
    }

    // El primer caso corresponde cuando el jugador no ha dividido
    if (isDividir == false){

        // Si el jugador de pasa de 21 pierde y se termina la ronda
        if (valorJugador > 21){
            ui -> label_retirada -> setText("Has perdido");
            ui -> pantalla_3 -> display(valorBanca);

            // Se muestran las cartas de la banca
            // Buscamos y creamos la direccion de la carta
            QString direccionB = UnJugador::CrearDireccion(ManoBanca[1].numero,ManoBanca[1].palo);
            QPixmap pix(direccionB);

            // Mostramos la carta resultate
            ui -> Carta2B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));

        }
        // En este caso se comprueba el ganador si se a activado alguno de los flags de BlackJack
        else if (BJB == true || BJ1 == true){
            if (BJB == true && BJ1 == true){
                ui -> label_retirada ->  setText("Te quedas igual");
                    saldo = saldo + apuesta;
            }
            else if (BJB == true){
                ui -> label_retirada -> setText("Has perdido");
            }
            else {
                ui -> label_retirada ->  setText("Enhorabuena");
                    saldo = saldo + apuesta * 3;
            }
            ui -> pantalla -> display(saldo);
        }
        else {
            // Si la banca tiene una valor menor de 17 pedira carta
            while (valorBanca < 17){
                ManoBanca[contadorBanca] = UnJugador::repartirCarta();
                valorBanca = ValorCarta(valorBanca,contadorBanca,ManoBanca,punB);
                contadorBanca++;
            }

            // Mostramos el valor de la banca y su mano
            ui -> pantalla_3 -> display(valorBanca);

            // Se muestran las cartas de la banca
            for (int i=1;i<contadorBanca;i++){
                // Buscamos y creamos la direccion de la carta
                QString direccionB = UnJugador::CrearDireccion(ManoBanca[i].numero,ManoBanca[i].palo);
                QPixmap pix(direccionB);

                // Mostramos la carta resultate
                switch (i){
                                case 1:
                                    ui -> Carta2B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 2:
                                    ui -> Carta3B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 3:
                                    ui -> Carta4B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 4:
                                    ui -> Carta5B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                }
            }

            // Se incluyen todas las posibilidades del resultado de la partida.
            if ((valorBanca > 21) || (valorBanca < valorJugador)){
                ui -> label_retirada ->  setText("Enhorabuena");
                if (isDoblar == true){
                    saldo = saldo + apuesta * 4;
                }
                else {
                    saldo = saldo + apuesta * 2;
                }
                ui -> pantalla -> display(saldo);
            }
            else if(valorBanca == valorJugador){
                ui -> label_retirada ->  setText("Te quedas igual");
                if (isDoblar == true){
                    saldo = saldo + apuesta * 2;
                }
                else {
                    saldo = saldo + apuesta;
                }
                ui -> pantalla -> display(saldo);

            }
            else {
                ui -> label_retirada -> setText("Has perdido");

            }
        }
    }
    // Si se ha dividido la mano
    else {
        ui -> boton_pedir_carta2 -> hide();
        ui -> boton_doblar2 -> hide();

        // Si el jugador se pasa con sus dos manos
        if ((valorJugador > 21) && (valorJugador2 > 21)){
            ui -> label_retirada -> setText("Has perdido");
            ui -> pantalla_3 -> display(valorBanca);

            // Se muestran las cartas de la banca

            // Buscamos y creamos la direccion de la carta
            QString direccionB = UnJugador::CrearDireccion(ManoBanca[1].numero,ManoBanca[1].palo);
            QPixmap pix(direccionB);

            // Mostramos la carta resultate
            ui -> Carta2B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));

            ui -> boton_retirarse -> hide();
            ui -> boton_doblar -> hide();
            ui -> boton_dividir -> hide();
            ui -> boton_pedir_carta -> hide();

            ui -> boton_continuar -> show();
        }

        // Si la banca tiene BlackJack
        else if (BJB == true){
            if (BJ1 == true && BJ2 == true){
                ui -> label_retirada ->  setText("Te quedas igual");
                saldo = saldo + apuesta * 2;
            }
            else if (BJ1 == true || BJ2 == true){
                ui -> label_retirada ->  setText("Has perdido");
                saldo = saldo + apuesta;
            }
            else {
                ui -> label_retirada ->  setText("Has perdido");
            }
            ui -> pantalla -> display(saldo);
        }
        else {

            // Si la banca tiene una valor menos de 17 pedira carta
            while (valorBanca < 17){
                ManoBanca[contadorBanca] = UnJugador::repartirCarta();
                valorBanca = ValorCarta(valorBanca,contadorBanca,ManoBanca,punB);
                contadorBanca++;
            }

            // Mostramos el valor de la banca y su mano
            ui -> pantalla_3 -> display(valorBanca);

            // Se muestran las cartas de la banca
            for (int i=1;i<contadorBanca;i++){
                // Buscamos y creamos la direccion de la carta
                QString direccionB = UnJugador::CrearDireccion(ManoBanca[i].numero,ManoBanca[i].palo);
                QPixmap pix(direccionB);

                // Mostramos la carta resultate
                switch (i){
                                case 1:
                                    ui -> Carta2B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 2:
                                    ui -> Carta3B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 3:
                                    ui -> Carta4B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 4:
                                    ui -> Carta5B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                }
            }

            // Se incluyen todas las posibilidades para ganar.
            if (valorBanca > 21) {
                // Si hay algun BlackJack del jugador
                if (BJ1 == true || BJ2 == true) {
                    if (BJ1 == true && BJ2 == true){
                        ui -> label_retirada ->  setText("Enhorabuena");
                        saldo = saldo + apuesta * 6;
                    }
                    else if (BJ1 == true){
                        if (valorJugador2<=21){
                            ui -> label_retirada ->  setText("Enhorabuena");
                            if (isDoblar2 == true){
                                saldo = saldo + apuesta * 7;
                            }
                            else {
                                saldo = saldo + apuesta * 5;
                            }
                        }
                        else {
                            if (isDoblar2 == true){
                                ui -> label_retirada ->  setText("Te quedas igual");
                                saldo = saldo + apuesta * 3;
                            }
                            else {
                                ui -> label_retirada ->  setText("Enhorabuena");
                                saldo = saldo + apuesta * 3;
                            }
                        }
                    }
                    else {
                        if (valorJugador<=21){
                            ui -> label_retirada ->  setText("Enhorabuena");
                            if (isDoblar == true){
                                saldo = saldo + apuesta * 7;
                            }
                            else {
                                saldo = saldo + apuesta * 5;
                            }
                        }
                        else {
                            if (isDoblar == true){
                                ui -> label_retirada ->  setText("Te quedas igual");
                                saldo = saldo + apuesta * 3;
                            }
                            else {
                                ui -> label_retirada ->  setText("Enhorabuena");
                                saldo = saldo + apuesta * 3;
                            }
                        }
                    }
                    ui -> pantalla -> display(saldo);
                }
                // Si las dos manos tiene menos de 21
                else if ((valorJugador<=21)&&(valorJugador2<=21))
                {
                    ui -> label_retirada ->  setText("Enhorabuena");

                    if ((isDoblar == true)&&(isDoblar2 == true)){
                        saldo = saldo + apuesta * 8;
                    }
                    else if ((isDoblar == true)||(isDoblar2 == true)) {
                        saldo = saldo + apuesta * 6;
                    }
                    else {
                        saldo = saldo + apuesta * 4;
                    }
                    ui -> pantalla -> display(saldo);

                }
                // Si la primera mano tiene mas de 21
                else if (valorJugador > 21) {

                    if ((isDoblar == true)&&(isDoblar2 == true)){
                        ui -> label_retirada ->  setText("Te quedas igual");
                        saldo = saldo + apuesta * 4;
                    }
                    else if (isDoblar == true) {
                        ui -> label_retirada -> setText("Has perdido");
                        saldo = saldo + apuesta * 2;
                    }
                    else if (isDoblar2 == true) {
                        ui -> label_retirada ->  setText("Enhorabuena");
                        saldo = saldo + apuesta * 4;
                    }
                    else {
                        ui -> label_retirada ->  setText("Te quedas igual");
                        saldo = saldo + apuesta * 2;
                    }

                    ui -> pantalla -> display(saldo);

                }
                // Si la segundo mano tiene mas de 21
                else {
                    if ((isDoblar == true)&&(isDoblar2 == true)){
                            ui -> label_retirada ->  setText("Te quedas igual");
                            saldo = saldo + apuesta * 4;
                    }
                    else if (isDoblar == true) {
                        ui -> label_retirada ->  setText("Enhorabuena");
                        saldo = saldo + apuesta * 4;
                    }
                    else if (isDoblar2 == true) {
                        ui -> label_retirada -> setText("Has perdido");
                        saldo = saldo + apuesta * 2;
                    }
                    else {
                        ui -> label_retirada ->  setText("Te quedas igual");
                        saldo = saldo + apuesta * 2;
                    }
                }

            }
            else if(valorJugador2 > 21 || valorJugador > 21) {
                if (valorJugador > 21) {
                    if (valorJugador2 > valorBanca){
                        if ((isDoblar == true)&&(isDoblar2 == true)){
                                ui -> label_retirada ->  setText("Te quedas igual");
                                saldo = saldo + apuesta * 4;
                        }
                        else if (isDoblar == true) {
                            ui -> label_retirada -> setText("Has perdido");
                            saldo = saldo + apuesta * 2;
                        }
                        else if (isDoblar2 == true) {
                            ui -> label_retirada ->  setText("Enhorabuena");
                            saldo = saldo + apuesta * 4;
                        }
                        else {
                            ui -> label_retirada ->  setText("Te quedas igual");
                            saldo = saldo + apuesta * 2;
                        }
                    }
                    else if (valorJugador2 == valorBanca){
                        if ((isDoblar == true)&&(isDoblar2 == true)){
                            ui -> label_retirada ->  setText("Has perdido");
                            saldo = saldo + apuesta * 2;
                        }
                        else if (isDoblar == true) {
                            ui -> label_retirada -> setText("Has perdido");
                            saldo = saldo + apuesta;
                        }
                        else if (isDoblar2 == true) {
                            ui -> label_retirada ->  setText("Has perdido");
                            saldo = saldo + apuesta * 2;
                        }
                        else {
                            ui -> label_retirada ->  setText("Has perdido");
                            saldo = saldo + apuesta;
                        }
                    }
                    else {
                        ui -> label_retirada -> setText("Has perdido");
                    }
                }
                else {
                    if (valorJugador > valorBanca){
                        if ((isDoblar == true)&&(isDoblar2 == true)){
                            ui -> label_retirada ->  setText("Te quedas igual");
                            saldo = saldo + apuesta * 4;
                        }
                        else if (isDoblar == true) {
                            ui -> label_retirada -> setText("Enhorabuena");
                            saldo = saldo + apuesta * 4;
                        }
                        else if (isDoblar2 == true) {
                            ui -> label_retirada ->  setText("Has perdido");
                            saldo = saldo + apuesta * 2;
                        }
                        else {
                            ui -> label_retirada ->  setText("Te quedas igual");
                            saldo = saldo + apuesta * 2;
                        }
                    }
                    else if (valorJugador == valorBanca){
                        ui -> label_retirada ->  setText("Has perdido");
                        if ((isDoblar == true)&&(isDoblar2 == true)){
                            saldo = saldo + apuesta * 2;
                        }
                        else if (isDoblar == true) {
                            saldo = saldo + apuesta * 2;
                        }
                        else if (isDoblar2 == true) {
                            saldo = saldo + apuesta;
                        }
                        else {
                            saldo = saldo + apuesta;
                        }
                    }
                    else {
                        ui -> label_retirada -> setText("Has perdido");
                    }
                }
            }
            else {
                if (BJ1 == true || BJ2 == true) {
                    if (BJ1 == true && BJ2 == true) {
                        ui -> label_retirada ->  setText("Enhorabuena");
                        saldo = saldo + apuesta * 6;
                    }
                    else if (BJ1 == true) {
                        ui -> label_retirada ->  setText("Enhorabuena");
                        if (valorJugador2 > valorBanca){
                            if (isDoblar2 == true){
                                saldo = saldo + apuesta * 7;
                            }
                            else {
                                saldo = saldo + apuesta * 5;
                            }
                        }
                        else if (valorJugador2 == valorBanca){
                            if (isDoblar2 == true){
                                saldo = saldo + apuesta * 5;
                            }
                            else {
                                saldo = saldo + apuesta * 4;
                            }
                        }
                        else {
                            saldo = saldo + apuesta * 3;
                        }
                    }
                    else {
                        ui -> label_retirada ->  setText("Enhorabuena");
                        if (valorJugador > valorBanca){
                            if (isDoblar == true){
                                saldo = saldo + apuesta * 7;
                            }
                            else {
                                saldo = saldo + apuesta * 5;
                            }
                        }
                        else if (valorJugador == valorBanca){
                            if (isDoblar == true){
                                saldo = saldo + apuesta * 5;
                            }
                            else {
                                saldo = saldo + apuesta * 4;
                            }
                        }
                        else {
                            saldo = saldo + apuesta * 3;
                        }
                    }
                    ui -> pantalla -> display(saldo);
                }
                else if ((valorBanca < valorJugador) && (valorBanca < valorJugador2) ){ // Ganan los dos

                    ui -> label_retirada ->  setText("Enhorabuena");
                    // Tenemos que delvolver las apuestas con los beneficion
                    if ((isDoblar == true)&&(isDoblar2 == true)){
                        saldo = saldo + apuesta * 8;
                    }
                    else if (isDoblar == true) {
                        saldo = saldo + apuesta * 6;
                    }
                    else if (isDoblar2 == true) {
                        saldo = saldo + apuesta * 6;
                    }
                    else {
                        saldo = saldo + apuesta * 4;
                    }

                    ui -> pantalla -> display(saldo);

                }
                else if ((valorBanca < valorJugador) && (valorBanca == valorJugador2)){
                    ui -> label_retirada ->  setText("Enhorabuena");
                    if ((isDoblar == true)&&(isDoblar2 == true)){
                        saldo = saldo + apuesta * 6;
                    }
                    else if (isDoblar == true) {
                        saldo = saldo + apuesta * 5;
                    }
                    else if (isDoblar2 == true) {
                        saldo = saldo + apuesta * 4;
                    }
                    else {
                        saldo = saldo + apuesta * 3;
                    }

                    ui -> pantalla -> display(saldo);

                }
                else if ((valorBanca == valorJugador) && (valorBanca < valorJugador2)){
                    ui -> label_retirada ->  setText("Enhorabuena");
                    if ((isDoblar == true)&&(isDoblar2 == true)){
                        saldo = saldo + apuesta * 6;
                    }
                    else if (isDoblar == true) {
                        saldo = saldo + apuesta * 4;
                    }
                    else if (isDoblar2 == true) {
                        saldo = saldo + apuesta * 5;
                    }
                    else {
                        saldo = saldo + apuesta * 3;
                    }

                    ui -> pantalla -> display(saldo);

                    }
                else if ((valorBanca < valorJugador) && (valorBanca > valorJugador2)){
                    if ((isDoblar == true)&&(isDoblar2 == true)){
                        ui -> label_retirada ->  setText("Te quedas igual");
                        saldo = saldo + apuesta * 4;
                    }
                    else if (isDoblar == true) {
                        ui -> label_retirada ->  setText("Enhorabuena");
                        saldo = saldo + apuesta * 4;
                    }
                    else if (isDoblar2 == true) {
                        ui -> label_retirada -> setText("Has perdido");
                        saldo = saldo + apuesta * 2;
                    }
                    else {
                        ui -> label_retirada ->  setText("Te quedas igual");
                        saldo = saldo + apuesta * 2;
                    }

                    ui -> pantalla -> display(saldo);

                }
                else if ((valorBanca > valorJugador) && (valorBanca < valorJugador2)){
                    if ((isDoblar == true)&&(isDoblar2 == true)){
                        ui -> label_retirada ->  setText("Te quedas igual");
                        saldo = saldo + apuesta * 4;
                    }
                    else if (isDoblar == true) {
                        ui -> label_retirada -> setText("Has perdido");
                        saldo = saldo + apuesta * 2;
                    }
                    else if (isDoblar2 == true) {
                        ui -> label_retirada ->  setText("Enhorabuena");
                        saldo = saldo + apuesta * 4;
                    }
                    else {
                        ui -> label_retirada ->  setText("Te quedas igual");
                        saldo = saldo + apuesta * 2;
                    }

                    ui -> pantalla -> display(saldo);

                }
                else if ((valorBanca == valorJugador) && (valorBanca == valorJugador2)){
                    ui -> label_retirada ->  setText("Te quedas igual");
                    if ((isDoblar == true)&&(isDoblar2 == true)){
                        saldo = saldo + apuesta * 4;
                    }
                    else if (isDoblar == true) {
                        saldo = saldo + apuesta * 3;
                    }
                    else if (isDoblar2 == true) {
                        saldo = saldo + apuesta * 3;
                    }
                    else {
                        saldo = saldo + apuesta * 2;
                    }

                    ui -> pantalla -> display(saldo);

                }
                else if ((valorBanca > valorJugador) && (valorBanca == valorJugador2)){
                    ui -> label_retirada -> setText("Has perdido");
                    if ((isDoblar == true)&&(isDoblar2 == true)){
                        saldo = saldo + apuesta * 2;
                    }
                    else if (isDoblar == true) {
                        saldo = saldo + apuesta;
                    }
                    else if (isDoblar2 == true) {
                        saldo = saldo + apuesta * 2;
                    }
                    else {
                        saldo = saldo + apuesta;
                    }

                    ui -> pantalla -> display(saldo);

                }
                else if ((valorBanca == valorJugador) && (valorBanca > valorJugador2)){
                    ui -> label_retirada -> setText("Has perdido");
                    if ((isDoblar == true)&&(isDoblar2 == true)){
                        saldo = saldo + apuesta * 2;
                    }
                    else if (isDoblar == true) {
                        saldo = saldo + apuesta * 2;
                    }
                    else if (isDoblar2 == true) {
                        saldo = saldo + apuesta;
                    }
                    else {
                        saldo = saldo + apuesta;
                    }

                    ui -> pantalla -> display(saldo);

                }
                else {
                    ui -> label_retirada -> setText("Has perdido");
                }
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

        isDoblar = true;

        UnJugador::crearMano();

        saldo = saldo - apuesta;
        ui -> pantalla -> display(saldo);

        // Al doblar ya al jugador solo se le permite jugar con esas cartas
        // Y solo se le permirira retirarase
        ui -> boton_doblar -> hide();
        ui -> boton_dividir -> hide();
        ui -> boton_pedir_carta -> hide();
    }
}



void UnJugador::on_boton_dividir_clicked()
{
    // Comprobamos si las primeras dos cartas son iguales
    if (true){

        // Activamos el flag
        isDividir = true;
        contadorManos2++;

        // Creamos una nueva mano y le asingnamos la segunda carta y actualizamos el valor
        ManoJugador2[0] = ManoJugador[1];
        valorJugador = ValorCarta(0,0,ManoJugador,punJ1);
        valorJugador2 = ValorCarta(valorJugador2,0,ManoJugador2,punJ2);
        // Creamos la direccion de la carta del jugador
        QString direccionJ = UnJugador::CrearDireccion(ManoJugador2[0].numero,ManoJugador2[0].palo);
        // Se muestra la carta del jugador
        int wi = ui -> Carta1J -> width();
        int he = ui -> Carta1J -> height();
        QPixmap pix(direccionJ);
        ui -> Carta1J2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
        // Reducimos el contador de manos porque hemos perdido una carta al traspasarla.
        // Repartimos una carta en la misma posición
        contadorManos--;

        saldo = saldo - apuesta;
        ui -> pantalla -> display(saldo);

        ui -> label_4 -> show();
        ui -> pantalla_4-> show();
        ui -> boton_pedir_carta2 -> show();
        ui -> boton_doblar2 -> show();
        ui -> boton_dividir -> hide();

        flagPedir = true;
        UnJugador::crearMano();
        flagPedir = false;

    } else {
         ui -> label_retirada -> setText("No se puede dividir la mano");
    }
}

void UnJugador::on_boton_pedir_carta2_clicked()
{
    flagPedir = true;
    UnJugador::crearMano();
    flagPedir = false;
    // Ya no se podra doblar
    ui -> boton_doblar2 -> hide();
}


void UnJugador::on_boton_doblar2_clicked()
{
    if ((2 * apuesta) > saldo){
        ui -> label_retirada -> show();
        ui -> label_retirada -> setText("No tienes saldo suficiente, subnormal");
        }
    else { // Si se dispone de salgo suficiente se dobla la apuesta y se reparte una carta.

        isDoblar2 = true;
        flagPedir = true;
        UnJugador::crearMano();
        flagPedir = false;

        saldo = saldo - apuesta;
        ui -> pantalla -> display(saldo);

        // Al doblar ya al jugador solo se le permite jugar con esas cartas
        // Y solo se le permirira retirarase
        ui -> boton_doblar2 -> hide();
        ui -> boton_pedir_carta2 -> hide();
    }
}
