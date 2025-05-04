#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <numeric>
using namespace std; 
    struct PeliculasOSerie{
        char nombre[150];
        char codigo[20];
        int puntuacion;
        char tipo;
    };
    struct Relaciones{
        char codeContent[20];
        char codigoRelacionado[20];
    };
    struct CapituloSerie{
        char codigoCapitulo[20];
        char codigoSerie[20];
    };
    class contenido{
        protected:
        string codigo, nombre;
        char tipo;
        int puntuacion;
        vector<contenido*>relaciones;
        public:
        contenido(PeliculasOSerie datos):codigo(datos.codigo),nombre(datos.nombre),tipo(datos.tipo),puntuacion(datos.puntuacion) {}
        string getCodigo(){return codigo;}
        string getNombre(){return nombre;}
        char getTipo(){return tipo;}
        int getPunt(){return puntuacion;}
        PeliculasOSerie getDatos(){
            PeliculasOSerie datos;
            datos.puntuacion=puntuacion;
            datos.tipo=tipo;
            strcpy(datos.codigo,codigo.c_str());
            strcpy(datos.nombre,nombre.c_str());
            return datos;
        }
        vector<contenido*>getRelaciones(){return relaciones;}
        Relaciones getRelacion(contenido*contentRelacionado){
            Relaciones datos;
            strcpy(datos.codeContent,codigo.c_str());
            strcpy(datos.codigoRelacionado,contentRelacionado->getCodigo().c_str());
            return datos;
        }
        virtual void agregarRelacion(contenido*)=0;
        virtual void agregarCapitulo(Capitulos*)=0;
    };
    class Peliculas:public contenido{
        public:
        Peliculas(PeliculasOSerie datos):contenido(datos){
            tipo='P';
        }
        void agregarRelacion(contenido*c){
            relaciones.push_back(c);
        }
        void agregarCapitulo(){
            //funcion vacia para esta clase
        }

    };
    class Series:public contenido{
        private:
        vector<Capitulos*> capitulos;
        public:
        Series(PeliculasOSerie datos):contenido(datos){
            tipo='S';
        }
        void agregarRelacion(contenido*c){
            relaciones.push_back(c);
        }
        void agregarCapitulo(Capitulos*c){
            capitulos.push_back(c);
        }
        vector<Capitulos*>getCapitulos(){
            return capitulos;
        }
        CapituloSerie getCapituloSerie(Capitulos*c){
            CapituloSerie datos;
            strcpy(datos.codigoSerie,codigo.c_str());
            strcpy(datos.codigoCapitulo,c->getCodigo().c_str());
            return datos;
        }
    };
    class Capitulos:public contenido{

        public:
        Capitulos(PeliculasOSerie datos):contenido(datos){
            tipo='C';
        }
        void agregarCapitulo(){
            //funcion vacia para esta clase
        }
        void agregarRelacion(contenido*c){
            relaciones.push_back(c);
        }

    };
    class Netflix{
        private:
        vector<contenido*>Catalogo;
        public:
        Netflix(){}
        void agregarPeliculaSerie(contenido*c){
            Capitulos*cap=dynamic_cast<Capitulos*>(c);
            if(cap==nullptr){
                Catalogo.push_back(c);
            }else{
                /*En caso de que queramos agregar un capitulo al catalogo
                este por defecto no lo agrega, ya que se debe agregar solamente 
                al vector de cada serie*/
            }
        }
        void guardarContenido(const char*URLContenido,const char*URLRelaciones){
            ofstream archivo(URLContenido,ios::binary|ios::ate);
            ofstream archivo1(URLRelaciones,ios::binary|ios::ate);
            if(archivo.fail() or archivo1.fail()) return;
            for(const auto&content:Catalogo){
                if(content->getTipo()=='P'){
                    PeliculasOSerie datos=content->getDatos();
                    archivo.write(reinterpret_cast<char*>(&datos),sizeof(PeliculasOSerie));
                    if(!content->getRelaciones().empty()){
                        for(auto relaciones:content->getRelaciones()){
                            Relaciones datos1=content->getRelacion(relaciones);
                            archivo1.write(reinterpret_cast<char*>(&datos1),sizeof(Relaciones));
                        }
                    }
                }else if(content->getTipo()=='S'){
                    PeliculasOSerie datos=content->getDatos();
                    archivo.write(reinterpret_cast<char*>(&datos),sizeof(PeliculasOSerie));
                    if(!content->getRelaciones().empty()){
                        for(auto relaciones:content->getRelaciones()){
                            Relaciones datos1=content->getRelacion(relaciones);
                            archivo1.write(reinterpret_cast<char*>(&datos1),sizeof(Relaciones));
                        }
                    }
                    Series*cap=dynamic_cast<Series*>(content);
                    if(!cap->getCapitulos().empty()){
                        string URLCapitulos=content->getNombre()+"_Capitulos";
                        ofstream archivo2(URLCapitulos,ios::binary|ios::ate);
                        if(archivo2.fail())return;
                    for(auto capitulo: cap->getCapitulos()){
                            CapituloSerie datos2= cap->getCapituloSerie(capitulo);
                            archivo2.write(reinterpret_cast<char*>(&datos2),sizeof(CapituloSerie));
                            for(auto relaciones:capitulo->getRelaciones()){
                                Relaciones datos1=capitulo->getRelacion(relaciones);
                                archivo1.write(reinterpret_cast<char*>(&datos1),sizeof(Relaciones));
                            }
                    }
                        archivo2.close();
                    }
                }
            }
            archivo.close();
            archivo1.close();
        }
        void imprimirPeliculas(Peliculas*pelicula){
            vector<Peliculas*>pelisRelacionadas;
            copy_if(pelicula->getRelaciones().begin(),pelicula->getRelaciones().end(),back_inserter(pelisRelacionadas),[](contenido*a){
                return  a->getTipo() == 'P';
            });
            if(!pelisRelacionadas.empty()){
                string url=pelicula->getNombre()+"_PelisRelacionadas";
                ofstream archivo(url);
                if(archivo.fail())return;
                archivo<<"Peliculas que estan relacionadas con "<<pelicula->getNombre()<<":"<<endl;
                for(auto p:pelisRelacionadas){
                    archivo<<p->getNombre()<<endl;
                }
                archivo.close();
            }
        }
        void masPelisRelacionadas(){
            auto maxElement=max_element(Catalogo.begin(),Catalogo.end(),[](contenido*a,contenido*b){
                return (a->getRelaciones().size() < (b->getRelaciones().size())) and ((a->getTipo()=='P') and (b->getTipo()=='P'));
            });
            int c=0;
            for(auto rel:(*maxElement)->getRelaciones()){
                if(rel->getTipo()=='P')c++;
            }
            cout<<"Series/Peliculas con mayor peliculas relacionadas:"<<endl;
            for(auto cat:Catalogo){
                int contador=0;
                for(auto rel:cat->getRelaciones()){
                    if(rel->getTipo()=='P')
                    contador++;
                }
                if(contador==c){
                    cout<<cat->getNombre()<<endl;
                }
            }
        }
        void sinRelaciones(){
            cout<<"Series que no tienen relaciones:"<<endl;
            for(auto cat:Catalogo){
                if(cat->getRelaciones().empty() and cat->getTipo()=='S'){
                    cout<<cat->getNombre()<<endl;
                }
            }
        }
        void imprimirCantidadPuntajes(){
            map<int,int> Puntajes;
            cout<<"Cantidad de series/peliculas por puntaje:"<<endl;
            for(auto cat:Catalogo){
                Puntajes[cat->getPunt()]++;
            }
            for(auto pair:Puntajes){
                cout<<pair.first<<":"<<pair.second<<"."<<endl;
            }
        }
    };
    /*Es necesario utilizar herencia cuando tenemos 2 o mas objetos que tienen atributos en comun
    pero que se comportan diferente.
    Al tener una clase padre con sus atributos y metodos, los hijos al heredar estas propiedades 
    tienen acceso a las mismas, lo que les permite utilizarlas sin tener escribirlas devuelta.
    No es bueno usar herencia cuando tenemos casos de composicion, donde el tipo de situacion
    es donde hay clases dentro de otras y no donde hay una clase con diferentes tipos.
    Por ejemplo, si peliculas y series son indiferentes y se comportan igual y decidimos usar
    herencia vamos a terminar escribiendo mas innecesariamente.
    */

int main(int argc, char *argv[]) {
    
    


system("pause");
    return 0;
}