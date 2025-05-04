#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <numeric>
using namespace std; 
struct datosInvestigacion{
    int id;
    char titulo[20];
    char tema[20];
    char url[30];
};
struct datosRelacion{
    int id1;
    int id2;
};
class Investigacion{
    private:
    vector<Investigacion*>relaciones;
    int id;
    string titulo, tema, url;
    public:
    Investigacion(int ID,string titu,string tem,string ur):id(ID),titulo(titu),tema(tem),url(ur){}
    datosInvestigacion getDatos(){
        datosInvestigacion datos;
        datos.id=id;
        strcpy(datos.titulo,titulo.c_str());
        strcpy(datos.tema,tema.c_str());
        strcpy(datos.url,url.c_str());
        return datos;
    }
    void addRelacion(Investigacion*a){
        relaciones.push_back(a);
    }
    vector<Investigacion*> getRelaciones(){return relaciones;}
    int getID(){return id;}
    friend ostream& operator<<(ostream&os, Investigacion&inv){
        os<<inv.id<<":"<<inv.titulo<<'-'<<inv.url;
        return os;
    }
    string getTema(){return tema;}
    string getTitulo(){return titulo;}
    string getUrl(){return url;}
};
class Catalogo{
    private:
    vector<Investigacion*>investigaciones;
    public:
    Catalogo(){}
    void guardarInvestigaciones(const char*URL){
        ofstream archi(URL,ios::binary);
        if(archi.fail())return;
        datosInvestigacion datos;
        for(const auto&inv:investigaciones){
            datos=inv->getDatos();
            archi.write(reinterpret_cast<char*>(&datos),sizeof(datosInvestigacion));
        }
        archi.close();
    }
    void guardarRelaciones(const char*URL){
        ofstream archi(URL,ios::binary);
        if(archi.fail())return;
        for(auto inv:investigaciones){
            for(auto rel:inv->getRelaciones()){
                datosRelacion datos;
                datos.id1=inv->getID();
                datos.id2=rel->getID();
                archi.write(reinterpret_cast<char*>(&datos),sizeof(datosRelacion));
            }
        }
        archi.close();
    }
    void addInv(Investigacion*a){
        investigaciones.push_back(a);
    }
    void guardarInvestigacionesTXT(string URL){
        ofstream archi(URL);
        if(archi.fail())return;
        for(const auto& inv:investigaciones){
            archi<<inv<<endl;
            int niv=0;
            for(auto rel:inv->getRelaciones()){
                for(int i=0;i<niv;i++){
                    archi<<"  ";
                }
                archi<<"|->Investigacion relacionada: "<<rel;
                niv++;
            }
        }
        archi.close();
    }
    void topCincoTemas(){
        map<string,int>contadorInv;
        for(auto inv:investigaciones){
            contadorInv[inv->getTema()]++;
        }
        vector<pair<string,int>> temas(contadorInv.begin(),contadorInv.end());
        sort(temas.begin(),temas.end(),[](pair<string,int>*a,pair<string,int>*b){
            return a->second < b->second;
        });
        int corte=0;
        for(auto tema:temas){
            cout<<tema.first<<endl;
            if(++corte==4)break;
        }
    }
    void videosNoRel(){
        vector<Investigacion*>videos;
        copy_if(investigaciones.begin(),investigaciones.end(),back_inserter(videos),[](Investigacion*a){
            return a->getTema()=="Video";
        });
        cout<<"Videos sin investigaciones relacionadas:"<<endl;
        for(auto video:videos){
            if(video->getRelaciones().empty()){
                cout<<video->getTitulo()<<endl;
            }
        }
    }
    void urlRep(){
        map<string,int>contador;
        for(auto inv:investigaciones){
            contador[inv->getUrl()]++;
        }
        cout<<"URLS que se repiten:"<<endl;
        for(auto pair:contador){
            if(pair.second>1){
                cout<<pair.first<<endl;
            }
        }
    }
};
/*
El uso de templates es necesario debido a que estas nos permiten
generalizar el codigo y ahorrarnos escritura del mismo.
Se utilizan cuando tenemos elementos que se comportan igual pero que
tienen diferente tipado en variables.
Al ser codigo generalizado lo que podrian ser varios metodos
lo optimizamos con solo un template(o los que necesitemos). A su vez,
esta tecnica es inflexible y presenta posibles problemas a futuro si es que
necesitamos editar el codigo y los metodos existentes.
*/
int main(int argc, char *argv[]) {
    
    
system("pause");
    return 0;
}