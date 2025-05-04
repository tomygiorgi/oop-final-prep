#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <numeric>
using namespace std; 
struct datosPlato{
    float precio;
    char nombre[20];
    char id[10];
};
struct datosIngredientes{
    float precio;
    char nombre[20];
    char id[10];
    char idPlato[10];
};
class ingredientes{
    protected:
    float precio;
    string nombre;
    string id;
    public:
    ingredientes(float pre,string nom,string ide):precio(pre),nombre(nom),id(ide) {}
    virtual float getPrecio()=0;
    string getNombre(){return nombre;}
    string getId(){return id;}
};
class basicos:public ingredientes{
    public:
    basicos(datosIngredientes datos):ingredientes(datos.precio,datos.nombre,datos.id){}
    float getPrecio(){return precio;}
    datosIngredientes getDatos(){
        datosIngredientes datos;
        strcpy(datos.nombre,nombre.c_str());
        strcpy(datos.id,id.c_str());
        datos.precio=precio;
        datos.idPlato[0]='\0';
        return datos;
    }
};
class platos:public ingredientes{
    private:
    vector<basicos*>ing;
    public:
    platos(datosPlato datos):ingredientes(0,datos.nombre,datos.id){}
    void addIngrediente(basicos*ingre){
        ing.push_back(ingre);
        precio+=ingre->getPrecio();
    }
    datosPlato getDatos(){
        datosPlato datos;
        strcpy(datos.nombre,nombre.c_str());
        strcpy(datos.id,id.c_str());
        datos.precio=precio;
        return datos;        
    }
    vector<basicos*> getIngredientes(){
        return ing;
    }
};
class sistema{
    private:
    vector<platos*>PLATOS;
    vector<basicos*>INGREDIENTES;
    public:
    sistema(){}
    void addPlato(platos* plato){
        PLATOS.push_back(plato);
    }
    void addIngrediente(basicos* ing){
        INGREDIENTES.push_back(ing);
    }
    void guardarListaPlatos(const char* URLPlatos){
        ofstream archi(URLPlatos,ios::binary);
        if(archi.fail())return;
        datosPlato datos;
        for(const auto&p:PLATOS){
            datos=p->getDatos();
            archi.write(reinterpret_cast<char*>(&datos),sizeof(datosPlato));
        }
        archi.close();
    }
    void guardarIngredientesPlato(const char*URLPlato){
        datosPlato datos;
        for(auto p:PLATOS){
            datos=p->getDatos();
            ofstream archi(p->getNombre()+".dat",ios::binary);
            if(archi.fail())return;
            for(auto in:p->getIngredientes()){
                datosIngredientes datos1;
                datos1=in->getDatos();
                strcpy(datos1.idPlato,datos.id);
                archi.write(reinterpret_cast<char*>(&datos1),sizeof(datosIngredientes));
            }
            archi.close();
        }
    }
    void guardarListaIngredientes(const char*URLIngredientes){
        ofstream archi(URLIngredientes,ios::binary);
        if(archi.fail())return;
        for(auto ing:INGREDIENTES){
            datosIngredientes datos=ing->getDatos();
            archi.write(reinterpret_cast<char*>(&datos),sizeof(datosIngredientes));
        }
    }
    void ordenarPlatosMenorCosto(){
        sort(PLATOS.begin(),PLATOS.end(),[](platos*a,platos*b){
            return a->getPrecio() < b->getPrecio();
        });
    }
    void saveLowcost(string url){
        ofstream archi(url);
        if(archi.fail());
        ordenarPlatosMenorCosto();
        archi<<"5 platos mas baratos:"<<endl;
        for(int i=0;i<5;i++){
            archi<<PLATOS[i]->getNombre()<<endl;
        }
        archi.close();
    }
    void guardarSinHarina(string url){
        ofstream archi(url);
        if(archi.fail())return;
        archi<<"Platos que no tienen harina:"<<endl;
        for(auto p:PLATOS){
            bool harina=false;
            for(auto ing:p->getIngredientes()){
                if(ing->getNombre()=="HARINA"){
                    harina=true;
                }
            }
            if(harina==false){
                archi<<p->getNombre()<<endl;
            }
        }
    }
};
int main(int argc, char *argv[]) {
    
    
system("pause");
    return 0;
}