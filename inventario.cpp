#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

struct Producto {
    int codigo;
    char nombre[31];
    double precio;
    int stock;
    char categoria[21];
    bool activo;
};

const string ARCHIVO = "inventario.dat";

void mostrarMenu();
void agregarProducto();
void mostrarProductosActivos();
bool existeCodigo(int codigo);
void limpiarBuffer();

int main() {
    int opcion;
    do {
        mostrarMenu();
        cin >> opcion;
        cin.ignore();

        switch(opcion) {
            case 1: agregarProducto(); break;
            case 2: mostrarProductosActivos(); break;
            case 9: cout << "Saliendo...\n"; break;
            default: cout << "Opcion invalida\n";
        }
    } while(opcion != 9);

    return 0;
}

void mostrarMenu() {
    cout << "\n===== MENU PRINCIPAL ====="
         << "\n1. Agregar nuevo producto"
         << "\n2. Mostrar productos activos"
         << "\n9. Salir"
         << "\nSeleccione una opcion: ";
}

void limpiarBuffer() {
    cin.clear();
    cin.ignore(10000, '\n');
}

bool existeCodigo(int codigo) {
    ifstream archivo(ARCHIVO, ios::binary);
    Producto p;
    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(p))) {
        if (p.codigo == codigo) return true;
    }
    return false;
}

void agregarProducto() {
    ofstream archivo(ARCHIVO, ios::binary | ios::app);
    if (!archivo) {
        cerr << "Error al abrir el archivo\n";
        return;
    }
    Producto nuevo;
    cout << "\n--- Agregar Producto ---\n";
    do {
        cout << "Codigo (entero positivo): ";
        cin >> nuevo.codigo;
        if (cin.fail() || nuevo.codigo < 1) {
            cout << "Codigo invalido. ";
            limpiarBuffer();
            continue;
        }
        if (existeCodigo(nuevo.codigo)) {
            cout << "Ya existe un producto con ese codigo. Intente otro.\n";
            limpiarBuffer();
            nuevo.codigo = -1;
        }
    } while (nuevo.codigo < 1);

    limpiarBuffer();
    cout << "Nombre (max 30): ";
    cin.getline(nuevo.nombre, 31);
    cout << "Precio: ";
    cin >> nuevo.precio;
    cout << "Stock: ";
    cin >> nuevo.stock;
    limpiarBuffer();
    cout << "Categoria (max 20): ";
    cin.getline(nuevo.categoria, 21);

    nuevo.activo = true;
    archivo.write(reinterpret_cast<char*>(&nuevo), sizeof(nuevo));
    cout << "Producto agregado exitosamente!\n";
}

void mostrarProductosActivos() {
    ifstream archivo(ARCHIVO, ios::binary);
    if (!archivo) {
        cout << "No hay productos registrados\n";
        return;
    }
    cout << "\n--- Productos Activos ---\n";
    Producto p;
    bool hay = false;
    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(p))) {
        if (p.activo) {
            cout << "Codigo: " << p.codigo
                 << " | Nombre: " << p.nombre
                 << " | Precio: " << p.precio
                 << " | Stock: " << p.stock
                 << " | Categoria: " << p.categoria << endl;
            hay = true;
        }
    }
    if (!hay) cout << "No hay productos activos\n";
}