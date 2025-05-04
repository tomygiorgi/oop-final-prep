#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>
#include <fstream>
using namespace std;

// Estructura para guardar usuarios en binario
struct datosUsuario {
    char nombre[200];
    int codigo;
    char cargo[50];
};

// Clase Cargo
class Cargo {
private:
    string nombre;
    vector<string> permisos;
    Cargo* cargoPadre;  // Cargo del que hereda permisos (puede ser nullptr)

public:
    Cargo(string n, vector<string> p, Cargo* padre = nullptr)
        : nombre(n), permisos(p), cargoPadre(padre) {}

    string getNombre() const { return nombre; }

    vector<string> getPermisos() const {
        vector<string> todosPermisos = permisos;
        if (cargoPadre) {
            vector<string> permisosPadre = cargoPadre->getPermisos();
            todosPermisos.insert(todosPermisos.end(), permisosPadre.begin(), permisosPadre.end());
        }
        return todosPermisos;
    }

    bool tienePermiso(const string& permiso) const {
        vector<string> todosPermisos = getPermisos();
        return find(todosPermisos.begin(), todosPermisos.end(), permiso) != todosPermisos.end();
    }
};

// Clase Usuario
class Usuario {
private:
    string nombre;
    int codigo;
    Cargo* cargo;  // Relación con un cargo

public:
    Usuario(string n, int c, Cargo* car) : nombre(n), codigo(c), cargo(car) {}

    string getNombre() const { return nombre; }
    int getCodigo() const { return codigo; }
    Cargo* getCargo() const { return cargo; }

    void guardarUsuario(const char* archivo) {
        datosUsuario datos;
        strcpy(datos.nombre, nombre.c_str());
        datos.codigo = codigo;
        strcpy(datos.cargo, cargo->getNombre().c_str());

        ofstream archivoBin(archivo, ios::binary | ios::app);
        if (archivoBin.is_open()) {
            archivoBin.write(reinterpret_cast<char*>(&datos), sizeof(datosUsuario));
            archivoBin.close();
        }
    }
};

// Clase Sistema
class Sistema {
private:
    vector<Usuario*> usuarios;
    map<string, Cargo*> cargos;  // Asociar el nombre del cargo con su instancia

public:
    void agregarCargo(Cargo* cargo) {
        cargos[cargo->getNombre()] = cargo;
    }

    void agregarUsuario(Usuario* usuario) {
        usuarios.push_back(usuario);
    }

    bool verificarPermiso(int codigo, const string& permiso) {
        for (const auto& user : usuarios) {
            if (user->getCodigo() == codigo) {
                return user->getCargo()->tienePermiso(permiso);
            }
        }
        return false;
    }

    vector<string> cargosConMasPermisos() {
        size_t maxPermisos = 0;
        vector<string> resultado;

        for (const auto& pair : cargos) {
            size_t cantidadPermisos = pair.second->getPermisos().size();
            if (cantidadPermisos > maxPermisos) {
                maxPermisos = cantidadPermisos;
                resultado.clear();
                resultado.push_back(pair.first);
            } else if (cantidadPermisos == maxPermisos) {
                resultado.push_back(pair.first);
            }
        }
        return resultado;
    }

    vector<string> permisosComunes() {
        map<string, int> contadorPermisos;
        for (const auto& pair : cargos) {
            for (const auto& permiso : pair.second->getPermisos()) {
                contadorPermisos[permiso]++;
            }
        }

        vector<string> comunes;
        for (const auto& p : contadorPermisos) {
            if (p.second > 1) comunes.push_back(p.first);
        }
        return comunes;
    }

    vector<string> usuariosConMasPermisos() {
        size_t maxPermisos = 0;
        vector<string> resultado;

        for (const auto& user : usuarios) {
            size_t cantidadPermisos = user->getCargo()->getPermisos().size();
            if (cantidadPermisos > maxPermisos) {
                maxPermisos = cantidadPermisos;
                resultado.clear();
                resultado.push_back(user->getNombre());
            } else if (cantidadPermisos == maxPermisos) {
                resultado.push_back(user->getNombre());
            }
        }
        return resultado;
    }
};

int main() {
    Sistema sistema;

    // Cargos básicos
    Cargo* vendedor = new Cargo("Vendedor", {"Ver stock", "Realizar venta"});
    Cargo* administracion = new Cargo("Administracion", {"Ver stock", "Ordenar productos", "Descontinuar producto"});
    Cargo* jefeVentas = new Cargo("Jefe de ventas", {"Auditar venta"}, vendedor);

    sistema.agregarCargo(vendedor);
    sistema.agregarCargo(administracion);
    sistema.agregarCargo(jefeVentas);

    // Usuarios
    Usuario* juan = new Usuario("Juan Perez", 101, vendedor);
    Usuario* maria = new Usuario("Maria Gomez", 102, administracion);
    Usuario* carlos = new Usuario("Carlos Ruiz", 103, jefeVentas);

    sistema.agregarUsuario(juan);
    sistema.agregarUsuario(maria);
    sistema.agregarUsuario(carlos);

    // Verificar permisos
    cout << (sistema.verificarPermiso(103, "Realizar venta") ? "Tiene permiso\n" : "No tiene permiso\n");
    cout << (sistema.verificarPermiso(103, "Auditar venta") ? "Tiene permiso\n" : "No tiene permiso\n");

    // Cargos con más permisos
    cout << "Cargos con más permisos:\n";
    for (const auto& cargo : sistema.cargosConMasPermisos()) {
        cout << cargo << endl;
    }

    // Permisos comunes
    cout << "Permisos comunes:\n";
    for (const auto& permiso : sistema.permisosComunes()) {
        cout << permiso << endl;
    }

    // Usuarios con más permisos
    cout << "Usuarios con más permisos:\n";
    for (const auto& user : sistema.usuariosConMasPermisos()) {
        cout << user << endl;
    }

    // Liberar memoria
    delete vendedor;
    delete administracion;
    delete jefeVentas;
    delete juan;
    delete maria;
    delete carlos;

    return 0;
}
