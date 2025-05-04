#include <iostream>
using namespace std; 
template<class k,class v>
class miMapa{
    private:
    struct mapa{
        k key;
        v value;
    };
    int cap;
    int tam;
    mapa*data;
    public:
    miMapa(int c=4):cap(c),tam(0){
        data=new mapa[cap];
    }
    void add(k&KEY,v&VALUE){
        for(int i=0;i<tam;i++){
            if(data[i].key==KEY)
            return;
        }
        if(tam==cap){
            int newCap=cap*2;
            mapa*nuevaData=new mapa[newCap];
            for(int i=0;i<tam;i++){
                nuevaData[i]=data[i];
            }
            delete[]data;
            data=nuevaData;
            cap=newCap;
        }
        data[tam].key=KEY;
        data[tam].value=VALUE;
        tam++;
    }
    v& getValue(const k&KEY){
        for(int i=0;i<tam;i++){
            if(data[i].key==KEY){
                return data[i].value;
            }
        }
        return;
    }
    void eliminar(const k&KEY){
        for(int i=0;i<tam;i++){
            if(data[i].key==KEY){
                data[i]=data[tam-1];
                tam--;
                return;
            }
        }
        cout<<"Clave invalida.";
    }
};
template<class h>
class Cola{
    private:
    struct nodo{
        h dato;
        nodo*link;
    };
    nodo*frente;
    nodo*fondo;
    int tam;
    public:
    Cola():tam(0),frente(nullptr),fondo(nullptr){}
    void agregarCola(const h& valor){
        nodo*nuevoNodo=new nodo{valor,nullptr};
        if(tam==0){
            frente=fondo=nuevoNodo;
        }else{
            fondo->link=nuevoNodo;
            fondo=nuevoNodo;
        }
        tam++;
    }
    h obtener(){
        nodo*tmp=frente;
        frente=frente->link;
        h dato=tmp->dato;
        delete tmp;
        tam--;
        if(frente==fondo)
        fondo=nullptr;
        return dato;
    }
};
template<class T>
class arregloDinamico{
    struct nodo{
        T valor;
    };
    nodo* data;
    int size;
    int cap;
    public:
    arregloDinamico(int c=4):cap(c),size(0){
        data=new nodo[cap];
    }
    void agregar(T dato){
        if(size==cap){
            int newCap=cap*2;
            nodo*nuevoNodo=new nodo[newCap];
            for(int i=0;i<size;i++)
            nuevoNodo[i]=data[i];
            delete[]data;
            data=nuevoNodo;
            cap=newCap;
        }
        data[size].valor=dato;
        size++;
        for(int i=0;i<size-1;i++){
            for(int j=1;j<size;j++){
                if(data[i].valor<data[j].valor){
                    T aux=data[i].valor;
                    data[i].valor=data[j].valor;
                    data[j].valor=aux;
                }
            }
        }
    }
    void eliminarIndice(const int&indice){
            for(int i=0;i<size;i++){
                if(i+1==indice){
                    data[i]=data[size-1];
                    --size;
                    return;
                }
            }
            cout<<"Indice invalido.";
    }
    void imprimir(){
        for(int i=0;i<size;i++){
            cout<<data[i].valor<<" ";
        }
    }

};




int main(int argc, char *argv[]) {
    
    
system("pause");
    return 0;
}