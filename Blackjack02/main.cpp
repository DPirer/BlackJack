/* Funcion donde la interaccion es más fluida y con las explicaciones de que ocurre en cada momento y que botones se
  habilitan en cada momento. */

//Librerias
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

//Variables globales.
char matriz[4][13]={{'A','2','3','4','5','6','7','8','9','X','J','Q','K'},{'A','2','3','4','5','6','7','8','9','X','J','Q','K'},
                   {'A','2','3','4','5','6','7','8','9','X','J','Q','K'},{'A','2','3','4','5','6','7','8','9','X','J','Q','K'}};
int saldo;
/* isDoblar y isDividir se crean debido a que solo se puede usar doblar y dividir una vez.*/
bool isDoblar, isDividir;

/*Estructura mano, donde se agrupan las cartas tanto para el jugador como para la banca, debido a que solo se compite con la banca
no se crea más jugadores.     **SUSTITUIR POR CLASES**   */
struct Mano{
    char numero;
    int palo;
} manoJugador[10], *pJugador=manoJugador, manoBanca[10], *pBanca=manoBanca;

//Cabeceras de las funciones.
void Menu(Mano *puntero, int apuesta);
void visualizarDatos(Mano *puntero);
void repartirCarta(Mano *puntero);
void mostrarMano(Mano *puntero);
void mostrarBanca(Mano* puntero);
int valorMano(Mano *puntero);
int apostar();
int doblar(Mano *puntero, int apuesta);
void retirar(int apuesta, Mano *puntero);
bool blackjack(Mano *puntero);
void dividir(Mano *puntero, int apuesta);
void resetMano(Mano *puntero);
void resetBaraja();

int main(){
    int apuesta;
    //Se inicia el juego y se asigna un saldo de 2000.
    cout << "\tBienvenido." << endl;
    saldo=2000;
    cout << "Dispone de un saldo inicial de: " << saldo << endl;
    cout << "El juego va a comenzar. " << endl;

    //Se crea un do.while() que se ejecuta siempre que el saldo sea mayor que cero.
    do{
        /*Se resetean las variables, ya que ha empezado una nueva partida, por lo que la baraja esta completa y las manos
        se vacian y no se llamado a la funcion doblar ni dividir. */
        isDoblar=false;
        isDividir=false;
        resetMano(pJugador);
        resetMano(pBanca);
        resetBaraja();

        //Antes de repartir se debe pedir al usuario que realize la apuesta.
        apuesta=apostar();

        //Un vez realiza la apuesta se reparte 2 cartas al jugador y a la banca.
        cout << "Repartiendo cartas ... ." << endl;
        for (int i=0; i<2; i++){
            repartirCarta(pJugador);
            repartirCarta(pBanca);
        }

        /*Se muestra por pantalla las dos cartas del jugador y una carta de la banca y el
          valor de la mano del jugador. */
        visualizarDatos(pJugador);

        /*A partir de aquí hay 4 opciones: retirarse, pedir carta, doblar y dividir, cada una
        sujeta a sus condiciones y se despliega en un menu. */
        Menu(pJugador, apuesta);
    }while (saldo>0);

    return 0;
}

/* La funcion repartirCarta, selecciona una carta de forma aleatoria siempre que su valor no sea 0, y dicha carta se añade
a la posicion de la mano, que se controla mediante el valor del palo (0 para detectar los huecos libres), y en el hueco de
a carta se define como 0, es decir que la carta ya se ha cogido. */
void repartirCarta(Mano *puntero){
    int num, palo, pos=0;
    do{
       srand(time(0));
       num=1+rand()%12;
       palo=1+rand()%3;
    }while(matriz[palo][num]=='0');

    while((puntero+pos)->palo!=0){
        pos++;
    }
    (puntero+pos)->numero=matriz[palo][num];
    (puntero+pos)->palo=palo+1;
    matriz[palo][num]='0';
}

/* La funcion mostrarMano imprime por pantalla el numero y el palo de todas las cartas de la mano, se controla
 gracias al palo se muestra hasta que detecta que palo es 0 (vacio). */
void mostrarMano(Mano *puntero){
    int i=0;

    do{
        cout << (puntero+i)->numero << " de ";
        switch ((puntero+i)->palo) {
            case 1: cout << " Trebol." << endl; break;
            case 2: cout << " Corazones." << endl; break;
            case 3: cout << " Picas." << endl; break;
            case 4: cout << " Diamantes." << endl; break;
        }
        i++;
    }while ((puntero+i)->palo!=0);
}

/* La funcion mostrarBanca es parecida a la anterior, salvo que solo muestra la primera carta y la siguiente esta
oculta. */
void mostrarBanca(Mano* puntero){
    cout << puntero->numero << " de ";
    switch (puntero->palo) {
        case 1: cout << " Trebol." << endl; break;
        case 2: cout << " Corazones." << endl; break;
        case 3: cout << " Picas." << endl; break;
        case 4: cout << " Diamantes." << endl; break;
    }
    cout << "?" << " de " << "?" << endl;
    return;
}

