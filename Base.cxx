#include "Base.h"
#include <math.h>

Base::Base(){
    codigo = ' ';
    i = -1;
    j = -1;
}

Base::Base(char ncodigo, int ni, int nj){
    codigo = ncodigo;
    i = ni;
    j = nj;
}

char Base::obtenerCodigo(){
  return codigo;
}

int Base::obteneri(){
  return i;
}

int Base::obtenerj(){
  return j;
}

void Base::fijarCodigo(char ncodigo){
  codigo = ncodigo;
}

void Base::fijari(int ni){
  i = ni;
}

void Base::fijarj(int nj){
  j = nj;
}

bool Base::operator==(const Base& b) const{
    return codigo == b.obtenerCodigo() && i == b.obteneri() && j == b.obtenerj();
}

bool Base::operator<(const Base& b) const{
    return (i < b.obteneri()) || (i == b.obteneri() && j < b.obtenerj());
}

double Base::pesoConexion(Base b){
    int ascii1 = (int)(codigo);
    int ascii2 = (int)(b.obtenerCodigo());

   return 1.0/(1.0 + std:abs(ascii1 - ascii2);
}