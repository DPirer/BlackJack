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

    ui -> label_retirada -> setText("Turno Jugador 1");

}

UnJugador::~UnJugador()
{
    delete ui;
}



/* VARIABLES GLOBALES ***********************************************************************************************************************/

int matriz[4][13] = {{1,2,3,4,5,6,7,8,9,10,11,12,13}, {1,2,3,4,5,6,7,8,9,10,11,12,13},{1,2,3,4,5,6,7,8,9,10,11,12,13},{1,2,3,4,5,6,7,8,9,10,11,12,13}};

// isDividir se crea debido a que solo se puede usar dividir una vez
// Tambien se le añade un flag de pedir al segundo jugador cuando pida carta
vector<bool> BJJugadores(4,false);
vector<bool> BJDivision(4,false);
vector<bool> Dividir(4,false);
bool BJB = false, flagPedir = false;

// isDoblar son variables para saber que el jugador a doblado su mano o sus manos
vector<bool> isDoblarJ(4,false);
vector<bool> isDoblarD(4,false);

// Varaibles para comprobar si estamos en la primera mano
vector<int> contadorManosJ(4);
vector<int> contadorManosD(4);
// Variable a contar las cartas de la banca
int contadorBanca = 0;
// Para recoger los saldos
int contadorSaldos = 0;

// Se inicia el juego y se asigna un saldo de 2000
vector<int> saldos(4,2000);
vector<int> apuestas(4);
vector<int> valorJugadores(4);
vector<int> valorDivisiones(4);
int valorBanca = 0;

// Inicializo un vector de la clase Mano cuyo tamaño es el máximo posible para las cartas más bajas
Mano ManoBanca[11] {{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}};
// Creamos matrices para las manos de los jugadores y sus divisiones
Mano Jugadores[4][11] {{{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}},{{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}},{{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}},{{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}}};
Mano Divisiones[4][11] {{{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}},{{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}},{{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}},{{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}}};

// Variables para saber si el As a cambiado de valor
int AsB = 0;
int AsJugadores[4] = {0,0,0,0};
int AsDivisiones[4] = {0,0,0,0};
// Y sus punteros
int *punB = &AsB;
int *AsJ = &AsJugadores[0];
int *AsD = &AsDivisiones[0];

// Numero de jugadores recogido en el mainwindow
extern int NumeroJ;



/* FUNCIONES *******************************************************************************************************************************/

