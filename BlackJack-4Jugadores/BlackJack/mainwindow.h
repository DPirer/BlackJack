#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "unjugador.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_boton_jugador1_clicked();

    void on_boton_jugador2_clicked();

    void on_boton_jugador3_clicked();

    void on_boton_jugador4_clicked();

    void on_boton_salir_clicked();


private:
    Ui::MainWindow *ui;
    UnJugador *jugador_uno = new UnJugador();
};
#endif // MAINWINDOW_H
