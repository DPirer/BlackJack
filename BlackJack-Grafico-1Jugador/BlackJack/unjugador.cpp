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
int contadorBanca = 0;

//Se inicia el juego y se asigna un saldo de 2000
int saldo = 2000, valorJugador = 0, valorJugador2 = 0, valorBanca = 0, apuesta;

// Inicializo un vector de la clase Mano cuyo tamaño es el máximo posible para las cartas más bajas
Mano ManoJugador[11] {{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}};
Mano ManoJugador2[11] {{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}};
Mano ManoBanca[11] {{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}};

// Rutas de las imagenes
char direccion[]="C:/Users/Administrator/Desktop/Baraja_de_cartas/"; //Direccion de la carpeta
char jpg[]=".jpg";


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

    // Calculamos el valor de la carta segun su palo y valor de esta para buscar la imagen
    int palos;
    if (ManoJugador[contadorManos].palo == "Picas") {
        palos = 0;
    }
    else if (ManoJugador[contadorManos].palo == "Diamantes") {
        palos = 1;
    }
    else if (ManoJugador[contadorManos].palo == "Treboles") {
        palos = 2;
    }
    else {
        palos = 3;
    }
    int numeroCarta = 13 * palos + ManoJugador[contadorManos].numero;

    // Creamos la linea de codigo que nos indique donde se encuentra la carta a mostrar
    char direccion_Final[200];
    strcpy(direccion_Final,direccion);
    char baraja[25];
    itoa(numeroCarta,baraja,10);
    strcat(baraja,jpg);
    strcat(direccion_Final,baraja);
    QPixmap pix(direccion_Final);

    // Para mostrar la carta dependera de la ronda en la que nos encontremos, para ir usando
    // cada una de las etiquetas que asignemos. Por lo que se usara un switch
    int wi = ui -> Carta1J -> width();
    int he = ui -> Carta1J -> height();
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

    // Creamos la mano de la banca si la banca no ha llegado a 21
    if (valorBanca < 21){
        ManoBanca[contadorManos] = UnJugador::repartirCarta();
        valorBanca = valorBanca + ManoBanca[contadorManos].numero;
        contadorBanca++;

        // La banca unicamente enseñara la primera carta y el resto estaran boca abajo
        // Por lo que tendremos que distinguir entre la primera ronda y el resto
        // Creamos la linea de codigo que nos indique donde se encuentra la carta a mostrar
        char direccion_Final[200];
        strcpy(direccion_Final,direccion);
        char baraja[25];

        if (contadorManos == 0){
            if (ManoBanca[contadorManos].palo == "Picas") {
                palos = 0;
            }
            else if (ManoBanca[contadorManos].palo == "Diamantes") {
                palos = 1;
            }
            else if (ManoBanca[contadorManos].palo == "Treboles") {
                palos = 2;
            }
            else {
                palos = 3;
            }
            numeroCarta = 13 * palos + ManoBanca[contadorManos].numero;
        }
        else {
            numeroCarta = 0;
        }

        itoa(numeroCarta,baraja,10);
        strcat(baraja,jpg);
        strcat(direccion_Final,baraja);
        QPixmap pix(direccion_Final);

        // Mostramos la carta resultate
        switch (contadorManos){
                        case 0:
                            ui -> Carta1B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                            break;
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
    valorJugador = 0, valorBanca = 0, valorJugador2 = 0, contadorManos = 0, contadorBanca = 0, isDividir = false, BJ = false;

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

    // Eliminamos la imagenes de las cartas creadas
    ui -> Carta1J -> clear();
    ui -> Carta2J -> clear();
    ui -> Carta3J -> clear();
    ui -> Carta4J -> clear();
    ui -> Carta5J -> clear();

    ui -> Carta1B -> clear();
    ui -> Carta2B -> clear();
    ui -> Carta3B -> clear();
    ui -> Carta4B -> clear();
    ui -> Carta5B -> clear();
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

            // Se muestran las cartas de la banca
            for (int i=1;i<contadorBanca;i++){
                // Calculamos el valor de la carta segun su palo y valor de esta para buscar la imagen
                int palos;
                if (ManoBanca[i].palo == "Picas") {
                    palos = 0;
                }
                else if (ManoBanca[i].palo == "Diamantes") {
                    palos = 1;
                }
                else if (ManoBanca[i].palo == "Treboles") {
                    palos = 2;
                }
                else {
                    palos = 3;
                }
                int numeroCarta = 13 * palos + ManoBanca[i].numero;

                // Creamos la linea de codigo que nos indique donde se encuentra la carta a mostrar
                char direccion_Final[200];
                strcpy(direccion_Final,direccion);
                char baraja[25];
                itoa(numeroCarta,baraja,10);
                strcat(baraja,jpg);
                strcat(direccion_Final,baraja);
                QPixmap pix(direccion_Final);

                // Para mostrar la carta dependera de la ronda en la que nos encontremos, para ir usando
                // cada una de las etiquetas que asignemos. Por lo que se usara un switch
                int wi = ui -> Carta1J -> width();
                int he = ui -> Carta1J -> height();
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

            ui -> boton_retirarse -> hide();
            ui -> boton_doblar -> hide();
            ui -> boton_dividir -> hide();
            ui -> boton_pedir_carta -> hide();

            ui -> boton_continuar -> show();
            }
        else {
            // Si la banca tiene menor valor que tú, intentará subirlo
            while (valorBanca < valorJugador){
                ManoBanca[contadorManos] = UnJugador::repartirCarta();
                valorBanca = valorBanca + ManoBanca[contadorManos].numero;
                contadorManos++;
                contadorBanca++;
            }

            // Mostramos el valor de la banca y su mano
            ui -> pantalla_3 -> display(valorBanca);

            // Se muestran las cartas de la banca
            for (int i=1;i<contadorBanca;i++){
                // Calculamos el valor de la carta segun su palo y valor de esta para buscar la imagen
                int palos;
                if (ManoBanca[i].palo == "Picas") {
                    palos = 0;
                }
                else if (ManoBanca[i].palo == "Diamantes") {
                    palos = 1;
                }
                else if (ManoBanca[i].palo == "Treboles") {
                    palos = 2;
                }
                else {
                    palos = 3;
                }
                int numeroCarta = 13 * palos + ManoBanca[i].numero;

                // Creamos la linea de codigo que nos indique donde se encuentra la carta a mostrar
                char direccion_Final[200];
                strcpy(direccion_Final,direccion);
                char baraja[25];
                itoa(numeroCarta,baraja,10);
                strcat(baraja,jpg);
                strcat(direccion_Final,baraja);
                QPixmap pix(direccion_Final);

                // Para mostrar la carta dependera de la ronda en la que nos encontremos, para ir usando
                // cada una de las etiquetas que asignemos. Por lo que se usara un switch
                int wi = ui -> Carta1J -> width();
                int he = ui -> Carta1J -> height();
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
            if ((valorBanca > 21) || (valorBanca < valorJugador)){      // Aquí faltan mas opciones. Mirar luego
                ui -> label_retirada ->  setText("Enhorabuena subnormal");

                saldo = saldo + apuesta * 2;
                ui -> pantalla -> display(saldo);

                ui -> boton_retirarse -> hide();
                ui -> boton_doblar -> hide();
                ui -> boton_dividir -> hide();
                ui -> boton_pedir_carta -> hide();

                ui -> boton_continuar -> show();
            }
            else {
                ui -> label_retirada -> setText("Has perdido, subnormal");

                ui -> boton_retirarse -> hide();
                ui -> boton_doblar -> hide();
                ui -> boton_dividir -> hide();
                ui -> boton_pedir_carta -> hide();

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

            // Se muestran las cartas de la banca
            for (int i=1;i<contadorBanca;i++){
                // Calculamos el valor de la carta segun su palo y valor de esta para buscar la imagen
                int palos;
                if (ManoBanca[i].palo == "Picas") {
                    palos = 0;
                }
                else if (ManoBanca[i].palo == "Diamantes") {
                    palos = 1;
                }
                else if (ManoBanca[i].palo == "Treboles") {
                    palos = 2;
                }
                else {
                    palos = 3;
                }
                int numeroCarta = 13 * palos + ManoBanca[i].numero;

                // Creamos la linea de codigo que nos indique donde se encuentra la carta a mostrar
                char direccion_Final[200];
                strcpy(direccion_Final,direccion);
                char baraja[25];
                itoa(numeroCarta,baraja,10);
                strcat(baraja,jpg);
                strcat(direccion_Final,baraja);
                QPixmap pix(direccion_Final);

                // Para mostrar la carta dependera de la ronda en la que nos encontremos, para ir usando
                // cada una de las etiquetas que asignemos. Por lo que se usara un switch
                int wi = ui -> Carta1J -> width();
                int he = ui -> Carta1J -> height();
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

            // Tenemos que restar la apuesta del saldo porque hemos perdido con las dos manos como subnormales
            saldo = saldo - apuesta;
            ui -> pantalla -> display(saldo);

            ui -> boton_retirarse -> hide();
            ui -> boton_doblar -> hide();
            ui -> boton_dividir -> hide();
            ui -> boton_pedir_carta -> hide();

            ui -> boton_continuar -> show();
            }
        else {

            // Si la banca tiene menor valor que tú, intentará subirlo
            while ((valorBanca < valorJugador) || (valorBanca < valorJugador2)){
                ManoBanca[contadorManos] = UnJugador::repartirCarta();
                valorBanca = valorBanca + ManoBanca[contadorManos].numero;
                contadorManos++;
                contadorBanca++;
            }

            // Mostramos el valor de la banca y su mano
            ui -> pantalla_3 -> display(valorBanca);

            // Se muestran las cartas de la banca
            for (int i=1;i<contadorBanca;i++){
                // Calculamos el valor de la carta segun su palo y valor de esta para buscar la imagen
                int palos;
                if (ManoBanca[i].palo == "Picas") {
                    palos = 0;
                }
                else if (ManoBanca[i].palo == "Diamantes") {
                    palos = 1;
                }
                else if (ManoBanca[i].palo == "Treboles") {
                    palos = 2;
                }
                else {
                    palos = 3;
                }
                int numeroCarta = 13 * palos + ManoBanca[i].numero;

                // Creamos la linea de codigo que nos indique donde se encuentra la carta a mostrar
                char direccion_Final[200];
                strcpy(direccion_Final,direccion);
                char baraja[25];
                itoa(numeroCarta,baraja,10);
                strcat(baraja,jpg);
                strcat(direccion_Final,baraja);
                QPixmap pix(direccion_Final);

                // Para mostrar la carta dependera de la ronda en la que nos encontremos, para ir usando
                // cada una de las etiquetas que asignemos. Por lo que se usara un switch
                int wi = ui -> Carta1J -> width();
                int he = ui -> Carta1J -> height();
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
            if (valorBanca > 21) {      // Aquí faltan mas opciones. Mirar luego
                ui -> label_retirada ->  setText("Enhorabuena subnormal");

                saldo = saldo + apuesta * 2;
                ui -> pantalla -> display(saldo);

                ui -> boton_retirarse -> hide();
                ui -> boton_doblar -> hide();
                ui -> boton_dividir -> hide();
                ui -> boton_pedir_carta -> hide();

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

                ui -> boton_continuar -> show();
            }
            else if ((valorBanca < 21) && (valorBanca > valorJugador) && (valorBanca < valorJugador2)){
                ui -> label_retirada ->  setText("Te quedas como estabas subnormal");

                ui -> boton_retirarse -> hide();
                ui -> boton_doblar -> hide();
                ui -> boton_dividir -> hide();
                ui -> boton_pedir_carta -> hide();

                ui -> boton_continuar -> show();
            }
            else if ((valorBanca < 21) && (valorBanca < valorJugador) && (valorBanca > valorJugador2)){
                ui -> label_retirada ->  setText("Te quedas como estabas subnormal");

                ui -> boton_retirarse -> hide();
                ui -> boton_doblar -> hide();
                ui -> boton_dividir -> hide();
                ui -> boton_pedir_carta -> hide();

                ui -> boton_continuar -> show();
                }
            else {
                ui -> label_retirada -> setText("Has perdido, subnormal");

                ui -> boton_retirarse -> hide();
                ui -> boton_doblar -> hide();
                ui -> boton_dividir -> hide();
                ui -> boton_pedir_carta -> hide();

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
