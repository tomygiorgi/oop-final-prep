#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <numeric>
using namespace std; 
struct datosVino{
    int cantLitros;
    char tipo;
    int codigo;
    int anioProd;
};
class vinos{
    protected:
    int cantLitros;
    char tipo;
    int codigo;
    int anioProd;
    public:
    vinos(datosVino datos):cantLitros(datos.cantLitros),tipo(datos.tipo),codigo(datos.codigo),anioProd(datos.anioProd){}
    int getLitros(){return cantLitros;}
    int getCodigo(){return codigo;}
    virtual char getTipo(){return tipo;}
    int getAnio(){return anioProd;}
    virtual float calcularPrecio()=0;
    virtual int calcularAnio()=0;

};
class mesa:public vinos{
    public:
    mesa(datosVino datos):vinos(datos){}
    float calcularPrecio(){
        return cantLitros*120;
    }
    int calcularAnio(){
        return anioProd+1;
    }
    char getTipo(){
        return 'M';
    }
};
class premium:public vinos{
    public:
    premium(datosVino datos):vinos(datos){}
    float calcularPrecio(){
        return cantLitros*200;
    }
    int calcularAnio(){
        return anioProd+2;
    }
    char getTipo(){
        return 'P';
    }
};
class especial:public vinos{
    public:
    especial(datosVino datos):vinos(datos){}
        float calcularPrecio(){
        return cantLitros*320;
    }
    int calcularAnio(){
        return anioProd+3;
    }
    char getTipo(){
        return 'E';
    }
};
typedef vinos* vv;
class paquete:public vinos{
    private:
    vv me;
    vv pe;
    vv es;
    public:
    paquete(datosVino datos,vv m,vv p,vv e):vinos(datos),me(m),pe(p),es(e){}
    float calcularPrecio(){
        return me->calcularPrecio()+pe->calcularPrecio()+es->calcularPrecio();
    }
    int litrosPaquete(){
        return me->getLitros()+pe->getLitros()+es->getLitros();
    }
    
};
class gestor{
    private:
    vector<vinos*>VINOS;
    public:
        void agregarVino(vinos*v){
        VINOS.push_back(v);
    }
    gestor(const char*archivo){
        ifstream archi(archivo,ios::binary);
        if (archi.fail())return;
        datosVino datos;
        while(archi.read(reinterpret_cast<char*>(&datos),sizeof(datosVino))){
            if(datos.tipo=='M')
            agregarVino(new mesa(datos));
            else if(datos.tipo=='P')
            agregarVino(new premium(datos));
            else if(datos.tipo=='E')
            agregarVino(new especial(datos));
        }
    }
    void litrosYmontoPorAnio(int &anio){
        int totalLitros;
        float totalMonto;
        for(const auto&vino:VINOS){
            if(anio==(vino->calcularAnio())){
                totalLitros+=vino->getLitros();
                totalMonto+=vino->calcularPrecio();
            }
        }
        cout<<"La cantidad de litros disponibles el anio"<<anio<<"son:"<<totalLitros;
        cout<<"En el caso de vender todo seria el monto de "<<totalMonto;
    }
    void guardarDatosAnios(const char*URL){
            ofstream archi(URL);
            if(archi.fail())return;
            int anios[4]={2020,2021,2022,2023};
            int litros[4];
            float montos[4];
            for(const auto&vino:VINOS){
                for(int i=0;i<4;i++){
                    if(vino->calcularAnio()==anios[i]){
                        litros[i]+=vino->getLitros();
                        montos[i]+=vino->calcularPrecio();
                    }
                }
            }
            for(int i=0;i<4;i++){
                archi<<anios[i]<<"..."<<litros[i]<<"..."<<montos[i];
            }
            archi.close();
        }
    void fiveLitros(){
        map<int,int> masLitros;
        for(const auto&vino:VINOS){
            masLitros[vino->calcularAnio()]+=vino->getLitros();
        }
        int c=0;       
        for(const auto&pair:masLitros){
            cout<<pair.first<<endl;
            c++;
            if(c==4)break;
        }
    }
    void anioMasVino(){
        map<int,int>maxAnio;
        for(const auto&vino:VINOS){
            maxAnio[vino->calcularAnio()]+=vino->getLitros();
        }
        auto maxVino=max_element(maxAnio.begin(),maxAnio.end(),[](const pair<int,int>*a,const pair<int,int>*b){
            return a->second < b->second;
        });
        cout<<"EL anio donde se hizo mas vino es en:"<<maxVino->first;
    }
    void mayorMontoVenta(){
        map<int,float>mayorAnio;
        for(const auto&vino:VINOS){
            mayorAnio[vino->calcularAnio()]+=vino->calcularPrecio();
        }
        auto primerElemento=mayorAnio.begin();
        cout<<primerElemento->first;
    }
    void calcularPaquetesAnio(int &anio){
        int m=0,p=0,e=0,paquetes=0;
        for(const auto&vino:VINOS){
            if(vino->calcularAnio()==anio){
                switch(vino->getTipo()){
                    case 'M':m++;break;
                    case 'P':p++;break;
                    case 'E':e++;break;
                }
                if(m==p==e)paquetes++;
            }
        }
        cout<<"Cantidad de paquetes posibles:"<<paquetes;
    }
};

int main(int argc, char *argv[]) {
    
    
system("pause");
    return 0;
}