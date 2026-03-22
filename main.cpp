#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class Ropa {
private:
    string nombre;
    string valoresCarac[5];

    // Nombres fijos de las características
    string nombresCarac[5] = {"Tipo de prenda", "Color", "Tipo de tela", "Talla", "Fit"};

public:
    void ingresar() {
        cout << "Nombre de la prenda: ";
        getline(cin, nombre);

        for (int i = 0; i < 5; i++) {
            cout << nombresCarac[i] << " de la prenda: ";
            getline(cin, valoresCarac[i]);
        }
    }

    string serializar() {
        string linea = nombre;

        for (int i = 0; i < 5; i++) {
            linea += "," + valoresCarac[i];
        }

        return linea;
    }

    void deserializar(string linea) {
        stringstream ss(linea);

        getline(ss, nombre, ',');

        for (int i = 0; i < 5; i++) {
            getline(ss, valoresCarac[i], ',');
        }
    }

    void mostrar() {
        cout << "\nNombre: " << nombre << endl;

        for (int i = 0; i < 5; i++) {
            cout << nombresCarac[i] << ": "
                 << (valoresCarac[i].empty() ? "N/A" : valoresCarac[i])
                 << endl;
        }
    }

    bool coincideNombre(string buscado) {
        return nombre == buscado;
    }

    bool coincideCaracteristica(string buscado) {
        for (int i = 0; i < 5; i++) {
            if (valoresCarac[i] == buscado) {
                return true;
            }
        }
        return false;
    }
};

// Guardar
void agregarRopa() {
    ofstream archivo("ropa.txt", ios::app);
    Ropa r;

    cin.ignore();
    r.ingresar();

    archivo << r.serializar() << endl;
    archivo.close();

    cout << "Prenda guardada.\n";
}

// Mostrar
void mostrarRopa() {
    ifstream archivo("ropa.txt");
    string linea;

    while (getline(archivo, linea)) {
        Ropa r;
        r.deserializar(linea);
        r.mostrar();
    }

    archivo.close();
}

// Buscar por nombre
void buscarPorNombre() {
    ifstream archivo("ropa.txt");
    string linea, buscado;

    cout << "Nombre a buscar: ";
    cin.ignore();
    getline(cin, buscado);

    bool encontrado = false;

    while (getline(archivo, linea)) {
        Ropa r;
        r.deserializar(linea);

        if (r.coincideNombre(buscado)) {
            r.mostrar();
            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "No se encontro la prenda.\n";
    }

    archivo.close();
}

// Buscar por característica
void buscarPorCaracteristica() {
    ifstream archivo("ropa.txt");
    string linea, buscado;

    cout << "Valor a buscar (ej: Rojo, S, Nike): ";
    cin.ignore();
    getline(cin, buscado);

    bool encontrado = false;

    while (getline(archivo, linea)) {
        Ropa r;
        r.deserializar(linea);

        if (r.coincideCaracteristica(buscado)) {
            r.mostrar();
            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "No se encontraron coincidencias.\n";
    }

    archivo.close();
}

// Menú
int main() {
    int opcion;

    do {
        cout << "\n--- MENU ---\n";
        cout << "1. Agregar ropa\n";
        cout << "2. Mostrar ropa\n";
        cout << "3. Buscar por nombre\n";
        cout << "4. Buscar por caracteristica\n";
        cout << "5. Salir\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                agregarRopa();
                break;
            case 2:
                mostrarRopa();
                break;
            case 3:
                buscarPorNombre();
                break;
            case 4:
                buscarPorCaracteristica();
                break;
        }

    } while (opcion != 5);

    return 0;
}