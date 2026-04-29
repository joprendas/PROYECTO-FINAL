#include "main_logic.h"

#include <QFile>
#include <QTextStream>

bool Ropa::estaAlmacenada() const {
    return perchero >= 0 && slot >= 0;
}

QString Ropa::serializar() const {
    // Formato:
    // nombre,tipo,color,tela,talla,fit,perchero,slot
    // Nota: se evita el uso de comas dentro de los campos.
    return QStringList{
        nombre,
        tipo,
        color,
        tela,
        talla,
        fit,
        QString::number(perchero),
        QString::number(slot)
    }.join(',');
}

QString Ropa::descripcion() const {
    QString ubicacion = estaAlmacenada()
        ? QString("Perchero %1 - Slot %2").arg(perchero + 1).arg(slot + 1)
        : "No almacenada";

    return QString("%1\nTipo: %2\nColor: %3\nTela: %4\nTalla: %5\nFit: %6\nUbicacion: %7")
        .arg(nombre, tipo, color, tela, talla, fit, ubicacion);
}

QString Ropa::descripcionCorta() const {
    QString ubicacion = estaAlmacenada()
        ? QString("P%1-S%2").arg(perchero + 1).arg(slot + 1)
        : "Sin perchero";

    return QString("%1 | %2, %3, %4, %5, %6 | %7")
        .arg(nombre, tipo, color, tela, talla, fit, ubicacion);
}

Ropa Ropa::deserializar(const QString& linea) {
    QStringList partes = linea.split(',');

    Ropa r;
    if (partes.size() > 0) r.nombre = partes.value(0).trimmed();
    if (partes.size() > 1) r.tipo   = partes.value(1).trimmed();
    if (partes.size() > 2) r.color  = partes.value(2).trimmed();
    if (partes.size() > 3) r.tela   = partes.value(3).trimmed();
    if (partes.size() > 4) r.talla  = partes.value(4).trimmed();
    if (partes.size() > 5) r.fit    = partes.value(5).trimmed();

    // Compatibilidad con el main_logic original:
    // si el archivo viejo solo tenia 6 campos, se carga como prenda conocida no almacenada.
    r.perchero = partes.size() > 6 ? partes.value(6).toInt() : -1;
    r.slot     = partes.size() > 7 ? partes.value(7).toInt() : -1;

    return r;
}

SistemaRopa::SistemaRopa(const QString& archivoDatos)
    : archivoDatos(archivoDatos) {
    cargar();
}

bool SistemaRopa::cargar(QString* error) {
    return cargarDesde(archivoDatos, error);
}

bool SistemaRopa::guardar(QString* error) const {
    return guardarComo(archivoDatos, error);
}

bool SistemaRopa::guardarComo(const QString& ruta, QString* error) const {
    QFile archivo(ruta);
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        if (error) *error = "No se pudo abrir el archivo para guardar: " + ruta;
        return false;
    }

    QTextStream out(&archivo);
    for (const Ropa& r : prendas) {
        out << r.serializar() << '\n';
    }

    return true;
}

bool SistemaRopa::cargarDesde(const QString& ruta, QString* error) {
    QFile archivo(ruta);
    if (!archivo.exists()) {
        if (ruta == archivoDatos) {
            prendas.clear();
            return true;
        }
        if (error) *error = "No existe el archivo: " + ruta;
        return false;
    }

    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (error) *error = "No se pudo abrir el archivo para cargar: " + ruta;
        return false;
    }

    QVector<Ropa> cargadas;
    QTextStream in(&archivo);
    while (!in.atEnd()) {
        QString linea = in.readLine().trimmed();
        if (linea.isEmpty()) continue;

        Ropa r = Ropa::deserializar(linea);
        if (!r.nombre.isEmpty()) {
            cargadas.append(r);
        }
    }

    prendas = cargadas;
    return true;
}

