#include "NodoCod.h"
#include <iostream>
#include <algorithm> 

NodoCod::NodoCod(){
    this->hijoIzq = NULL;
    this->hijoDer = NULL;
    this->dato = '\0';
}

NodoCod::NodoCod(char dato, int frecuencia){
    this->hijoIzq = NULL;
    this->hijoDer = NULL;
    this->dato = dato;
    this->frecuencia = frecuencia;
}

NodoCod::~NodoCod() {
    if(this->hijoIzq != NULL){
	delete this->hijoIzq;
	this->hijoIzq = NULL;
    }

    if(this->hijoDer != NULL){
	delete this->hijoDer;
	this->hijoDer = NULL;
    }
}

char& NodoCod::obtenerDato(){
    return this->dato;
}

void NodoCod::fijarDato(char val) {
    this->dato = val;
}

int& NodoCod::obtenerFrecuencia(){
    return this->frecuencia;
}

void NodoCod::fijarFrecuencia(int freq){
    this->frecuencia = freq;
}

NodoCod* NodoCod::obtenerHijoIzq(){
    return this->hijoIzq;
}

NodoCod* NodoCod::obtenerHijoDer(){
    return this->hijoDer;
}

void NodoCod::fijarHijoIzq(NodoCod* izq){
    this->hijoIzq = izq;
}

void NodoCod::fijarHijoDer(NodoCod* der){
    this->hijoDer = der;
}

bool NodoCod::esHoja() {
  return ((this->hijoIzq == NULL) && (this->hijoDer == NULL));
}

bool NodoCod::codigoCar(char c, std::string& fraseCod){
    if(this->obtenerDato() == c){
	return true;
    }

    if(this->hijoIzq != NULL){
	fraseCod.push_back('0');
	if((this->hijoIzq)->codigoCar(c,fraseCod)) return true;
	fraseCod.pop_back();
    }

    if(this->hijoDer != NULL){
	fraseCod.push_back('1');
	if((this->hijoDer)->codigoCar(c,fraseCod)) return true;
	fraseCod.pop_back();
    }

    return false;
}
