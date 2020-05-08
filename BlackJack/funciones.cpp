#include "unjugador.h"
#include "ui_unjugador.h"
#include <Windows.h>
#include "mainwindow.h"
#include <vector>
#include <string.h>

using namespace std;

/* VARIABLES GLOBALES ***********************************************************************************************************************/

// Barajas que usaremos en el juego, se ponen mas barajas para facilitar el BlackJack o dividir
int matriz[4][13] = {{1,2,3,4,5,6,7,8,9,10,11,12,13}, {1,2,3,4,5,6,7,8,9,10,11,12,13},{1,2,3,4,5,6,7,8,9,10,11,12,13},{1,2,3,4,5,6,7,8,9,10,11,12,13}};
int matriz1[4][13] = {{1,2,3,4,5,6,7,8,9,10,11,12,13}, {1,2,3,4,5,6,7,8,9,10,11,12,13},{1,2,3,4,5,6,7,8,9,10,11,12,13},{1,2,3,4,5,6,7,8,9,10,11,12,13}};
int matriz2[4][13] = {{1,2,3,4,5,6,7,8,9,10,11,12,13}, {1,2,3,4,5,6,7,8,9,10,11,12,13},{1,2,3,4,5,6,7,8,9,10,11,12,13},{1,2,3,4,5,6,7,8,9,10,11,12,13}};

// Dividir se crea debido a que solo se puede usar dividir una vez en cada turno
// Tambien se le añade un flag de pedir a la mano dividida cuando pida carta
vector<bool> BJJugadores(4,false);
vector<bool> BJDivision(4,false);
vector<bool> Dividir(4,false);
bool BJB = false, flagPedir = false;

// isDoblar son variables para saber que el jugador a doblado su mano o sus manos
vector<bool> isDoblarJ(4,false);
vector<bool> isDoblarD(4,false);

// Varaibles para comprobar si estamos en la primera mano y poder ir mostrando las cartas
vector<int> contadorManosJ(4);
vector<int> contadorManosD(4);
// Variable a contar las cartas de la banca
int contadorBanca = 0;
// Para recoger los saldos ademas tambien se recicla para controlar los turnos de los jugadores
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

// Variables para saber si el As a cambiado de valor pasando de valer 11 a 1
int AsB = 0;
int AsJugadores[4] = {0,0,0,0};
int AsDivisiones[4] = {0,0,0,0};
// Y sus punteros
int *punB = &AsB;
int *AsJ = &AsJugadores[0];
int *AsD = &AsDivisiones[0];

// Numero de jugadores recogido en el mainwindow
extern int NumeroJ;

// Vector que nos indicar si el jugador se ha quedado sin saldo
vector<bool> Bancarrota(4,false);


/* FUNCIONES BOTON *******************************************************************************************************************************/

