#include "carta.h"
#include <iostream>


// DEFINIMOS EL CONSTRUCTOR
Carta::Carta(int _valor,int _palo){
    this -> valor = _valor;
    switch (_palo) {
    case 1:
        palo = "Picas";
        break;
    case 2:
        palo = "Diamantes";
        break;
    case 3:
        palo = "Treboles";
        break;
    case 4:
        palo = "Corazones";
        break;
    }
}
