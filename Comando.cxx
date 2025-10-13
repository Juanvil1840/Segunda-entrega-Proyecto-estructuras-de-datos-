#include "Comando.h"

// constructor 
Comando::Comando(){
    nombre = "";
    entrada = 0;
    uso = "";
}

// obtener nombre
std::string Comando::ObtenerNombre() const {
  return( nombre );
}

// obtener entrada
int Comando::ObtenerEntrada() const {
  return( entrada );
}

// obtener uso
std::string Comando::ObtenerUso() const {
  return( uso );
}

// fijar nombre
void Comando::FijarNombre(std::string name) {
  this->nombre = name;
}

// fijar entrada
void Comando::FijarEntrada(int num) {
  this->entrada = num;
}

// fijar uso
void Comando::FijarUso(std::string usage) {
  this->uso = usage;
}
