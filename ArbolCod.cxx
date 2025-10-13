#include "ArbolCod.h"
#include <queue>
#include <vector>

ArbolCod::ArbolCod(){
    this->raiz = NULL;
}

ArbolCod::~ArbolCod(){
    if(!this->esVacio()){
        delete this->raiz;
        this->raiz = NULL;
    }
}

bool ArbolCod::esVacio(){
    return this->raiz == NULL;
}

struct comparacion{
    bool operator()(NodoCod* a, NodoCod* b)const{
	return a->obtenerFrecuencia() > b->obtenerFrecuencia();
    }
};

void ArbolCod::Huffman(std::map<char,int>& codigos){
    if(codigos.empty()){
	this->raiz = NULL;
	return;
    }
	
    int n = codigos.size();
    std::priority_queue<NodoCod*, std::vector<NodoCod*>, comparacion> cpmin;

    std::map<char,int>::iterator it;

    for(it=codigos.begin(); it!= codigos.end(); it++){
	cpmin.push( new NodoCod(it->first,it->second) );
    }

    for(int i=1; i<= n-1; i++){
	NodoCod* z = new NodoCod();
	z->fijarHijoIzq(cpmin.top());
	cpmin.pop();
	z->fijarHijoDer(cpmin.top());
	cpmin.pop();
	z->fijarFrecuencia((z->obtenerHijoIzq()->obtenerFrecuencia()) + (z->obtenerHijoDer()->obtenerFrecuencia()));
	z->fijarDato('\0');

	cpmin.push(z);
    }

    this->raiz = cpmin.top();
}

std::string ArbolCod::codificacion(std::string frase){
    if(this->esVacio()){
	return "x";
    }else{
	std::string fraseCod;
	for(size_t i=0; i < frase.size(); i++){
	    this->raiz->codigoCar(frase[i],fraseCod);
	}

	return fraseCod;
    }
}

std::string ArbolCod::decodificacion(std::string fraseCod){
    if(this->esVacio()){
	return "";
    }else{
	NodoCod* nodo = this->raiz;
	std::string fraseDecod;
	
	for(size_t i = 0; i < fraseCod.size(); i++){
	    if(fraseCod[i] == '0'){
		nodo = nodo->obtenerHijoIzq();
	    }else if(fraseCod[i] == '1'){
		nodo = nodo->obtenerHijoDer();
	    }else{
		return std::string("El codigo ") + fraseCod[i] + std::string(" no es valido");
	    }

	    if(nodo==NULL){
		return "Secuencia invalida para el arbol cargado en el sistema";
	    }

	    if(nodo->esHoja()){
	        fraseDecod.push_back(nodo->obtenerDato());
	    	nodo = this->raiz;
	    }
	}

	if(nodo!=this->raiz) {
	    return "Codigo incompleto";    
	}
	
	return fraseDecod;
    }

}