Mano UnJugador::repartirCarta(){

    // Se crea los numeros aleatorios que corresponden al palo y numero y se comprueban si existen
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

void UnJugador::crearMano(int jugador){
    // Creamos la mano del jugador
    // Al jugador se le repartira carta al inicio, cuando se divida o cuando se halla pulsado
    // el boton de pedir carta correspondiente
    if ((flagPedir == 0)||(contadorManosJ[jugador]<=1)){
        // Se le reparte una carta usando la funcion repartirCarta
        Jugadores[jugador][contadorManosJ[jugador]] = UnJugador::repartirCarta();
        // Se calcula el valor de la carta en funcion del valor de la mano
        valorJugadores[jugador] = ValorCarta(valorJugadores[jugador],contadorManosJ[jugador],Jugadores[jugador],AsJ);

        ui -> pantalla_2 -> display(valorJugadores[jugador]);

        // Creamos la direccion de la imagen de la carta del jugador
        QString direccionJ = UnJugador::CrearDireccion(Jugadores[jugador][contadorManosJ[jugador]].numero,Jugadores[jugador][contadorManosJ[jugador]].palo);
        // Mostramos las cartas en la etiquetas
        mostrarCarta(jugador,direccionJ);
        // Si la primera mano el jugador se pasara de 21 se le impide coger mas cartas
        if (valorJugadores[jugador] > 21) {
            ui -> boton_pedir_carta -> hide();
        }
        contadorManosJ[jugador]++;
    }

    // Creamos una tercera mano para el caso en el que hayamos dividido cartas
    if ((Dividir[jugador] == true)&&(flagPedir == true)) {
        Divisiones[jugador][contadorManosD[jugador]] = UnJugador::repartirCarta();
        valorDivisiones[jugador] = ValorCarta(valorDivisiones[jugador],contadorManosD[jugador],Divisiones[jugador],AsD);
        ui -> pantalla_4 -> display(valorDivisiones[jugador]);
        // Creamos la direccion de la carta del jugador
        QString direccionJ = UnJugador::CrearDireccion(Divisiones[jugador][contadorManosD[jugador]].numero,Divisiones[jugador][contadorManosD[jugador]].palo);
        // Se muestra la carta del jugador
        mostrarCarta(jugador,direccionJ);
        contadorManosD[jugador]++;
        // Si la segunda mano el jugador se pasara de 21 se le impide coger mas cartas
        if (valorDivisiones[jugador] > 21){
            ui -> boton_pedir_carta2 -> hide();
        }
    }
}

void UnJugador::resetear(){
    // Si reinicializo la matriz de forma normal me da error excess elements of scalar initializer, asi que uso un bucle
    for (int i = 0; i<4 ; i++){
        for (int j = 0; j<13; j++){
            matriz[i][j] = j;
        }
    }

    // Reseteamos las variables
    // Primera los de la banca
    valorBanca = 0;
    contadorBanca = 0;
    BJB = false;;
    AsB = 0;
    contadorSaldos = 0;
    // Las que estan contenidas en vectores
    for (int i=0;i<NumeroJ;i++){
        BJJugadores[i] = false;
        BJDivision[i] = false;
        Dividir[i] = false;
        isDoblarJ[i] = false;
        isDoblarD[i] = false;
        contadorManosJ[i] = 0;
        contadorManosD[i] = 0;
        apuestas[i] = 0;
        valorJugadores[i] = 0;
        valorDivisiones[i] = 0;
        AsJugadores[i] = 0;
        AsDivisiones[i] = 0;
        AsJ = &AsJugadores[0];
        AsD = &AsDivisiones[0];
    }

    // Ahora ocultamos y mostramos los elementos necesarios
    ui -> label_retirada -> setText("Turno Jugador 1");
    ui -> label_3 -> hide();
    ui -> label_4 -> hide();
    ui -> label_valor_banca -> hide();
    ui -> pantalla_2 -> hide();
    ui -> pantalla_3 -> hide();
    ui -> pantalla_4 -> hide();
    ui -> label_final2 -> hide();
    ui -> label_final3 -> hide();
    ui -> label_final4 -> hide();

    ui -> boton_empezar -> show();
    ui -> label_2 -> show();
    ui -> selector_apuesta -> show();

    // Eliminamos la imagenes de las cartas creadas
    ui -> Carta1J1 -> clear();
    ui -> Carta2J1 -> clear();
    ui -> Carta3J1 -> clear();
    ui -> Carta4J1 -> clear();
    ui -> Carta5J1 -> clear();

    ui -> Carta1D1 -> clear();
    ui -> Carta2D1 -> clear();
    ui -> Carta3D1 -> clear();
    ui -> Carta4D1 -> clear();
    ui -> Carta5D1 -> clear();

    ui -> Carta1J2 -> clear();
    ui -> Carta2J2 -> clear();
    ui -> Carta3J2 -> clear();
    ui -> Carta4J2 -> clear();
    ui -> Carta5J2 -> clear();

    ui -> Carta1D2 -> clear();
    ui -> Carta2D2 -> clear();
    ui -> Carta3D2 -> clear();
    ui -> Carta4D2 -> clear();
    ui -> Carta5D2 -> clear();

    ui -> Carta1J3 -> clear();
    ui -> Carta2J3 -> clear();
    ui -> Carta3J3 -> clear();
    ui -> Carta4J3 -> clear();
    ui -> Carta5J3 -> clear();

    ui -> Carta1D3 -> clear();
    ui -> Carta2D3 -> clear();
    ui -> Carta3D3 -> clear();
    ui -> Carta4D3 -> clear();
    ui -> Carta5D3 -> clear();

    ui -> Carta1J4 -> clear();
    ui -> Carta2J4 -> clear();
    ui -> Carta3J4 -> clear();
    ui -> Carta4J4 -> clear();
    ui -> Carta5J4 -> clear();

    ui -> Carta1D4 -> clear();
    ui -> Carta2D4 -> clear();
    ui -> Carta3D4 -> clear();
    ui -> Carta4D4 -> clear();
    ui -> Carta5D4 -> clear();

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

void UnJugador::crearBanca(){
    ManoBanca[contadorBanca] = UnJugador::repartirCarta();
    valorBanca = ValorCarta(valorBanca,contadorBanca,ManoBanca,punB);

    // La banca unicamente enseñara la primera carta y el resto estaran boca abajo
    // Por lo que tendremos que distinguir entre la primera ronda y el resto
    // Usamos la funcion que nos indique donde se encuentra la carta a mostrar
    QString direccionB;

    if (contadorBanca == 0){
        direccionB = UnJugador::CrearDireccion(ManoBanca[contadorBanca].numero,ManoBanca[contadorBanca].palo);
        ui -> pantalla_3 -> display(valorBanca);
    }
    else if (contadorBanca == 1) {
        direccionB = UnJugador::CrearDireccion(0,"Picas");
    }
    else {
        direccionB = UnJugador::CrearDireccion(ManoBanca[contadorBanca].numero,ManoBanca[contadorBanca].palo);
    }

    QPixmap pix(direccionB);
    int wi = ui -> Carta1J1 -> width();
    int he = ui -> Carta1J1 -> height();
    // Mostramos la carta resultate
    switch (contadorBanca){
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
    contadorBanca++;
}

void UnJugador::mostrarCarta(int jugador,QString direccion){

    int wi = ui -> Carta1J1 -> width();
    int he = ui -> Carta1J1 -> height();
    QPixmap pix(direccion);
    switch (jugador){
        case 0:
            if (flagPedir == false || contadorManosJ[jugador] <= 1){
                switch (contadorManosJ[jugador]){
                                case 0:
                                    ui -> Carta1J1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 1:
                                    ui -> Carta2J1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 2:
                                    ui -> Carta3J1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 3:
                                    ui -> Carta4J1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 4:
                                    ui -> Carta5J1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                }
            }
            else {
                switch (contadorManosD[jugador]){
                                case 1:
                                    ui -> Carta2D1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 2:
                                    ui -> Carta3D1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 3:
                                    ui -> Carta4D1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 4:
                                    ui -> Carta5D1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                }
            }
            break;
        case 1:
            if (flagPedir == false || contadorManosJ[jugador] <= 1){
                switch (contadorManosJ[jugador]){
                                case 0:
                                    ui -> Carta1J2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
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
            }
            else {
                switch (contadorManosD[jugador]){
                                case 1:
                                    ui -> Carta2D2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 2:
                                    ui -> Carta3D2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 3:
                                    ui -> Carta4D2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 4:
                                    ui -> Carta5D2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                }
            }
            break;
        case 2:
            if (flagPedir == false || contadorManosJ[jugador] <= 1){
                switch (contadorManosJ[jugador]){
                                case 0:
                                    ui -> Carta1J3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 1:
                                    ui -> Carta2J3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 2:
                                    ui -> Carta3J3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 3:
                                    ui -> Carta4J3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 4:
                                    ui -> Carta5J3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                }
            }
            else {
                switch (contadorManosD[jugador]){
                                case 1:
                                    ui -> Carta2D3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 2:
                                    ui -> Carta3D3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 3:
                                    ui -> Carta4D3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 4:
                                    ui -> Carta5D3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                }
            }
            break;
        case 3:
            if (flagPedir == false || contadorManosJ[jugador] <= 1){
                switch (contadorManosJ[jugador]){
                                case 0:
                                    ui -> Carta1J4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 1:
                                    ui -> Carta2J4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 2:
                                    ui -> Carta3J4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 3:
                                    ui -> Carta4J4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 4:
                                    ui -> Carta5J4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                }
            }
            else {
                switch (contadorManosD[jugador]){
                                case 1:
                                    ui -> Carta2D4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 2:
                                    ui -> Carta3D4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 3:
                                    ui -> Carta4D4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 4:
                                    ui -> Carta5D4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                }
            }
            break;
    }
}

void UnJugador::resultado(int jugador){
    // Se crea variable para imprimir resultado
    QString cadena;
    // Primero se comprueba si el jugador y ,si ha dividido, la mano dividida tienen BlackJack
    BJJugadores[jugador] = comprobarBlackjack(Jugadores[jugador]);
    if (Dividir[jugador] == true){
        BJDivision[jugador] = comprobarBlackjack(Divisiones[jugador]);
    }

    // Divimos lo resultados en si se ha dividido la mano o no
    if (Dividir[jugador] == false){
        // Si el jugador de pasa de 21 pierde y se termina la ronda
        if (valorJugadores[jugador] > 21){
            cadena = "Has perdido";
        }
        // Si la banca o el jugador tiene BlackJack
        else if (BJB == true || BJJugadores[jugador] == true){
            // Si el jugador y la banca tienen BJ
            if (BJB == true && BJJugadores[jugador] == true){
                cadena = "Te quedas igual";
                saldos[jugador] = saldos[jugador] + apuestas[jugador];
            }
            // Si la banca tiene BJ
            else if (BJB == true){
                cadena = "Has perdido";
            }
            // Si el jugador tiene BJ
            else {
                cadena = "Enhorabuena";
                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 3;
            }
        }
        else {
            // Si la banca tiene mas de 21 o el jugador tiene mayor valor
            if ((valorBanca > 21) || (valorBanca < valorJugadores[jugador])){
                cadena = "Enhorabuena";
                if (isDoblarJ[jugador] == true){
                    saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                }
                else {
                    saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                }
            }
            // Si la banca y el jugador empatan
            else if (valorBanca == valorJugadores[jugador]){
                cadena = "Te quedas igual";
                if (isDoblarJ[jugador] == true){
                    saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                }
                else {
                    saldos[jugador] = saldos[jugador] + apuestas[jugador];
                }
            }
            // Si la banca supera al jugador
            else {
                cadena = "Has perdido";
            }
        }
    }
    // Esta es la parte si la mano se ha dividido
    else {
        // Si el jugador se pasa de 21 con las dos manos
        if (valorJugadores[jugador] > 21 && valorDivisiones[jugador] > 21) {
            cadena = "Has perdido";
        }
        // Si la banca tiene BlackJack
        else if (BJB == true) {
            if (BJJugadores[jugador] == true && BJDivision[jugador] == true){
                cadena = "Te quedas igual";
                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
            }
            else if (BJJugadores[jugador] == true || BJDivision[jugador] == true) {
                cadena = "Has perdido";
                saldos[jugador] = saldos[jugador] + apuestas[jugador];
            }
            else {
                cadena = "Has perdido";
            }
        }
        // Resto de casos que se iran explicando
        else {
            // Si la banca se pasa de 21
            if (valorBanca > 21) {
                // Ademas una de las manos tiene BlackJack
                if (BJJugadores[jugador] == true || BJDivision[jugador] == true) {
                    // Si las dos manos tienen BlackJack
                    if (BJJugadores[jugador] == true && BJDivision[jugador] == true) {
                        cadena = "Enhorabuena";
                        saldos[jugador] = saldos[jugador] + apuestas[jugador] * 6;
                    }
                    // Si al primera mano tiene BlackJack
                    else if (BJJugadores[jugador] == true) {
                        // Si la segunda mano tiene un valor menor de 21
                        if (valorDivisiones[jugador] <= 21){
                            cadena = "Enhorabuena";
                            if (isDoblarD[jugador] == true) {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 7;
                            }
                            else {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 5;
                            }
                        }
                        // Si la segunda mano tiene un valor mayor de 21
                        else {
                            if (isDoblarD[jugador] == true) {
                                cadena = "Te quedas igual";
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 3;
                            }
                            else {
                                cadena = "Enhorabuena";
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 3;
                            }
                        }
                    }
                    // Si la mano dividida tiene BlackJack
                    else {
                        // Si la primera mano tiene un valor menor de 21
                        if (valorJugadores[jugador] <= 21){
                            cadena = "Enhorabuena";
                            if (isDoblarJ[jugador] == true) {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 7;
                            }
                            else {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 5;
                            }
                        }
                        // Si la primera mano tiene un valor mayor de 21
                        else {
                            if (isDoblarJ[jugador] == true) {
                                cadena = "Te quedas igual";
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 3;
                            }
                            else {
                                cadena = "Enhorabuena";
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 3;
                            }
                        }
                    }
                }
                // El resto de posibilidades con la banca mayor de 21
                else {
                    // Cuando las dos manos con menores o iguales a 21
                    if (valorJugadores[jugador] <= 21 && valorDivisiones[jugador] <= 21) {
                        cadena = "Enhorabuena";
                        if (isDoblarJ[jugador] == true && isDoblarD[jugador] == true){
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 8;
                        }
                        else if (isDoblarJ[jugador] == true || isDoblarD[jugador] == true) {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 6;
                        }
                        else {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                        }
                    }
                    // Cuando la primera mano es <= a 21 y la segundo mayor que 21
                    else if (valorJugadores[jugador] <= 21) {
                        if (isDoblarJ[jugador] == true && isDoblarD[jugador] == true){
                            cadena = "Te quedas igual";
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                        }
                        else if (isDoblarJ[jugador] == true) {
                            cadena = "Has perdido";
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                        }
                        else if (isDoblarD[jugador] == true) {
                            cadena = "Enhorabuena";
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                        }
                        else {
                            cadena = "Te quedas igual";
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                        }
                    }
                    // Cuando la segunda mano es <= a 21 y la primera mayor que 21
                    else {
                        if (isDoblarJ[jugador] == true && isDoblarD[jugador] == true){
                            cadena = "Te quedas igual";
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                        }
                        else if (isDoblarJ[jugador] == true) {
                            cadena = "Enhorabuena";
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                        }
                        else if (isDoblarD[jugador] == true) {
                            cadena = "Has perdido";
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                        }
                        else {
                            cadena = "Te quedas igual";
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                        }
                    }
                }
            }
            // Si la banca tiene un valor menor o igual a 21
            else {
                // Si algun jugador tiene BlackJack
                if (BJJugadores[jugador] == true || BJDivision[jugador] == true) {
                    // Si las dos manos tienen BlackJack
                    if (BJJugadores[jugador] == true && BJDivision[jugador] == true) {
                        cadena = "Enhorabuena";
                        saldos[jugador] = saldos[jugador] + apuestas[jugador] * 6;
                    }
                    // Si al primera mano tiene BlackJack
                    else if (BJJugadores[jugador] == true) {
                        // Si la segunda mano tiene un valor mayor que la banca
                        if (valorDivisiones[jugador] > valorBanca){
                            cadena = "Enhorabuena";
                            if (isDoblarD[jugador] == true) {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 7;
                            }
                            else {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 5;
                            }
                        }
                        // Si la segunda mano tiene un valor igual que la banca
                        else if (valorDivisiones[jugador] == valorBanca) {
                            cadena = "Enhorabuena";
                            if (isDoblarD[jugador] == true) {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 5;
                            }
                            else {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                            }
                        }
                        // Si la segunda mano tiene un valor menor que la banca
                        else {
                            if (isDoblarD[jugador] == true) {
                                cadena = "Te quedas igual";
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 3;
                            }
                            else {
                                cadena = "Enhorabuena";
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 3;
                            }
                        }
                    }
                    // Si la mano dividida tiene BlackJack
                    else {
                        // Si la primera mano tiene un valor mayor que la banca
                        if (valorJugadores[jugador] > valorBanca){
                            cadena = "Enhorabuena";
                            if (isDoblarJ[jugador] == true) {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 7;
                            }
                            else {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 5;
                            }
                        }
                        // Si la primera mano tiene un valor igual que la banca
                        else if (valorJugadores[jugador] == valorBanca) {
                            cadena = "Enhorabuena";
                            if (isDoblarJ[jugador] == true) {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 5;
                            }
                            else {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                            }
                        }
                        // Si la primera mano tiene un valor menor que la banca
                        else {
                            if (isDoblarJ[jugador] == true) {
                                cadena = "Te quedas igual";
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 3;
                            }
                            else {
                                cadena = "Enhorabuena";
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 3;
                            }
                        }
                    }
                }
                // Si alguna de las manos tiene un valor mayor de 21
                else if (valorJugadores[jugador] > 21 || valorDivisiones[jugador] > 21) {
                    if (valorJugadores[jugador] > 21) {
                        // Si la segunda mano tiene un valor mayor que la banca
                        if (valorDivisiones[jugador] > valorBanca){
                            if (isDoblarJ[jugador] == true && isDoblarD[jugador] == true) {
                                cadena = "Te quedas igual";
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                            }
                            else if (isDoblarJ[jugador] == true) {
                                cadena = "Has perdido";
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                            }
                            else if (isDoblarD[jugador] == true) {
                                cadena = "Enhorabuena";
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                            }
                            else {
                                cadena = "Te quedas igual";
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                            }
                        }
                        // Si la segunda mano tiene un valor igual que la banca
                        else if (valorDivisiones[jugador] == valorBanca) {
                            cadena = "Has perdido";
                            if (isDoblarJ[jugador] == true && isDoblarD[jugador] == true) {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                            }
                            else if (isDoblarJ[jugador] == true) {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador];
                            }
                            else if (isDoblarD[jugador] == true) {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                            }
                            else {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador];
                            }
                        }
                        // Si la segunda mano tiene un valor menor que la banca
                        else {
                            cadena = "Has perdido";
                        }
                    }
                    else {
                        // Si la primera mano tiene un valor mayor que la banca
                        if (valorJugadores[jugador] > valorBanca){
                            if (isDoblarJ[jugador] == true && isDoblarD[jugador] == true) {
                                cadena = "Te quedas igual";
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                            }
                            else if (isDoblarJ[jugador] == true) {
                                cadena = "Enhorabuena";
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                            }
                            else if (isDoblarD[jugador] == true) {
                                cadena = "Has perdido";
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                            }
                            else {
                                cadena = "Te quedas igual";
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                            }
                        }
                        // Si la primera mano tiene un valor igual que la banca
                        else if (valorJugadores[jugador] == valorBanca) {
                            cadena = "Has perdido" ;
                            if (isDoblarJ[jugador] == true && isDoblarD[jugador] == true) {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                            }
                            else if (isDoblarJ[jugador] == true) {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                            }
                            else if (isDoblarD[jugador] == true) {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador];
                            }
                            else {
                                saldos[jugador] = saldos[jugador] + apuestas[jugador];
                            }
                        }
                        // Si la primera mano tiene un valor menor que la banca
                        else {
                            cadena = "Has perdido";
                        }
                    }
                }
                // Si las dos manos tienen un valor menor o igual a 21
                else {
                    // Si las dos manos ganan a la banca
                    if (valorJugadores[jugador] > valorBanca && valorDivisiones[jugador] > valorBanca) {
                        cadena = "Enhorabuena";
                        if (isDoblarJ[jugador] == true && isDoblarD[jugador] == true) {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 8;
                        }
                        else if (isDoblarJ[jugador] == true) {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 6;
                        }
                        else if (isDoblarD[jugador] == true) {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 6;
                        }
                        else {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                        }
                    }
                    else if (valorJugadores[jugador] > valorBanca && valorDivisiones[jugador] == valorBanca) {
                        cadena = "Enhorabuena";
                        if (isDoblarJ[jugador] == true && isDoblarD[jugador] == true) {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 6;
                        }
                        else if (isDoblarJ[jugador] == true) {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 5;
                        }
                        else if (isDoblarD[jugador] == true) {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                        }
                        else {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 3;
                        }
                    }
                    else if (valorJugadores[jugador] == valorBanca && valorDivisiones[jugador] > valorBanca) {
                        cadena = "Enhorabuena";
                        if (isDoblarJ[jugador] == true && isDoblarD[jugador] == true) {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 6;
                        }
                        else if (isDoblarJ[jugador] == true) {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                        }
                        else if (isDoblarD[jugador] == true) {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 5;
                        }
                        else {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 3;
                        }
                    }
                    else if (valorJugadores[jugador] > valorBanca && valorDivisiones[jugador] < valorBanca) {
                        if (isDoblarJ[jugador] == true && isDoblarD[jugador] == true) {
                            cadena = "Te quedas igual";
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                        }
                        else if (isDoblarJ[jugador] == true) {
                            cadena = "Enhorabuena";
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                        }
                        else if (isDoblarD[jugador] == true) {
                            cadena = "Has perdido";
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                        }
                        else {
                            cadena = "Te quedas igual";
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                        }
                    }
                    else if (valorJugadores[jugador] < valorBanca && valorDivisiones[jugador] > valorBanca) {
                        if (isDoblarJ[jugador] == true && isDoblarD[jugador] == true) {
                            cadena = "Te quedas igual";
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                        }
                        else if (isDoblarJ[jugador] == true) {
                            cadena = "Has perdido";
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                        }
                        else if (isDoblarD[jugador] == true) {
                            cadena = "Enhorabuena";
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                        }
                        else {
                            cadena = "Te quedas igual";
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                        }
                    }
                    else if (valorJugadores[jugador] == valorBanca && valorDivisiones[jugador] == valorBanca) {
                        cadena = "Te quedas igual";
                        if (isDoblarJ[jugador] == true && isDoblarD[jugador] == true) {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 4;
                        }
                        else if (isDoblarJ[jugador] == true) {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 3;
                        }
                        else if (isDoblarD[jugador] == true) {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 3;
                        }
                        else {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                        }
                    }
                    else if (valorJugadores[jugador] < valorBanca && valorDivisiones[jugador] == valorBanca) {
                        cadena = "Has perdido";
                        if (isDoblarJ[jugador] == true && isDoblarD[jugador] == true) {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                        }
                        else if (isDoblarJ[jugador] == true) {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador];
                        }
                        else if (isDoblarD[jugador] == true) {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                        }
                        else {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador];
                        }
                    }
                    else if (valorJugadores[jugador] == valorBanca && valorDivisiones[jugador] < valorBanca) {
                        cadena = "Has perdido";
                        if (isDoblarJ[jugador] == true && isDoblarD[jugador] == true) {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                        }
                        else if (isDoblarJ[jugador] == true) {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador] * 2;
                        }
                        else if (isDoblarD[jugador] == true) {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador];
                        }
                        else {
                            saldos[jugador] = saldos[jugador] + apuestas[jugador];
                        }
                    }
                    else {
                        cadena = "Has perdido";
                    }
                }
            }
        }
    }

    if (jugador == 0){
        ui -> pantalla -> display(saldos[jugador]);
        cadena.prepend("Jugador 1 ");
        ui -> label_retirada ->  setText(cadena);
    }
    else if (jugador == 1) {
        ui -> saldo2 -> display(saldos[jugador]);
        cadena.prepend("Jugador 2 ");
        ui -> label_final2 -> show();
        ui -> label_final2 ->  setText(cadena);
    }
    else if (jugador == 2) {
        ui -> saldo3 -> display(saldos[jugador]);
        cadena.prepend("Jugador 3 ");
        ui -> label_final3 -> show();
        ui -> label_final3 ->  setText(cadena);
    }
    else {
        ui -> saldo4 -> display(saldos[jugador]);
        cadena.prepend("Jugador 4 ");
        ui -> label_final4 -> show();
        ui -> label_final4 ->  setText(cadena);
    }
}


