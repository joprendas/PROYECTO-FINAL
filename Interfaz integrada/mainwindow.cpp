#include "mainwindow.h"
#include <QMessageBox>

// Opciones fijas del sistema
const QVector<QString> TIPOS     = {"T-Shirt", "Pantalon", "Short", "Camisa Manga Larga", "Enagua"};
const QVector<QString> COLORES   = {"Blanco", "Negro", "Cafe", "Azul", "Gris"};
const QVector<QString> TELAS     = {"Denim", "Algodon", "Polyester", "Seda", "Lino"};
const QVector<QString> TALLAS    = {"S", "M", "L", "XL", "XXL"};
const QVector<QString> FITS      = {"Regular", "Skinny", "Slim", "Loose", "Oversized"};
const QVector<QString> PERCHEROS = {"Perchero 1", "Perchero 2", "Perchero 3"};

// ─────────────────────────────────────────────
// CONSTRUCTOR
// ─────────────────────────────────────────────
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), sistema("ropa.txt") {

    for (int p = 0; p < 3; ++p) {
        for (int s = 0; s < 5; ++s) {
            etiquetasSlots[p][s] = nullptr;
        }
    }

    pantallas = new QStackedWidget(this);
    setCentralWidget(pantallas);

    pantallas->addWidget(crearMenuPrincipal());           // 0
    pantallas->addWidget(crearMenuAlmacenar());           // 1
    pantallas->addWidget(crearPrendaNueva());             // 2
    pantallas->addWidget(crearPrendaPrevia());            // 3
    pantallas->addWidget(crearExtraerNombre());           // 4
    pantallas->addWidget(crearExtraerCaracteristicas());  // 5
    pantallas->addWidget(crearInformacion());             // 6
    pantallas->addWidget(crearPrendasActuales());         // 7
    pantallas->addWidget(crearPrendasConocidas());        // 8
    pantallas->addWidget(crearFiltroCaracteristicas());   // 9
    pantallas->addWidget(crearOpciones());                // 10
    pantallas->addWidget(crearEliminarPrenda());          // 11

    refrescarVistas();
    pantallas->setCurrentIndex(MENU_PRINCIPAL);
}

MainWindow::~MainWindow() {}

// ─────────────────────────────────────────────
// UTILIDADES VISUALES
// ─────────────────────────────────────────────
static QLabel* crearTitulo(const QString& texto) {
    QLabel* label = new QLabel(texto);
    label->setObjectName("titulo");
    label->setAlignment(Qt::AlignCenter);
    return label;
}

static QListWidget* crearLista(const QVector<QString>& opciones) {
    QListWidget* lista = new QListWidget();
    for (const QString& op : opciones) {
        lista->addItem(op);
    }
    if (!opciones.isEmpty()) lista->setCurrentRow(0);
    return lista;
}

void MainWindow::mostrarPantalla(int pantalla) {
    refrescarVistas();
    pantallas->setCurrentIndex(pantalla);
}

void MainWindow::llenarLista(QListWidget* lista, const QStringList& elementos, const QString& textoVacio) {
    if (!lista) return;

    lista->clear();
    if (elementos.isEmpty()) {
        lista->addItem(textoVacio);
    } else {
        lista->addItems(elementos);
    }
    lista->setCurrentRow(0);
}

QString MainWindow::textoSeleccionado(QListWidget* lista) const {
    if (!lista || !lista->currentItem()) return "";

    QString texto = lista->currentItem()->text().trimmed();
    if (texto.startsWith("[")) return "";
    return texto;
}

QString MainWindow::textoFiltro(QListWidget* lista) const {
    QString texto = textoSeleccionado(lista);
    if (texto.compare("Ninguno", Qt::CaseInsensitive) == 0) return "";
    return texto;
}

QString MainWindow::formatearResultados(const QVector<Ropa>& resultados) const {
    if (resultados.isEmpty()) {
        return "No se encontraron coincidencias.";
    }

    QStringList lineas;
    for (const Ropa& r : resultados) {
        lineas << QString::fromUtf8("• ") + r.descripcionCorta();
    }
    return lineas.join('\n');
}

