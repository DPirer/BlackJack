#ifndef UNJUGADOR_H
#define UNJUGADOR_H

#include <QWidget>
#include "mano.h"
#include <vector>

namespace Ui {
class UnJugador;
}

class UnJugador : public QWidget
{
    Q_OBJECT

public:
    explicit UnJugador(QWidget *parent = nullptr);
    ~UnJugador();

private slots:
    Mano repartirCarta();

    void crearMano(int);

    void resetear();

    bool comprobarBlackjack(Mano [11]);

    QString CrearDireccion(int,QString);

    int ValorCarta(int,int,Mano [11],int *);

    void crearBanca();

    void mostrarCarta(int,QString);

    void resultado(int);

    void BotonEmpezar();

    void BotonPedirCarta();

    void BotonRetirarse();

    void BotonContinuar();

    void BotonReiniciar();

    void BotonSalir();

    void BotonDoblar();

    void BotonDividir();

    void BotonPedirCarta2();

    void BotonDoblar2();

    void on_boton_empezar_clicked();

    void on_boton_pedir_carta_clicked();

    void on_boton_retirarse_clicked();

    void on_boton_continuar_clicked();

    void on_boton_reiniciar_clicked();

    void on_boton_salir_clicked();

    void on_boton_doblar_clicked();

    void on_boton_dividir_clicked();

    void on_boton_pedir_carta2_clicked();

    void on_boton_doblar2_clicked();

private:
    Ui::UnJugador *ui;
};

#endif // UNJUGADOR_H
