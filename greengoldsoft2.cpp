#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <set>
#include <numeric>
using namespace std; 
struct datosCurso{
    char codigo[50];
    char titulo[200];
    float time;
};
struct datosCorrelativa{
    char codigoPrincipal[50];
    char codigoCorrelativa[50];
};
struct datosCompuesto{
    char codigoPrincipal[50];
    char codigoCompuesto[50];
};
class Curso{
    protected:
    string codigo;
    string titulo;
    float time;
    vector<CursoBasico*>Correlativas;
    public:
    Curso(datosCurso datos):codigo(datos.codigo),titulo(datos.titulo),time(datos.time){}
    datosCurso getDatos(){
        datosCurso datos;
        datos.time=time;
        strcpy(datos.codigo,codigo.c_str());
        strcpy(datos.titulo,titulo.c_str());
        return datos;
    }
    datosCorrelativa getCorrelativa(Curso*c1){
        datosCorrelativa datos;
        strcpy(datos.codigoPrincipal,codigo.c_str());
        strcpy(datos.codigoCorrelativa,c1->getCodigo().c_str());
        return datos;
    }
    vector<CursoBasico*> getVecCorrelativas(){return Correlativas;}
    void agregarCorrelativa(CursoBasico*c1){
        Correlativas.push_back(c1);
    }
    virtual void guardarDatos(ofstream& archivo,ofstream& archivo1,ofstream& archivo2)=0;
    virtual void agregarCompuesto(Curso*c1)=0;
    string getCodigo(){return codigo;}
    string getTitulo(){return titulo;}
    float getTime(){return time;}

};
class CursoBasico:public Curso{
    public:
    CursoBasico(datosCurso datos):Curso(datos){}
    void guardarDatos(ofstream& archivo,ofstream& archivo1,ofstream& archivo2)override{
        datosCurso datos=getDatos();
        archivo.write(reinterpret_cast<char*>(&datos),sizeof(datosCurso));
        for(const auto&Correlativa:Correlativas){
            datosCorrelativa datosC=getCorrelativa(Correlativa);
            archivo1.write(reinterpret_cast<char*>(&datosC),sizeof(datosCorrelativa));
        }
    }
    void agregarCompuesto(Curso*c1){/*esta vacia pq no hace nada si agregamos un compuesto en curso basico*/}
};
class CursoCompuesto:public Curso{
    private:
    vector<Curso*>Compuestos;
    public:
    CursoCompuesto(datosCurso datos):Curso(datos){
        datos.time=0;//inicilizamos en 0 el tiempo del curso mixto
    }
    datosCompuesto getCompuesto(Curso*c1){
        datosCompuesto datos;
        strcpy(datos.codigoPrincipal,codigo.c_str());
        strcpy(datos.codigoCompuesto,c1->getCodigo().c_str());
        return datos;
    }
    vector<Curso*> getVecCompuestos(){
        return Compuestos;
    }
    void agregarCompuesto(Curso*c1){
        Compuestos.push_back(c1);
        for(const auto&correlativa:c1->getVecCorrelativas()){
            bool existe=false;
            for(auto c:Correlativas){ //chequeamos que no se repitan las correlativas
                if(c->getCodigo()==correlativa->getCodigo()){
                    existe=true;
                    break;
                }
            }
            if(existe)continue;
            Correlativas.push_back(correlativa);
        }
        time+=c1->getTime(); //acumulamos el tiempo del curso mixto
    }
    void guardarDatos(ofstream& archivo,ofstream& archivo1,ofstream& archivo2)override{
        datosCurso datos=getDatos();
        archivo.write(reinterpret_cast<char*>(&datos),sizeof(datosCurso));
        for(const auto&Correlativa:Correlativas){
            datosCorrelativa datosC=getCorrelativa(Correlativa);
            archivo1.write(reinterpret_cast<char*>(&datosC),sizeof(datosCorrelativa));
        }
        for(const auto&Compuesto:Compuestos){
            datosCompuesto datosCom=getCompuesto(Compuesto);
            archivo2.write(reinterpret_cast<char*>(&datosCom),sizeof(datosCompuesto));
        }
    }
};
class Gestor{
    private:
    vector<Curso*>Cursos;
    public:
    Gestor(){}
    void agregarCurso(Curso*c1){
        Cursos.push_back(c1);
    }
    void guardarCursos(const char*URLCurso,const char*URLCorrelativas,const char*URLCompuestos){
        ofstream archivo(URLCurso,ios::binary|ios::ate);
        if(archivo.fail())return;
        ofstream archivo1(URLCorrelativas,ios::binary|ios::ate);
        if(archivo1.fail())return;
        ofstream archivo2(URLCompuestos,ios::binary|ios::ate);
        if(archivo2.fail())return;
        for(const auto&curso:Cursos){
            curso->guardarDatos(archivo,archivo1,archivo2);
        }
        archivo.close();
        archivo1.close();
        archivo2.close();
    }
    void mostrarYguardar(string&URL){
        ofstream archivo(URL);
        if(archivo.fail())return;
        for(const  auto&Curso:Cursos){
            cout<<"Curso:"<<Curso->getCodigo()<<"  "<<Curso->getTitulo()<<endl;
            archivo<<"Curso:"<<Curso->getCodigo()<<"  "<<Curso->getTitulo()<<endl;
            cout<<"-Correlativas:"<<endl;
            archivo<<"-Correlativas:"<<endl;
            for(const auto&Correlativa:Curso->getVecCorrelativas()){
                cout<<Correlativa->getCodigo()<<"  "<<Correlativa->getTitulo()<<endl;
                archivo<<Correlativa->getCodigo()<<"  "<<Correlativa->getTitulo()<<endl;
            }
            CursoCompuesto*CompuestoTYPE=dynamic_cast<CursoCompuesto*>(Curso);
            if(CompuestoTYPE!=nullptr){
                cout<<"Compuestos:"<<endl;
                archivo<<"Compuestos:"<<endl;
                for(const auto&Compuesto:CompuestoTYPE->getVecCompuestos()){
                    cout<<Compuesto->getCodigo()<<"  "<<Compuesto->getTitulo()<<endl;
                    archivo<<Compuesto->getCodigo()<<"  "<<Compuesto->getTitulo()<<endl;
                }
            }
        }
        archivo.close();
    }
    void mayorCantCorrelativas(){
            auto maxElemento=max_element(Cursos.begin(),Cursos.end(),[](Curso*a,Curso*b){
                return a->getVecCorrelativas().size() < b->getVecCorrelativas().size();
            });
            float cant=(*maxElemento)->getVecCorrelativas().size();
            cout<<"Cursos con la mayor cantidad de correlativas("<<cant<<"):";
            for(auto Curso:Cursos){
                if(cant==Curso->getVecCorrelativas().size()){
                    cout<<Curso->getTitulo()<<endl;
                }
            }
    }
    void tiempoTotal(Curso*c1){
        map<string,float> tiempo;
        for(auto c:c1->getVecCorrelativas()){
            tiempo[c1->getCodigo()]+=c->getTime();
        }
        tiempo[c1->getCodigo()]+=c1->getTime();
        cout<<"El tiempo total del curso es de "<<tiempo[c1->getCodigo()];
    }
};
/*Una clase abstracta es necesaria en la programacion orientada a objetos porque nos
permite reutilizar codigo en ciertos casos, hacerlo mas corto y performante.
Cuando tenemos dos o mas objetos que tienen atributos en comun
pero tienen comportamientos distintos podemos hacer una clase abstracta con sus clases
derivadas, que hereden los atributos y metodos de la misma.
La ventaja de usar clases abstractas es que nos permite controlar varias clases
en una sola simplificando el codigo, pero es inflexible y si se utiliza de forma erronea
tendremos como resultado una logica confusa y en la mayoria de los casos inutil.  */

int main(int argc, char *argv[]) {
    
    
system("pause");
    return 0;
}