void MainWindow::refrescarVistas() {
    llenarLista(listaPrendasPrevias,
                sistema.nombresNoAlmacenadas(),
                "[No hay prendas conocidas fuera de los percheros]");

    llenarLista(listaExtraerNombre,
                sistema.nombresAlmacenadas(),
                "[No hay prendas almacenadas]");

    llenarLista(listaPrendasConocidas,
                sistema.nombresNoAlmacenadas(),
                "[No hay prendas conocidas fuera de los percheros]");

    llenarLista(listaEliminarPrenda,
                sistema.nombresNoAlmacenadas(),
                "[No hay prendas eliminables]");

    for (int p = 0; p < 3; ++p) {
        for (int s = 0; s < 5; ++s) {
            if (etiquetasSlots[p][s]) {
                etiquetasSlots[p][s]->setText(QString("Slot %1: %2").arg(s + 1).arg(sistema.prendaEnSlot(p, s)));
            }
        }
    }
}

// ─────────────────────────────────────────────
// MENÚ PRINCIPAL
// ─────────────────────────────────────────────
QWidget* MainWindow::crearMenuPrincipal() {
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);
    layout->setSpacing(15);
    layout->setContentsMargins(80, 40, 80, 40);

    layout->addWidget(crearTitulo("SISTEMA DE PERCHEROS INTELIGENTES"));
    layout->addWidget(new QLabel("Seleccione una opcion:"));

    QPushButton* btnAlmacenar   = new QPushButton("Almacenar Prenda");
    QPushButton* btnExtraer     = new QPushButton("Extraer Prenda");
    QPushButton* btnInformacion = new QPushButton("Informacion de Prendas");
    QPushButton* btnOpciones    = new QPushButton("Opciones");
    QPushButton* btnSalir       = new QPushButton("Salir");

    btnSalir->setStyleSheet("background-color: #c0392b;");

    layout->addWidget(btnAlmacenar);
    layout->addWidget(btnExtraer);
    layout->addWidget(btnInformacion);
    layout->addWidget(btnOpciones);
    layout->addStretch();
    layout->addWidget(btnSalir);

    connect(btnAlmacenar,   &QPushButton::clicked, this, [this]{ mostrarPantalla(MENU_ALMACENAR); });
    connect(btnExtraer,     &QPushButton::clicked, this, [this]{ mostrarPantalla(EXTRAER_NOMBRE); });
    connect(btnInformacion, &QPushButton::clicked, this, [this]{ mostrarPantalla(MENU_INFORMACION); });
    connect(btnOpciones,    &QPushButton::clicked, this, [this]{ mostrarPantalla(MENU_OPCIONES); });
    connect(btnSalir,       &QPushButton::clicked, this, &MainWindow::close);

    return w;
}

// ─────────────────────────────────────────────
// MENÚ ALMACENAR
// ─────────────────────────────────────────────
QWidget* MainWindow::crearMenuAlmacenar() {
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);
    layout->setSpacing(15);
    layout->setContentsMargins(80, 40, 80, 40);

    layout->addWidget(crearTitulo("ALMACENAR PRENDA"));

    QPushButton* btnNueva  = new QPushButton("Prenda Nueva");
    QPushButton* btnPrevia = new QPushButton("Prenda Previa");
    QPushButton* btnVolver = new QPushButton("Volver");
    btnVolver->setStyleSheet("background-color: #555;");

    layout->addWidget(btnNueva);
    layout->addWidget(btnPrevia);
    layout->addStretch();
    layout->addWidget(btnVolver);

    connect(btnNueva,  &QPushButton::clicked, this, [this]{ mostrarPantalla(PRENDA_NUEVA); });
    connect(btnPrevia, &QPushButton::clicked, this, [this]{ mostrarPantalla(PRENDA_PREVIA); });
    connect(btnVolver, &QPushButton::clicked, this, [this]{ mostrarPantalla(MENU_PRINCIPAL); });

    return w;
}

