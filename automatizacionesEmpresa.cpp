#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <numeric>
using namespace std; 
struct datosUsuario{
    char nombre[200];
    int codigo;
    char cargo[50];
};
struct datosCargo{
    char nombre[50];
    int cantidadPermisos;
    char permisos[10][50];
};
class Cargo{
    private:
    string nombre;
    vector<string>permisos;
    Cargo* cargoPadre;
    public:
    Cargo(string n,vector<string>per,Cargo*car=nullptr):nombre(n),permisos(per),cargoPadre(car){}
    string getNombre(){return;}
    vector<string> getPermisos(){ //incluimos la funcion de que se agreguen mas permisos derivados de otros
        vector<string> totalPermisos=permisos;
        if(cargoPadre){
            vector<string>Padre=cargoPadre->getPermisos();
            totalPermisos.insert(totalPermisos.end(),Padre.begin(),Padre.end());
        }
        return totalPermisos;
    }
    void añadirPermiso(Cargo*cargo,string permiso){
        vector<string> permi=getPermisos();
        permi.push_back(permiso);
    }
    datosCargo getDatos(){
        datosCargo datos;
        strcpy(datos.nombre,nombre.c_str());
        datos.cantidadPermisos=permisos.size();
        for(int i=0;i<datos.cantidadPermisos;i++){
            strcpy(datos.permisos[i],permisos[i].c_str());
        }
        return datos;
    }
    bool tienePermiso(string permiso){
        return find(permisos.begin(),permisos.end(),permiso) != permisos.end();
    }
};
class Usuario{
    private:
    string nombre;
    int codigo;
    Cargo* cargo;
    public:
    Usuario(string n, int cod, Cargo* car):nombre(n),codigo(cod),cargo(car){}
    string getNombre(){return nombre;}
    int getCodigo(){return codigo;}
    Cargo* getCargo(){return cargo;}
    datosUsuario getDatos(){
        datosUsuario datos;
        datos.codigo=codigo;
        strcpy(datos.nombre,nombre.c_str());
        strcpy(datos.cargo,cargo->getNombre().c_str());
        return datos;
    }
};
class Sistema{
    private:
    vector<Usuario*>usuarios;
    map<string,Cargo*>cargos;
    public:
    Sistema();
    void agregarUsuarios(Usuario*user){
        usuarios.push_back(user);
    }
    void agregarCargo(Cargo*cargo){
        cargos[cargo->getNombre()]=cargo;
    }
    void guardarUsers(const char*URLDatos,bool variosOuno){//verdadero uno, falso varios
        if(variosOuno){
        ofstream archi(URLDatos,ios::binary);
        if(archi.fail())return;
        for(auto &u:usuarios){
            datosUsuario datos=u->getDatos();
            archi.write(reinterpret_cast<char*>(&datos),sizeof(datosUsuario));
        }
        for(auto pair:cargos){
            datosCargo datos=pair.second->getDatos();
            archi.write(reinterpret_cast<char*>(&datos),sizeof(datosCargo));
        }
        archi.close();
        }else{
            for(auto users:usuarios){
                datosUsuario datos=users->getDatos();
                string archivo=users->getNombre()+".dat";
                ofstream archi(archivo,ios::binary);
                if(archi.fail())return;
                archi.write(reinterpret_cast<char*>(&datos),sizeof(datosUsuario));
                archi.close();
            }
            for(auto pair:cargos){
                datosCargo datos=pair.second->getDatos();
                string archivo=pair.second->getNombre()+".dat";
                ofstream archi(archivo,ios::binary);
                if(archi.fail())return;
                archi.write(reinterpret_cast<char*>(&datos),sizeof(datosCargo)); 
                archi.close();
            }
        }
    }
    void tienePermiso(int codigo,string permiso){
        Cargo* cargo;
        for(const auto&u:usuarios){
            if(u->getCodigo()==codigo){
                cargo=u->getCargo();
                break;
            }
        }
        if(cargo==nullptr)return;
        if(cargo->tienePermiso(permiso))
        cout<<"Tiene permiso de: "<<permiso;
        else
        cout<<"No tiene permiso de: "<<permiso;
    }
    void CargosMaxPermisos(){
        auto maxPerm=max_element(cargos.begin(),cargos.end(),[](auto&a,auto& b){
            return a.second->getPermisos().size() < b.second->getPermisos().size();
        })->second->getPermisos().size();
        cout<<"Cargos con mas permisos:"<<endl;
        for(auto pair:cargos){
            if(maxPerm==pair.second->getPermisos().size()){
                cout<<pair.first<<"."<<endl;
            }
        }
    }
    void permisosEnVariosCargos(){
        map<string,int>contador;
        for(auto pair:cargos){
            for(auto permis:pair.second->getPermisos()){
                contador[permis]++;
            }}
        cout<<"Permisos que estan en mas de 1 cargo:"<<endl;
        for(auto pair:contador){
            if(pair.second>1)cout<<pair.first<<"."<<endl;
            }
        
    }
    void usuariosMasPermisos(){
        sort(usuarios.begin(),usuarios.end(),[](Usuario*a,Usuario*b){
            return a->getCargo()->getPermisos().size() > b->getCargo()->getPermisos().size();
        });
        int maxPerm=usuarios[0]->getCargo()->getPermisos().size();
        cout<<"Usuarios con mas permisos:"<<endl;
        for(const auto&users:usuarios){
            if(maxPerm==users->getCargo()->getPermisos().size())
            cout<<users->getNombre()<<".";
        }
    }
};
int main(int argc, char *argv[]) {
    
    
system("pause");
    return 0;
}