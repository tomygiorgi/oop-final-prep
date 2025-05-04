#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

/*
  Sistema de archivos en memoria, para fines educativos.
  - Existe una carpeta root ("/") que puede contener archivos y subcarpetas.
  - Cada nodo (archivo o carpeta) tiene un nombre único dentro de la carpeta que lo contiene.
  - Se implementan los comandos:
       mkdir <nombre>        (crea carpeta)
       touch <nombre> <peso> (crea archivo con un peso específico)
       cd <nombre>           (cambia a la carpeta <nombre>, cd .. sube de nivel, cd / va a root)
       ls                    (lista contenido ordenado)
       lsp                   (lista contenido ordenado y lo guarda en display.txt)
       pwd                   (muestra la ruta completa de la carpeta actual)
       size                  (muestra el peso de la carpeta actual)
       exit                  (finaliza)
*/

/*----------------------------------------------------------------
 * Clase base abstracta: Node
 * Representa un elemento en el sistema de archivos (archivo o carpeta).
 ----------------------------------------------------------------*/
class Node {
protected:
    string name;
    Node* parent; // Puntero a la carpeta padre
public:
    Node(const string& n, Node* p) : name(n), parent(p) {}
    virtual ~Node() {}

    string getName() const { return name; }
    Node* getParent() const { return parent; }
    void setParent(Node* p) { parent = p; }

    // El peso puede variar según sea carpeta o archivo:
    virtual long long getSize() const = 0;
    // Saber si es un directorio o no (para distinguir comportamiento):
    virtual bool isDirectory() const = 0;
};

/*----------------------------------------------------------------
 * Clase File: representa un archivo con un peso específico
 ----------------------------------------------------------------*/
class File : public Node {
private:
    long long size; // Peso del archivo
public:
    File(const string& n, Node* p, long long sz)
        : Node(n, p), size(sz) {}

    long long getSize() const override {
        return size;
    }

    bool isDirectory() const override {
        return false;
    }
};

/*----------------------------------------------------------------
 * Clase Directory: representa una carpeta que puede contener nodos
 ----------------------------------------------------------------*/
class Directory : public Node {
private:
    vector<Node*> children; // Almacena archivos y subcarpetas
public:
    Directory(const string& n, Node* p)
        : Node(n, p) {}

    ~Directory() {
        // Al destruir la carpeta, se liberan todos los nodos hijos
        for (auto c : children) {
            delete c;
        }
        children.clear();
    }

    bool isDirectory() const override {
        return true;
    }

    // Agregar un nodo (archivo o carpeta) como hijo
    bool addChild(Node* node) {
        // Verificamos si ya existe un hijo con el mismo nombre
        for (auto c : children) {
            if (c->getName() == node->getName()) {
                return false; // No se puede duplicar nombre
            }
        }
        children.push_back(node);
        return true;
    }

    // Retornar referencia a los hijos
    vector<Node*>& getChildren() {
        return children;
    }

    // Peso de la carpeta = suma de pesos de todos sus hijos (recursivo)
    long long getSize() const override {
        long long total = 0;
        for (auto c : children) {
            total += c->getSize();
        }
        return total;
    }

    // Búsqueda de subcarpeta por nombre
    Directory* findSubDirectory(const string& subName) {
        for (auto c : children) {
            if (c->isDirectory() && c->getName() == subName) {
                return static_cast<Directory*>(c);
            }
        }
        return nullptr;
    }
};

/*----------------------------------------------------------------
 * Clase FileSystem: Administra la carpeta root y la carpeta actual
 ----------------------------------------------------------------*/
class FileSystem {
private:
    Directory* root;        // Carpeta raíz ("/")
    Directory* currentDir;  // Carpeta actual
public:
    FileSystem() {
        // Creamos la carpeta root; su "padre" será ella misma por conveniencia
        root = new Directory("/", nullptr);
        root->setParent(root);
        currentDir = root;
    }

    ~FileSystem() {
        // Al destruirse el FileSystem, se libera root (lo que elimina todo recursivamente)
        delete root;
    }

    // Comando: mkdir <nombre>
    void makeDirectory(const string& name) {
        if (name.empty()) {
            cout << "Nombre inválido.\n";
            return;
        }
        Directory* d = new Directory(name, currentDir);
        if (!currentDir->addChild(d)) {
            delete d; // Liberamos si falla
            cout << "Error: ya existe un elemento con ese nombre.\n";
        }
    }

    // Comando: touch <nombre> <peso>
    void makeFile(const string& name, long long size) {
        if (name.empty() || size < 0) {
            cout << "Datos inválidos.\n";
            return;
        }
        File* f = new File(name, currentDir, size);
        if (!currentDir->addChild(f)) {
            delete f;
            cout << "Error: ya existe un elemento con ese nombre.\n";
        }
    }

    // Comando: cd <nombre>
    void changeDirectory(const string& name) {
        if (name == "/") {
            // Ir directamente a root
            currentDir = root;
            return;
        }
        if (name == "..") {
            // Subir al padre, a menos que ya estemos en root
            if (currentDir == root) return;
            currentDir = static_cast<Directory*>(currentDir->getParent());
            return;
        }
        // Buscar subcarpeta
        Directory* sub = currentDir->findSubDirectory(name);
        if (!sub) {
            cout << "Error: la carpeta '" << name << "' no existe.\n";
        } else {
            currentDir = sub;
        }
    }

