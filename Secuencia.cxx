#include "Secuencia.h"
#include <cctype>
#include <algorithm>

Secuencia::Secuencia() : descripcion(""), lineas_secuencia(), frecuenciaCodigos() {}
Secuencia::~Secuencia() {}

std::string Secuencia::ObtenerDescripcion() const {
    return descripcion;
}

std::vector<std::string>& Secuencia::ObtenerLineasSecuencia() {
    return lineas_secuencia;
}

const std::map<char, int, orden>& Secuencia::ObtenerFrecuencias() const{
    return frecuenciaCodigos;
}

void Secuencia::FijarDescripcion(const std::string &ndescripcion) {
    descripcion = ndescripcion;
}

void Secuencia::FijarLineasSecuencia(const std::vector<std::string> &n_lineas_secuencia) {
    lineas_secuencia = n_lineas_secuencia;
}

void Secuencia::AgregarLineaSecuencia(const std::string &linea) {
    lineas_secuencia.push_back(linea);
}


void Secuencia::EstablecerCodigosYFrecuencias(){
    //Recorrer las lineas de secuencias
    std::vector< std::string >::iterator itL;
    std::map<char,int>::iterator itM;

    for(itL = this->ObtenerLineasSecuencia().begin(); itL != this->ObtenerLineasSecuencia().end(); itL ++){
        //Recorrer la cadena de caracteres
        for(char c: *itL){
	itM = frecuenciaCodigos.find(c);
	    if(itM != frecuenciaCodigos.end()){
		++(itM->second); 
	    }else{
		frecuenciaCodigos[c]=1;
	    }
	}
    }

}