// ─────────────────────────────────────────────
// PRENDA NUEVA
// ─────────────────────────────────────────────
QWidget* MainWindow::crearPrendaNueva() {
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);
    layout->setSpacing(10);
    layout->setContentsMargins(60, 20, 60, 20);

    layout->addWidget(crearTitulo("NUEVA PRENDA"));

    QLineEdit* campoNombre = new QLineEdit();
    campoNombre->setPlaceholderText("Nombre unico de la prenda...");

    QListWidget* listaTipo     = crearLista(TIPOS);
    QListWidget* listaColor    = crearLista(COLORES);
    QListWidget* listaTela     = crearLista(TELAS);
    QListWidget* listaTalla    = crearLista(TALLAS);
    QListWidget* listaFit      = crearLista(FITS);
    QListWidget* listaPerchero = crearLista(PERCHEROS);

    layout->addWidget(new QLabel("Nombre unico:"));
    layout->addWidget(campoNombre);

    QHBoxLayout* filasCarac = new QHBoxLayout();
    auto agregarColumna = [&](const QString& titulo, QListWidget* lista) {
        QVBoxLayout* col = new QVBoxLayout();
        col->addWidget(new QLabel(titulo));
        col->addWidget(lista);
        filasCarac->addLayout(col);
    };

    agregarColumna("Tipo",     listaTipo);
    agregarColumna("Color",    listaColor);
    agregarColumna("Tela",     listaTela);
    agregarColumna("Talla",    listaTalla);
    agregarColumna("Fit",      listaFit);
    agregarColumna("Perchero", listaPerchero);

    layout->addLayout(filasCarac);

    QHBoxLayout* botones = new QHBoxLayout();
    QPushButton* btnConfirmar = new QPushButton("Confirmar");
    QPushButton* btnVolver    = new QPushButton("Volver");
    btnVolver->setStyleSheet("background-color: #555;");
    botones->addWidget(btnVolver);
    botones->addWidget(btnConfirmar);
    layout->addLayout(botones);

    connect(btnConfirmar, &QPushButton::clicked, this, [=]() {
        QString error;
        bool ok = sistema.agregarRopa(
            campoNombre->text(),
            listaTipo->currentItem()->text(),
            listaColor->currentItem()->text(),
            listaTela->currentItem()->text(),
            listaTalla->currentItem()->text(),
            listaFit->currentItem()->text(),
            listaPerchero->currentRow(),
            &error
        );

        if (!ok) {
            QMessageBox::warning(this, "Error", error);
            return;
        }

        campoNombre->clear();
        QMessageBox::information(this, "Exito", "Prenda guardada correctamente.");
        mostrarPantalla(MENU_PRINCIPAL);
    });

    connect(btnVolver, &QPushButton::clicked, this, [this]{ mostrarPantalla(MENU_ALMACENAR); });

    return w;
}

// ─────────────────────────────────────────────
// PRENDA PREVIA
// ─────────────────────────────────────────────
QWidget* MainWindow::crearPrendaPrevia() {
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);
    layout->setSpacing(15);
    layout->setContentsMargins(80, 40, 80, 40);

    layout->addWidget(crearTitulo("ALMACENAR PRENDA PREVIA"));

    listaPrendasPrevias = crearLista({"[Prendas conocidas se cargaran aqui]"});
    QListWidget* listaPerchero = crearLista(PERCHEROS);

    layout->addWidget(new QLabel("Seleccione la prenda:"));
    layout->addWidget(listaPrendasPrevias);
    layout->addWidget(new QLabel("Perchero destino:"));
    layout->addWidget(listaPerchero);

    QHBoxLayout* botones = new QHBoxLayout();
    QPushButton* btnConfirmar = new QPushButton("Confirmar");
    QPushButton* btnVolver    = new QPushButton("Volver");
    btnVolver->setStyleSheet("background-color: #555;");
    botones->addWidget(btnVolver);
    botones->addWidget(btnConfirmar);
    layout->addLayout(botones);

    connect(btnConfirmar, &QPushButton::clicked, this, [=]() {
        QString nombre = textoSeleccionado(listaPrendasPrevias);
        if (nombre.isEmpty()) {
            QMessageBox::warning(this, "Error", "No hay una prenda valida seleccionada.");
            return;
        }

        QString error;
        if (!sistema.almacenarPrevia(nombre, listaPerchero->currentRow(), &error)) {
            QMessageBox::warning(this, "Error", error);
            return;
        }

        QMessageBox::information(this, "Exito", "Prenda previa almacenada correctamente.");
        mostrarPantalla(MENU_PRINCIPAL);
    });

    connect(btnVolver, &QPushButton::clicked, this, [this]{ mostrarPantalla(MENU_ALMACENAR); });

    return w;
}

