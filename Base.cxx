#include "Base.h"

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

bool operator==(const Base& b) const{
    return codigo == b.obtenerCodigo() && i == b.obteneri() && j == b.obtenerj();
}

bool operator<(const Base& b) const{
    return (i < b.obteneri()) || (i == b.obteneri() && j < b.obtenerj());
}
