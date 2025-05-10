#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
using namespace std; 
struct enlaceCompuesto{
    int codigoProd;
    int codigoComp;
};
struct datosProducto{
    int codigo;
    char descripcion[200];
    double precio;
    char tipo;
};
class Producto{
    protected:
    int codigo;
    string descripcion;
    double precio;
    char tipo;
    public:
    Producto()  {}
    Producto(datosProducto datos):codigo(datos.codigo),descripcion(datos.descripcion),precio(datos.precio),tipo(datos.tipo) {}
    int getCodigo(){return codigo;}
    double getPrecio(){return precio;}
    virtual void actualizarPrecio(double porcentaje,double cotAnt,double cotAct)=0;
};
class Nacional:public Producto{
    public:
    Nacional(datosProducto datos):Producto(datos){}
    void actualizarPrecio(double porcentaje,double cotAnt,double cotAct){
        precio+=(precio*(porcentaje/100));
    }
};
class Importado:public Producto{
    public:
    Importado(datosProducto datos):Producto(datos){}
    void actualizarPrecio(double porcentaje,double cotAnt,double cotAct){
        precio=(precio/cotAnt)*cotAct;
    }
};
class Compuesto:public Producto{
    private:
    vector<Producto*>prods;
    public:
    Compuesto(datosProducto datos):Producto(datos){}
    void actualizarPrecio(double porcentaje,double cotAnt,double cotAct){
        for(auto prod:prods){precio+=prod->getPrecio();}
    }
    void agregarProducto(Producto*p){prods.push_back(p);}
};
class Gestor{
    private:
    vector<Producto*>productos;
    public:
    Gestor(){}
    void leerArchivo(const char*URLArchivo,const char*URLenlaces){
        ifstream archivo(URLArchivo,ios::binary);
        ifstream archivo1(URLenlaces,ios::binary);
        if(archivo.fail())return;
        datosProducto datos;
        Producto* producto= nullptr;
        while(archivo.read(reinterpret_cast<char*>(&datos),sizeof(datosProducto))){
            if(datos.tipo=='N'){
                producto=new Nacional(datos);
            }
            if(datos.tipo=='I'){
                producto=new Importado(datos);
            }
            if(datos.tipo=='C'){
                producto=new Compuesto(datos);
            }
            productos.push_back(producto);
        }
        archivo.close();
        enlaceCompuesto enlace;
        while(archivo1.read(reinterpret_cast<char*>(&enlace),sizeof(enlaceCompuesto))){
            for(auto producto:productos){
                if (enlace.codigoProd==producto->getCodigo())
                {
                    for(auto comp:productos){
                        if(enlace.codigoComp==comp->getCodigo()){
                            Compuesto* prod=dynamic_cast<Compuesto*>(comp);
                            prod->agregarProducto(producto);
                        }
                    }
                }
                
            }
        }
        archivo1.close();
    }


};
int main(int argc, char *argv[]) {
    
    
system("pause");
    return 0;
}