// ─────────────────────────────────────────────
// EXTRAER POR NOMBRE
// ─────────────────────────────────────────────
QWidget* MainWindow::crearExtraerNombre() {
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);
    layout->setSpacing(15);
    layout->setContentsMargins(80, 40, 80, 40);

    layout->addWidget(crearTitulo("EXTRAER PRENDA POR NOMBRE"));

    listaExtraerNombre = crearLista({"[Prendas disponibles se cargaran aqui]"});
    layout->addWidget(new QLabel("Seleccione la prenda a extraer:"));
    layout->addWidget(listaExtraerNombre);

    QHBoxLayout* botones = new QHBoxLayout();
    QPushButton* btnConfirmar = new QPushButton("Confirmar Extraccion");
    QPushButton* btnCaract    = new QPushButton("Extraer por Caracteristicas");
    QPushButton* btnVolver    = new QPushButton("Volver");
    btnVolver->setStyleSheet("background-color: #555;");
    botones->addWidget(btnVolver);
    botones->addWidget(btnCaract);
    botones->addWidget(btnConfirmar);
    layout->addLayout(botones);

    connect(btnConfirmar, &QPushButton::clicked, this, [=]() {
        QString nombre = textoSeleccionado(listaExtraerNombre);
        if (nombre.isEmpty()) {
            QMessageBox::warning(this, "Error", "No hay una prenda valida seleccionada.");
            return;
        }

        QString error;
        if (!sistema.extraerPorNombre(nombre, &error)) {
            QMessageBox::warning(this, "Error", error);
            return;
        }

        QMessageBox::information(this, "Extraccion", "El sistema esta presentando la prenda.\nRetirela del perchero.");
        mostrarPantalla(MENU_PRINCIPAL);
    });

    connect(btnCaract, &QPushButton::clicked, this, [this]{ mostrarPantalla(EXTRAER_CARACTERISTICAS); });
    connect(btnVolver, &QPushButton::clicked, this, [this]{ mostrarPantalla(MENU_PRINCIPAL); });

    return w;
}

// ─────────────────────────────────────────────
// EXTRAER POR CARACTERÍSTICAS
// ─────────────────────────────────────────────
QWidget* MainWindow::crearExtraerCaracteristicas() {
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);
    layout->setSpacing(15);
    layout->setContentsMargins(80, 40, 80, 40);

    layout->addWidget(crearTitulo("EXTRAER POR CARACTERISTICAS"));

    QLabel* instrucciones = new QLabel(
        "Ingrese un valor de caracteristica, por ejemplo: Negro, S, Algodon, Slim.\n"
        "El sistema buscara solo entre prendas actualmente almacenadas."
    );
    instrucciones->setStyleSheet("color: #f0c040; font-size: 13px;");
    instrucciones->setWordWrap(true);

    QLineEdit* campoBusqueda = new QLineEdit();
    campoBusqueda->setPlaceholderText("Valor a buscar...");

    listaExtraerCaracteristicas = new QListWidget();
    listaExtraerCaracteristicas->addItem("[Resultados de busqueda]");

    QLabel* detalle = new QLabel("Los resultados apareceran aqui...");
    detalle->setWordWrap(true);
    detalle->setStyleSheet("color: #aaaaaa;");

    layout->addWidget(instrucciones);
    layout->addWidget(new QLabel("Caracteristica:"));
    layout->addWidget(campoBusqueda);
    layout->addWidget(new QLabel("Seleccione resultado:"));
    layout->addWidget(listaExtraerCaracteristicas);
    layout->addWidget(detalle);

    QHBoxLayout* botones = new QHBoxLayout();
    QPushButton* btnBuscar    = new QPushButton("Buscar");
    QPushButton* btnConfirmar = new QPushButton("Confirmar Extraccion");
    QPushButton* btnVolver    = new QPushButton("Volver");
    btnVolver->setStyleSheet("background-color: #555;");
    botones->addWidget(btnVolver);
    botones->addWidget(btnBuscar);
    botones->addWidget(btnConfirmar);
    layout->addLayout(botones);

    connect(btnBuscar, &QPushButton::clicked, this, [=]() {
        QVector<Ropa> resultados = sistema.buscarPorCaracteristica(campoBusqueda->text(), true);
        listaExtraerCaracteristicas->clear();

        if (resultados.isEmpty()) {
            listaExtraerCaracteristicas->addItem("[No hay coincidencias]");
        } else {
            for (const Ropa& r : resultados) {
                listaExtraerCaracteristicas->addItem(r.nombre);
            }
        }
        listaExtraerCaracteristicas->setCurrentRow(0);
        detalle->setText(formatearResultados(resultados));
    });

    connect(btnConfirmar, &QPushButton::clicked, this, [=]() {
        QString nombre = textoSeleccionado(listaExtraerCaracteristicas);
        if (nombre.isEmpty()) {
            QMessageBox::warning(this, "Error", "Primero busque y seleccione una prenda valida.");
            return;
        }

        QString error;
        if (!sistema.extraerPorNombre(nombre, &error)) {
            QMessageBox::warning(this, "Error", error);
            return;
        }

        QMessageBox::information(this, "Extraccion", "El sistema esta presentando la prenda.\nRetirela del perchero.");
        mostrarPantalla(MENU_PRINCIPAL);
    });

    connect(btnVolver, &QPushButton::clicked, this, [this]{ mostrarPantalla(EXTRAER_NOMBRE); });

    return w;
}

