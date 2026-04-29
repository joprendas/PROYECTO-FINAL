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

#include "main_logic.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QStackedWidget *pantallas;
    SistemaRopa sistema;

    QString nombreTemp;
    int tipoTemp, colorTemp, telaTemp, tallaTemp, fitTemp, percheroTemp;

    QListWidget* listaPrendasPrevias = nullptr;
    QListWidget* listaExtraerNombre = nullptr;
    QListWidget* listaExtraerCaracteristicas = nullptr;
    QListWidget* listaPrendasConocidas = nullptr;
    QListWidget* listaEliminarPrenda = nullptr;
    QLabel* etiquetasSlots[3][5];

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

    void mostrarPantalla(int pantalla);
    void refrescarVistas();
    void llenarLista(QListWidget* lista, const QStringList& elementos, const QString& textoVacio);
    QString textoSeleccionado(QListWidget* lista) const;
    QString textoFiltro(QListWidget* lista) const;
    QString formatearResultados(const QVector<Ropa>& resultados) const;

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
