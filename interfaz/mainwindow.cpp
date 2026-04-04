#include "mainwindow.h"
#include <QMessageBox>

// Opciones fijas del sistema
const QVector<QString> TIPOS    = {"T-Shirt", "Pantalon", "Short", "Camisa Manga Larga", "Enagua"};
const QVector<QString> COLORES  = {"Blanco", "Negro", "Cafe", "Azul", "Gris"};
const QVector<QString> TELAS    = {"Denim", "Algodon", "Polyester", "Seda", "Lino"};
const QVector<QString> TALLAS   = {"S", "M", "L", "XL", "XXL"};
const QVector<QString> FITS     = {"Regular", "Skinny", "Slim", "Loose", "Oversized"};
const QVector<QString> PERCHEROS = {"Perchero 1", "Perchero 2", "Perchero 3"};

// ─────────────────────────────────────────────
// CONSTRUCTOR — Crea todas las pantallas y las
// agrega al QStackedWidget en orden
// ─────────────────────────────────────────────
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
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

    pantallas->setCurrentIndex(MENU_PRINCIPAL);
}

MainWindow::~MainWindow() {}

// ─────────────────────────────────────────────
// UTILIDAD: crea un QLabel con estilo de título
// ─────────────────────────────────────────────
static QLabel* crearTitulo(const QString& texto) {
    QLabel* label = new QLabel(texto);
    label->setObjectName("titulo");
    label->setAlignment(Qt::AlignCenter);
    return label;
}

// ─────────────────────────────────────────────
// UTILIDAD: crea una QListWidget con opciones
// ─────────────────────────────────────────────
static QListWidget* crearLista(const QVector<QString>& opciones) {
    QListWidget* lista = new QListWidget();
    for (const QString& op : opciones)
        lista->addItem(op);
    lista->setCurrentRow(0);
    return lista;
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

    QPushButton* btnAlmacenar    = new QPushButton("Almacenar Prenda");
    QPushButton* btnExtraer      = new QPushButton("Extraer Prenda");
    QPushButton* btnInformacion  = new QPushButton("Informacion de Prendas");
    QPushButton* btnOpciones     = new QPushButton("Opciones");
    QPushButton* btnSalir        = new QPushButton("Salir");

    btnSalir->setStyleSheet("background-color: #c0392b;");

    layout->addWidget(btnAlmacenar);
    layout->addWidget(btnExtraer);
    layout->addWidget(btnInformacion);
    layout->addWidget(btnOpciones);
    layout->addStretch();
    layout->addWidget(btnSalir);

    connect(btnAlmacenar,   &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(MENU_ALMACENAR); });
    connect(btnExtraer,     &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(EXTRAER_NOMBRE); });
    connect(btnInformacion, &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(MENU_INFORMACION); });
    connect(btnOpciones,    &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(MENU_OPCIONES); });
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

    connect(btnNueva,  &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(PRENDA_NUEVA); });
    connect(btnPrevia, &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(PRENDA_PREVIA); });
    connect(btnVolver, &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(MENU_PRINCIPAL); });

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
        if (campoNombre->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Error", "Debe ingresar un nombre unico para la prenda.");
            return;
        }
        // Guardar datos temporales
        nombreTemp   = campoNombre->text().trimmed();
        tipoTemp     = listaTipo->currentRow();
        colorTemp    = listaColor->currentRow();
        telaTemp     = listaTela->currentRow();
        tallaTemp    = listaTalla->currentRow();
        fitTemp      = listaFit->currentRow();
        percheroTemp = listaPerchero->currentRow();

        // TODO: Llamar a sistema.almacenarNueva(nombreTemp, TIPOS[tipoTemp], ...)
        QMessageBox::information(this, "Exito", "Prenda guardada correctamente.");
        pantallas->setCurrentIndex(MENU_PRINCIPAL);
    });

    connect(btnVolver, &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(MENU_ALMACENAR); });

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

    // TODO: Reemplazar con lista real: sistema.getPrendasNoAlmacenadas()
    QListWidget* listaPrendas = crearLista({"[Prendas conocidas se cargaran aqui]"});
    QListWidget* listaPerchero = crearLista(PERCHEROS);

    layout->addWidget(new QLabel("Seleccione la prenda:"));
    layout->addWidget(listaPrendas);
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
        // TODO: sistema.almacenarPrevia(listaPrendas->currentItem()->text(), listaPerchero->currentRow()+1)
        QMessageBox::information(this, "Exito", "Prenda previa almacenada correctamente.");
        pantallas->setCurrentIndex(MENU_PRINCIPAL);
    });
    connect(btnVolver, &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(MENU_ALMACENAR); });

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

    // TODO: Reemplazar con lista real: sistema.getPrendasAlmacenadas()
    QListWidget* listaPrendas = crearLista({"[Prendas disponibles se cargaran aqui]"});
    layout->addWidget(new QLabel("Seleccione la prenda a extraer:"));
    layout->addWidget(listaPrendas);

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
        // TODO: sistema.extraerPorNombre(listaPrendas->currentItem()->text())
        QMessageBox::information(this, "Extraccion", "El sistema esta presentando la prenda.\nRetirela del perchero.");
        pantallas->setCurrentIndex(MENU_PRINCIPAL);
    });
    connect(btnCaract, &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(EXTRAER_CARACTERISTICAS); });
    connect(btnVolver, &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(MENU_PRINCIPAL); });

    return w;
}