void UnJugador::BotonEmpezar(){
    // Hacemos aparecer el resto de saldos de los jugadores
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
    if ((saldos[contadorSaldos] - apuestas[contadorSaldos]) < 0) {
        // Si la apuesta es mayor que el saldo, se apuesta el saldo total
        apuestas[contadorSaldos] = saldos[contadorSaldos];
        // Y el saldo se queda a cero
        saldos[contadorSaldos] = 0;
    }
    else {
        saldos[contadorSaldos] = saldos[contadorSaldos] - apuestas[contadorSaldos];
    }

    // Imprimimos los saldos de los jugadores segun su turno
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

    // Si alguno de los jugadores esta en bancarrota se le saltara hasta que haya uno que no
    do {
        contadorSaldos++;
    } while ((Bancarrota[contadorSaldos] == true) && (contadorSaldos < NumeroJ));

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

    if (contadorSaldos >= NumeroJ){

        // Y ocultamos y mostramos los botones que necesitamos
        ui -> boton_retirarse -> show();
        ui -> boton_doblar -> show();
        ui -> boton_dividir -> show();
        ui -> boton_pedir_carta -> show();
        ui -> label_3 -> show();
        ui -> label_valor_banca -> show();

        ui -> pantalla_2 -> show();
        ui -> pantalla_3 -> show();

        ui -> boton_empezar -> hide();
        ui -> label_2 -> hide();
        ui -> selector_apuesta -> hide();

        // Llamamos a la función crear carta dos veces, que me creará las manos del jugador y de la banca
        // Lo metemos en una funcón para no repetir código porque en la función pedir carta hay que hacer lo mismo
        for (int i=0;i<NumeroJ;i++){
            // Si el jugador esta en bancarrota se salta
            if (Bancarrota[i] == false) {
                UnJugador::crearMano(i);
                UnJugador::crearMano(i);
            }
        }
        // Se reparte a la banca
        UnJugador::crearBanca();
        UnJugador::crearBanca();
        // Si alguno de los jugadores esta en bancarrota se le saltara hasta que salga uno que no
        if (Bancarrota[0] == false){
            contadorSaldos = 0;
            ui -> label_retirada -> setText("Turno Jugador 1");
            ui -> Flecha1 -> show();
            // Para que cuando el primer jugador le toque decidir tenga el valor de su mano
            ui -> pantalla_2 -> display(valorJugadores[0]);
        }
        else if (Bancarrota[1] == false){
            contadorSaldos = 1;
            AsJ++;
            AsD++;
            ui -> label_retirada -> setText("Turno Jugador 2");
            ui -> Flecha2 -> show();
            // Para que cuando el segundo jugador le toque decidir tenga el valor de su mano
            ui -> pantalla_2 -> display(valorJugadores[1]);
        }
        else if (Bancarrota[2] == false){
            contadorSaldos = 2;
            ui -> label_retirada -> setText("Turno Jugador 3");
            ui -> Flecha3 -> show();
            // Para que cuando el tercer jugador le toque decidir tenga el valor de su mano
            ui -> pantalla_2 -> display(valorJugadores[2]);
            AsJ+=2;
            AsD+=2;
        }
        else if (Bancarrota[3] == false){
            contadorSaldos = 3;
            ui -> label_retirada -> setText("Turno Jugador 4");
            ui -> Flecha4 -> show();
            // Para que cuando el cuarto jugador le toque decidir tenga el valor de su mano
            ui -> pantalla_2 -> display(valorJugadores[3]);
            AsJ+=3;
            AsD+=3;
        }
    }
}

void UnJugador::BotonPedirCarta(){

    // Repartimos una carta al jugador
    UnJugador::crearMano(contadorSaldos);
    // Ya no se podra doblar ni dividir
    ui -> boton_doblar -> hide();
    ui -> boton_dividir -> hide();
}

void UnJugador::BotonRetirarse(){
    // El contador aumenta para que el siguiente jugador pueda tomar sus decisiones
    // Al igual que los punteros que cuentas los As
    // Si algun jugador esta en bancarrota se le saltara
    do {
        contadorSaldos++;
        AsJ++;
        AsD++;
    } while ((Bancarrota[contadorSaldos] == true) && (contadorSaldos < NumeroJ));
    // Se muestra el turno del siguiente jugador
    switch (contadorSaldos) {
        case 1:
            ui -> label_retirada -> setText("Turno Jugador 2");
            ui -> Flecha1 -> hide();
            ui -> Flecha2 -> show();
            break;
        case 2:
            ui -> label_retirada -> setText("Turno Jugador 3");
            ui -> Flecha2 -> hide();
            ui -> Flecha3 -> show();
            break;
        case 3:
            ui -> label_retirada -> setText("Turno Jugador 4");
            ui -> Flecha3 -> hide();
            ui -> Flecha4 -> show();
            break;
    }
    // Se muestra el valor de la mano del siguiente jugador
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
        // Se ocuptan los botones que no se vayan a usar
        ui -> label_retirada -> show();
        ui -> boton_retirarse -> hide();
        ui -> boton_doblar -> hide();
        ui -> boton_dividir -> hide();
        ui -> boton_pedir_carta -> hide();
        ui -> boton_pedir_carta2 -> hide();
        ui -> boton_doblar2 -> hide();
        ui -> Flecha4 -> hide();
        // Se crean las varibales para saber el tamaño de las cartas
        int wi = ui -> Carta1B -> width();
        int he = ui -> Carta1B -> height();

        // Comprobamos si la banca o el jugador tiene BlackJack
        BJB = comprobarBlackjack(ManoBanca);
        // Se muestran la segunda carta de la banca
        // Buscamos y creamos la direccion de la carta
        QString direccionB = UnJugador::CrearDireccion(ManoBanca[1].numero,ManoBanca[1].palo);
        QPixmap pix(direccionB);
        // Mostramos la carta resultate
        ui -> Carta2B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
        // Si la banca tiene un valor de mano menor de 17 pedira carta
        while (valorBanca < 17){
            UnJugador::crearBanca();
        }
        // Se muestra el valor de la mano de la banca
        ui -> pantalla_3 -> display(valorBanca);
        // Se comprueba los resultados de la ronda de cada jugador
        for (int i=0;i<NumeroJ;i++){
            // Comprobamos que el jugador participa en partida
            if (Bancarrota[i] == false) {
                resultado(i);
            }
        }
        // Si el jugador uno no participa se oculta su etiqueta de resultado para que no moleste
        if (Bancarrota[0] == true) {
            ui -> label_retirada -> hide();
        }
        // Y se muestra el boton continuar para pasar a la siguiente ronda
        ui -> boton_continuar -> show();
    }
}