/* La funcion valorMano recorre toda la mano y devuelve el valor de la mano, al igual que las anteriores se controla
hasta que el palo es 0.*/
int valorMano(Mano *puntero){
    int num, valor, pos=0;
    valor=0;
    do{
        switch ((puntero+pos)->numero) {
        case 'A':{
            if(valor > 10){
                valor=valor+1;
            }
            else{
                cout << "Que valor desea considerar para el A: 1 o 11. "; cin >> num;
                valor=valor+num;
            }
            break;
        };
        case '2':valor=valor+2; break;
        case '3':valor=valor+3; break;
        case '4':valor=valor+4; break;
        case '5':valor=valor+5; break;
        case '6':valor=valor+6; break;
        case '7':valor=valor+7; break;
        case '8':valor=valor+8; break;
        case '9':valor=valor+9; break;
        case 'X':
        case 'J':
        case 'Q':
        case 'K':valor=valor+10; break;
        }
        pos++;
    }while((puntero+pos)->palo!=0);
    return valor;
}

int apostar(){
    int apuesta;
    //Se introduce la apuesta siempre que sea menor al saldo.
    do{
        cout << "Introduce la apuesta inicial: ";cin >> apuesta;
        if (apuesta > saldo){
            cout << "No dispone de saldo suficiente. " << endl;
        }

    }while (apuesta>saldo && apuesta<=0);
    //Se resta la apuesta del saldo.
    saldo=saldo-apuesta;
    return apuesta;
}

int doblar(Mano *puntero, int apuesta){
    //Si comprueba que hay saldo suficiente si nos, no se puede doblar.
    if (2*apuesta>saldo){
        cout << "No dispone de saldo sufiente." << endl;
        isDoblar=true;
        return apuesta;
    }
    else{
        //Si se dispone de salgo suficiente se doble y se reparte una carta.
        repartirCarta(puntero);
        isDoblar=true;
        return 2*apuesta;
    }
}

void retirar(int apuesta, Mano *puntero){
    //Si el jugador no llega a 16 no se puede retirar.
    if (valorMano(puntero)<16){
        cout <<"No se puede retirar" << endl;
    }
    else{
        //Si se ha pasado de 21, ha perdido automaticamente y no se le devuelve la apuesta.
        if (valorMano(puntero)>21){
        cout << "Has perdido, vuelve a intentarlo " << endl;
       }
        else{
            /*Si lo cumple se muestra el valor del jugador, la banca cogera cartas para superar al jugador, y se muestra
            por pantalla el contenido de las manos. */
            cout << "Jugador: " << endl;
            mostrarMano(pJugador);
            cout << "Valor de la mano: " << valorMano(puntero) << endl;
            if (valorMano(pBanca)<valorMano(puntero)){
                repartirCarta(pBanca);
            }
            cout << "\n\n";
            cout << "Banca: " << endl;
            mostrarMano(pBanca);
            cout << "Valor de la mano: " << valorMano(pBanca) << endl;

            //Se incluyen todas las posibilidades para ganar.
            if (valorMano(pBanca)>21 || (valorMano(puntero)>valorMano(pBanca)) || (blackjack(puntero)==true && blackjack(pBanca)==false)){
                cout << "Enhorabuena, has ganado!! " << endl;
                cout << "Ganacias: " << apuesta*2 << endl;
                saldo=saldo+apuesta*2;
            }
            else if ((blackjack(puntero)==true && blackjack(pBanca)==true) || valorMano(puntero)==valorMano(pBanca)){
                cout << "Empate, se devuelve la apuesta. " << endl;
                saldo=saldo+apuesta;
            }
            else{
                cout << "Has perdido, vuelve a intentarlo " << endl;
            }
        }
        cout << "\n\n";
        system("pause");
        system("cls");
        cout << "Saldo actual: " << saldo << endl;
    }

}

/* La funcion blackjack se comprueba si se cumple blackjack con sus condiciones. */
bool blackjack(Mano *puntero){
    if ((puntero->numero=='A' || (puntero+1)->numero=='A') && ((puntero->numero=='J' || (puntero+1)->numero=='J') ||(puntero->numero=='Q' || (puntero+1)->numero=='Q')||(puntero->numero=='K' || (puntero+1)->numero=='K'))){
        return true;
    }
    else
        return false;
}