// ─────────────────────────────────────────────
// EXTRAER POR CARACTERÍSTICAS (KEYPAD)
// ─────────────────────────────────────────────
QWidget* MainWindow::crearExtraerCaracteristicas() {
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);
    layout->setSpacing(15);
    layout->setContentsMargins(80, 40, 80, 40);

    layout->addWidget(crearTitulo("EXTRAER POR CARACTERISTICAS"));

    QLabel* instrucciones = new QLabel(
        "Use el teclado matricial 4x4 para ingresar las caracteristicas.\n\n"
        "- El tipo de prenda es OBLIGATORIO.\n"
        "- Presione B en el keypad para omitir una caracteristica.\n"
        "- Presione D en el keypad para ejecutar la busqueda."
    );
    instrucciones->setStyleSheet("color: #f0c040; font-size: 13px;");
    instrucciones->setWordWrap(true);

    QLabel* estado = new QLabel("Esperando entrada del keypad...");
    estado->setStyleSheet("color: #aaaaaa;");

    layout->addWidget(instrucciones);
    layout->addWidget(estado);
    layout->addStretch();

    QPushButton* btnVolver = new QPushButton("Volver");
    btnVolver->setStyleSheet("background-color: #555;");
    layout->addWidget(btnVolver);

    // TODO: Implementar lectura del keypad 4x4 con WiringPi en Raspberry Pi

    connect(btnVolver, &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(EXTRAER_NOMBRE); });

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

    connect(btnActuales,  &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(PRENDAS_ACTUALES); });
    connect(btnConocidas, &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(PRENDAS_CONOCIDAS); });
    connect(btnFiltro,    &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(FILTRO_CARACTERISTICAS); });
    connect(btnVolver,    &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(MENU_PRINCIPAL); });

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
            // TODO: Reemplazar "[vacio]" con sistema.getPrendaEnSlot(p, s)
            QLabel* slot = new QLabel(QString("Slot %1: [vacio]").arg(s + 1));
            slot->setStyleSheet("color: #aaaaaa; padding: 3px;");
            col->addWidget(slot);
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
        // TODO: sistema.generarInformeTxt()
        QMessageBox::information(this, "Informe", "Informe generado en estado.txt");
    });
    connect(btnVolver, &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(MENU_INFORMACION); });

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

    // TODO: Reemplazar con sistema.getPrendasNoAlmacenadas()
    QListWidget* lista = crearLista({"[Lista se cargara aqui cuando haya datos]"});
    layout->addWidget(lista);

    QPushButton* btnVolver = new QPushButton("Volver");
    btnVolver->setStyleSheet("background-color: #555;");
    layout->addStretch();
    layout->addWidget(btnVolver);

    connect(btnVolver, &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(MENU_INFORMACION); });

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

    QVector<QString> conNinguno;

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
    resultados->setStyleSheet("color: #aaaaaa;");
    layout->addWidget(resultados);

    connect(btnBuscar, &QPushButton::clicked, this, [=]() {
        // TODO: sistema.filtrarPrendas(tipo, color, tela, talla, fit)
        resultados->setText("(Resultados se mostraran aqui cuando el sistema tenga datos)");
    });
    connect(btnVolver, &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(MENU_INFORMACION); });

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

    connect(btnEliminar, &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(ELIMINAR_PRENDA); });
    connect(btnGuardar,  &QPushButton::clicked, this, [=]() {
        // TODO: sistema.guardarEstado("estado.txt")
        QMessageBox::information(this, "Guardado", "Estado guardado correctamente en estado.txt");
    });
    connect(btnCargar,   &QPushButton::clicked, this, [=]() {
        // TODO: sistema.cargarEstado("estado.txt")
        QMessageBox::information(this, "Cargado", "Estado cargado correctamente desde estado.txt");
    });
    connect(btnVolver,   &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(MENU_PRINCIPAL); });

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

    // TODO: Reemplazar con sistema.getPrendasNoAlmacenadas()
    QListWidget* lista = crearLista({"[Prendas eliminables se cargaran aqui]"});
    layout->addWidget(lista);

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
        // TODO: sistema.eliminarPrenda(lista->currentItem()->text())
        QMessageBox::information(this, "Eliminado", "Prenda eliminada del sistema.");
        pantallas->setCurrentIndex(MENU_OPCIONES);
    });
    connect(btnVolver, &QPushButton::clicked, this, [this]{ pantallas->setCurrentIndex(MENU_OPCIONES); });

    return w;
}