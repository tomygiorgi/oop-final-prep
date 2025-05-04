#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <iomanip>  // setw, fixed, setprecision
using namespace std;

// ------------------------------------------------------------
// Estructura binaria utilizada para lectura/escritura
// ------------------------------------------------------------
struct BinProducto {
    int    codigo;
    char   descripcion[200];
    double precio;
    char   tipo;  // 'N' (nacional) o 'I' (importado), 'C' (compuesto) en la parte libre
};

// ------------------------------------------------------------
// Clase base abstracta: Producto
// ------------------------------------------------------------
class Producto {
protected:
    int    codigo;
    string descripcion;
    double precioOriginal;   // precio antes de actualización
    double precioActualizado; // precio tras actualizar

public:
    Producto(int c, const string &d, double p)
        : codigo(c), descripcion(d), precioOriginal(p), precioActualizado(p) {}

    virtual ~Producto() {}

    // Métodos de acceso
    int    getCodigo()       const { return codigo; }
    string getDescripcion()  const { return descripcion; }
    double getPrecioOriginal()     const { return precioOriginal; }
    double getPrecioActual()       const { return precioActualizado; }

    // Método polimórfico para actualizar el precio (dependiendo del tipo)
    virtual void actualizar(double porcentaje,
                            double cotizAnterior,
                            double cotizActual) = 0;

    // Sobrecarga << para imprimir en texto
    // (La usaremos para "Todos los productos ordenados por código")
    friend ostream& operator<<(ostream& os, const Producto& prod) {
        // Ejemplo de formato: "Codigo: 1  Desc: 'Aceite'  PrecioNuevo: 123.45"
        os << "Codigo: " << prod.codigo
           << " | Desc: " << prod.descripcion
           << " | PrecioOriginal: " << prod.precioOriginal
           << " | PrecioActualizado: " << prod.precioActualizado;
        return os;
    }
};

// ------------------------------------------------------------
// ProductoNacional
// ------------------------------------------------------------
class ProductoNacional : public Producto {
public:
    ProductoNacional(int c, const string& d, double p)
        : Producto(c, d, p) {}

    void actualizar(double porcentaje,
                    double /*cotizAnterior*/,
                    double /*cotizActual*/) override {
        // nuevoPrecio = precioOriginal * (1 + porcentaje/100)
        precioActualizado = precioOriginal * (1.0 + porcentaje/100.0);
    }
};

// ------------------------------------------------------------
// ProductoImportado
// ------------------------------------------------------------
class ProductoImportado : public Producto {
public:
    ProductoImportado(int c, const string& d, double p)
        : Producto(c, d, p) {}

    void actualizar(double /*porcentaje*/,
                    double cotizAnterior,
                    double cotizActual) override {
        // nuevoPrecio = (precioOriginal / cotizAnterior) * cotizActual
        if (cotizAnterior <= 0) {
            // Evitar división por cero
            precioActualizado = precioOriginal;
        } else {
            precioActualizado = (precioOriginal / cotizAnterior) * cotizActual;
        }
    }
};

// ------------------------------------------------------------
// (Libre) ProductoCompuesto
// ------------------------------------------------------------
class ProductoCompuesto : public Producto {
private:
    // Este producto contiene un conjunto de otros productos
    // (por simplicidad, usaremos punteros a la base).
    vector<Producto*> componentes;

public:
    // En la binaria original, 'C' implicaría un producto compuesto.
    // La "descripcion" puede ser un nombre general,
    // La "precioOriginal" podría ser la suma de sus componentes. (?)
    // Depende de la lógica que quieras.
    ProductoCompuesto(int c, const string& d, double p)
        : Producto(c, d, p) {}

    ~ProductoCompuesto() {
        for (auto ptr : componentes) {
            delete ptr;
        }
        componentes.clear();
    }

    // Agregar sub-producto
    void addComponente(Producto* p) {
        componentes.push_back(p);
    }

    // Actualizar:
    // - Actualizamos cada subproducto
    // - Luego recalculamos el "precioActualizado" como la suma de subproductos
    void actualizar(double porcentaje,
                    double cotizAnterior,
                    double cotizActual) override {
        for (auto c : componentes) {
            c->actualizar(porcentaje, cotizAnterior, cotizActual);
        }
        // Recalcular el precio final
        double suma = 0;
        for (auto c : componentes) {
            suma += c->getPrecioActual();
        }
        precioActualizado = suma;
    }
};

