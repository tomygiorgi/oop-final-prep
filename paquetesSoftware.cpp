#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <numeric>
#include <set>
using namespace std; 
struct datosPaquetes{
    char codigo[100];
    char nombre[100];
    int version;
};
struct datosDependencias{
    char codigoPaquete[100];
    char codigoPaqueteDependencia[100];
};
class Paquetes{
    private:
    string codigo;
    string nombre;
    int version;
    vector<Paquetes*>dependencias;
    public:
    Paquetes(datosPaquetes datos):codigo(datos.codigo),nombre(datos.nombre),version(datos.version){}
    string getCodigo(){return codigo;}
    string getNombre(){return nombre;}
    int getVersion(){return version;}
    vector<Paquetes*>getDependencias(){return dependencias;}
    void añadirDependencias(Paquetes*paquete){
        dependencias.push_back(paquete);
    }
    friend ostream& operator<<(ostream& os,const Paquetes &paquete){
        static int nivel=0;
        for(int i=0;i<nivel;i++){os<<" ";}
        os<<paquete.nombre;
        nivel++;
        for(auto depen:paquete.dependencias){
            os<<*depen;
        }
        nivel--;
        return os;
    }
    int contarTransitivas(){
        int contador=0;
        for(auto depen:dependencias){
            if(depen->dependencias.empty())
                contador++;
        }
        return contador;
    }
    
};
    struct conjunto{
        string nombre;
        string codigo;
        int version;
        vector<Paquetes*>paq;
    };
class gestorPaquetes{
    private:
    vector<Paquetes*>paquetes;
    vector<string>visitados;
    public:
    gestorPaquetes(const char*URLPaquetes,const char*URLDependencias){
        ifstream archi(URLPaquetes,ios::binary);
        ifstream archi1(URLDependencias,ios::binary);
        if (archi.fail() or archi1.fail())return;
        datosPaquetes datos;
        datosDependencias datos1;
        while(archi.read(reinterpret_cast<char*>(&datos),sizeof(datosPaquetes))){
            paquetes.push_back(new Paquetes(datos));
        };
        while(archi1.read(reinterpret_cast<char*>(&datos1),sizeof(datosDependencias))){
            string codigo1=datos1.codigoPaquete;
            for(const auto&paquete:paquetes){
                if(paquete->getCodigo()==codigo1){
                    for(const auto&paq:paquetes){
                        string codigo2=datos1.codigoPaqueteDependencia;
                        if(paq->getCodigo()==(codigo2)){
                            paquete->añadirDependencias(paq);
                        }
                    }
                }
            }
        }
        archi.close();
        archi1.close();
    }
    void addPaquete(Paquetes*paquete){
        paquetes.push_back(paquete);
    }
    void instalarPack(conjunto pack){
        set<Paquetes*>instalados;
        for(auto paque:pack.paq){
            if(!instalados.count(paque)){
                cout<<"Instalando "<<paque->getNombre();
            instalados.insert(paque);
            }
            for(auto depen:paque->getDependencias()){
                if(!instalados.count(depen)){
                    cout<<"Instalando"<<depen->getNombre();
                    instalados.insert(depen);
                }
            }
        }
    }
    void verDependencias(const string&codigo,int nivel=0){
        for(auto vis:visitados){
            if(vis==codigo)return;
        }
        visitados.push_back(codigo);
    
        for(auto paquete:paquetes){
            if(paquete->getCodigo()==codigo){
                for(int i=0;i<nivel;i++){cout<<" "<<endl;}
                cout<<paquete->getCodigo()<<endl;
                for(auto depen:paquete->getDependencias()){
                    verDependencias(depen->getCodigo(),nivel+1);
                }
                return;
            }
        }
    }
    void guardarArbol(string codigo){
        string URLArbol=codigo+".txt";
        ofstream archi(URLArbol);
        if(archi.fail())return;
        for(auto paquete:paquetes){
            if(codigo==paquete->getCodigo()){
                archi<<*paquete;
                break;
            }
        }
        archi.close();
    }
    void fiveDepenNoTransitivas(){
        sort(paquetes.begin(),paquetes.end(),[](Paquetes*a,Paquetes*b){
            return a->contarTransitivas()<b->contarTransitivas();
        });
        for(int i=0;i<5;i++){
            cout<<paquetes[i];
        }
    }
    void noDependencias(){
        vector<Paquetes*>noDependencias;
        copy_if(paquetes.begin(),paquetes.end(),back_inserter(noDependencias),[](Paquetes*a){
            return a->getDependencias().empty();
        });
        for(auto noDepen:noDependencias){
            cout<<noDepen->getNombre();
        }
    }
};





int main(int argc, char *argv[]) {
    
    
system("pause");
    return 0;
}