// ─────────────────────────────────────────────
// MENÚ INFORMACIÓN
// ─────────────────────────────────────────────
QWidget* MainWindow::crearInformacion() {
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);
    layout->setSpacing(15);
    layout->setContentsMargins(80, 40, 80, 40);

    layout->addWidget(crearTitulo("INFORMACION DE PRENDAS"));

    QPushButton* btnActuales  = new QPushButton("Prendas actuales en percheros");
    QPushButton* btnConocidas = new QPushButton("Prendas conocidas no almacenadas");
    QPushButton* btnFiltro    = new QPushButton("Informe por caracteristicas");
    QPushButton* btnVolver    = new QPushButton("Volver");
    btnVolver->setStyleSheet("background-color: #555;");

    layout->addWidget(btnActuales);
    layout->addWidget(btnConocidas);
    layout->addWidget(btnFiltro);
    layout->addStretch();
    layout->addWidget(btnVolver);

    connect(btnActuales,  &QPushButton::clicked, this, [this]{ mostrarPantalla(PRENDAS_ACTUALES); });
    connect(btnConocidas, &QPushButton::clicked, this, [this]{ mostrarPantalla(PRENDAS_CONOCIDAS); });
    connect(btnFiltro,    &QPushButton::clicked, this, [this]{ mostrarPantalla(FILTRO_CARACTERISTICAS); });
    connect(btnVolver,    &QPushButton::clicked, this, [this]{ mostrarPantalla(MENU_PRINCIPAL); });

    return w;
}

// ─────────────────────────────────────────────
// PRENDAS ACTUALES EN PERCHEROS
// ─────────────────────────────────────────────
QWidget* MainWindow::crearPrendasActuales() {
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);
    layout->setSpacing(10);
    layout->setContentsMargins(40, 20, 40, 20);

    layout->addWidget(crearTitulo("PRENDAS ACTUALES EN PERCHEROS"));

    QHBoxLayout* percheros = new QHBoxLayout();
    for (int p = 0; p < 3; p++) {
        QVBoxLayout* col = new QVBoxLayout();
        QLabel* titulo = new QLabel(QString("PERCHERO %1").arg(p + 1));
        titulo->setStyleSheet("font-weight: bold; color: #00b4d8;");
        col->addWidget(titulo);

        for (int s = 0; s < 5; s++) {
            etiquetasSlots[p][s] = new QLabel(QString("Slot %1: [vacio]").arg(s + 1));
            etiquetasSlots[p][s]->setStyleSheet("color: #aaaaaa; padding: 3px;");
            col->addWidget(etiquetasSlots[p][s]);
        }
        percheros->addLayout(col);
    }
    layout->addLayout(percheros);

    QHBoxLayout* botones = new QHBoxLayout();
    QPushButton* btnInforme = new QPushButton("Generar informe .txt");
    QPushButton* btnVolver  = new QPushButton("Volver");
    btnVolver->setStyleSheet("background-color: #555;");
    botones->addWidget(btnVolver);
    botones->addWidget(btnInforme);
    layout->addStretch();
    layout->addLayout(botones);

    connect(btnInforme, &QPushButton::clicked, this, [=]() {
        QString error;
        if (!sistema.generarInformeTxt("informe_percheros.txt", &error)) {
            QMessageBox::warning(this, "Error", error);
            return;
        }
        QMessageBox::information(this, "Informe", "Informe generado en informe_percheros.txt");
    });

    connect(btnVolver, &QPushButton::clicked, this, [this]{ mostrarPantalla(MENU_INFORMACION); });

    return w;
}

