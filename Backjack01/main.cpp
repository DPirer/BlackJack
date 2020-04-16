#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

char matriz[4][13]={{'A','2','3','4','5','6','7','8','9','X','J','Q','K'},{'A','2','3','4','5','6','7','8','9','X','J','Q','K'},
                   {'A','2','3','4','5','6','7','8','9','X','J','Q','K'},{'A','2','3','4','5','6','7','8','9','X','J','Q','K'}};
int saldo;

struct Mano{
    char numero;
    int palo;
} manoJugador[10], *pJugador=manoJugador, manoBanca[10], *pBanca=manoBanca;

void repartirCarta(Mano *puntero);
void mostrarMano(Mano *puntero);
int valorMano(Mano *puntero);
int apostar();
int doblar(int apuesta);
void mostrarBanca(Mano* puntero);
void retirar(int apuesta);

int main(){
    int opc, apuesta;

    cout << "\tBienvenido." << endl;
    saldo=2000;
    cout << "Dispone de un saldo inicial de: " << saldo << endl;
    cout << "El juego va a comenzar. " << endl;

    for (int i=0; i<2; i++){
        repartirCarta(pJugador);
        repartirCarta(pBanca);
    }

    do{
        cout << "Que desea hacer: " << endl;
        cout << "1. Consultar mano. " << endl;
        cout << "2. Consultar valor. " << endl;
        cout << "3. Consultar carta Banca. " << endl;
        cout << "4. Apostar" << endl;
        cout << "5. Siguiente ronda. " << endl;
        cout << "Opcion: "; cin >> opc;

        switch (opc) {
        case 1:{
            system("cls");

            mostrarMano(pJugador);
            cout << "\n\n";
            system("pause");
        } break;
        case 2:{
            system("cls");

            cout << "Valor de la mano: " << valorMano(pJugador) <<  endl;
            cout << "\n\n";
            system("pause");
        }break;
        case 3:{
            system("cls");

            mostrarBanca(pBanca);
            cout << "\n\n";
            system("pause");
        }break;
        case 4:{
            system("cls");

            apuesta=apostar();
            cout << "\n\n";
            system("pause");
        }break;
        }
    }while(opc!=5);


    cout << "Siguiente ronda: " << endl;
    do{
        cout << "Que desea hacer: " << endl;
        cout << "1. Consultar mano. " << endl;
        cout << "2. Consultar valor. " << endl;
        cout << "3. Consultar carta Banca. " << endl;
        cout << "4. Doblar. " << endl;
        cout << "5. Pedir Carta. " << endl;
        cout << "6. Retirarse. " << endl;
        cout << "Opcion: "; cin >> opc;

        switch (opc) {
        case 1:{
            system("cls");

            mostrarMano(pJugador);
            cout << "\n\n";
            system("pause");
        } break;
        case 2:{
            system("cls");

            cout << "Valor de la mano: " << valorMano(pJugador) <<  endl;
            cout << "\n\n";
            system("pause");
        }break;
        case 3:{
            system("cls");

            mostrarBanca(pBanca);
            cout << "\n\n";
            system("pause");
        }break;
        case 4:{
            system("cls");

            apuesta=doblar(apuesta);
            cout << "\n\n";
            system("pause");
        }break;
        case 5:{
            system("cls");

            repartirCarta(pJugador);
            cout << "\n\n";
            system("pause");
        }break;
        }
    }while(opc!=6);

    retirar( apuesta);

    return 0;
}

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

void mostrarBanca(Mano* puntero){
    cout << puntero->numero << " de ";
    switch (puntero->palo) {
        case 1: cout << " Trebol." << endl; break;
        case 2: cout << " Corazones." << endl; break;
        case 3: cout << " Picas." << endl; break;
        case 4: cout << " Diamantes." << endl; break;
    }
    return;
}

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
    do{
        cout << "Dinero como apuesta inicial: ";cin >> apuesta;
        if (apuesta > saldo){
            cout << "No dispone de saldo suficiente. " << endl;
        }
    }while (apuesta>saldo);
    saldo=saldo-apuesta;
    return apuesta;
}

int doblar(int apuesta){
    if (2*apuesta>saldo){
        cout << "No dispone de saldo sufiente." << endl;
        return apuesta;
    }
    else{
        return 2*apuesta;
    }
}

void retirar(int apuesta){
    if (valorMano(pJugador)<16){
        cout <<"No se puede retirar" << endl;
    }
    else{
        cout << "Jugador: " << valorMano(pJugador) << endl;
        if (valorMano(pBanca)<18){
            repartirCarta(pBanca);
        }
        cout << "Banca: " << valorMano(pBanca) << endl;

        if (valorMano(pBanca)>21 || (valorMano(pJugador)<=21 && valorMano(pJugador)>valorMano(pBanca))){
            cout << "Enhorabuena, has ganado!! " << endl;
            cout << "Ganacias: " << apuesta*3/2 << endl;
            saldo=saldo+apuesta*3/2;
        }
        else{
            cout << "Has perdido, vuelve a intentarlo " << endl;
        }
    }
    cout << "Saldo actual: " << saldo << endl;
}