bool SistemaRopa::agregarRopa(const QString& nombre,
                              const QString& tipo,
                              const QString& color,
                              const QString& tela,
                              const QString& talla,
                              const QString& fit,
                              int perchero,
                              QString* error) {
    QString nombreLimpio = limpiarCampoCsv(nombre.trimmed());

    if (nombreLimpio.isEmpty()) {
        if (error) *error = "El nombre de la prenda no puede estar vacio.";
        return false;
    }

    if (indicePorNombre(nombreLimpio) != -1) {
        if (error) *error = "Ya existe una prenda con ese nombre.";
        return false;
    }

    int slotLibre = primerSlotLibre(perchero);
    if (slotLibre == -1) {
        if (error) *error = QString("El perchero %1 no tiene espacios libres.").arg(perchero + 1);
        return false;
    }

    Ropa r;
    r.nombre = nombreLimpio;
    r.tipo = limpiarCampoCsv(tipo);
    r.color = limpiarCampoCsv(color);
    r.tela = limpiarCampoCsv(tela);
    r.talla = limpiarCampoCsv(talla);
    r.fit = limpiarCampoCsv(fit);
    r.perchero = perchero;
    r.slot = slotLibre;

    prendas.append(r);
    return guardar(error);
}

bool SistemaRopa::almacenarPrevia(const QString& nombre, int perchero, QString* error) {
    int idx = indicePorNombre(nombre);
    if (idx == -1) {
        if (error) *error = "No se encontro la prenda seleccionada.";
        return false;
    }

    if (prendas[idx].estaAlmacenada()) {
        if (error) *error = "La prenda ya esta almacenada en un perchero.";
        return false;
    }

    int slotLibre = primerSlotLibre(perchero);
    if (slotLibre == -1) {
        if (error) *error = QString("El perchero %1 no tiene espacios libres.").arg(perchero + 1);
        return false;
    }

    prendas[idx].perchero = perchero;
    prendas[idx].slot = slotLibre;
    return guardar(error);
}

bool SistemaRopa::extraerPorNombre(const QString& nombre, QString* error) {
    int idx = indicePorNombre(nombre);
    if (idx == -1) {
        if (error) *error = "No se encontro la prenda seleccionada.";
        return false;
    }

    if (!prendas[idx].estaAlmacenada()) {
        if (error) *error = "La prenda seleccionada no esta almacenada actualmente.";
        return false;
    }

    prendas[idx].perchero = -1;
    prendas[idx].slot = -1;
    return guardar(error);
}

bool SistemaRopa::eliminarPrenda(const QString& nombre, QString* error) {
    int idx = indicePorNombre(nombre);
    if (idx == -1) {
        if (error) *error = "No se encontro la prenda seleccionada.";
        return false;
    }

    if (prendas[idx].estaAlmacenada()) {
        if (error) *error = "No se puede eliminar una prenda que esta en un perchero.";
        return false;
    }

    prendas.remove(idx);
    return guardar(error);
}

QStringList SistemaRopa::nombresAlmacenadas() const {
    QStringList lista;
    for (const Ropa& r : prendas) {
        if (r.estaAlmacenada()) lista << r.nombre;
    }
    lista.sort(Qt::CaseInsensitive);
    return lista;
}

QStringList SistemaRopa::nombresNoAlmacenadas() const {
    QStringList lista;
    for (const Ropa& r : prendas) {
        if (!r.estaAlmacenada()) lista << r.nombre;
    }
    lista.sort(Qt::CaseInsensitive);
    return lista;
}

QStringList SistemaRopa::nombresTodas() const {
    QStringList lista;
    for (const Ropa& r : prendas) lista << r.nombre;
    lista.sort(Qt::CaseInsensitive);
    return lista;
}

QVector<Ropa> SistemaRopa::buscarPorNombre(const QString& nombre, bool soloAlmacenadas) const {
    QVector<Ropa> resultados;
    QString buscado = normalizar(nombre);

    for (const Ropa& r : prendas) {
        if (soloAlmacenadas && !r.estaAlmacenada()) continue;
        if (normalizar(r.nombre).contains(buscado)) resultados.append(r);
    }

    return resultados;
}

