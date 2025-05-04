#include <iostream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>
#include <fstream>
using namespace std; 
struct datosProductos{
    int id;
    char titulo[100];
    char descripcion[200];
    float precio;
    bool esParche;
};
class Producto{
    static int contadorID;
    protected:
    int id;
    string titulo;
    string descripcion;
    float precio;
    public:
    Producto(datosProductos datos):id(contadorID++),titulo(datos.titulo),descripcion(datos.descripcion),precio(datos.precio) {}

};
class Juego:public Producto{
    private:
    vector<Parche*>parches;
    public:
    Juego(datosProductos datos):Producto(datos) {}
    void addParche(Parche*p){parches.push_back(p);}
    
};
class Parche:public Producto{
    private:
    vector<Producto*>requerido; //puede ser el juego/parche o una lista de parches.
    public:

};
class Catalogo{
    private:
    vector<Producto*> productos;
    public:
    Catalogo(){}
};


int main(int argc, char *argv[]) {
    
    
system("pause");
    return 0;
}