    // Comando: pwd (muestra la ruta completa)
    void printWorkingDirectory() {
        if (currentDir == root) {
            cout << "/" << endl;
            return;
        }
        // Reconstruir la ruta ascendiendo hasta root
        vector<string> pathParts;
        Directory* temp = currentDir;
        while (temp != root) {
            pathParts.push_back(temp->getName());
            temp = static_cast<Directory*>(temp->getParent());
        }
        // Invertimos e imprimimos, con un "/" al inicio
        cout << "/";
        for (int i = pathParts.size() - 1; i >= 0; --i) {
            cout << pathParts[i];
            if (i > 0) cout << "/";
        }
        cout << endl;
    }

    // Comando: ls (lista contenido actual, ordenado por nombre)
    void listDirectory() {
        vector<Node*>& children = currentDir->getChildren();
        // Ordenar alfabéticamente
        sort(children.begin(), children.end(), [](Node* a, Node* b){
            return a->getName() < b->getName();
        });
        // Mostrar
        for (auto c : children) {
            if (c->isDirectory()) {
                cout << c->getName() << "/\n";
            } else {
                File* f = static_cast<File*>(c);
                cout << f->getName() << " (archivo, " << f->getSize() << ")\n";
            }
        }
    }

    // Comando: lsp (lista y guarda en display.txt)
    void listAndSave() {
        vector<Node*>& children = currentDir->getChildren();
        sort(children.begin(), children.end(), [](Node* a, Node* b){
            return a->getName() < b->getName();
        });
        // Mostrar en pantalla
        for (auto c : children) {
            if (c->isDirectory()) {
                cout << c->getName() << "/\n";
            } else {
                cout << c->getName() << "\n";
            }
        }
        // Guardar en display.txt
        ofstream out("display.txt");
        if (!out) {
            cout << "No se pudo crear el archivo display.txt\n";
            return;
        }
        for (auto c : children) {
            if (c->isDirectory()) {
                out << c->getName() << "/" << endl;
            } else {
                out << c->getName() << endl;
            }
        }
        out.close();
        cout << "Listado guardado en display.txt\n";
    }

    // Comando libre: mostrar peso de la carpeta actual
    void showCurrentWeight() {
        cout << "Peso de la carpeta actual (" 
             << currentDir->getName() << "): "
             << currentDir->getSize() << endl;
    }
};

/*----------------------------------------------------------------
 * Función main: Ciclo de lectura de comandos
 ----------------------------------------------------------------*/
int main() {
    FileSystem fs;
    cout << "Sistema de archivos en memoria (Ámbito Educativo)\n"
         << "Comandos disponibles:\n"
         << "  mkdir <nombre>        -> crea carpeta\n"
         << "  touch <nombre> <peso> -> crea archivo con peso\n"
         << "  cd <nombre>           -> entra a carpeta\n"
         << "  ls                    -> lista elementos\n"
         << "  lsp                   -> lista y guarda en display.txt\n"
         << "  pwd                   -> muestra ruta actual\n"
         << "  size                  -> muestra peso de carpeta actual\n"
         << "  exit                  -> salir del programa\n\n";

    string line;
    while (true) {
        cout << "> ";
        if (!getline(cin, line)) break; // Fin de entrada
        if (line.empty()) continue;

        stringstream ss(line);
        string cmd;
        ss >> cmd;

        if (cmd == "exit") {
            break;
        } else if (cmd == "mkdir") {
            string name;
            ss >> name;
            fs.makeDirectory(name);
        } else if (cmd == "touch") {
            string name;
            long long sz = 0;
            ss >> name >> sz;
            fs.makeFile(name, sz);
        } else if (cmd == "cd") {
            string name;
            ss >> name;
            fs.changeDirectory(name);
        } else if (cmd == "ls") {
            fs.listDirectory();
        } else if (cmd == "lsp") {
            fs.listAndSave();
        } else if (cmd == "pwd") {
            fs.printWorkingDirectory();
        } else if (cmd == "size") {
            fs.showCurrentWeight();
        } else {
            cout << "Comando no reconocido.\n";
        }
    }

    cout << "Saliendo del sistema de archivos.\n";
    return 0;
}

/*  -- TEORÍA --
1) Diferencias entre archivo binario y archivo de texto:
   - Un archivo de texto almacena los datos en forma legible para humanos
     (caracteres ASCII, por ejemplo). Cada número se representa como cadenas
     de dígitos, puede haber saltos de línea, espacios, etc.
   - Un archivo binario almacena la información en la forma interna
     de las variables (por ejemplo, 4 bytes directos para un int).
     No está pensado para ser leído por humanos.
   - Los archivos de texto son más portables y fáciles de editar, pero
     menos eficientes en espacio y lectura/escritura. Los binarios son más
     compactos y rápidos, pero se requiere un programa específico
     para interpretarlos.

2) ¿La herencia genera acoplamiento? Justifique:
   - La herencia produce un acoplamiento "vertical" entre la clase base
     y las clases derivadas. Si la clase base cambia su interfaz o implementación,
     podría afectar a todas las clases hijas que dependen de ella.
   - El acoplamiento puede minimizarse diseñando interfaces estables,
     cumpliendo el Principio de Sustitución de Liskov y otras buenas prácticas
     de herencia. Sin embargo, sigue siendo un acoplamiento mayor que
     el que se obtiene con composición o interfaces puras.
*/
