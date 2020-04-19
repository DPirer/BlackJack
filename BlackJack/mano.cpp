#include "mano.h"

// Constructor
Mano::Mano(int _numero, int _palo)
{
    numero = _numero;

    switch (_palo) {
        case 1:
        palo = "Corazones";
        break;
    case 2:
        palo = "Diamantes";
        break;
    case 3:
        palo = "Picas";
        break;
    case 4:
        palo = "Treboles";
    }
}
