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
    float tiempo;
};
struct datosCorrelativa{
    char codigo1[50];
    char codigo2[50];
};
class Cursos{
    protected:
    string codigo, titulo;
    float tiempo;
    public:
    Cursos(string cod,string ti,float tim):codigo(cod),titulo(ti),tiempo(tim){}
    string getCodigo(){return codigo;}
    string getTitulo(){return titulo;}
    float getTiempo(){return tiempo;}
    datosCurso getDatos(){
        datosCurso datos;
        datos.tiempo=tiempo;
        strcpy(datos.codigo,codigo.c_str());
        strcpy(datos.titulo,titulo.c_str());
        return datos;
    }
    datosCorrelativa getRelacion(Cursos*corre){
        datosCorrelativa datos;
        strcpy(datos.codigo1,codigo.c_str());
        strcpy(datos.codigo2,corre->codigo.c_str());
        return datos;
    }
    virtual  vector<Cursos*>getCorrelativas()=0;
    virtual float cargaHoraria()=0;
};
class cursoBasico:public Cursos{
    private:
    vector<Cursos*> correlativas;
    public:
    cursoBasico(string codigo,string titulo,float tiempo):Cursos(codigo,titulo,tiempo){}
    void addCorrelativa(Cursos*c){
        correlativas.push_back(c);
    }
    vector<Cursos*>getCorrelativas(){return correlativas;}
    float cargaHoraria(){
        float tot=tiempo;
        tot+=accumulate(correlativas.begin(),correlativas.end(),0,[this](float acum,Cursos*a){
            return acum+a->cargaHoraria();
        });
        return tot;
    }
};
class cursoCompuesto:public Cursos{
    private:
    vector<Cursos*>compuestos;
    public:
    cursoCompuesto(string codigo, string titulo, float tiempo):Cursos(codigo,titulo,tiempo){}
    void addCompuesto(Cursos*curso){
        compuestos.push_back(curso);
    }
    
};
class gestor{
    private:
    vector<Cursos*>cursos;
    public:
    gestor(){}
    void guardarCursos(const char*URL){
        ofstream archi(URL,ios::binary);
        if(archi.fail())return;
            for(auto curso:cursos){
                datosCurso datos=curso->getDatos();
                archi.write(reinterpret_cast<char*>(&datos),sizeof(datosCurso));
            }
        
        archi.close();
        ofstream archi1 ("Correlativas.dat",ios::binary);
         if(archi1.fail())return;
         for(auto curso:cursos){
            for(auto correlativa:curso->getCorrelativas()){
                datosCorrelativa datos=curso->getRelacion(correlativa);
                archi1.write(reinterpret_cast<char*>(&datos),sizeof(datosCorrelativa));
            }
        }
        archi1.close();
    }
    void mostrarYguardar(string &url){
        ofstream archi(url);
        for(auto curso:cursos){
            cout<<"Curso:"<<curso->getTitulo()<<endl;
            archi<<"Curso:"<<curso->getTitulo()<<endl;
            cout<<"Correlativas:"<<endl;
            archi<<"Correlativas:"<<endl;
            for(auto corre:curso->getCorrelativas()){
                cout<<corre->getTitulo()<<"."<<endl;
                archi<<corre->getTitulo()<<"."<<endl;
            }
        }
    }
    void masCorrelativas(){
        sort(cursos.begin(),cursos.end(),[](Cursos&a,Cursos&b){
            return a.getCorrelativas().size() > b.getCorrelativas().size();
        });
        auto maxCorrelativas=cursos[0]->getCorrelativas().size();
        cout<<"Cursos con mas correlativas("<<maxCorrelativas<<")."<<endl;
        for(auto curso:cursos){
            if(maxCorrelativas==curso->getCorrelativas().size()){
                cout<<curso->getTitulo()<<"."<<endl;;
            }
        }
    }
    void tiempoTotal(Cursos*curso){
        float tot=curso->cargaHoraria();
        cout<<"Tiempo del curso"<<tot;
    }
};
int main(int argc, char *argv[]) {
    
    
system("pause");
    return 0;
}