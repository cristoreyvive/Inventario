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
void seleccionarCategoria(char* destino);

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
         << "\n3. Mostrar productos por categoria"
         << "\n4. Buscar producto por codigo"
         << "\n5. Modificar producto"
         << "\n6. Eliminar producto (logico)"
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

bool entradaValida(double valor) {
    return !cin.fail() && valor >= 0;
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
            cout << "Ya existe un producto con este codigo. Intente con otro.\n";
            limpiarBuffer();
            nuevo.codigo = -1;
        }
    } while (nuevo.codigo < 1);
    limpiarBuffer();
    cout << "Nombre (max 30): ";
    cin.getline(nuevo.nombre, 31);
    while(strlen(nuevo.nombre) == 0) {
        cout << "El nombre no puede estar vacio: ";
        cin.getline(nuevo.nombre, 31);
    }
    cout << "Precio: ";
    cin >> nuevo.precio;
    while (!entradaValida(nuevo.precio)) {
        cout << "Precio invalido. Intente de nuevo: ";
        limpiarBuffer();
        cin >> nuevo.precio;
    }
    cout << "Stock: ";
    cin >> nuevo.stock;
    while (cin.fail() || nuevo.stock < 0) {
        cout << "Stock invalido. Intente de nuevo: ";
        limpiarBuffer();
        cin >> nuevo.stock;
    }
    limpiarBuffer();
    seleccionarCategoria(nuevo.categoria);
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

void mostrarPorCategoria() {
    ifstream archivo(ARCHIVO, ios::binary);
    if (!archivo) {
        cout << "No hay productos registrados\n";
        return;
    }
    char cat[21];
    cout << "Ingrese la categoria a buscar (max 20): ";
    cin.getline(cat, 21);
    cout << "\n--- Productos en '" << cat << "' ---\n";
    Producto p;
    bool found = false;
    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(p))) {
        if (p.activo && strcmp(p.categoria, cat) == 0) {
            cout << "Codigo: " << p.codigo
                 << " | Nombre: " << p.nombre
                 << " | Precio: " << p.precio
                 << " | Stock: " << p.stock << endl;
            found = true;
        }
    }
    if (!found) cout << "No se encontraron productos\n";
}

void buscarPorCodigo() {
    ifstream archivo(ARCHIVO, ios::binary);
    if (!archivo) {
        cout << "No hay productos registrados\n";
        return;
    }
    int cod;
    cout << "Ingrese el codigo a buscar: ";
    cin >> cod;
    limpiarBuffer();
    Producto p;
    bool found = false;
    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(p))) {
        if (p.codigo == cod) {
            cout << "\n--- Producto Encontrado ---"
                 << "\nCodigo: " << p.codigo
                 << "\nNombre: " << p.nombre
                 << "\nPrecio: " << p.precio
                 << "\nStock: " << p.stock
                 << "\nCategoria: " << p.categoria
                 << "\nEstado: " << (p.activo ? "Activo" : "Inactivo") << endl;
            found = true;
            break;
        }
    }
    if (!found) cout << "Producto no encontrado\n";
}

void mostrarInfoProducto(const Producto& p) {
    cout << "\n--- Producto Encontrado ---"
         << "\nCodigo: " << p.codigo
         << "\nNombre: " << p.nombre
         << "\nPrecio: " << p.precio
         << "\nStock: " << p.stock
         << "\nCategoria: " << p.categoria
         << "\nEstado: " << (p.activo ? "Activo" : "Inactivo") << endl;
}

void modificarProducto() {
    fstream archivo(ARCHIVO, ios::binary | ios::in | ios::out);
    if (!archivo) {
        cout << "No hay productos registrados\n";
        return;
    }
    int cod;
    cout << "Ingrese el codigo del producto a modificar: ";
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
        cout << "Producto no encontrado\n";
        return;
    }
    mostrarInfoProducto(p);
    int opcion;
    do {
        cout << "\nQue desea modificar?\n";
        cout << "1. Precio\n2. Stock\n3. Categoria\n4. Nombre\n5. Salir\nOpcion: ";
        cin >> opcion;
        limpiarBuffer();
        switch(opcion) {
            case 1:
                cout << "Nuevo precio: ";
                cin >> p.precio;
                while (!entradaValida(p.precio)) {
                    cout << "Precio invalido. Intente de nuevo: ";
                    limpiarBuffer();
                    cin >> p.precio;
                }
                limpiarBuffer();
                break;
            case 2:
                cout << "Nuevo stock: ";
                cin >> p.stock;
                while (cin.fail() || p.stock < 0) {
                    cout << "Stock invalido. Intente de nuevo: ";
                    limpiarBuffer();
                    cin >> p.stock;
                }
                limpiarBuffer();
                break;
            case 3:
                seleccionarCategoria(p.categoria);
                break;
            case 4:
                cout << "Nuevo nombre (max 30): ";
                cin.getline(p.nombre, 31);
                while(strlen(p.nombre) == 0) {
                    cout << "El nombre no puede estar vacio: ";
                    cin.getline(p.nombre, 31);
                }
                break;
            case 5:
                break;
            default:
                cout << "Opcion invalida\n";
        }
    } while(opcion != 5);
    archivo.seekp(pos);
    archivo.write(reinterpret_cast<char*>(&p), sizeof(p));
    cout << "Producto modificado exitosamente!\n";
}

void eliminarProducto() {
    fstream archivo(ARCHIVO, ios::binary | ios::in | ios::out);
    if (!archivo) {
        cout << "No hay productos registrados\n";
        return;
    }
    int cod;
    cout << "Ingrese el codigo del producto a eliminar: ";
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
        cout << "Producto no encontrado o ya inactivo\n";
        return;
    }
    p.activo = false;
    archivo.seekp(pos);
    archivo.write(reinterpret_cast<char*>(&p), sizeof(p));
    cout << "Producto dado de baja!\n";
}

int obtenerCategorias(char categorias[][21], int maxCat = 10) {
    int count = 0;
    ifstream archivo(ARCHIVO, ios::binary);
    Producto p;
    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(p))) {
        if (p.activo) {
            bool existe = false;
            for (int i = 0; i < count; ++i) {
                if (strcmp(categorias[i], p.categoria) == 0) {
                    existe = true;
                    break;
                }
            }
            if (!existe && strlen(p.categoria) > 0) {
                strncpy(categorias[count], p.categoria, 20);
                categorias[count][20] = '\0';
                ++count;
                if (count >= maxCat) break;
            }
        }
    }
    return count;
}

void seleccionarCategoria(char* destino) {
    char categorias[10][21];
    int n = obtenerCategorias(categorias);
    cout << "Seleccione una categoria:\n";
    for (int i = 0; i < n; ++i) {
        cout << (i+1) << ". " << categorias[i] << '\n';
    }
    cout << (n+1) << ". Nueva categoria\n";
    cout << "Opcion: ";
    int opcion;
    cin >> opcion;
    limpiarBuffer();
    if (opcion >= 1 && opcion <= n) {
        strncpy(destino, categorias[opcion-1], 20);
        destino[20] = '\0';
    } else if (opcion == n+1) {
        cout << "Ingrese la nueva categoria (max 20): ";
        cin.getline(destino, 21);
        while(strlen(destino) == 0) {
            cout << "La categoria no puede estar vacia: ";
            cin.getline(destino, 21);
        }
    } else {
        cout << "Opcion invalida. Intente de nuevo.\n";
        seleccionarCategoria(destino);
    }
}