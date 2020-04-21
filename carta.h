#ifndef CARTA_H
#define CARTA_H
#include <iostream>
#include <QString>

using namespace std;

class Carta{
   public:
       int valor;
       QString palo;

   public:
       // Constructor
       Carta(int,int);
};

#endif // CARTA_H
