#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <algorithm> // Por si se requiere sort o max_element, etc.

using namespace std;

/* -----------
   ESTRUCTURAS
   ----------- */

// Datos mínimos de un Autor, para guardarlo en binario
struct datosAutor {
    int idAutor;            // Identificador único del autor
    char nombre[50];        // Nombre del autor
};

// Datos mínimos de un Libro, para guardarlo en binario
struct datosLibro {
    int idLibro;            // Identificador único del libro
    char nombre[50];        // Título del libro
};

// Datos mínimos de un Ejemplar, para guardarlo en binario
struct datosEjemplar {
    int idLibro;            // Relación: a qué libro pertenece
    int isbn;               // ISBN del ejemplar
    int anio;               // Año de impresión
    char editorial[50];     // Nombre de la editorial
};

// Estructura para guardar la relación muchos-a-muchos entre Libro y Autor
struct datosLibroAutor {
    int idLibro;
    int idAutor;
};

/* -----------
   CLASE AUTOR
   ----------- */

class Autor {
private:
    int idAutor;
    string nombre;

public:
    // Constructor desde la estructura
    Autor(const datosAutor &da) {
        idAutor = da.idAutor;
        nombre  = da.nombre;
    }

    // Convierte la clase a la estructura para guardarla binariamente
    datosAutor getDatos() const {
        datosAutor da{};
        da.idAutor = idAutor;
        strcpy(da.nombre, nombre.c_str());
        return da;
    }

    int  getIdAutor() const { return idAutor; }
    string getNombre() const { return nombre; }
};

/* -----------
   CLASE EJEMPLAR
   ----------- */

class Ejemplar {
private:
    int isbn;
    int anio;
    string editorial;

public:
    // Constructor desde la estructura
    Ejemplar(const datosEjemplar &de) {
        isbn = de.isbn;
        anio = de.anio;
        editorial = de.editorial;
    }

    // Para guardarlo binariamente
    datosEjemplar getDatos(int idLibro) const {
        datosEjemplar de{};
        de.idLibro   = idLibro; // se inyecta después
        de.isbn      = isbn;
        de.anio      = anio;
        strcpy(de.editorial, editorial.c_str());
        return de;
    }

    int    getISBN()      const { return isbn; }
    int    getAnio()      const { return anio; }
    string getEditorial() const { return editorial; }
};

/* -----------
   CLASE LIBRO
   ----------- */

class Libro {
private:
    int idLibro;
    string nombre;
    vector<Ejemplar*> ejemplares; // Podríamos usar punteros inteligentes o por valor

public:
    // Constructor desde la estructura
    Libro(const datosLibro &dl) {
        idLibro = dl.idLibro;
        nombre  = dl.nombre;
    }

    // Para guardarlo binariamente
    datosLibro getDatos() const {
        datosLibro dl{};
        dl.idLibro = idLibro;
        strcpy(dl.nombre, nombre.c_str());
        return dl;
    }

    int getIdLibro() const { return idLibro; }
    string getNombre() const { return nombre; }

    // Agregar ejemplares
    void agregarEjemplar(Ejemplar *e) {
        ejemplares.push_back(e);
    }

    const vector<Ejemplar*>& getEjemplares() const {
        return ejemplares;
    }
};

/* -----------
   CLASE BIBLIOTECA
   -----------
   - Guarda:
       * vector<Autor*>    (todos los autores)
       * vector<Libro*>    (todos los libros)
       * vector<datosLibroAutor> relacionesLibroAutor
         donde cada elemento dice (idLibro, idAutor)
   - Ofrece métodos para:
       * agregarAutor(...)
       * agregarLibro(...)
       * asociarAutorLibro(idAutor, idLibro)
       * agregarEjemplar(idLibro, datosEjemplar)
       * guardarDatos(...) -> 4 archivos binarios
   -----------
*/

class Biblioteca {
private:
    vector<Autor*> autores;
    vector<Libro*> libros;

    // Aquí guardamos la relación muchos-a-muchos: (idLibro, idAutor)
    vector<datosLibroAutor> relacionesLibroAutor;

public:
    // Agrega un autor a la lista
    void agregarAutor(Autor *a) {
        autores.push_back(a);
    }

    // Agrega un libro a la lista
    void agregarLibro(Libro *l) {
        libros.push_back(l);
    }

    // Relaciona un autor con un libro (muchos-a-muchos)
    void asociarAutorLibro(int idAutor, int idLibro) {
        datosLibroAutor rela{};
        rela.idAutor = idAutor;
        rela.idLibro = idLibro;
        relacionesLibroAutor.push_back(rela);
    }

