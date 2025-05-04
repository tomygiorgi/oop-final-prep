#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>  // si quieres usar funciones C-string
#include <cctype>   // para isspace
#include <string>
using namespace std;

// -------------------------------------------------------
// Clase Comentario
// -------------------------------------------------------
class Comentario {
private:
    string texto;
    int puntaje;
    vector<Comentario> subcomentarios; // Para nivel 2

public:
    Comentario() : puntaje(0) {}
    Comentario(const string &t, int p) : texto(t), puntaje(p) {}

    // Setters y getters sencillos
    void setTexto(const string &t) { texto = t; }
    void setPuntaje(int p)         { puntaje = p; }
    string getTexto()              { return texto; }
    int getPuntaje()               { return puntaje; }

    // Agregar subcomentario (nivel 2)
    void agregarSubcomentario(const Comentario &sub) {
        subcomentarios.push_back(sub);
    }

    vector<Comentario>& getSubcomentarios() {
        return subcomentarios;
    }
};

// -------------------------------------------------------
// Clase Producto
// -------------------------------------------------------
class Producto {
private:
    string nombre;
    vector<Comentario> comentarios; // nivel 1

public:
    Producto() {}
    Producto(const string &nom) : nombre(nom) {}

    void setNombre(const string &nom) {
        nombre = nom;
    }
    string getNombre() {
        return nombre;
    }

    void agregarComentario(const Comentario &c) {
        comentarios.push_back(c);
    }

    vector<Comentario>& getComentarios() {
        return comentarios;
    }
};

// -------------------------------------------------------
// Clase Sistema (o como quieras llamarla)
// Contiene el método para leer el archivo y almacenar
// los productos y comentarios.
// -------------------------------------------------------
class Sistema {
private:
    vector<Producto> productos;  // todos los productos leídos

public:
    // Método para leer el archivo de texto
    // Formato asumido:
    //   NombreDeProducto
    //       -Comentario : 5
    //       -Otro comentario : 2
    //           --Subcomentario : 4
    //       -Otro comentario : 1
    //
    // Se asume que no habrá más de 2 niveles: '-' y '--'
    void leerArchivo(const char* ruta) {
        ifstream in(ruta);
        if(!in.is_open()) {
            cout << "No se pudo abrir el archivo: " << ruta << endl;
            return;
        }

        string linea;
        // punteros "auxiliares" para saber dónde colocar comentarios
        Producto* productoActual = nullptr;        // último producto creado
        Comentario* ultimoComentario = nullptr;    // último comentario nivel 1

        while(getline(in, linea)) {
            // Si la línea está vacía, la ignoramos
            if(linea.empty()) {
                continue;
            }

            // Contamos cuántos '-' hay al inicio
            int dashCount = contarGuiones(linea);

            // Quitamos esos guiones de la línea para quedarnos solo con el texto
            string contenido = linea.substr(dashCount);

            // Caso 1: dashCount == 0 => es un nombre de producto
            if(dashCount == 0) {
                // Creamos un nuevo producto
                productos.push_back(Producto(contenido));
                // Apuntamos productoActual al recién creado
                productoActual = &productos.back();
                // Reseteamos último comentario
                ultimoComentario = nullptr;
            }
            // Caso 2: dashCount == 1 => comentario de primer nivel
            else if(dashCount == 1) {
                // parseamos la línea "Texto : puntaje"
                Comentario c = parsearComentario(contenido);
                // Lo agregamos al producto actual
                if(productoActual) {
                    productoActual->agregarComentario(c);
                    // Guardamos la dirección del comentario recién agregado
                    // para poder meter subcomentarios en él.
                    // Ojo: getComentarios() retorna un vector<Comentario>&
                    //      el back() es el último elemento por referencia.
                    ultimoComentario = &productoActual->getComentarios().back();
                }
            }
            // Caso 3: dashCount == 2 => subcomentario (nivel 2)
            else if(dashCount == 2) {
                // parseamos subcomentario
                Comentario sub = parsearComentario(contenido);
                // Lo agregamos al "últimoComentario"
                if(ultimoComentario) {
                    ultimoComentario->agregarSubcomentario(sub);
                }
            }
            // Si hay más de 2 guiones, lo ignoramos o podría manejarse, 
            // pero dijimos que sólo haremos 2 niveles para simplificar.
        }

        in.close();
    }

    // Método para mostrar los datos leídos (opcional, para probar)
    void mostrarDatos() {
        for(auto &p : productos) {
            cout << "Producto: " << p.getNombre() << "\n";
            // Comentarios de primer nivel
            for(auto &c : p.getComentarios()) {
                cout << "  - " << c.getTexto()
                     << " : " << c.getPuntaje() << "\n";

                // Subcomentarios de este comentario
                for(auto &sub : c.getSubcomentarios()) {
                    cout << "    -- " << sub.getTexto()
                         << " : " << sub.getPuntaje() << "\n";
                }
            }
            cout << "\n";
        }
    }

    // Getter si lo necesitas
    vector<Producto>& getProductos() {
        return productos;
    }

private:
    // -------------------------------------------------------
    // Funciones "auxiliares" privadas
    // -------------------------------------------------------
    // Cuenta cuántos '-' hay al inicio de la línea
    int contarGuiones(const string &line) {
        int cont = 0;
        for(char c : line) {
            if(c == '-') cont++;
            else break;
        }
        return cont;
    }

    // Dada una línea "Texto : 5" extrae "Texto" y 5
    Comentario parsearComentario(const string &linea) {
        // Buscamos la última aparición de ':'
        size_t pos = linea.rfind(':');
        if(pos == string::npos) {
            // Si no hay ':', asumimos puntaje = 0
            return Comentario(trim(linea), 0);
        }
        string texto = linea.substr(0, pos);
        string puntStr = linea.substr(pos + 1);

        texto   = trim(texto);
        puntStr = trim(puntStr);

        int puntos = 0;
        // stoi puede lanzar excepción, pero asumimos formato válido
        puntos = stoi(puntStr);

        return Comentario(texto, puntos);
    }

    // Quita espacios en blanco a izq y der
    string trim(const string &s) {
        int start = 0;
        while (start < (int)s.size() && isspace((unsigned char)s[start])) {
            start++;
        }
        int end = s.size() - 1;
        while (end >= start && isspace((unsigned char)s[end])) {
            end--;
        }
        if(start > end) return "";
        return s.substr(start, end - start + 1);
    }
};

// -------------------------------------------------------
// main de ejemplo
// -------------------------------------------------------
int main(int argc, char *argv[]){
    // Creamos el sistema y leemos el archivo
    Sistema sis;
    sis.leerArchivo("comentarios.txt"); 
    // El archivo "comentarios.txt" debe tener el formato simplificado.

    // Para probar, mostramos lo que leímos
    sis.mostrarDatos();

    // Si fuera necesario, aquí podrías hacer:
    // - Cálculos de puntaje
    // - Guardar en binario
    // - etc.

    return 0;
}
