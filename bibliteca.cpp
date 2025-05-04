#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <set>
#include <numeric>
using namespace std;
    struct datosLibro{
        int id;
        char nombre[20];
    };
    struct datosEjemplar{
        int idLibro;
        int isbn;
        int anio;
        char editorial[20];
    };
    struct LibroAutor{
        int idLibro;
        char Autor[40];
    };
    class Ejemplar{
        private:
        int isbn;
        int anio;
        string editorial;
        public:
        Ejemplar(datosEjemplar datos):isbn(datos.isbn),anio(datos.anio),editorial(datos.editorial) {}
        datosEjemplar getDatos(){
            datosEjemplar datos;
            datos.anio=anio;
            datos.isbn=isbn;
            strcpy(datos.editorial,editorial.c_str());
            return datos;
        }
        int getISBN(){return isbn;}
        int getAnio(){return anio;}
        string getEditorial(){return editorial;}
    };
    class Libros{
        private:
        int id;
        string nombre;
        vector<string> autores;
        vector<Ejemplar*>ejemplares;
        public:
        Libros(datosLibro datos):id(datos.id),nombre(datos.nombre) {}
        datosLibro getDatos(){
            datosLibro datos;
            datos.id=id;
            strcpy(datos.nombre,nombre.c_str());
            return datos;
        }
        LibroAutor getRelacion(string &autor){
            LibroAutor datos;
            datos.idLibro=id;
            strcpy(datos.Autor,autor.c_str());
            return datos;
        }
        void agregarAutor(string&autor){
            autores.push_back(autor);
        }
        vector<string>getAutores(){return autores;}
        int getID(){return id;}
        string getNombre(){return nombre;}
        void agregarEjemplar(Ejemplar* ejemplar){
            ejemplares.push_back(ejemplar);
        }
        vector<Ejemplar*>getEjemplares(){return ejemplares;}
    };    
    class Biblioteca{
        private:
        vector<Libros*>libros;
        public:
        Biblioteca(){}
        void guardarDatos(const char*URLLibros,const char*URLEjemplares,const char*URLAutores){
            ofstream archivo(URLLibros,ios::binary|ios::ate);
            ofstream archivo1(URLEjemplares,ios::binary|ios::ate);
            ofstream archivo2(URLAutores,ios::binary|ios::ate);
            if(archivo.fail() or archivo1.fail() or archivo2.fail())return;
            for(const auto&libro:libros){
                datosLibro datos=libro->getDatos();
                archivo.write(reinterpret_cast<char*>(&datos),sizeof(datosLibro));
                for(const auto&ejemplar:libro->getEjemplares()){
                    datosEjemplar datos1=ejemplar->getDatos();
                    datos1.idLibro=libro->getID();
                    archivo1.write(reinterpret_cast<char*>(&datos1),sizeof(datosEjemplar));
                }
                for(auto&autor:libro->getAutores()){
                    LibroAutor datos=libro->getRelacion(autor);
                    archivo2.write(reinterpret_cast<char*>(&datos),sizeof(LibroAutor));
                }
            }
            archivo.close();
            archivo1.close();
            archivo2.close();
        }
        void imprimirEjemplares(string&libro){
            for(auto lib:libros){
                if(lib->getNombre()==libro){
                    string URL=libro+"_Ejemplares.txt";
                    ofstream archivo(URL);
                    if(archivo.fail())return;
                    cout<<"Lista de ejemplares:"<<endl;
                    archivo<<"Lista de ejemplres:"<<endl;
                    for(auto ejemplar:lib->getEjemplares()){
                        cout<<ejemplar->getISBN();
                        archivo<<ejemplar->getISBN();
                        cout<<ejemplar->getAnio();
                        archivo<<ejemplar->getAnio();
                        cout<<ejemplar->getEditorial();
                        archivo<<ejemplar->getEditorial();
                    }
                }
            }
        }
        void masEjemplares(){
            sort(libros.begin(),libros.end(),[](Libros*a,Libros*b){
                return a->getEjemplares().size() > b->getEjemplares().size();
            });
            cout<<"Libros con mayor cantidad de ejemplares("<<libros[0]->getEjemplares().size()<<"):"<<endl;
            for(auto libro:libros){
                if(libro->getEjemplares().size()==libros[0]->getEjemplares().size()){
                    cout<<libro->getNombre()<<endl;
                }
            }
        }
        void diferentesEditoriales(){
            int cont;
            cout<<"Lista de libros con diferentes editoriales:"<<endl;
            for(auto libro:libros){
                set<string> Editoriales;
                for(auto ejemplar:libro->getEjemplares()){
                    if(!Editoriales.count(ejemplar->getEditorial())){
                        Editoriales.insert(ejemplar->getEditorial());
                    }
                    if(Editoriales.size()>1){
                        cout<<libro->getNombre()<<endl;
                        break;
                    }
                }
            }
        }
        void editorialMasEjemplares(){
            map<string,int>Editoriales;
            for(auto libro:libros){
                for(auto ejemplar:libro->getEjemplares()){
                    Editoriales[ejemplar->getEditorial()]++;
                }
            }
            auto maxElement=max_element(Editoriales.begin(),Editoriales.end(),[](pair<string,int>*a,pair<string,int>*b){
                return a->second < b->second;
            });
            cout<<"Editorial con mas cantidad de ejemplares impresos:"<<(*maxElement).first;
        }
    };
int main(int argc, char *argv[]) {
    
    
system("pause");
    return 0;
}