#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVector>
#include <QString>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QStackedWidget *pantallas;

    QString nombreTemp;
    int tipoTemp, colorTemp, telaTemp, tallaTemp, fitTemp, percheroTemp;

    QWidget* crearMenuPrincipal();
    QWidget* crearMenuAlmacenar();
    QWidget* crearPrendaNueva();
    QWidget* crearPrendaPrevia();
    QWidget* crearExtraerNombre();
    QWidget* crearExtraerCaracteristicas();
    QWidget* crearInformacion();
    QWidget* crearPrendasActuales();
    QWidget* crearPrendasConocidas();
    QWidget* crearFiltroCaracteristicas();
    QWidget* crearOpciones();
    QWidget* crearEliminarPrenda();

    enum Pantalla {
        MENU_PRINCIPAL = 0,
        MENU_ALMACENAR,
        PRENDA_NUEVA,
        PRENDA_PREVIA,
        EXTRAER_NOMBRE,
        EXTRAER_CARACTERISTICAS,
        MENU_INFORMACION,
        PRENDAS_ACTUALES,
        PRENDAS_CONOCIDAS,
        FILTRO_CARACTERISTICAS,
        MENU_OPCIONES,
        ELIMINAR_PRENDA
    };
};

#endif // MAINWINDOW_H