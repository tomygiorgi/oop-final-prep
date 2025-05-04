#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>
#include <fstream>
using namespace std; 

struct datosProducto{ //Juegos y Parche
    char titulo[100];
    char descripcion[200];
    float precio;
};
struct JuegoParches{
    char tituloJuego[100];
    char tituloParche[100];
};
struct ParcheDependencia{
    char tituloParche[100];
    char tituloDependencia[100];
};
class Parche{
    protected:
    string titulo;
    string descripcion;
    float precio;
    public:
    Parche(datosProducto datos):titulo(datos.titulo),descripcion(datos.descripcion),precio(datos.precio) {}
    datosProducto getDatos(){
        datosProducto datos;
        strcpy(datos.titulo,titulo.c_str());
        strcpy(datos.descripcion,descripcion.c_str());
        datos.precio=precio;
        return datos;
    }
    string getTitulo(){return titulo;}
    virtual vector<ParcheBasico*>getDependencias()=0;
};
class ParcheBasico:public Parche{
    private:
    ParcheBasico* dependencia;
    public:
    ParcheBasico(datosProducto datos,ParcheBasico*d):Parche(datos),dependencia(d){}
    vector<ParcheBasico*>getDependencias(){
        vector<ParcheBasico*>a;
        a.push_back(dependencia);
        return a;
    }
};
class ParcheCompuesto:public Parche{
    private:
    vector<ParcheBasico*> dependencias;
    public:
    ParcheCompuesto(datosProducto datos,vector<ParcheBasico*>d):Parche(datos),dependencias(d) {}
    vector<ParcheBasico*>getDependencias(){
        return dependencias;
    }
};
class Juego{
    private:
    string titulo;
    string descripcion;
    float precio;
    vector<Parche*>parches;
    public:
    Juego(datosProducto datos):titulo(datos.titulo),descripcion(datos.descripcion),precio(datos.precio) {}
    void addParche(Parche*p){
        parches.push_back(p);
    }
    datosProducto getDatos(){
        datosProducto datos;
        strcpy(datos.titulo,titulo.c_str());
        strcpy(datos.descripcion,descripcion.c_str());
        datos.precio=precio;
        return datos;
    }
    vector<Parche*> getParches(){return parches;}
    string getTitulo(){return titulo;}
    float getPrecio(){return precio;}
};
class Catalogo{
    private:
    vector<Juego*> Juegos;
    public:
    Catalogo();
    void addJuego(Juego*j){
        Juegos.push_back(j);
    }
    void guardarDatos(const char*JuegosU, const char*ParchesU,const char*JuegoParchesU,const char*ParchesDependenciasU){
        ofstream AJuegos(JuegosU,ios::binary);
        ofstream AParches(ParchesU,ios::binary);
        ofstream AJuegoParches(JuegoParchesU,ios::binary);
        ofstream AParchesDependencias(ParchesDependenciasU,ios::binary);
        if(AJuegos.fail() ||AParches.fail() ||AJuegoParches.fail() ||AParchesDependencias.fail())return;
        for(auto juego:Juegos){
            datosProducto datos=juego->getDatos();
            AJuegos.write(reinterpret_cast<char*>(&datos),sizeof(datosProducto));
            for(auto parche:juego->getParches()){
                datosProducto datos1=parche->getDatos();
                AParches.write(reinterpret_cast<char*>(&datos1),sizeof(datosProducto));
                JuegoParches relacion;
                strcpy(relacion.tituloJuego,juego->getTitulo().c_str());
                strcpy(relacion.tituloParche,parche->getTitulo().c_str());
                AJuegoParches.write(reinterpret_cast<char*>(&relacion),sizeof(JuegoParches));
                for(auto dependencia:parche->getDependencias()){
                    ParcheDependencia relacionParcheDependencia;
                    strcpy(relacionParcheDependencia.tituloParche,parche->getTitulo().c_str());
                    strcpy(relacionParcheDependencia.tituloDependencia,dependencia->getTitulo().c_str());
                    AParchesDependencias.write(reinterpret_cast<char*>(&relacionParcheDependencia),sizeof(ParcheDependencia));
                }
                
            }
        }
        AJuegos.close();
        AParches.close();
        AJuegoParches.close();
        AParchesDependencias.close();
    }
    string masCaros(){
        string linea;
        auto max=(*max_element(Juegos.begin(),Juegos.end(),[](Juego*a,Juego*b){
            return a->getPrecio() < b->getPrecio();
        }));
        for(auto juego:Juegos){
            if(max->getPrecio() == juego->getPrecio()){
                linea+=juego->getTitulo()+"\n";
            }
        }
        return linea;
    }
    string masParches(){
        string linea;
        auto max=(*max_element(Juegos.begin(),Juegos.end(),[](Juego*a,Juego*b){
            return a->getParches().size() < b->getParches().size();
        }));
        for(auto juego:Juegos){
            if(max->getParches().size() == juego->getParches().size()){
                linea+=juego->getTitulo()+"\n";
            }
        }
        return linea;
    }
    void guardarFunciones(string URL){
        ofstream archivo(URL) ;
        if(archivo.fail())return;
        archivo<<masCaros()<<masParches();
    }

};

int main(int argc, char *argv[]) {
    


    
system("pause");
    return 0;
}