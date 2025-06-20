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
void mostrarPorCategoria();
void buscarPorCodigo();
void modificarProducto();
void eliminarProducto();
bool existeCodigo(int codigo);
bool entradaValida(double valor);
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
            case 3: mostrarPorCategoria(); break;
            case 4: buscarPorCodigo(); break;
            case 5: modificarProducto(); break;
            case 6: eliminarProducto(); break;
            case 9: cout << "Exiting...\n"; break;
            default: cout << "Invalid option\n";
        }
    } while(opcion != 9);

    return 0;
}

void mostrarMenu() {
    cout << "\n===== MAIN MENU ====="
         << "\n1. Add new product"
         << "\n2. Show active products"
         << "\n3. Show products by category"
         << "\n4. Search product by code"
         << "\n5. Modify product"
         << "\n6. Delete product (logical)"
         << "\n9. Exit"
         << "\nSelect an option: ";
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

bool entradaValida(double valor) {
    return !cin.fail() && valor >= 0;
}

void agregarProducto() {
    ofstream archivo(ARCHIVO, ios::binary | ios::app);
    if (!archivo) {
        cerr << "Error opening file\n";
        return;
    }
    Producto nuevo;
    cout << "\n--- Add Product ---\n";
    do {
        cout << "Code (positive integer): ";
        cin >> nuevo.codigo;
        if (cin.fail() || nuevo.codigo < 1) {
            cout << "Invalid code. ";
            limpiarBuffer();
            continue;
        }
        if (existeCodigo(nuevo.codigo)) {
            cout << "A product with this code already exists. Try another.\n";
            limpiarBuffer();
            nuevo.codigo = -1;
        }
    } while (nuevo.codigo < 1);

    limpiarBuffer();
    cout << "Name (max 30): ";
    cin.getline(nuevo.nombre, 31);
    while(strlen(nuevo.nombre) == 0) {
        cout << "Name cannot be empty: ";
        cin.getline(nuevo.nombre, 31);
    }

    cout << "Price: ";
    cin >> nuevo.precio;
    while (!entradaValida(nuevo.precio)) {
        cout << "Invalid price. Try again: ";
        limpiarBuffer();
        cin >> nuevo.precio;
    }

    cout << "Stock: ";
    cin >> nuevo.stock;
    while (cin.fail() || nuevo.stock < 0) {
        cout << "Invalid stock. Try again: ";
        limpiarBuffer();
        cin >> nuevo.stock;
    }
    limpiarBuffer();

    cout << "Category (max 20): ";
    cin.getline(nuevo.categoria, 21);
    while(strlen(nuevo.categoria) == 0) {
        cout << "Category cannot be empty: ";
        cin.getline(nuevo.categoria, 21);
    }

    nuevo.activo = true;
    archivo.write(reinterpret_cast<char*>(&nuevo), sizeof(nuevo));
    cout << "Product added successfully!\n";
}

void mostrarProductosActivos() {
    ifstream archivo(ARCHIVO, ios::binary);
    if (!archivo) {
        cout << "No products registered\n";
        return;
    }
    cout << "\n--- Active Products ---\n";
    Producto p;
    bool hay = false;
    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(p))) {
        if (p.activo) {
            cout << "Code: " << p.codigo
                 << " | Name: " << p.nombre
                 << " | Price: " << p.precio
                 << " | Stock: " << p.stock
                 << " | Category: " << p.categoria << endl;
            hay = true;
        }
    }
    if (!hay) cout << "No active products\n";
}

void mostrarPorCategoria() {
    ifstream archivo(ARCHIVO, ios::binary);
    if (!archivo) {
        cout << "No products registered\n";
        return;
    }
    char cat[21];
    cout << "Enter category to search (max 20): ";
    cin.getline(cat, 21);

    cout << "\n--- Products in '" << cat << "' ---\n";
    Producto p;
    bool found = false;

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(p))) {
        if (p.activo && strcmp(p.categoria, cat) == 0) {
            cout << "Code: " << p.codigo
                 << " | Name: " << p.nombre
                 << " | Price: " << p.precio
                 << " | Stock: " << p.stock << endl;
            found = true;
        }
    }

    if (!found) cout << "No products found\n";
}

void buscarPorCodigo() {
    ifstream archivo(ARCHIVO, ios::binary);
    if (!archivo) {
        cout << "No products registered\n";
        return;
    }
    int cod;
    cout << "Enter code to search: ";
    cin >> cod;
    limpiarBuffer();

    Producto p;
    bool found = false;

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(p))) {
        if (p.codigo == cod) {
            cout << "\n--- Product Found ---"
                 << "\nCode: " << p.codigo
                 << "\nName: " << p.nombre
                 << "\nPrice: " << p.precio
                 << "\nStock: " << p.stock
                 << "\nCategory: " << p.categoria
                 << "\nStatus: " << (p.activo ? "Active" : "Inactive") << endl;
            found = true;
            break;
        }
    }

    if (!found) cout << "Product not found\n";
}

void modificarProducto() {
    fstream archivo(ARCHIVO, ios::binary | ios::in | ios::out);
    if (!archivo) {
        cout << "No products registered\n";
        return;
    }
    int cod;
    cout << "Enter code of the product to modify: ";
    cin >> cod;
    limpiarBuffer();

    Producto p;
    bool found = false;
    streampos pos;

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(p))) {
        if (p.codigo == cod) {
            pos = archivo.tellg() - static_cast<streampos>(sizeof(p));
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Product not found\n";
        return;
    }

    cout << "New price: ";
    cin >> p.precio;
    while (!entradaValida(p.precio)) {
        cout << "Invalid price. Try again: ";
        limpiarBuffer();
        cin >> p.precio;
    }

    cout << "New stock: ";
    cin >> p.stock;
    while (cin.fail() || p.stock < 0) {
        cout << "Invalid stock. Try again: ";
        limpiarBuffer();
        cin >> p.stock;
    }
    limpiarBuffer();

    cout << "New category (max 20): ";
    cin.getline(p.categoria, 21);
    while(strlen(p.categoria) == 0) {
        cout << "Category cannot be empty: ";
        cin.getline(p.categoria, 21);
    }

    archivo.seekp(pos);
    archivo.write(reinterpret_cast<char*>(&p), sizeof(p));
    cout << "Product modified successfully!\n";
}

void eliminarProducto() {
    fstream archivo(ARCHIVO, ios::binary | ios::in | ios::out);
    if (!archivo) {
        cout << "No products registered\n";
        return;
    }
    int cod;
    cout << "Enter code of the product to delete: ";
    cin >> cod;
    limpiarBuffer();

    Producto p;
    bool found = false;
    streampos pos;

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(p))) {
        if (p.codigo == cod && p.activo) {
            pos = archivo.tellg() - static_cast<streampos>(sizeof(p));
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Product not found or already inactive\n";
        return;
    }

    p.activo = false;
    archivo.seekp(pos);
    archivo.write(reinterpret_cast<char*>(&p), sizeof(p));
    cout << "Product set as inactive!\n";
}