// ─────────────────────────────────────────────
// PRENDAS CONOCIDAS NO ALMACENADAS
// ─────────────────────────────────────────────
QWidget* MainWindow::crearPrendasConocidas() {
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);
    layout->setSpacing(15);
    layout->setContentsMargins(80, 40, 80, 40);

    layout->addWidget(crearTitulo("PRENDAS CONOCIDAS NO ALMACENADAS"));

    listaPrendasConocidas = crearLista({"[Lista se cargara aqui cuando haya datos]"});
    layout->addWidget(listaPrendasConocidas);

    QPushButton* btnVolver = new QPushButton("Volver");
    btnVolver->setStyleSheet("background-color: #555;");
    layout->addStretch();
    layout->addWidget(btnVolver);

    connect(btnVolver, &QPushButton::clicked, this, [this]{ mostrarPantalla(MENU_INFORMACION); });

    return w;
}

// ─────────────────────────────────────────────
// FILTRO POR CARACTERÍSTICAS
// ─────────────────────────────────────────────
QWidget* MainWindow::crearFiltroCaracteristicas() {
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);
    layout->setSpacing(10);
    layout->setContentsMargins(40, 20, 40, 20);

    layout->addWidget(crearTitulo("FILTRO POR CARACTERISTICAS"));
    layout->addWidget(new QLabel("Seleccione filtros ('Ninguno' para omitir):"));

    auto crearListaConNinguno = [&](const QVector<QString>& base) {
        QVector<QString> lista = {"Ninguno"};
        for (const QString& s : base) lista.append(s);
        return crearLista(lista);
    };

    QListWidget* listaTipo  = crearListaConNinguno(TIPOS);
    QListWidget* listaColor = crearListaConNinguno(COLORES);
    QListWidget* listaTela  = crearListaConNinguno(TELAS);
    QListWidget* listaTalla = crearListaConNinguno(TALLAS);
    QListWidget* listaFit   = crearListaConNinguno(FITS);

    QHBoxLayout* filtros = new QHBoxLayout();
    auto agregarFiltro = [&](const QString& titulo, QListWidget* lista) {
        QVBoxLayout* col = new QVBoxLayout();
        col->addWidget(new QLabel(titulo));
        col->addWidget(lista);
        filtros->addLayout(col);
    };
    agregarFiltro("Tipo",  listaTipo);
    agregarFiltro("Color", listaColor);
    agregarFiltro("Tela",  listaTela);
    agregarFiltro("Talla", listaTalla);
    agregarFiltro("Fit",   listaFit);
    layout->addLayout(filtros);

    QHBoxLayout* botones = new QHBoxLayout();
    QPushButton* btnBuscar = new QPushButton("Buscar");
    QPushButton* btnVolver = new QPushButton("Volver");
    btnVolver->setStyleSheet("background-color: #555;");
    botones->addWidget(btnVolver);
    botones->addWidget(btnBuscar);
    layout->addLayout(botones);

    QLabel* resultados = new QLabel("Los resultados apareceran aqui...");
    resultados->setWordWrap(true);
    resultados->setStyleSheet("color: #aaaaaa;");
    layout->addWidget(resultados);

    connect(btnBuscar, &QPushButton::clicked, this, [=]() {
        QVector<Ropa> encontrados = sistema.filtrarPrendas(
            textoFiltro(listaTipo),
            textoFiltro(listaColor),
            textoFiltro(listaTela),
            textoFiltro(listaTalla),
            textoFiltro(listaFit),
            false
        );
        resultados->setText(formatearResultados(encontrados));
    });

    connect(btnVolver, &QPushButton::clicked, this, [this]{ mostrarPantalla(MENU_INFORMACION); });

    return w;
}

