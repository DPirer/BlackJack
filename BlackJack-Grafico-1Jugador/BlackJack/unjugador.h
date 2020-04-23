#ifndef UNJUGADOR_H
#define UNJUGADOR_H

#include <QWidget>
#include "mano.h"

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

    void crearMano();

    void resetear();

    void comprobarBlackjack();

    QString CrearDireccion(int,QString);

    void on_boton_empezar_clicked();

    void on_boton_pedir_carta_clicked();

    void on_boton_retirarse_clicked();

    void on_boton_continuar_clicked();

    void on_boton_reiniciar_clicked();

    void on_boton_salir_clicked();

    void on_boton_doblar_clicked();

    void on_boton_dividir_clicked();

private:
    Ui::UnJugador *ui;
};

#endif // UNJUGADOR_H