/* BOTONES *********************************************************************************************************************************/


void UnJugador::on_boton_empezar_clicked()
{
    // Hacemos aparecer el resto de saldos de los jugadore
    // No veiamos otra forma de hacerlo
    if (NumeroJ >= 2) {
        ui -> saldo2 -> show();
        ui -> label_6 -> show();
    }
    if (NumeroJ >= 3) {
        ui -> saldo3 -> show();
        ui -> label_7 -> show();
    }
    if (NumeroJ >=4) {
        ui -> saldo4 -> show();
        ui -> label_8 -> show();
    }

    // Una vez presionamos el botón empezar, cogemos la apuesta del usuario
    apuestas[contadorSaldos] = ui -> selector_apuesta -> value();

    // Actualizamos el valor del saldo y lo mostramos
    saldos[contadorSaldos] = saldos[contadorSaldos] - apuestas[contadorSaldos];
    switch (contadorSaldos) {
        case 0:
            ui -> pantalla -> display(saldos[contadorSaldos]);
            ui -> label_retirada -> setText("Turno Jugador 2");
            break;
        case 1:
            ui -> saldo2 -> display(saldos[contadorSaldos]);
            ui -> label_retirada -> setText("Turno Jugador 3");
            break;
        case 2:
            ui -> saldo3 -> display(saldos[contadorSaldos]);
            ui -> label_retirada -> setText("Turno Jugador 4");
            break;
        case 3:
            ui -> saldo4 -> display(saldos[contadorSaldos]);
            break;

    }

    contadorSaldos++;

    if (contadorSaldos == NumeroJ){

        // Y ocultamos y mostramos los botones que necesitamos
        ui -> boton_retirarse -> show();
        ui -> boton_doblar -> show();
        ui -> boton_dividir -> show();
        ui -> boton_pedir_carta -> show();
        ui -> label_3 -> show();
        ui -> label_valor_banca -> show();

        ui -> pantalla_2 -> show();
        ui -> pantalla_2 -> display(valorJugadores[0]);
        ui -> pantalla_3 -> show();

        ui -> boton_empezar -> hide();
        ui -> label_2 -> hide();
        ui -> selector_apuesta -> hide();

        // Llamamos a la función crear carta dos veces, que me creará las manos del jugador y de la banca
        // Lo metemos en una funcón para no repetir código porque en la función pedir carta hay que hacer lo mismo
        for (int i=0;i<NumeroJ;i++){
            UnJugador::crearMano(i);
            UnJugador::crearMano(i);
        }
        UnJugador::crearBanca();
        UnJugador::crearBanca();
        contadorSaldos = 0;
        ui -> label_retirada -> setText("Turno Jugador 1");
        // Para que cuando el primer jugador le toque decidir tenga el valor de su mano
        ui -> pantalla_2 -> display(valorJugadores[0]);
    }
}



