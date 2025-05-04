#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <numeric>
using namespace std; 
class Viajes{
    protected:
    float costo;
    float tiempoVuelo;
    float tiempoColectivo;
    string nombre;
    char tipo;
    public:
    Viajes(float c, float time, string n, char t) : costo(c), tiempoVuelo(time), nombre(n), tipo(t) {}
    virtual float getMonto()=0;
    virtual float getTiempo()=0;
    virtual char getTipo()=0;
    string getNombre(){
        return nombre;
    }
    float getCosto(){
        return costo;
    }
};
class Avion:public Viajes{
    Avion(float c, float time, string n, char t):Viajes(c,time,n,t){}
    public:
    float getMonto(){
        return costo+=costo*1.20;
    }
    float getTiempo(){
        return tiempoVuelo+2;
    }
    char getTipo(){
        return 'A';
    }
};
class Colectivo:public Viajes{
    private:
    float km;
    public:
    Colectivo(float c, float time, string n, char t, float km):Viajes(c,time,n,t),km(km){}
    float getMonto(){
        return 8*km;
    }
    void ingresarTiempoColectivo(float &tiempo){
        tiempoColectivo=tiempo;
    }
    float getTiempo(){
        return tiempoColectivo;
    }
    char getTipo(){
        return 'C';
    }
};
class AvionColectivo:public Viajes{
    private:
    float km;
    float tiempoC;
    public:
    AvionColectivo(float c, float time, string n, char t, float k,float tiemp):Viajes(c,time,n,t),km(k),tiempoC(tiemp){}
    float getMonto(){
        return (costo+=costo*1.20)+km*8;
    }
    float getTiempo(){
        return tiempoC+tiempoVuelo+2;
    }
    char getTipo(){
        return 'AC';
    }
};
class Compuestos:public Viajes{
    private:
    vector<Viajes*>compuesto;
    public:
    Compuestos(float c, float time, string n, char t, vector<Viajes*>comp):Viajes(c,time,n,t),compuesto(comp){}
    float getMonto(){
        float total;
        for(const auto&comp:compuesto){
            total+=comp->getMonto();
        }
        return total;
    }
    float getTiempo(){
        float total;
        for(const auto&comp:compuesto){
            total+=comp->getTiempo();
        }
        return total;
    }
};
class gestorViajes{
    private:
    vector<Viajes*> viajes;
    public:
    gestorViajes(){}
    void cargarViaje(Viajes* viaje){
        viajes.push_back(viaje);
    }
    void guardarViajes(const char*URLViajes){
        ofstream archivo(URLViajes);
        if(archivo.fail())return;
        for(const auto&viaj:viajes){
            archivo<<viaj->getNombre();
            archivo<<viaj->getTipo();
            archivo<<viaj->getMonto();
            archivo<<viaj->getTiempo();
            archivo<<viaj->getCosto();
        }
        archivo.close();
    }
    void FiveFlysLows(){
        sort(viajes.begin(),viajes.end(),[](Viajes*a,Viajes*b){
            return a->getMonto() < b->getMonto();
        });
        for(int i=0;i<5;i++){
            cout<<viajes[i]->getNombre();
        }
    }
    void ViajeMasTiempo(){
        auto max=max_element(viajes.begin(),viajes.end(),[](Viajes*a,Viajes*b){
        return a->getTiempo() < b->getTiempo();});
        cout<<(*max)->getNombre();
    }
    void filtrarPorTiempo(float tiempo){
        vector<Viajes*> viajesFiltrados;
        copy_if(viajes.begin(),viajes.end(),back_inserter(viajesFiltrados),[tiempo](Viajes*a){
            return a->getTiempo()==tiempo;
        });
        for(auto viajeS:viajesFiltrados){
            cout<<viajeS->getNombre();
        }
    }
};

int main(int argc, char *argv[]) {
    
    
system("pause");
    return 0;
}