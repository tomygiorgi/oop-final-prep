#include <iostream>
using namespace std; 

//okey el primero es hacer un map pero sin stl o sea vamos a utilizar template
//para la tupla de valores, utilizando el conocimiento de estructuras de datos para poder crear un arreglo dinamico
//que simule un map de la biblioteca STL
template<class K,class V>
class miMapa{
    private:
        struct mapa{
            K clave;
            V valor;
        };
    mapa*data;
    int tam;
    int cap;
    public:
    miMapa(int c=4):cap(c),tam(0){
        data=mapa[cap];
    }
    void agregar(K&c,V&v){
        for(int i=0;i<cap;i++){
            if(data[i]->clave==c)
            return;
        }
        if(cap==tam){
            int newCap=cap*2;
            mapa*nuevaData=new mapa[newCap];
            for(int i=0;i<tam;i++){
                nuevaData[i]=data[i];
            }
            delete[]data;
            data=nuevaData;
            cap=newCap;
        }
        data[tam].valor=v;
        data[tam].clave=c;
        tam++;
    }
    V& obtener(K&key){
        for(int i=0;i<tam;i++){
            if(data[i].clave=key)
            return data[i].valor;
            else
        }
        cout<<"No se encontro la clave.";
    }
    void eliminar(const K&key){
        for(int i=0;i<tam;i++){
            if(data[i].clave==key){
                data[i]=data[tam-1];
                --tam;
                return;
            }
        }
        cout<<"Clave no encontrada.";
    }
}; 
    template<class H>
    class Cola{
        private:
        struct nodo{
            H dato;
            nodo*link;
        };
        nodo*frente;
        nodo*fondo;
        int size;
        public:
        Cola():frente(nullptr),fondo(nullptr),size(0){}
        void agregar(const &H valor){
            nodo*nuevoNodo=new nodo{valor,nullptr};
            if(size==0){
                frente=fondo=nuevoNodo;
            }else{
                fondo->link=nuevoNodo;
                fondo=nuevoNodo;
            }
            size++;
        }
        H& obtener(){
            if(size==0)return;
            H data=frente->dato;
            nodo*tmp=frente;
            frente=frente->link;
            delete tmp;
            size--;
            if(size==0){
                fondo=nullptr;
            }
            return H data;
        }

    };
int main(int argc, char *argv[]) {
    
    
system("pause");
    return 0;
}