#include "unjugador.h"
#include "ui_unjugador.h"
#include <Windows.h>
#include "mainwindow.h"
#include <vector>
#include <string.h>
#include "unjugador.cpp"

Mano UnJugador::repartirCarta(){

    // Se crea los numeros aleatorios que corresponden al palo y numero y se comprueban si existen
    int num, palo;
    bool baraja = false, baraja1 = false;
    do{

       num = 1 + rand() % 12;
       palo = 1 + rand() % 3;
       // Comprobamos en que baraja no esta la carta
       if (matriz[palo][num] != 0) {
           baraja = true;
       }
       else if (matriz1[palo][num] != 0) {
           baraja1 = true;
       }
    } while((matriz[palo][num] == 0) && (matriz1[palo][num] == 0) && (matriz2[palo][num] == 0));

    // Ponemos esa posición a 0 para marcarla
    if (baraja == true) {
        matriz[palo][num] = 0;
    }
    else if (baraja1 == true) {
        matriz1[palo][num] = 0;
    }
    else {
        matriz2[palo][num] = 0;
    }

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
        // Y se muestra dicho valor
        ui -> pantalla_2 -> display(valorJugadores[jugador]);

        // Creamos la direccion de la imagen de la carta del jugador
        QString direccionJ = UnJugador::CrearDireccion(Jugadores[jugador][contadorManosJ[jugador]].numero,Jugadores[jugador][contadorManosJ[jugador]].palo);
        // Mostramos las cartas en la etiquetas
        mostrarCarta(jugador,direccionJ);
        // Si la primera mano el jugador se pasara de 21 se le impide coger mas cartas
        if (valorJugadores[jugador] > 21) {
            ui -> boton_pedir_carta -> hide();
        }
        // Y aumentamos el numero de cartas del jugador
        contadorManosJ[jugador]++;
    }

    // Creamos una segunda mano para el caso en el que hayamos dividido cartas, el contenido es igual que la anterior
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
            matriz1[i][j] = j;
            matriz2[i][j] = j;
        }
    }

    // Reseteamos las variables
    // Primera los de la banca
    valorBanca = 0;
    contadorBanca = 0;
    BJB = false;;
    AsB = 0;
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
    }
    AsJ = &AsJugadores[0];
    AsD = &AsDivisiones[0];

    ui -> boton_empezar -> show();
    ui -> label_2 -> show();
    ui -> selector_apuesta -> show();
    ui -> label_retirada -> show();

    // Al cambiar de turno vemos que jugar empieza si alguno se ha agotado su saldo
    if (Bancarrota[0] == false){
        ui -> label_retirada -> setText("Turno Jugador 1");
        contadorSaldos = 0;
    }
    else if (Bancarrota[1] == false) {
        ui -> label_retirada -> setText("Turno Jugador 2");
        contadorSaldos = 1;
        AsJ++;
        AsD++;
    }
    else if (Bancarrota[2] == false) {
        ui -> label_retirada -> setText("Turno Jugador 3");
        contadorSaldos = 2;
        AsJ+=2;
        AsD+=2;
    }
    else if (Bancarrota[3] == false) {
        ui -> label_retirada -> setText("Turno Jugador 4");
        contadorSaldos = 3;
        AsJ+=3;
        AsD+=3;
    }
    else {
        ui -> label_retirada -> setText("END GAME");
        ui -> boton_empezar -> hide();
        ui -> label_2 -> hide();
        ui -> selector_apuesta -> hide();

    }
    // Ahora ocultamos y mostramos los elementos necesarios

    ui -> label_3 -> hide();
    ui -> label_4 -> hide();
    ui -> label_valor_banca -> hide();
    ui -> pantalla_2 -> hide();
    ui -> pantalla_3 -> hide();
    ui -> pantalla_4 -> hide();
    ui -> label_final2 -> hide();
    ui -> label_final3 -> hide();
    ui -> label_final4 -> hide();


    // Limpiamos la imagenes de las cartas creadas
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
    // Se inicializa la variable que nos indique si tiene o no BlackJack
    bool flag = false;
    // Introducimos el condicional del Blackjack
    if (((JugadorX[0].numero == 1) && (JugadorX[1].numero >= 10))||((JugadorX[1].numero == 1) && (JugadorX[0].numero >= 10))){
        // Si cumple activamos el flag
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
            // Primero se cuentan el numero de Ases
            if (JugadorX[i].numero == 1){
                contadorAs++;
            }
        }
        // Si el numero de Ases es menor que las veces que se ha reducido el valor se le resta 10 a valor de la mano
        if ( contadorAs > *puntero){
            ValorMano = ValorMano - 10;
            // Y se aumenta las veces reducida la mano
            *puntero = *puntero + 1;
        }
    }
    return ValorMano;
}

void UnJugador::crearBanca(){
    // Creamos la carta para la banca y calculamos el valor de la mano
    ManoBanca[contadorBanca] = UnJugador::repartirCarta();
    valorBanca = ValorCarta(valorBanca,contadorBanca,ManoBanca,punB);

    // La banca unicamente enseñara la primera carta y el resto estaran boca abajo
    // Por lo que tendremos que distinguir entre la primera ronda y el resto
    // Usamos la funcion que nos indique donde se encuentra la carta a mostrar
    QString direccionB;

    if (contadorBanca == 0){
        direccionB = UnJugador::CrearDireccion(ManoBanca[contadorBanca].numero,ManoBanca[contadorBanca].palo);
        // Se muestra el valor de la primera carta de la banca
        ui -> pantalla_3 -> display(valorBanca);
    }
    else if (contadorBanca == 1) {
        // Se muestra una carta boca abajo
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
    // Aumentamos el numero de cartas de la banca
    contadorBanca++;
}

void UnJugador::mostrarCarta(int jugador,QString direccion){
    // Calculamos el tamalo de las etiquetas
    int wi = ui -> Carta1J1 -> width();
    int he = ui -> Carta1J1 -> height();
    // Se guarda la imagen de la direccion en la variable pix
    QPixmap pix(direccion);
    // Se muestra la carta  dependiedo del jugador y si es la mano original o dividida
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
    // Primero se comprueba si la banca, el jugador y ,si ha dividido, la mano dividida tienen BlackJack
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
    // Dependiendo del jugador se le muestra en una etiqueta con su numero de jugador
    if (jugador == 0){
        ui -> pantalla -> display(saldos[jugador]);
        cadena.prepend("Jugador 1 ");
        ui -> label_retirada -> show();
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
