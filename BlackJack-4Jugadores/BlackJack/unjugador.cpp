#include "unjugador.h"
#include "ui_unjugador.h"
#include <Windows.h>
#include "mainwindow.h"
#include <vector>
#include <string.h>
#include "funciones.cpp"

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

    // Mostramos quien empieza introduciendo la apuesta
    ui -> label_retirada -> setText("Turno Jugador 1");
    // Iniciamos la semilla de los numeros alatorios de las cartas
    srand(time(NULL));

}

UnJugador::~UnJugador()
{
    delete ui;
}



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


/* BOTONES *********************************************************************************************************************************/


void UnJugador::on_boton_empezar_clicked()
{
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
            // Para que cuando el primer jugador le toque decidir tenga el valor de su mano
            ui -> pantalla_2 -> display(valorJugadores[0]);
        }
        else if (Bancarrota[1] == false){
            contadorSaldos = 1;
            AsJ++;
            AsD++;
            ui -> label_retirada -> setText("Turno Jugador 2");
            // Para que cuando el segundo jugador le toque decidir tenga el valor de su mano
            ui -> pantalla_2 -> display(valorJugadores[1]);
        }
        else if (Bancarrota[2] == false){
            contadorSaldos = 2;
            ui -> label_retirada -> setText("Turno Jugador 3");
            // Para que cuando el tercer jugador le toque decidir tenga el valor de su mano
            ui -> pantalla_2 -> display(valorJugadores[2]);
            AsJ+=2;
            AsD+=2;
        }
        else if (Bancarrota[3] == false){
            contadorSaldos = 3;
            ui -> label_retirada -> setText("Turno Jugador 4");
            // Para que cuando el cuarto jugador le toque decidir tenga el valor de su mano
            ui -> pantalla_2 -> display(valorJugadores[3]);
            AsJ+=3;
            AsD+=3;
        }
    }
}



void UnJugador::on_boton_pedir_carta_clicked()
{
    // Repartimos una carta al jugador
    UnJugador::crearMano(contadorSaldos);
    // Ya no se podra doblar ni dividir
    ui -> boton_doblar -> hide();
    ui -> boton_dividir -> hide();
}



void UnJugador::on_boton_retirarse_clicked()
{
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
            break;
        case 2:
            ui -> label_retirada -> setText("Turno Jugador 3");
            break;
        case 3:
            ui -> label_retirada -> setText("Turno Jugador 4");
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

void UnJugador::on_boton_continuar_clicked()
{
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

void UnJugador::on_boton_reiniciar_clicked()
{
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

void UnJugador::on_boton_salir_clicked()
{
    this -> close();
    MainWindow *newmain = new MainWindow();
    newmain -> show();
}

void UnJugador::on_boton_doblar_clicked()
{
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



void UnJugador::on_boton_dividir_clicked()
{
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

    } else {
        // En este caso el jugador no tiene pareja y no puede dividir
         ui -> label_retirada -> setText("No se puede dividir la mano");
         ui -> boton_dividir -> hide();
    }
}

void UnJugador::on_boton_pedir_carta2_clicked()
{
    // Se activa y desactiva el flag de la segunda mano y se pide carta
    flagPedir = true;
    UnJugador::crearMano(contadorSaldos);
    flagPedir = false;
    // Ya no se podra doblar
    ui -> boton_doblar2 -> hide();
}


void UnJugador::on_boton_doblar2_clicked()
{
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