void dividir(Mano *puntero, int apuesta){
    //Se comprueba si las dos primeras cartas son iguales.
    if (puntero->numero == (puntero+1)->numero){
        // se crea una nueva mano y se asigna la segunda carta de la mano 1.
        Mano manoJugador2[10], *pJugador2=manoJugador2;
        pJugador2->numero=(puntero+1)->numero;
        pJugador2->palo=(puntero+1)->palo;
        //El palo de la segunda carta de la mano 1 se pone a 0 para que considere que esta vacio.
        (puntero+1)->palo=0;

        //Se reparte una carta para cada mano.
        repartirCarta(puntero);
        repartirCarta(pJugador2);

        isDividir=true;

        //Se depliega el menu:
        //Primero se juego para la primera mano.
        cout << "Jugando con la mano 1: " << endl;
        visualizarDatos(puntero);
        Menu(puntero, apuesta);

        cout << "Jugando con la mano 2: " << endl;
        /*Posteriormente se juega para la segunda mano, al finalizar se resetea la mano por si posteriormente
        vuelve a salir dobles, y se fuerza para salir.*/
        visualizarDatos(pJugador2);
        Menu(pJugador2, apuesta);
        resetMano(pJugador2);
        exit(0);
    }
    else{
        cout << "No se puede dividir la mano." << endl;
    }
}

/* Esto es una mierda, mejorar por todos lados. */
void Menu(Mano *puntero, int apuesta){
    int opc;
    do{
        //Se despliega una menu con todas las opciones.
        cout << "Que desea hacer: " << endl;
        cout << "1. Retirarse " << endl;
        cout << "2. Pedir carta. " << endl;
        cout << "3. Doblar . " << endl;
        cout << "4. Dividir. " << endl;
        cout << "Opcion: "; cin >> opc;

        switch (opc) {
        case 1:{
            //retirar la apuesta.
            system("cls");
            retirar(apuesta, puntero);
        } break;
        case 2:{
            system("cls");
            //Repartir carta.
            repartirCarta(puntero);
            //Se visualiza las manos y si el jugador se pasa de 21 se fuerza a retirarse.
            visualizarDatos(puntero);
            if (valorMano(puntero)>21){
                retirar(apuesta, puntero);
            }

            cout << "\n\n";
            system("pause");
        }break;
        case 3:{
            system("cls");
            //Solo entra si no se ha entrado antes.
            //HABILITAR BOTON DOBLAR.
            if (isDoblar==false){
                doblar(puntero, apuesta);
                /*Se dobla la apuesta y se reparte una carta, se visualiza las manos y si el
                jugador se ha pasado de 21 pues se fuerza a retirarse. */
                visualizarDatos(puntero);
                if (valorMano(puntero)>21){
                    retirar(apuesta, puntero);
                }
            }
            else{
                cout << "Ya se ha doblado. " << endl;
            }

            cout << "\n\n";
            system("pause");
        }break;
        case 4:{
            system("cls");
            /* Se divide si no se ha dividido antes y se ejecuta la funcion que a saber si funciona
             bien. */
            //HABILITAR BOTON DIVIDIR.
            if (isDividir==false){
                dividir(puntero, apuesta);
            }
            else{
                cout << "Ya se ha dividido. " << endl;
            }

            cout << "\n\n";
            system("pause");
        }break;
        }
        //Se ejecuta siempre que no se retire de forma voluntaria y su mano sea menor que 21.
    }while(opc != 1 && valorMano(pJugador)<=21);
    return;
}

/* Pues eso que visualiza los datos de las manos y ademas te indica si tienes un blackjack, tanto
para el jugador como la banca. */
void visualizarDatos(Mano *puntero){
    cout << "Jugador: " << endl;
    mostrarMano(puntero);
    cout << "Valor de la mano: " << valorMano(puntero) <<  endl;
    if (blackjack(pJugador)==true){
        cout  << "ENHORABUENA TIENES BLACKJACK" << endl;
    }
    cout << "\n\n";

    cout << "Banca: " << endl;
    mostrarBanca(pBanca);
    cout << "\n\n";
    return;
}

//Se resetea la mano, es decir se señalo el palo a 0.
void resetMano(Mano *puntero){
    int pos=0;
    while((puntero+pos)->palo!=0){
        (puntero+pos)->palo=0;
        pos++;
    }
    return;
}

//Se resetea la baraja, pero esto es una puta chapuza, tiene que haber algo mejor, no me jodas.
void resetBaraja(){
    for (int i=0; i<4; i++){
        for (int j=0; j<13; j++){
            if (j==0){
                matriz[i][j]='A';
            }
            //SI ES QUE ESTO ES UNA MIEEERDA
            else if (j==1){
                matriz[i][j]='2';
            }
            else if (j==2){
                matriz[i][j]='3';
            }
            else if (j==3){
                matriz[i][j]='4';
            }
            else if (j==4){
                matriz[i][j]='5';
            }
            else if (j==5){
                matriz[i][j]='6';
            }
            else if (j==6){
                matriz[i][j]='7';
            }
            else if (j==7){
                matriz[i][j]='8';
            }
            else if (j==8){
                matriz[i][j]='9';
            }
            else if (j==9){
                matriz[i][j]='X';
            }
            else if (j==10){
                matriz[i][j]='J';
            }
            else if (j==11){
                matriz[i][j]='Q';
            }
            else if (j==12){
                matriz[i][j]='K';
            }
        }
    }
    return;
}
