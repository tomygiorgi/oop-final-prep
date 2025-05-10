#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <set>
using namespace std; 
struct datosProducto{//datos de los juegos 
    int id;
    char titulo[100];
    char descripcion[200];
    float precio;
};
struct datosRequeridos{
    int id1;//parche
    int id2;//parche
};
class Producto{
    static int contadorID;
    protected:
    int id;
    string titulo, descripcion;
    float precio;
    public:
    Producto(datosProducto datos):id(contadorID++),titulo(datos.titulo),descripcion(datos.descripcion),precio(datos.precio) {}
    virtual bool esParche()=0;
    datosProducto getDatos(){
        datosProducto datos;
        datos.id=id;
        datos.precio=precio;
        strcpy(datos.titulo,titulo.c_str());
        strcpy(datos.descripcion,descripcion.c_str());
        return datos;
    }
    int getID(){return id;}
};
class Juego:public Producto{
    private:
    vector<Parche*> parches;
    public:
    Juego(datosProducto datos):Producto(datos) {}
    void addParche(Parche*p){
        parches.push_back(p);
    }
    bool esParche(){return false;}
};
class Parche:public Producto{
    private:
    vector<Producto*> requeridos;
    public:
    Parche(datosProducto datos, int ID):Producto(datos) {}
    void addRequerido(Producto*p){
        requeridos.push_back(p);
    }
    bool esParche(){return true;}
    
    vector<Producto*>getRequeridos(){return requeridos;}
};
class Catalogo{
    private:
    vector<Producto*> productos;
    public:
    Catalogo(){}
    void guardarArchivo(const char*URLJuegos, const char*URLParches,const char*DependenciasParches){
        ofstream FileGames(URLJuegos,ios::binary);
        ofstream FilePatches(URLParches,ios::binary);
        ofstream FileDep(DependenciasParches,ios::binary);
        if(FileDep.fail() || FileGames.fail() || FilePatches.fail())return;
        for(const auto&producto:productos){
            datosProducto datos=producto->getDatos();
            if(!producto->esParche()){
                FileGames.write(reinterpret_cast<char*>(&datos),sizeof(datosProducto));
            }
            if(producto->esParche()){
                FilePatches.write(reinterpret_cast<char*>(&datos),sizeof(datosProducto));
                Parche*p=dynamic_cast<Parche*>(producto);
                for(auto dep:p->getRequeridos()){
                    datosRequeridos datos1;
                    datos1.id1=producto->getID();
                    datos1.id2=dep->getID();
                    FileDep.write(reinterpret_cast<char*>(&datos1),sizeof(datosRequeridos));
                }
            }
        }
        FileGames.close();
        FilePatches.close();
        FileDep.close();
    }
};


int main(int argc, char *argv[]) {
    


    
system("pause");
    return 0;
}