#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>


int percheroActual = 1;
int posicionActual = 1;


using namespace std;
//////////////////////////////////////////////////////////////
// Definición de la clase Ropa con 5 características fijas //
//////////////////////////////////////////////////////////////
class Ropa {
private:
    string nombre;
    string valoresCarac[5];

    int perchero; //Posición del perchero (1-3)
    int posicion; //Posición dentro del perchero (1-5)

    // Nombres fijos de las características
    string nombresCarac[5] = {"Tipo", "Color", "Tipo de tela", "Talla", "Fit"};

public:
    void ingresar() {
        cout << "Nombre de la prenda: ";
        getline(cin, nombre);

        for (int i = 0; i < 5; i++) {
            cout << nombresCarac[i] << " de la prenda: ";
            getline(cin, valoresCarac[i]);
        }
    }

    void setUbicacion(int p,int pos){
        perchero = p;
        posicion = pos;
    }

    string serializar() {
        string linea = nombre;

        for (int i = 0; i < 5; i++) {
            linea += "," + valoresCarac[i];
        }
        linea += "," + to_string(perchero);
        linea += "," + to_string(posicion);

        return linea;
    }

    void deserializar(string linea){

        stringstream ss(linea);

        getline(ss,nombre,',');

        for(int i=0;i<5;i++)
            getline(ss,valoresCarac[i],',');

        ss >> perchero;
        ss.ignore();
        ss >> posicion;
    }
    void mostrar() {
        cout << "\nNombre: " << nombre << endl;

        for (int i = 0; i < 5; i++) {
            cout << nombresCarac[i] << ": "
                 << (valoresCarac[i].empty() ? "N/A" : valoresCarac[i])
                 << endl;
        }
        cout<<"Perchero: "<<perchero<<endl;
        cout<<"Posicion: "<<posicion<<endl;
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

    int getPerchero(){
    return perchero;
    }

    int getPosicion(){
        return posicion;
    }
};

//////////////////////////////////////////////////////////////
//            Función buscar un espacio libre               //
//////////////////////////////////////////////////////////////
bool encontrarPosicionLibre(int &perchero, int &posicion) {

    bool ocupado[3][5] = {false};

    ifstream archivo("ropa.txt");
    string linea;

    while (getline(archivo, linea)) {

        stringstream ss(linea);
        string temp;

        for(int i=0;i<6;i++)
            getline(ss,temp,',');

        int p, pos;
        ss >> p;
        ss.ignore();
        ss >> pos;

        ocupado[p-1][pos-1] = true;
    }

    archivo.close();

    for(int i=0;i<3;i++){
        for(int j=0;j<5;j++){
            if(!ocupado[i][j]){
                perchero = i+1;
                posicion = j+1;
                return true;
            }
        }
    }

    return false;
}

//////////////////////////////////////////////////////////////
//              Función agregar ropa                        //
//////////////////////////////////////////////////////////////
void agregarRopa(){

    int perchero,posicion;

    if(!encontrarPosicionLibre(perchero,posicion)){
        cout<<"\nNo es posible añadir otra prenda ya que el armario esta lleno\n";
        return;
    }

    Ropa r;

    cin.ignore();
    r.ingresar();

    r.setUbicacion(perchero,posicion);

    ofstream archivo("ropa.txt",ios::app);

    archivo<<r.serializar()<<endl;

    archivo.close();

    cout<<"\nPrenda guardada en:\n";
    cout<<"Perchero "<<perchero<<" - Posicion "<<posicion<<endl;

}

//////////////////////////////////////////////////////////////
//           Función mostrar toda la ropa                   //
//////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////
//              Función control de motores                  //
//////////////////////////////////////////////////////////////

void moverArmario(int percheroDestino, int posicionDestino) {

    cout << "\nMovimiento del armario:\n";

    cout << "(" << percheroActual << "," << posicionActual << ")" << endl;

    // Si cambia de perchero
    if (percheroActual != percheroDestino) {

        // volver a posición 1
        while (posicionActual > 1) {
            posicionActual--;
            cout << "(" << percheroActual << "," << posicionActual << ")" << endl;
            this_thread::sleep_for(chrono::milliseconds(300));
        }

        // cambiar perchero
        while (percheroActual < percheroDestino) {
            percheroActual++;
            cout << "(" << percheroActual << "," << posicionActual << ")" << endl;
            this_thread::sleep_for(chrono::milliseconds(300));
        }

        while (percheroActual > percheroDestino) {
            percheroActual--;
            cout << "(" << percheroActual << "," << posicionActual << ")" << endl;
        }
    }

    // mover posición
    while (posicionActual < posicionDestino) {
        posicionActual++;
        cout << "(" << percheroActual << "," << posicionActual << ")" << endl;
    }

    while (posicionActual > posicionDestino) {
        posicionActual--;
        cout << "(" << percheroActual << "," << posicionActual << ")" << endl;
    }
}

//////////////////////////////////////////////////////////////
//              Función buscar por nombre                   //
//////////////////////////////////////////////////////////////
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
            int perchero = r.getPerchero();
            int posicion = r.getPosicion();

            moverArmario(perchero,posicion);

            r.mostrar();
            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "No se encontro la prenda.\n";
    }

    archivo.close();
}

//////////////////////////////////////////////////////////////
//            Función buscar por característica             //
//////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////
//                        Menú                              //
//////////////////////////////////////////////////////////////
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
    cout << "\nRegresando armario a posicion inicial (1,1)\n";

    moverArmario(1,1);

    cout << "Sistema en posicion inicial.\n";
    cout << "Saliendo del programa...\n";
    return 0;
}