void UnJugador::BotonContinuar(){

    // Comprobamos si los jugadores que ha quedado en bancarrota
    for (int i=0;i<NumeroJ;i++){
        // Y si es asi le impedimos jugar
        if (saldos[i] == 0) {
            Bancarrota[i] = true;
        }
    }
    // Se resetean los valores de la ronda
    UnJugador::resetear();
}

void UnJugador::BotonReiniciar(){

    // Se resetean los valores de la ronda
    UnJugador::resetear();
    // Por si algun jugador esta en bancarrota se pone a cero el contador
    contadorSaldos = 0;
    // Se esconden los botones de jugar
    ui -> boton_retirarse -> hide();
    ui -> boton_doblar -> hide();
    ui -> boton_dividir -> hide();
    ui -> boton_pedir_carta -> hide();
    ui -> boton_continuar -> hide();
    // Y se muestran los de introducir el saldo
    ui -> boton_empezar -> show();
    ui -> label_2 -> show();
    ui -> selector_apuesta -> show();

    // Se reinician los saldos y la bancarrota
    for (int i=0;i<NumeroJ;i++){
        saldos[i] = 2000;
        Bancarrota[i] = false;
    }
    // Y se muestras los nuevos saldos de los jugadores
    ui -> pantalla -> display(saldos[0]);
    if (NumeroJ >= 2) {
        ui -> saldo2 -> display(saldos[1]);
    }
    if (NumeroJ >= 3) {
        ui -> saldo3 -> display(saldos[2]);
    }
    if (NumeroJ >=4) {
        ui -> saldo4 -> display(saldos[3]);
    }
    // Y se muestra del turno del jugador
    ui -> label_retirada -> setText("Turno Jugador 1");
}

void UnJugador::BotonSalir(){
    this -> close();
    MainWindow *newmain = new MainWindow();
    newmain -> show();
}