QVector<Ropa> SistemaRopa::buscarPorCaracteristica(const QString& valor, bool soloAlmacenadas) const {
    QVector<Ropa> resultados;
    QString buscado = normalizar(valor);

    if (buscado.isEmpty()) return resultados;

    for (const Ropa& r : prendas) {
        if (soloAlmacenadas && !r.estaAlmacenada()) continue;

        if (normalizar(r.tipo).contains(buscado)  ||
            normalizar(r.color).contains(buscado) ||
            normalizar(r.tela).contains(buscado)  ||
            normalizar(r.talla).contains(buscado) ||
            normalizar(r.fit).contains(buscado)) {
            resultados.append(r);
        }
    }

    return resultados;
}

QVector<Ropa> SistemaRopa::filtrarPrendas(const QString& tipo,
                                          const QString& color,
                                          const QString& tela,
                                          const QString& talla,
                                          const QString& fit,
                                          bool soloAlmacenadas) const {
    QVector<Ropa> resultados;

    for (const Ropa& r : prendas) {
        if (soloAlmacenadas && !r.estaAlmacenada()) continue;

        bool coincide = true;
        if (!tipo.isEmpty()  && normalizar(r.tipo)  != normalizar(tipo))  coincide = false;
        if (!color.isEmpty() && normalizar(r.color) != normalizar(color)) coincide = false;
        if (!tela.isEmpty()  && normalizar(r.tela)  != normalizar(tela))  coincide = false;
        if (!talla.isEmpty() && normalizar(r.talla) != normalizar(talla)) coincide = false;
        if (!fit.isEmpty()   && normalizar(r.fit)   != normalizar(fit))   coincide = false;

        if (coincide) resultados.append(r);
    }

    return resultados;
}

QString SistemaRopa::prendaEnSlot(int perchero, int slot) const {
    for (const Ropa& r : prendas) {
        if (r.perchero == perchero && r.slot == slot) {
            return r.nombre;
        }
    }
    return "[vacio]";
}

bool SistemaRopa::generarInformeTxt(const QString& ruta, QString* error) const {
    QFile archivo(ruta);
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        if (error) *error = "No se pudo generar el informe: " + ruta;
        return false;
    }

    QTextStream out(&archivo);
    out << "INFORME DEL SISTEMA DE PERCHEROS\n";
    out << "================================\n\n";

    out << "PRENDAS ACTUALES EN PERCHEROS\n";
    for (int p = 0; p < 3; ++p) {
        out << "\nPerchero " << (p + 1) << "\n";
        for (int s = 0; s < 5; ++s) {
            out << "  Slot " << (s + 1) << ": " << prendaEnSlot(p, s) << "\n";
        }
    }

    out << "\n\nPRENDAS CONOCIDAS NO ALMACENADAS\n";
    QStringList noAlmacenadas = nombresNoAlmacenadas();
    if (noAlmacenadas.isEmpty()) {
        out << "No hay prendas conocidas fuera de los percheros.\n";
    } else {
        for (const QString& nombre : noAlmacenadas) {
            out << "- " << nombre << "\n";
        }
    }

    out << "\n\nDETALLE COMPLETO\n";
    for (const Ropa& r : prendas) {
        out << "\n" << r.descripcion() << "\n";
    }

    return true;
}

int SistemaRopa::indicePorNombre(const QString& nombre) const {
    QString buscado = normalizar(nombre);
    for (int i = 0; i < prendas.size(); ++i) {
        if (normalizar(prendas[i].nombre) == buscado) return i;
    }
    return -1;
}

int SistemaRopa::primerSlotLibre(int perchero) const {
    if (perchero < 0 || perchero >= 3) return -1;

    for (int slot = 0; slot < 5; ++slot) {
        if (!slotOcupado(perchero, slot)) return slot;
    }
    return -1;
}

bool SistemaRopa::slotOcupado(int perchero, int slot) const {
    for (const Ropa& r : prendas) {
        if (r.perchero == perchero && r.slot == slot) return true;
    }
    return false;
}

QString SistemaRopa::normalizar(const QString& texto) {
    return texto.trimmed().toLower();
}

QString SistemaRopa::limpiarCampoCsv(const QString& campo) {
    QString limpio = campo.trimmed();
    limpio.replace(',', ' ');
    return limpio;
}
