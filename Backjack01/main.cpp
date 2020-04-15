#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct Mano{
    char numero;
    int palo;
} manoJugador[10], *pJugador=manoJugador, manoBanca[10], *pBanca=manoBanca;

void repartirCarta(Mano *puntero);
void mostrarMano(Mano *puntero);
int valorMano(Mano *puntero);

int main(){
    repartirCarta(pJugador);
    repartirCarta(pJugador);
    mostrarMano(pJugador);
    cout << "Valor de la mano: " << valorMano(pJugador) <<  endl;
    return 0;
}

void repartirCarta(Mano *puntero){
    int num, palo, pos=0;
    char matriz[4][13]={{'A','2','3','4','5','6','7','8','9','X','J','Q','K'},{'A','2','3','4','5','6','7','8','9','X','J','Q','K'},
                       {'A','2','3','4','5','6','7','8','9','X','J','Q','K'},{'A','2','3','4','5','6','7','8','9','X','J','Q','K'}};

    srand(time(NULL));
    do{
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
        cout << (puntero+i)->numero << " de " << (puntero+i)->palo << endl;
        i++;
    }while ((puntero+i)->palo!=0);
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
