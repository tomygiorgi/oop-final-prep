#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <set>
using namespace std; 
struct datosProductos{
    char nombre[20];
    int codigo;
};
struct datosIngredientes{
    char nombre[20];
    int codigo;    
};
struct relaciones{
    int codigoProd;
    int codigoIng;
};
class Ingredientes{
    private:
    string nombre;
    int codigo;
    public:
    Ingredientes(string nom, int code):nombre(nom),codigo(code){}
    datosIngredientes getDatos(){
        datosIngredientes datos;
        datos.codigo=codigo;
        strcpy(datos.nombre,nombre.c_str());
        return datos;
    }
    int getCodigo(){return codigo;}
    string getNombre(){return nombre;}
};
class Productos{
    private:
    string nombre;
    int codigo;
    vector<Ingredientes*>ingredientes;
    vector<Productos*>compuesto;
    public:
    Productos(string nom,int cod):nombre(nom),codigo(cod){}
    void addIngredientes(Ingredientes*ing){
        ingredientes.push_back(ing);
    }
    void addCompuesto(Productos*prod){
        compuesto.push_back(prod);
    }
    int getCodigo(){return codigo;}
    string getNombre(){return nombre;}
    vector<Ingredientes*>getIng(){return ingredientes;}
    vector<Productos*>getComp(){return compuesto;}
    datosProductos getDatos(){
        datosProductos datos;
        datos.codigo=codigo;
        strcpy(datos.nombre,nombre.c_str());
        return datos;
    }
    relaciones getRelacionIng(Ingredientes*ing){
        relaciones datos;
        datos.codigoIng=ing->getCodigo();
        datos.codigoProd=codigo;
        return datos;
    }
    relaciones getRelacionComp(Productos*prod){
        relaciones datos;
        datos.codigoIng=prod->codigo;
        datos.codigoProd=codigo;
        return datos;
    }
    friend ostream& operator <<(ostream&os,Productos* prod){
        set<int>existentes;
        os<<"Producto:"<<prod->nombre<<"--"<<prod->codigo<<endl;
        os<<"Ingredientes:"<<endl;
        for(auto ing:prod->getIng()){
                os<<ing->getNombre()<<"--"<<ing->getCodigo();
                existentes.insert(ing->getCodigo());
            os<<endl;
        }
        for(auto comp:prod->getComp()){
            os<<comp->getNombre()<<"--"<<comp->getCodigo()<<endl;
            for(auto ing:comp->getIng()){
                if(!existentes.count(ing->getCodigo())){
                    os<<ing->getNombre()<<"--"<<ing->getCodigo();
                    existentes.insert(ing->getCodigo());
                }
            }
        }
        return os;
    }
};
class gestor{
    private:
    vector<Productos*>productos;
    public:
    gestor(){}
    void addProducto(Productos*prod){
        productos.push_back(prod);
    }
    void guardarBinario(const char*URL){
        ofstream archi(URL,ios::binary);
        if(archi.fail())return;
        for(auto prod:productos){
            datosProductos datos=prod->getDatos();
            archi.write(reinterpret_cast<char*>(&datos),sizeof(datosProductos));
            for(auto ing:prod->getIng()){
                datosIngredientes datos1=ing->getDatos();
                archi.write(reinterpret_cast<char*>(&datos1),sizeof(datosIngredientes));
            }
            for(auto comp:prod->getComp()){
                datosProductos datos1=comp->getDatos();
                archi.write(reinterpret_cast<char*>(&datos1),sizeof(datosProductos));
            }
        }
        archi.close();
        ofstream archi("relaciones.dat",ios::binary);
        if(archi.fail())return;
        for(auto prod:productos){
            for(auto ing:prod->getIng()){
                relaciones datos=prod->getRelacionIng(ing);
                archi.write(reinterpret_cast<char*>(&datos),sizeof(relaciones));
            }
        }
        for(auto prod:productos){
            for(auto comp:prod->getComp()){
                relaciones datos=prod->getRelacionComp(comp);
                archi.write(reinterpret_cast<char*>(&datos),sizeof(relaciones));
            }
        }
        archi.close();
    }
    void ingredienteMasUtilizado(){
        map<Ingredientes*,int>contador;
        for(auto prod:productos){
            for(auto ing:prod->getIng()){
                contador[ing]++;
            }
        }
        auto maxElement=max_element(contador.begin(),contador.end(),[](pair<Ingredientes*,int>*a,pair<Ingredientes*,int>*b){
            return a->second < b->second;
        });
        for(auto pair:contador){
            if(pair.second==maxElement->second){
                cout<<pair.first->getNombre();
            }
        }
    }
    void productoMasIngredientes(){
        sort(productos.begin(),productos.end(),[](Productos*a,Productos*b){
            return a->getIng().size() > b->getIng().size();
        });
        cout<<"Producto con mas ingredientes:"<<productos[0]->getNombre();
    }
    void guardarArchivoTexto(string url){
        ofstream archi(url);
        if(archi.fail())return;
        for(auto prod:productos){
            archi<<prod;
        }
        archi.close();
    }
};
/*Las clases abstractas son importantes cuando tenemos varios objetos
que son similares, comparten atributos y comportamientos (aunque estos
dependiendo del objeto pueden variar, por eso utilizamos herencia).
Es una tecnica de reutilizacion de codigo y es muy util para este
tipo de situaciones donde tenemos objetos parecidos pero que se comportan
distinto.
La ventaja es que nos permite reutilizar el codigo, la desventaja es que
es inflexible.
Se relaciona con el polimorfismo ya que una tecnica conlleva a la otra,
ya que podemos utilizar metodos de una clase en otra siempre y cuando
sean parte de la misma herencia.*/

int main(int argc, char *argv[]) {
    
    
system("pause");
    return 0;
}