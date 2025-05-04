#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <numeric>
using namespace std; 
struct datosAlumno{
    char nombre[20];
    char codigo[20];
    char tipo;
    vector<int> notas;
    bool aprobo;
};
class alumno{
    protected://atributos
    char tipo;
    string nombre;
    string codigo;
    public://metodos
    alumno(datosAlumno dato):nombre(dato.nombre),codigo(dato.codigo),tipo(dato.tipo){}
    virtual datosAlumno getDatos()=0;
    virtual void guardarAlumno()=0; //para archivos individuales
    virtual bool aprobar()=0;
    string getNombre(){
        return nombre;
    }
};
class invitado:public alumno{
    private:
    int nota;
    public:
    invitado(datosAlumno dato):alumno(dato),nota(dato.notas[0]){}
    datosAlumno getDatos()override{
        datosAlumno datos;
        datos.tipo='I';
        strcpy(datos.codigo,codigo.c_str());
        strcpy(datos.nombre,nombre.c_str());
        datos.notas={nota};
        datos.aprobo=aprobar();
        return datos;
    }
    void guardarAlumno()override{
        string archi;
        archi=nombre+".dat";
        ofstream archivoAlumno (archi,ios::binary);
        if(archivoAlumno.fail())return;
        datosAlumno datos=getDatos();
        archivoAlumno.write(reinterpret_cast<char*>(&datos), sizeof(datosAlumno));
        archivoAlumno.close();
    }
    bool aprobar()override{
        return nota>60;
    }
};
class medio:public alumno{
    private:
    vector<int> notas;
    public:
    medio(datosAlumno dato):alumno(dato),notas(dato.notas){}

    datosAlumno getDatos()override{
        datosAlumno datos;
        strcpy(datos.codigo,codigo.c_str());
        strcpy(datos.nombre,nombre.c_str());
        datos.tipo='M';
        datos.notas=notas;
        datos.aprobo=aprobar();
        return datos;
    }

    void guardarAlumno()override{
        string archi;
        archi=nombre+".dat";
        ofstream archivoAlumno (archi,ios::binary);
        if(archivoAlumno.fail())return;
        datosAlumno datos=getDatos();
        archivoAlumno.write(reinterpret_cast<char*>(&datos), sizeof(datosAlumno));
        archivoAlumno.close();
    }

    bool aprobar()override{
        float total;
        total=accumulate(notas.begin(),notas.end(),0);
        return total/3>70;
    }
};
class premium:public alumno{
    private:
    vector<int>notas;
    public:
    premium(datosAlumno dato):alumno(dato),notas(dato.notas){}
    datosAlumno getDatos()override{
        datosAlumno datos;
        strcpy(datos.codigo,codigo.c_str());
        strcpy(datos.nombre,nombre.c_str());
        datos.tipo='P';
        datos.notas=notas;
        datos.aprobo=aprobar();
        return datos;
    }
    void guardarAlumno()override{
        string archi;
        archi=nombre+".dat";
        ofstream archivoAlumno (archi,ios::binary);
        if(archivoAlumno.fail())return;
        datosAlumno datos=getDatos();
        archivoAlumno.write(reinterpret_cast<char*>(&datos), sizeof(datosAlumno));
        archivoAlumno.close();
    }
    bool aprobar()override{
        float suma;
        suma=accumulate(notas.begin(),notas.end(),0);
        return suma/5>80;
    }
};
class CursoSys{
    private:
    vector<alumno*> alumnos;
    vector<alumno*> aprobados;
    public:
    void agregarAlumno (alumno* Alumno){
        alumnos.push_back(Alumno);
    }
    vector<alumno*> getAlumnos(){
        return alumnos;
    }
    void guardarAlumnos(const char* NOMBRECURSO){ //guardar en un solo archivo
        ofstream archivoAlumnos (NOMBRECURSO,ios::binary);
        if (archivoAlumnos.fail()){return;}
        for (const auto &alum:alumnos){
            datosAlumno datos=alum->getDatos();
            archivoAlumnos.write(reinterpret_cast<char*>(&datos),sizeof(datosAlumno));
        }
    }
    void listarAprobados(){
        for (const auto&alum:alumnos){
            datosAlumno datos=alum->getDatos();
            if (datos.aprobo){
                aprobados.push_back(alum);
            }
        }
    }
    void mejorarPlan(string codigo, char tipo){
        for (size_t i=0;i<alumnos.size();i++){
            datosAlumno datos=alumnos[i]->getDatos();
            if(codigo==datos.codigo){
                alumno* nuevoAlumno=nullptr;
                if(tipo=='M'){
                    nuevoAlumno=new medio(datos);
                }else if(tipo=='P'){
                    nuevoAlumno=new premium(datos);
                }
                if(nuevoAlumno){   
                    delete alumnos[i];
                    alumnos[i]=nuevoAlumno;
                }
                return;
            }
        }
    }
    double calcularPromedio (alumno* alum){
        if(alumnos.empty())return;
        datosAlumno Alum=alum->getDatos();
        return accumulate(Alum.notas.begin(),Alum.notas.end(),0.0)/Alum.notas.size();
    }
    vector<alumno*>mejorProm(CursoSys*curso){
        vector<alumno*> ALUMNOS=curso->alumnos;
        if(ALUMNOS.empty())return;
        auto maxProm=calcularPromedio(*max_element(ALUMNOS.begin(),ALUMNOS.end(),[this](alumno*a,alumno*b){
            return calcularPromedio(a)<calcularPromedio(b);
        }));
        vector<alumno*>mejores;
        copy_if(ALUMNOS.begin(),ALUMNOS.end(),back_inserter(mejores),[this,maxProm](alumno*a){
            return calcularPromedio(a)==maxProm;
        });
        return mejores;
    }
    void contarAlumnos(CursoSys*curso){
        vector<alumno*> ALUMNOS=curso->alumnos;
        map<char,int>cantidad;
        for(const auto& alum:ALUMNOS){
            cantidad[alum->getDatos().tipo]++;
        }
        for(const auto&[tipo,cantidad]:cantidad){
            cout<<"Tipo:"<<tipo<<" "<<"Cantidad:"<<cantidad;
        }
    }


};
class multiCurso{
    private:
    vector<CursoSys*>cursos;
    public:
    void agregarCurso(CursoSys*curso){
        cursos.push_back(curso);
    }
    void guardarAlumnos(const char*URLCursoAlumnos,CursoSys*curso){
        ofstream archi(URLCursoAlumnos,ios::binary);  
        if(archi.fail())return;
        curso->guardarAlumnos(URLCursoAlumnos);    
    }

};

int main(int argc, char *argv[]) 
    {
        vector<alumno*>alumnos;
    multiCurso sistema;
    CursoSys* curso1 = new CursoSys();
    CursoSys* curso2 = new CursoSys();

    datosAlumno datosInvitado = {"Juan", "A001", 'I', {85}};
    datosAlumno datosMedio = {"Maria", "A002", 'M', {70, 75, 80},};
    datosAlumno datosPremium = {"Luis", "A003", 'P', {85, 90, 95, 88, 92}};

    curso1->agregarAlumno(new invitado(datosInvitado));
    curso1->agregarAlumno(new medio(datosMedio));
    curso1->agregarAlumno(new premium(datosPremium));

    sistema.agregarCurso(curso1);
    sistema.agregarCurso(curso2);
    


    
system("pause");
    return 0;
}