#ifndef __COMANDO__H__
#define __COMANDO__H__
#include<iostream>

class Comando {
  public:
    Comando ();
    std:: string ObtenerNombre() const;
    int ObtenerEntrada() const;
    std:: string ObtenerUso() const;
    void FijarNombre(std::string name);
    void FijarEntrada(int num);
    void FijarUso(std::string usage); 
  protected:
    std::string nombre;
    int entrada;
    std::string uso;
};

#endif // __COMANDO__H__

// eof - Comando.h