// ------------------------------------------------------------
// Clase Gestor: maneja la lectura, actualización y consultas
// ------------------------------------------------------------
class Gestor {
private:
    vector<Producto*> productos;
    double porcentajeNac;     // Porcentaje de aumento para nacionales
    double cotizacionAnterior; 
    double cotizacionActual;  

public:
    Gestor(double porc, double cAnt, double cAct)
        : porcentajeNac(porc), cotizacionAnterior(cAnt), cotizacionActual(cAct) {}

    ~Gestor() {
        for (auto p : productos) {
            delete p;
        }
        productos.clear();
    }

    // Leer el binario original y cargar en memoria
    void leerBinario(const string& nombreArchivo) {
        ifstream in(nombreArchivo, ios::binary);
        if (!in.is_open()) {
            cerr << "No se pudo abrir " << nombreArchivo << " para lectura.\n";
            return;
        }
        while (true) {
            BinProducto bp;
            in.read(reinterpret_cast<char*>(&bp), sizeof(BinProducto));
            if (!in.good()) break; // fin o error

            // Crear objeto polimórfico segun 'tipo'
            Producto* prod = nullptr;
            string desc(bp.descripcion); // convertir char[] a string

            if (bp.tipo == 'N') {
                prod = new ProductoNacional(bp.codigo, desc, bp.precio);
            } else if (bp.tipo == 'I') {
                prod = new ProductoImportado(bp.codigo, desc, bp.precio);
            } else if (bp.tipo == 'C') {
                // Parte "libre" -> crear ProductoCompuesto
                prod = new ProductoCompuesto(bp.codigo, desc, bp.precio);
                // Podríamos en un caso real, leer "sub-productos" de alguna forma extra
                // o asumiendo que precioOriginal es la suma. Depende del diseño.
            } else {
                // Si no coincide, lo tratamos como nacional, o ignoramos
                cerr << "Tipo desconocido (" << bp.tipo << "). Se tomará como nacional.\n";
                prod = new ProductoNacional(bp.codigo, desc, bp.precio);
            }
            productos.push_back(prod);
        }
        in.close();
    }
    //----------------------------------------------------
    // Actualizar precios
    void actualizarPrecios() {
        for (auto p : productos) {
            p->actualizar(porcentajeNac, cotizacionAnterior, cotizacionActual);
        }
    }

    // Crear un nuevo archivo binario con los precios actualizados
    void guardarBinario(const string& nombreArchivo) {
        ofstream out(nombreArchivo, ios::binary);
        if (!out.is_open()) {
            cerr << "No se pudo abrir " << nombreArchivo << " para escritura.\n";
            return;
        }
        for (auto p : productos) {
            BinProducto bp;
            bp.codigo = p->getCodigo();
            // Copiamos la descripción
            memset(bp.descripcion, 0, 200);
            auto d = p->getDescripcion();
            strncpy(bp.descripcion, d.c_str(), 199);
            bp.precio = p->getPrecioActual();

            // Determinamos el tipo real (usando dynamic_cast)
            if (dynamic_cast<ProductoNacional*>(p)) {
                bp.tipo = 'N';
            } else if (dynamic_cast<ProductoImportado*>(p)) {
                bp.tipo = 'I';
            } else if (dynamic_cast<ProductoCompuesto*>(p)) {
                bp.tipo = 'C';
            } else {
                bp.tipo = 'N'; // fallback
            }
            out.write(reinterpret_cast<char*>(&bp), sizeof(BinProducto));
        }
        out.close();
        cout << "Archivo binario con precios actualizados: " << nombreArchivo << endl;
    }

    // 1) El o los productos más caros (luego de actualización)
    vector<Producto*> getMasCaros() {
        vector<Producto*> resultado;
        if (productos.empty()) return resultado;
        // Hallar el precio mayor
        double maxPrice = 0;
        for (auto p : productos) {
            double pa = p->getPrecioActual();
            if (pa > maxPrice) {
                maxPrice = pa;
            }
        }
        // Agregar todos los que coinciden con maxPrice
        for (auto p : productos) {
            if (p->getPrecioActual() == maxPrice) {
                resultado.push_back(p);
            }
        }
        return resultado;
    }

    // 2) Todos los productos ordenados por código
    //    (Sobrecarga operator<< en clase base para imprimir)
    vector<Producto*> getOrdenadosPorCodigo() {
        vector<Producto*> v = productos;
        sort(v.begin(), v.end(), [](Producto* a, Producto* b){
            return a->getCodigo() < b->getCodigo();
        });
        return v;
    }