// ─────────────────────────────────────────────
// MENÚ OPCIONES
// ─────────────────────────────────────────────
QWidget* MainWindow::crearOpciones() {
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);
    layout->setSpacing(15);
    layout->setContentsMargins(80, 40, 80, 40);

    layout->addWidget(crearTitulo("OPCIONES DEL SISTEMA"));

    QPushButton* btnEliminar = new QPushButton("Eliminar prenda");
    QPushButton* btnGuardar  = new QPushButton("Guardar estado (.txt)");
    QPushButton* btnCargar   = new QPushButton("Cargar estado (.txt)");
    QPushButton* btnVolver   = new QPushButton("Volver");
    btnVolver->setStyleSheet("background-color: #555;");

    layout->addWidget(btnEliminar);
    layout->addWidget(btnGuardar);
    layout->addWidget(btnCargar);
    layout->addStretch();
    layout->addWidget(btnVolver);

    connect(btnEliminar, &QPushButton::clicked, this, [this]{ mostrarPantalla(ELIMINAR_PRENDA); });

    connect(btnGuardar, &QPushButton::clicked, this, [=]() {
        QString error;
        if (!sistema.guardarComo("estado.txt", &error)) {
            QMessageBox::warning(this, "Error", error);
            return;
        }
        QMessageBox::information(this, "Guardado", "Estado guardado correctamente en estado.txt");
    });

    connect(btnCargar, &QPushButton::clicked, this, [=]() {
        QString error;
        if (!sistema.cargarDesde("estado.txt", &error)) {
            QMessageBox::warning(this, "Error", error);
            return;
        }
        sistema.guardar(); // actualiza tambien ropa.txt
        refrescarVistas();
        QMessageBox::information(this, "Cargado", "Estado cargado correctamente desde estado.txt");
    });

    connect(btnVolver, &QPushButton::clicked, this, [this]{ mostrarPantalla(MENU_PRINCIPAL); });

    return w;
}

// ─────────────────────────────────────────────
// ELIMINAR PRENDA
// ─────────────────────────────────────────────
QWidget* MainWindow::crearEliminarPrenda() {
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);
    layout->setSpacing(15);
    layout->setContentsMargins(80, 40, 80, 40);

    layout->addWidget(crearTitulo("ELIMINAR PRENDA"));
    layout->addWidget(new QLabel("Solo se pueden eliminar prendas que no esten en ningun perchero:"));

    listaEliminarPrenda = crearLista({"[Prendas eliminables se cargaran aqui]"});
    layout->addWidget(listaEliminarPrenda);

    QHBoxLayout* botones = new QHBoxLayout();
    QPushButton* btnEliminar = new QPushButton("Eliminar");
    QPushButton* btnVolver   = new QPushButton("Volver");
    btnEliminar->setStyleSheet("background-color: #c0392b;");
    btnVolver->setStyleSheet("background-color: #555;");
    botones->addWidget(btnVolver);
    botones->addWidget(btnEliminar);
    layout->addStretch();
    layout->addLayout(botones);

    connect(btnEliminar, &QPushButton::clicked, this, [=]() {
        QString nombre = textoSeleccionado(listaEliminarPrenda);
        if (nombre.isEmpty()) {
            QMessageBox::warning(this, "Error", "No hay una prenda valida seleccionada.");
            return;
        }

        QString error;
        if (!sistema.eliminarPrenda(nombre, &error)) {
            QMessageBox::warning(this, "Error", error);
            return;
        }

        QMessageBox::information(this, "Eliminado", "Prenda eliminada del sistema.");
        mostrarPantalla(MENU_OPCIONES);
    });

    connect(btnVolver, &QPushButton::clicked, this, [this]{ mostrarPantalla(MENU_OPCIONES); });

    return w;
}
