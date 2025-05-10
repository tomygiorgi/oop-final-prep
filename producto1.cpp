#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <set>
using namespace std; 
struct datosProducto{
    char nombre[100];
    double costo;
};
struct datosRelacion{
    char n1[100];
    char n2[100];
};
class Producto{
    protected:
    string nombre;
    double costo;
    public:
    Producto(datosProducto datos):nombre(datos.nombre),costo(datos.costo) {}
    datosProducto getDatos(){
        datosProducto datos;
        datos.costo=costo;
        strcpy(datos.nombre,nombre.c_str());
        return datos;
    }
    string getNombre(){return nombre;}
    virtual void calcularCosto()=0;
};
class Platos:public Producto{
    private:
    vector<Producto*> ingredientes;
    public:
    Platos(datosProducto datos):Producto(datos) {}
    vector<Producto*> getIngredientes(){return ingredientes;}
};
class Ingredientes:public Producto{ 
    public:
    Ingredientes(datosProducto datos):Producto(datos) {}

};
class gestor{
    private:
    vector<Producto*>productos;
    public:
    gestor(){}
    void guardarPlatos(const char*URLPlatos,const char*URLIngredientes,const char*URLlink){
        ofstream archivo(URLPlatos,ios::binary);
        ofstream archivo1(URLIngredientes,ios::binary);
        ofstream archivo2(URLlink,ios::binary);
        if(archivo.fail()||archivo1.fail()||archivo2.fail())return;
        for(auto producto:productos){
            Ingredientes* p1=dynamic_cast<Ingredientes*>(producto);
            Platos* p2=dynamic_cast<Platos*>(producto);
            datosProducto datos=producto->getDatos();
            if(p1){//INGREDIENTES
                archivo1.write(reinterpret_cast<char*>(&datos),sizeof(datosProducto));
            }
            if(p2){//PLATOS
                archivo.write(reinterpret_cast<char*>(&datos),sizeof(datosProducto));
                for(auto ing:p2->getIngredientes()){
                    datosRelacion datos1;
                    strcpy(datos1.n1,producto->getNombre().c_str());
                    strcpy(datos1.n2,ing->getNombre().c_str());
                    archivo2.write(reinterpret_cast<char*>(&datos1),sizeof(datosRelacion));
                }
            }
        }
        archivo.close();//PLATOS
        archivo1.close();//INGREDIENTES
        archivo2.close()//RELACION PLATO-INGREDIENTE
    }

};
int main(int argc, char *argv[]) {
    
    
system("pause");
    return 0;
}