void UnJugador::BotonDoblar(){

    // Si al doblar el jugador tiene un saldo negativo no se le permite
    if ((saldos[contadorSaldos] - apuestas[contadorSaldos]) < 0){
        ui -> label_retirada -> show();
        ui -> label_retirada -> setText("No tienes saldo suficiente");
        }
    else { // Si se dispone de salgo suficiente se dobla la apuesta y se reparte una carta.
        // Se pone el flag de doblar a verdadero
        isDoblarJ[contadorSaldos] = true;
        // Se le reparte una unica carta al jugador
        UnJugador::crearMano(contadorSaldos);
        // Se le resta la apuesta nueva
        saldos[contadorSaldos] = saldos[contadorSaldos] - apuestas[contadorSaldos];
        // Se muestra el nuevo saldo del jugador
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

void UnJugador::BotonDividir(){

    //  Se comprueba que puede pagar la nueva apuesta
    if ((saldos[contadorSaldos] - apuestas[contadorSaldos]) < 0) {
        ui -> label_retirada -> setText("No se puede dividir la mano por su saldo");
        ui -> boton_dividir -> hide();
    }
    // Comprobamos si las primeras dos cartas son iguales
    else if (Jugadores[contadorSaldos][0].numero == Jugadores[contadorSaldos][1].numero){

        // Activamos el flag
        Dividir[contadorSaldos] = true;
        // Y se aumenta el contador de cartas de la mano dividida
        contadorManosD[contadorSaldos]++;

        // Creamos una nueva mano y le asingnamos la segunda carta de la primera mano y actualizamos los valores
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
        // Se le resta la nueva apuesta al saldo del jugador
        saldos[contadorSaldos] = saldos[contadorSaldos] - apuestas[contadorSaldos];
        // Se muestra el nuevo valor del saldo
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
        // Se muestran los motones para controlar la segunda manos y se impide volver a dividir
        ui -> label_4 -> show();
        ui -> pantalla_4-> show();
        ui -> boton_pedir_carta2 -> show();
        ui -> boton_doblar2 -> show();
        ui -> boton_dividir -> hide();
        // Se le reparte una nueva carta a la segunda mano
        flagPedir = true;
        UnJugador::crearMano(contadorSaldos);
        flagPedir = false;

    }
    else {
        // En este caso el jugador no tiene pareja y no puede dividir
         ui -> label_retirada -> setText("No se puede dividir la mano");
         ui -> boton_dividir -> hide();
    }
}

void UnJugador::BotonPedirCarta2(){

    // Se activa y desactiva el flag de la segunda mano y se pide carta
    flagPedir = true;
    UnJugador::crearMano(contadorSaldos);
    flagPedir = false;
    // Ya no se podra doblar
    ui -> boton_doblar2 -> hide();
}

void UnJugador::BotonDoblar2(){

    // Si al doblar el jugador tiene un saldo negativo no se le permite
    if ((saldos[contadorSaldos] - apuestas[contadorSaldos]) < 0){
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


/* FUNCIONES *******************************************************************************************************************************/

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
    ui -> Carta6J1 -> clear();
    ui -> Carta7J1 -> clear();
    ui -> Carta8J1 -> clear();
    ui -> Carta9J1 -> clear();
    ui -> Carta10J1 -> clear();
    ui -> Carta11J1 -> clear();

    ui -> Carta1D1 -> clear();
    ui -> Carta2D1 -> clear();
    ui -> Carta3D1 -> clear();
    ui -> Carta4D1 -> clear();
    ui -> Carta5D1 -> clear();
    ui -> Carta6D1 -> clear();
    ui -> Carta7D1 -> clear();
    ui -> Carta8D1 -> clear();
    ui -> Carta9D1 -> clear();
    ui -> Carta10D1 -> clear();
    ui -> Carta11D1 -> clear();

    ui -> Carta1J2 -> clear();
    ui -> Carta2J2 -> clear();
    ui -> Carta3J2 -> clear();
    ui -> Carta4J2 -> clear();
    ui -> Carta5J2 -> clear();
    ui -> Carta6J2 -> clear();
    ui -> Carta7J2 -> clear();
    ui -> Carta8J2 -> clear();
    ui -> Carta9J2 -> clear();
    ui -> Carta10J2 -> clear();
    ui -> Carta11J2 -> clear();

    ui -> Carta1D2 -> clear();
    ui -> Carta2D2 -> clear();
    ui -> Carta3D2 -> clear();
    ui -> Carta4D2 -> clear();
    ui -> Carta5D2 -> clear();
    ui -> Carta6D2 -> clear();
    ui -> Carta7D2 -> clear();
    ui -> Carta8D2 -> clear();
    ui -> Carta9D2 -> clear();
    ui -> Carta10D2 -> clear();
    ui -> Carta11D2 -> clear();

    ui -> Carta1J3 -> clear();
    ui -> Carta2J3 -> clear();
    ui -> Carta3J3 -> clear();
    ui -> Carta4J3 -> clear();
    ui -> Carta5J3 -> clear();
    ui -> Carta6J3 -> clear();
    ui -> Carta7J3 -> clear();
    ui -> Carta8J3 -> clear();
    ui -> Carta9J3 -> clear();
    ui -> Carta10J3 -> clear();
    ui -> Carta11J3 -> clear();

    ui -> Carta1D3 -> clear();
    ui -> Carta2D3 -> clear();
    ui -> Carta3D3 -> clear();
    ui -> Carta4D3 -> clear();
    ui -> Carta5D3 -> clear();
    ui -> Carta6D3 -> clear();
    ui -> Carta7D3 -> clear();
    ui -> Carta8D3 -> clear();
    ui -> Carta9D3 -> clear();
    ui -> Carta10D3 -> clear();
    ui -> Carta11D3 -> clear();

    ui -> Carta1J4 -> clear();
    ui -> Carta2J4 -> clear();
    ui -> Carta3J4 -> clear();
    ui -> Carta4J4 -> clear();
    ui -> Carta5J4 -> clear();
    ui -> Carta6J4 -> clear();
    ui -> Carta7J4 -> clear();
    ui -> Carta8J4 -> clear();
    ui -> Carta9J4 -> clear();
    ui -> Carta10J4 -> clear();
    ui -> Carta11J4 -> clear();

    ui -> Carta1D4 -> clear();
    ui -> Carta2D4 -> clear();
    ui -> Carta3D4 -> clear();
    ui -> Carta4D4 -> clear();
    ui -> Carta5D4 -> clear();
    ui -> Carta6D4 -> clear();
    ui -> Carta7D4 -> clear();
    ui -> Carta8D4 -> clear();
    ui -> Carta9D4 -> clear();
    ui -> Carta10D4 -> clear();
    ui -> Carta11D4 -> clear();

    ui -> Carta1B -> clear();
    ui -> Carta2B -> clear();
    ui -> Carta3B -> clear();
    ui -> Carta4B -> clear();
    ui -> Carta5B -> clear();
    ui -> Carta6B -> clear();
    ui -> Carta7B -> clear();
    ui -> Carta8B -> clear();
    ui -> Carta9B -> clear();
    ui -> Carta10B -> clear();
    ui -> Carta11B -> clear();
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
                    case 5:
                        ui -> Carta6B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                        break;
                    case 6:
                        ui -> Carta7B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                        break;
                    case 7:
                        ui -> Carta8B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                        break;
                    case 8:
                        ui -> Carta9B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                        break;
                    case 9:
                        ui -> Carta10B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                        break;
                    case 10:
                        ui -> Carta11B -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
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
                                case 5:
                                    ui -> Carta6J1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 6:
                                    ui -> Carta7J1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 7:
                                    ui -> Carta8J1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 8:
                                    ui -> Carta9J1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 9:
                                    ui -> Carta10J1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 10:
                                    ui -> Carta11J1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
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
                                case 5:
                                    ui -> Carta6D1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 6:
                                    ui -> Carta7D1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 7:
                                    ui -> Carta8D1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 8:
                                    ui -> Carta9D1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 9:
                                    ui -> Carta10D1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 10:
                                    ui -> Carta11D1 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
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
                                case 5:
                                    ui -> Carta6J2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 6:
                                    ui -> Carta7J2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 7:
                                    ui -> Carta8J2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 8:
                                    ui -> Carta9J2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 9:
                                    ui -> Carta10J2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 10:
                                    ui -> Carta11J2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
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
                                case 5:
                                    ui -> Carta6D2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 6:
                                    ui -> Carta7D2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 7:
                                    ui -> Carta8D2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 8:
                                    ui -> Carta9D2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 9:
                                    ui -> Carta10D2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 10:
                                    ui -> Carta11D2 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
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
                                case 5:
                                    ui -> Carta6J3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 6:
                                    ui -> Carta7J3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 7:
                                    ui -> Carta8J3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 8:
                                    ui -> Carta9J3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 9:
                                    ui -> Carta10J3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 10:
                                    ui -> Carta11J3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
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
                                case 5:
                                    ui -> Carta6D3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 6:
                                    ui -> Carta7D3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 7:
                                    ui -> Carta8D3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 8:
                                    ui -> Carta9D3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 9:
                                    ui -> Carta10D3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 10:
                                    ui -> Carta11D3 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
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
                                case 5:
                                    ui -> Carta6J4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 6:
                                    ui -> Carta7J4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 7:
                                    ui -> Carta8J4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 8:
                                    ui -> Carta9J4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 9:
                                    ui -> Carta10J4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 10:
                                    ui -> Carta11J4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
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
                                case 5:
                                    ui -> Carta6D4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 6:
                                    ui -> Carta7D4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 7:
                                    ui -> Carta8D4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 8:
                                    ui -> Carta9D4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 9:
                                    ui -> Carta10D4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
                                    break;
                                case 10:
                                    ui -> Carta11D4 -> setPixmap(pix.scaled(wi,he,Qt::KeepAspectRatio));
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
