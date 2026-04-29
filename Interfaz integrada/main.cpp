#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Estilo visual oscuro para toda la aplicación
    app.setStyleSheet(
        "QMainWindow { background-color: #1e1e1e; }"
        "QWidget { background-color: #1e1e1e; color: #ffffff; font-size: 14px; }"
        "QPushButton {"
        "   background-color: #0078d4;"
        "   color: white;"
        "   border: none;"
        "   padding: 10px 20px;"
        "   border-radius: 5px;"
        "   font-size: 14px;"
        "}"
        "QPushButton:hover { background-color: #106ebe; }"
        "QPushButton:pressed { background-color: #005a9e; }"
        "QListWidget {"
        "   background-color: #2d2d2d;"
        "   border: 1px solid #444;"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "}"
        "QListWidget::item:selected {"
        "   background-color: #0078d4;"
        "   color: white;"
        "}"
        "QLabel { color: #ffffff; }"
        "QLabel#titulo {"
        "   font-size: 20px;"
        "   font-weight: bold;"
        "   color: #00b4d8;"
        "   padding: 10px;"
        "}"
        "QLineEdit {"
        "   background-color: #2d2d2d;"
        "   border: 1px solid #444;"
        "   border-radius: 5px;"
        "   padding: 8px;"
        "   color: white;"
        "}"
    );

    MainWindow ventana;
    ventana.setWindowTitle("Sistema de Percheros Inteligentes - TEC MT-7003");
    ventana.resize(700, 500);
    ventana.show();

    return app.exec();
}