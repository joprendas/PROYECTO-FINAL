#ifndef MAIN_LOGIC_H
#define MAIN_LOGIC_H

#include <QString>
#include <QStringList>
#include <QVector>

struct Ropa {
    QString nombre;
    QString tipo;
    QString color;
    QString tela;
    QString talla;
    QString fit;
    int perchero = -1; // -1 significa que no esta almacenada
    int slot = -1;     // -1 significa que no esta almacenada

    bool estaAlmacenada() const;
    QString serializar() const;
    QString descripcion() const;
    QString descripcionCorta() const;

    static Ropa deserializar(const QString& linea);
};

class SistemaRopa {
public:
    explicit SistemaRopa(const QString& archivoDatos = "ropa.txt");

    bool cargar(QString* error = nullptr);
    bool guardar(QString* error = nullptr) const;
    bool guardarComo(const QString& ruta, QString* error = nullptr) const;
    bool cargarDesde(const QString& ruta, QString* error = nullptr);

    bool agregarRopa(const QString& nombre,
                     const QString& tipo,
                     const QString& color,
                     const QString& tela,
                     const QString& talla,
                     const QString& fit,
                     int perchero,
                     QString* error = nullptr);

    bool almacenarPrevia(const QString& nombre, int perchero, QString* error = nullptr);
    bool extraerPorNombre(const QString& nombre, QString* error = nullptr);
    bool eliminarPrenda(const QString& nombre, QString* error = nullptr);

    QStringList nombresAlmacenadas() const;
    QStringList nombresNoAlmacenadas() const;
    QStringList nombresTodas() const;

    QVector<Ropa> buscarPorNombre(const QString& nombre, bool soloAlmacenadas = false) const;
    QVector<Ropa> buscarPorCaracteristica(const QString& valor, bool soloAlmacenadas = false) const;
    QVector<Ropa> filtrarPrendas(const QString& tipo,
                                 const QString& color,
                                 const QString& tela,
                                 const QString& talla,
                                 const QString& fit,
                                 bool soloAlmacenadas = false) const;

    QString prendaEnSlot(int perchero, int slot) const;
    bool generarInformeTxt(const QString& ruta, QString* error = nullptr) const;

private:
    QVector<Ropa> prendas;
    QString archivoDatos;

    int indicePorNombre(const QString& nombre) const;
    int primerSlotLibre(int perchero) const;
    bool slotOcupado(int perchero, int slot) const;

    static QString normalizar(const QString& texto);
    static QString limpiarCampoCsv(const QString& campo);
};

#endif // MAIN_LOGIC_H
