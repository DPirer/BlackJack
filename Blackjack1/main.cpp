#include <iostream>
#include <stdlib.h>

using namespace std;

char **baraja;
int n;

//Cabecera de las funciones.
void crearBaraja(int n);
void barajar();
void intercambiar(char **var1, char **var2);

int main()
{
    cout << "Introduce el numero de barajas: "; cin >> n;
    crearBaraja(n);
    barajar();
    return 0;
}

//funcion para crear una baraja el mazo de cartas, en funcion de la cantidad de barahas con los que se juegue.
void crearBaraja(int n){
    char numeros[]="A234567890JQK";
    char palo[]="DTPC";

    baraja=(char**)malloc((n*52)*sizeof(char*));
    for (int i=1; i<=n; i++){
        for (int j=0; j<4; j++){
            for (int k=0; k<12; k++){
                baraja[(j*12+k)*i]=(char*)malloc(2*sizeof(char*));
                baraja[(j*12+k)*i][1]=numeros[k];
                baraja[(j*12+k)*i][2]=palo[j];
            }
        }
    }
}

//funcion para barajear las cartas del mazo.
void barajar(){
    int nrand;
    for (int i=0; i<2; i++){
        for (int j=0; j<n*52; j++){
            nrand=arc4random_uniform(n*52);
            intercambiar(*&baraja[j][1], *&baraja[nrand][1]);
            intercambiar(*&baraja[j][2], *&baraja[nrand][2]);
        }
    }
}

void intercambiar(char **var1, char **var2){
    char aux;
    aux=**var1;
    **var1=**var2;
    **var2=aux;
}