void UnJugador::on_boton_pedir_carta_clicked()
{
    UnJugador::crearMano(contadorSaldos);
    // Ya no se podra doblar ni dividir
    ui -> boton_doblar -> hide();
    ui -> boton_dividir -> hide();
}



void UnJugador::on_boton_retirarse_clicked()
{
    // El contador aumenta para que el siguiente jugador tomar sus decisiones
    // Al igual que los punteros que cuentas los As
    contadorSaldos++;
    AsJ++;
    AsD++;
    switch (contadorSaldos) {
        case 1:
            ui -> label_retirada -> setText("Turno Jugador 2");
            break;
        case 2:
            ui -> label_retirada -> setText("Turno Jugador 3");
            break;
        case 3:
            ui -> label_retirada -> setText("Turno Jugador 4");
            break;
    }
    ui -> pantalla_2 -> display(valorJugadores[contadorSaldos]);
    // Volvemos a mortrar los botones por si el anterior jugador los a bloqueado
    ui -> boton_doblar -> show();
    ui -> boton_dividir -> show();
    ui -> boton_pedir_carta -> show();
    ui -> boton_pedir_carta2 -> hide();
    ui -> boton_doblar2 -> hide();
    ui -> label_4 -> hide();
    ui -> pantalla_4-> hide();
    // Cuando el contador llega al numero de jugadores ya entra en la fase de comprobar resultados
    if (contadorSaldos >= NumeroJ) {

        ui -> label_retirada -> show();
        ui -> boton_retirarse -> hide();
        ui -> boton_doblar -> hide();
        ui -> boton_dividir -> hide();
        ui -> boton_pedir_carta -> hide();
        ui -> boton_pedir_carta2 -> hide();
        ui -> boton_doblar2 -> hide();

        int wi = ui -> Carta1B -> width();
        int he = ui -> Carta1B -> height();

        // Comprobamos si la banca o el jugador tiene BlackJack
        BJB = comprobarBlackjack(ManoBanca);
        // Se muestran las cartas de la banca
        // Buscamos y creamos la direccion de la carta
        QString direccionB = UnJugador::CrearDireccion(ManoBanca[1].numero,ManoBanca[1].palo);
        QPixmap pix(direccionB);
        // Mostramos la carta resultate
        ui -> Carta2B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
        while (valorBanca < 17){
            UnJugador::crearBanca();
            valorBanca = ValorCarta(valorBanca,contadorBanca,ManoBanca,punB);
        }
        ui -> pantalla_3 -> display(valorBanca);

        for (int i=0;i<NumeroJ;i++){
            resultado(i);
        }
        ui -> boton_continuar -> show();
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

    for (int i=0;i<NumeroJ;i++){
        saldos[i] = 2000;
    }
}

void UnJugador::on_boton_salir_clicked()
{
    this -> close();
    MainWindow *newmain = new MainWindow();
    newmain -> show();
}

void UnJugador::on_boton_doblar_clicked()
{
    if ((2 * apuestas[contadorSaldos]) > saldos[contadorSaldos]){
        ui -> label_retirada -> show();
        ui -> label_retirada -> setText("No tienes saldo suficiente, subnormal");
        }
    else { // Si se dispone de salgo suficiente se dobla la apuesta y se reparte una carta.

        isDoblarJ[contadorSaldos] = true;

        UnJugador::crearMano(contadorSaldos);

        saldos[contadorSaldos] = saldos[contadorSaldos] - apuestas[contadorSaldos];

        if (contadorSaldos == 0){
            ui -> pantalla -> display(saldos[contadorSaldos]);
        }
        else if (contadorSaldos == 1) {
            ui -> saldo2 -> display(saldos[contadorSaldos]);
        }
        else if (contadorSaldos == 2) {
            ui -> saldo3 -> display(saldos[contadorSaldos]);
        }
        else {
            ui -> saldo4 -> display(saldos[contadorSaldos]);
        }

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
        Dividir[contadorSaldos] = true;
        contadorManosD[contadorSaldos]++;

        // Creamos una nueva mano y le asingnamos la segunda carta y actualizamos el valor
        Divisiones[contadorSaldos][0] = Jugadores[contadorSaldos][1];
        valorJugadores[contadorSaldos] = ValorCarta(0,0,Jugadores[contadorSaldos],AsJ);
        valorDivisiones[contadorSaldos] = ValorCarta(valorDivisiones[contadorSaldos],0,Divisiones[contadorSaldos],AsD);
        // Creamos la direccion de la carta del jugador
        QString direccionJ = UnJugador::CrearDireccion(Divisiones[contadorSaldos][0].numero,Divisiones[contadorSaldos][0].palo);
        // Se muestra la carta del jugador
        int wi = ui -> Carta1J1 -> width();
        int he = ui -> Carta1J1 -> height();
        QPixmap pix(direccionJ);
        switch (contadorSaldos) {
            case 0:
                ui -> Carta1D1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                break;
            case 1:
                ui -> Carta1D2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                break;
            case 2:
                ui -> Carta1D3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                break;
            case 3:
                ui -> Carta1D4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                break;

        }
        // Reducimos el contador de manos porque hemos perdido una carta al traspasarla.
        // Repartimos una carta en la misma posición
        contadorManosJ[contadorSaldos]--;

        saldos[contadorSaldos] = saldos[contadorSaldos] - apuestas[contadorSaldos];
        switch (contadorSaldos) {
            case 0:
                ui -> pantalla -> display(saldos[contadorSaldos]);
                break;
            case 1:
                ui -> saldo2 -> display(saldos[contadorSaldos]);
                break;
            case 2:
                ui -> saldo3 -> display(saldos[contadorSaldos]);
                break;
            case 3:
                ui -> saldo4 -> display(saldos[contadorSaldos]);
                break;

        }

        ui -> label_4 -> show();
        ui -> pantalla_4-> show();
        ui -> boton_pedir_carta2 -> show();
        ui -> boton_doblar2 -> show();
        ui -> boton_dividir -> hide();

        flagPedir = true;
        UnJugador::crearMano(contadorSaldos);
        flagPedir = false;

    } else {
         ui -> label_retirada -> setText("No se puede dividir la mano");
    }
}

void UnJugador::on_boton_pedir_carta2_clicked()
{
    flagPedir = true;
    UnJugador::crearMano(contadorSaldos);
    flagPedir = false;
    // Ya no se podra doblar
    ui -> boton_doblar2 -> hide();
}


void UnJugador::on_boton_doblar2_clicked()
{
    if ((2 * apuestas[contadorSaldos]) > saldos[contadorSaldos]){
        ui -> label_retirada -> show();
        ui -> label_retirada -> setText("No tienes saldo suficiente, subnormal");
        }
    else { // Si se dispone de saldo suficiente se dobla la apuesta y se reparte una carta.

        isDoblarD[contadorSaldos] = true;
        flagPedir = true;
        UnJugador::crearMano(contadorSaldos);
        flagPedir = false;

        saldos[contadorSaldos] = saldos[contadorSaldos] - apuestas[contadorSaldos];

        switch (contadorSaldos) {
            case 0:
                ui -> pantalla -> display(saldos[contadorSaldos]);
                break;
            case 1:
                ui -> saldo2 -> display(saldos[contadorSaldos]);
                break;
            case 2:
                ui -> saldo3 -> display(saldos[contadorSaldos]);
                break;
            case 3:
                ui -> saldo4 -> display(saldos[contadorSaldos]);
                break;

        }

        // Al doblar ya al jugador solo se le permite jugar con esas cartas
        // Y solo se le permirira retirarase
        ui -> boton_doblar2 -> hide();
        ui -> boton_pedir_carta2 -> hide();
    }
}