    // Agregar un ejemplar a un libro específico
    void agregarEjemplar(int idLibro, Ejemplar *ej) {
        // Buscar el libro con idLibro y agregarle el ejemplar
        for (auto &lib : libros) {
            if (lib->getIdLibro() == idLibro) {
                lib->agregarEjemplar(ej);
                break;
            }
        }
    }

    // GUARDAR TODO EN 4 ARCHIVOS
    void guardarDatos(
        const char* archivoAutores,
        const char* archivoLibros,
        const char* archivoEjemplares,
        const char* archivoLibroAutor
    ) {
        // 1) Guardar Autores
        ofstream fAut(archivoAutores, ios::binary | ios::trunc);
        if(!fAut.is_open()) {
            cerr << "Error al abrir " << archivoAutores << endl;
            return;
        }
        for (auto &aut : autores) {
            datosAutor da = aut->getDatos();
            fAut.write(reinterpret_cast<char*>(&da), sizeof(datosAutor));
        }
        fAut.close();

        // 2) Guardar Libros
        ofstream fLib(archivoLibros, ios::binary | ios::trunc);
        if(!fLib.is_open()) {
            cerr << "Error al abrir " << archivoLibros << endl;
            return;
        }
        for (auto &lib : libros) {
            datosLibro dl = lib->getDatos();
            fLib.write(reinterpret_cast<char*>(&dl), sizeof(datosLibro));
        }
        fLib.close();

        // 3) Guardar Ejemplares
        ofstream fEjem(archivoEjemplares, ios::binary | ios::trunc);
        if(!fEjem.is_open()) {
            cerr << "Error al abrir " << archivoEjemplares << endl;
            return;
        }
        // Recorremos cada libro, y cada uno de sus ejemplares
        for (auto &lib : libros) {
            int id = lib->getIdLibro();
            for (auto &ej : lib->getEjemplares()) {
                // Tomamos la estructura con idLibro correcto
                datosEjemplar de = ej->getDatos(id);
                fEjem.write(reinterpret_cast<char*>(&de), sizeof(datosEjemplar));
            }
        }
        fEjem.close();

        // 4) Guardar Relación Libro-Autor
        ofstream fLA(archivoLibroAutor, ios::binary | ios::trunc);
        if(!fLA.is_open()) {
            cerr << "Error al abrir " << archivoLibroAutor << endl;
            return;
        }
        for (auto &rela : relacionesLibroAutor) {
            fLA.write(reinterpret_cast<char*>(&rela), sizeof(datosLibroAutor));
        }
        fLA.close();

        cout << "Datos guardados correctamente en los archivos binarios." << endl;
    }
};

/* -----------
   EJEMPLO DE USO EN main()
   -----------
   - Creamos algunos autores, libros y ejemplares
   - Asociamos autores a libros
   - Guardamos todo
*/

int main() {
    Biblioteca bib;

    // Crear y agregar Autores
    datosAutor da1{100, "Gabriel Garcia Marquez"};
    datosAutor da2{101, "Jorge Luis Borges"};
    Autor* autor1 = new Autor(da1);
    Autor* autor2 = new Autor(da2);
    bib.agregarAutor(autor1);
    bib.agregarAutor(autor2);

    // Crear y agregar Libros
    datosLibro dl1{1, "Cien Anos de Soledad"};
    datosLibro dl2{2, "El Aleph"};
    Libro* libro1 = new Libro(dl1);
    Libro* libro2 = new Libro(dl2);
    bib.agregarLibro(libro1);
    bib.agregarLibro(libro2);

    // Asociar autores con libros (relacion M:N)
    // Gabriel Garcia Marquez (100) -> Cien Anos de Soledad (1)
    bib.asociarAutorLibro(100, 1);
    // Jorge Luis Borges (101) -> El Aleph (2)
    bib.asociarAutorLibro(101, 2);

    // Agregar ejemplares a libro 1
    datosEjemplar e1{0, 12345, 1967, "EditorialSur"};
    datosEjemplar e2{0, 99999, 1970, "EditorialNorte"};
    Ejemplar* ejem1 = new Ejemplar(e1);
    Ejemplar* ejem2 = new Ejemplar(e2);
    bib.agregarEjemplar(1, ejem1);
    bib.agregarEjemplar(1, ejem2);

    // Agregar un ejemplar a libro 2
    datosEjemplar e3{0, 88888, 1949, "EdicionesBorges"};
    Ejemplar* ejem3 = new Ejemplar(e3);
    bib.agregarEjemplar(2, ejem3);

    // Guardar todo en binarios
    bib.guardarDatos(
        "Autores.bin",
        "Libros.bin",
        "Ejemplares.bin",
        "LibroAutor.bin"
    );

    // Liberar memoria dinámica (en un proyecto real, usar punteros inteligentes)
    // ...
    // for (auto &lib : ...)  delete lib;
    // for (auto &aut : ...)  delete aut;

    return 0;
}
