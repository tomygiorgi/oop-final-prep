#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <numeric>
using namespace std; 
struct datosPregunta{
   char pregunta[50];
   int cantRespuestas;
   char respuestas[10][50]; 
};
struct datosRespuesta{
    char respuesta[50];
    char preguntaEnlazada[50];
};
class Respuesta{
    private:
    Pregunta* preguntaEnlazada;
    string respuesta;
    public:
    Respuesta(string resp, Pregunta*pre=nullptr):respuesta(resp),preguntaEnlazada(pre){}
    string getRespuesta(){return respuesta;}
    Pregunta* getPregEnl(){return preguntaEnlazada;}
    datosRespuesta getDatos(){
        datosRespuesta datos;
        strcpy(datos.respuesta,respuesta.c_str());
        strcpy(datos.preguntaEnlazada,preguntaEnlazada->getPregunta().c_str());
        return datos;
    }
    friend ostream& operator<<(ostream& os,const Respuesta& resp){
        os<<resp.respuesta;
        if(resp.preguntaEnlazada!=nullptr){
            os<<resp.preguntaEnlazada->getPregunta();
        }
        return os;
    }

};
class Pregunta{
    private:
    string pregunta;
    vector<Respuesta*>respuestas;
    public:
    Pregunta(string pre,vector<Respuesta*>vec):pregunta(pre),respuestas(vec){}
    string getPregunta(){return pregunta;}
    vector<Respuesta*> getRespuestas(){return respuestas;}
    void addRespuesta(Respuesta*resp){
        respuestas.push_back(resp);
    }
    datosPregunta getDatos(){
        datosPregunta datos;
        strcpy(datos.pregunta,pregunta.c_str());
        datos.cantRespuestas=respuestas.size();
        for(int i=0;i<datos.cantRespuestas;i++){
            strcpy(datos.respuestas[i],respuestas[i]->getRespuesta().c_str());
        }
        return datos;
    }
    friend ostream& operator<<(ostream& os,const Pregunta&preg){
        os<<preg.pregunta;
        for(const auto&resp:preg.respuestas){
            os<<*resp;
        }
        return os;
    }
};
class Encuesta{
    private:
    vector<Pregunta*>preguntas;
    public:
    Encuesta(){}
    void guardarPreguntasResp(const char*URLPreguntas){
        ofstream archi(URLPreguntas,ios::binary);
        if(archi.fail())return;
        for(const auto&preg:preguntas){
            datosPregunta datos=preg->getDatos();
            vector<Respuesta*>vec=preg->getRespuestas();
            size_t cantResp=vec.size();
            archi.write(reinterpret_cast<char*>(&datos),sizeof(datosPregunta));
        }
        archi.close();
    }
    void guardarSobrecargando(const char* archivo){
        ofstream archi(archivo);
        if(archi.fail())return;
        for(const auto&preg:preguntas){
            archi<<*preg;
        }
        archi.close();
    }
    void addPreguntas(Pregunta*pregunta){
        preguntas.push_back(pregunta);
    }
    void ResponderEncuesta(){
        for(const auto&preg:preguntas){
            int nivel=1;
            string respuestA;
            cout<<"Pregunta "<<nivel<<endl;
            cout<<preg<<endl<<"Opciones:"<<endl;
            vector<Respuesta*>vec=preg->getRespuestas();
            for(const auto&resp:vec){
                cout<<resp->getRespuesta()<<endl;
            }
            cout<<"Responda:";
            cin>>respuestA;
            bool noSigue=true;Pregunta*pregExtra;
            for(const auto&resp:vec){
            if(respuestA==resp->getRespuesta()){
                if(resp->getPregEnl()){pregExtra=resp->getPregEnl();noSigue=false;}
            }
        }     
            do{
                noSigue=false;
                cout<<pregExtra;
                for(auto resp:pregExtra->getRespuestas()){
                    cout<<resp;
                }
                cin>>respuestA;
                for(auto resp:pregExtra->getRespuestas()){
                    if(respuestA==resp->getRespuesta() and resp->getPregEnl())noSigue=false;
                    else noSigue=true;
                }
                
            }while(noSigue);
            nivel++;
        }
    }
    void pregMasRespuestas(){

        sort(preguntas.begin(),preguntas.end(),[](Pregunta* a, Pregunta*b){
            return a->getRespuestas().size()>b->getRespuestas().size();
        });
        auto maxResp=preguntas[0]->getRespuestas().size();
        cout<<"Preguntas con mas respuestas:"<<endl;
        for(const auto&vec:preguntas){
            if(maxResp==vec->getRespuestas().size()){
            cout<<vec->getPregunta()<<"."<<endl;
            }
        }
    }
    void respEncadenadas(){
        map<string,int>respEnc;
        vector<Respuesta*> respuestas;
        for(const auto&preg:preguntas){
            respuestas=preg->getRespuestas();
            for(const auto&resp:respuestas){
                if(resp->getPregEnl())
                respEnc[resp->getRespuesta()]++;
            }
        }
        cout<<"Respuestas con preguntas encadenadas:";
        for(auto &pair:respEnc){
            cout<<pair.first<<endl;
        }
    }
};




int main(int argc, char *argv[]) {
    
    
system("pause");
    return 0;
}