    // 3) El promedio de aumentos de precios
    double getPromedioAumento() {
        if (productos.empty()) return 0.0;
        double suma = 0.0;
        for (auto p : productos) {
            suma += (p->getPrecioActual() - p->getPrecioOriginal());
        }
        return suma / productos.size();
    }

    // Guardar en un archivo de texto:
    //  * Los productos más caros
    //  * Todos los productos (ordenados) => se recorre y se imprime con << 
    //  * El promedio de aumentos
    void guardarConsultas(const string& archivoTxt) {
        ofstream out(archivoTxt);
        if (!out.is_open()) {
            cerr << "No se pudo abrir " << archivoTxt << " para escritura.\n";
            return;
        }

        // (A) Productos más caros
        auto masCaros = getMasCaros();
        out << "=== PRODUCTOS MAS CAROS ===\n";
        for (auto p : masCaros) {
            out << *p << "\n";  // Usa operator<<(ostream&, const Producto&)
        }
        out << "\n";

        // (B) Todos ordenados por código
        out << "=== TODOS LOS PRODUCTOS ORDENADOS POR CODIGO ===\n";
        auto ordenados = getOrdenadosPorCodigo();
        for (auto p : ordenados) {
            out << *p << "\n";
        }
        out << "\n";

        // (C) Promedio de aumentos
        double prom = getPromedioAumento();
        out << "=== PROMEDIO DE AUMENTOS ===\n";
        out << prom << "\n";

        out.close();
        cout << "Consultas guardadas en " << archivoTxt << endl;
    }
};

// ------------------------------------------------------------
// Función principal de ejemplo
// ------------------------------------------------------------
int main() {
    // Pedir datos al usuario:
    // - Porcentaje de aumento para nacionales
    // - Cotización anterior y actual
    double porcentaje, cotizAnt, cotizAct;
    cout << "Ingrese porcentaje de aumento para productos nacionales: ";
    cin >> porcentaje;
    cout << "Ingrese cotizacion anterior para productos importados: ";
    cin >> cotizAnt;
    cout << "Ingrese cotizacion actual para productos importados: ";
    cin >> cotizAct;

    // Creamos gestor
    Gestor g(porcentaje, cotizAnt, cotizAct);

    // 1) Leer archivo binario original
    g.leerBinario("productos.dat");  // <--- asume que existe

    // 2) Actualizar precios
    g.actualizarPrecios();

    // 3) Guardar nuevo binario con precios actualizados
    g.guardarBinario("productos_actualizados.dat");

    // 4) Guardar consultas en un archivo de texto
    g.guardarConsultas("consultas.txt");

    cout << "Proceso completado.\n";
    return 0;
}

/* 
--------------------------------------------------------------------------------
(3) Teoría:

¿Cuándo es necesario utilizar herencia y cómo ayuda a reutilizar código?
- Usamos herencia cuando varios objetos comparten atributos y comportamientos
  comunes. Podemos encapsular estos en una clase base, y que las clases derivadas
  extiendan o personalicen ciertos métodos. De esta manera, reutilizamos el
  código y habilitamos el polimorfismo (llamadas virtuales).
- Por ejemplo, en este ejercicio, un Producto "Nacional" y un Producto "Importado"
  comparten parte de su información (código, descripción, precioOriginal), pero
  tienen una forma distinta de actualizar su precio. La herencia nos ayuda a
  mantener un método abstracto "actualizar(...)" y cada derivada lo implementa
  a su manera.

¿Cuándo no es bueno usar herencia y por qué?
- No es bueno usar herencia cuando la relación "es-un" no está clara o cuando
  la jerarquía forzaría dependencias innecesarias. Si no existe una clara
  necesidad de polimorfismo ni de extensiones, introducir herencia complica
  el diseño y aumenta el acoplamiento entre la clase base y las derivadas.
- También, si la clase base cambia con frecuencia, puede romper a las derivadas.
  En esos casos, a veces es mejor usar composición ("has-a") o interfaces puras.

--------------------------------------------------------------------------------
(Libre) ProductoCompuesto:
- Se añadió la clase "ProductoCompuesto" para ilustrar un producto formado
  por un conjunto de otros productos. Se almacena un vector<Producto*>
  y en el método "actualizar" se delega la actualización a cada sub-producto.
- El "precioActualizado" resulta ser la suma de los precios actualizados
  de sus componentes, lo que requiere un diseño polimórfico para sub-productos.
- Para manejar la lectura/escritura de estos productos compuestos en binario,
  podrían requerirse estructuras o "banderas" especiales que indiquen
  los sub-productos (quedaría a gusto del diseño final, no está 100% implementado).
--------------------------------------------------------------------------------
*/
