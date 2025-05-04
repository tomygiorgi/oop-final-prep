#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <numeric>
#include <set>
using namespace std; 
struct datosUsuarios{
    char nombre[20];
    char password[100];
    char tipo;
};
struct datosPermisos{
    char nombre[20];
    char url[150];
};
class Usuario{
    private:
    string nombre;
    string password;
    char tipo;
    vector<string>url;
    vector<string>views;
    public:
    Usuario(datosUsuarios datos):nombre(datos.nombre),password(datos.password),tipo(datos.tipo){}
    string getNombre(){return nombre;}
    void agregarUrl(string ur){url.push_back(ur);}
    string getPassword(){return password;}
    char getTipo(){return tipo;}
    vector<string>getUrls(){return url;}
    void agregarVisita(string &URL){views.push_back(URL);}
    friend ostream& operator<<(ostream&os,Usuario &user){
        os<<user.nombre<<"->";
        auto fin=user.views.size();
        for(int i=0;i<fin;i++){
            if(i==fin-1){os<<user.views[i]<<".";}else{
            os<<user.views[i]<<", ";
            }
        }
        return os;
    }
};
class ConjuntoUsuarios{
    private:
    string nombre;
    string password;
    vector<Usuario*>conjunto;
    set<string>permisos;
    public:
    ConjuntoUsuarios(vector<Usuario*>con,string nom,string pass):conjunto(con),nombre(nom),password(pass){}
    set<string>getPermisos(){
        for(auto conj:conjunto){
            for(auto perm:conj->getUrls()){
                if(!permisos.count(perm)){
                    permisos.insert(perm);
                }
            }
        }
        return permisos;
    }
};

class sistema{
    private:
    vector<Usuario*>usuarios;
    sistema(const char*URLUsuarios,const char*URLPermisos){
        ifstream archi(URLUsuarios,ios::binary);
        ifstream archi1(URLPermisos,ios::binary);
        if(archi.fail() or archi1.fail())return;
        datosUsuarios datos;
        datosPermisos datos1;
        while(reinterpret_cast<char*>(&datos),sizeof(datosUsuarios)){
            usuarios.push_back(new Usuario(datos));
        }
        while(reinterpret_cast<char*>(&datos1),sizeof(datosPermisos)){
            for(const auto& usu:usuarios){
                    if(datos1.nombre==usu->getNombre()){
                        usu->agregarUrl(datos1.url);
                    }
                }
            }
        }
    void comprobarAcceso(const string&nom,const string &pass,string url){
        for(const auto &usu:usuarios){
            if(usu->getNombre()==nom){
                if(usu->getTipo()=='A'){
                    cout<<usu->getNombre()<<" tiene acceso al siguiente enlace.";
                    return;
                }else if(usu->getTipo()=='N'){
                    for(auto urls:usu->getUrls()){
                        if(urls==url){
                            cout<<usu->getNombre()<<" tiene acceso al siguiente enlace.";
                            return;
                        }
                    }
                    cout<<usu->getNombre()<<" no tiene acceso al siguiente enlace.";
                    return;
                }
                cout<<"URL invalida.";
            return;
            }
            
        }
        cout<<"Usuario invalido.";
    }
    void visitarSitio(Usuario*user, string&URL){
        if(user->getTipo()=='A'){
            user->agregarVisita(URL);
        }
        if(user->getTipo()=='N'){
            for(auto vis:user->getUrls()){
                if(vis==URL){
                    user->agregarVisita(URL);
                }
            }
        }
    }
    void guardarAccesos(const char*URLAccesos){
        ofstream archi(URLAccesos);
        if(archi.fail())return;
        for(const auto&usu:usuarios){
            archi<<*usu;
        }
        archi.close();
    }
    void fiveUsersPerm(){
        vector<Usuario*>noAdmins;
        copy_if(usuarios.begin(),usuarios.end(),back_inserter(noAdmins),[](Usuario*a){
            return (a->getTipo()=='N');
        });
        sort(noAdmins.begin(),noAdmins.end(),[](Usuario*a,Usuario*b){
            return a->getUrls().size()<b->getUrls().size();
        });
        cout<<"5 usuarios con mas permisos:"<<endl;
        for(int i=0;i<5;i++){
            cout<<noAdmins[i]->getNombre()<<endl;
        }
    }
    void urlMostViewed(){
        map<string,int> URL;
        for(const auto&usu:usuarios){
            for(const auto&url:usu->getUrls()){
                URL[url]++;
            }
        }
        auto max=max_element(URL.begin(),URL.end(),[](pair<string,int>*a,pair<string,int>*b){
            return a->second<b->second;
        });
        cout<<"URL MAS VISTA:"<<max->first;
    }

    };
    template<class T>
    class Conjunto{
        private:
        struct elemento{
            T valor;
        };
        elemento*data;
        int cap;
        int tam;
        Conjunto(int c=4,conjunto*dat=nullptr):cap(c),data(dat),tam(0){
            elemento*data=new elemento[cap];
        }
        void agregarElemento(T dato){
            for(int i=0;i<tam;i++){
                if(data[i].valor==dato){
                    cout<<"Elemento existente.";
                    return;
                }
            }
            if(cap==tam){
                int newCap=cap*2;
                elemento*nuevoElemento=new elemento[newCap];
                for(int i=0;i<tam;i++){
                    nuevoElemento[i]=elemento[i];
                }
                delete[]elemento;
                elemento=nuevoElemento;
                cap=newCap;
            }
            data[tam].valor=dato;
            tam++;
        }
        void eliminarElemento(T dato){
            for(int i=0;i<tam;i++){
                if(data[i].valor==dato){
                    data[i]=data[tam-1];
                    tam--;
                    cout<<"Elemento "<<dato<<" eliminado.";
                    return;
                }
            }
            cout<<"Elemento no encontrado.";
        }
    };
    /*El operador que debe sobreescribir al tipo T para no admitir repetidos es ==.

        Es necesario usar polimorfismo cuando tengamos un sistema que utilice herencia, que tenga 
        objetos con atributos y metodos en comun entre si.
        Si, simplifica el codigo ya que nos permite hacer que un mismo metodo adopte diferentes comportamientos,
        podemos amoldarlo dependiendo la necesidad del sistema. Debido a esto esta tecnica nos ayuda a simplificar
        el codigo ya que reutilizamos el mismo.
        
    */
int main(int argc, char *argv[]) {
    
    
system("pause");
    return 0;
}