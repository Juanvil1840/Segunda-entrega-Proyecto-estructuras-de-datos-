#ifndef __SECUENCIA__H__
#define __SECUENCIA__H__
#include<iostream>
#include <vector>

class Secuencia {
  public:
    Secuencia();
    virtual ~Secuencia();
    std::string& ObtenerDescripcion();
    std::vector< std::string >& ObtenerLineasSecuencia();
    int ObtenerNumbases();
    int ObtenerNumcodigos();
    std::vector< char >& ObtenerCodigos();
    void FijarDescripcion(std::string ndescripcion);
    void FijarLineasSecuencia(std::vector< std::string > n_lineas_secuencia );
    void FijarNumbases(int n_numbases);
    void FijarNumcodigos(int n_numcodigos);
    void FijarCodigos(std::vector< char > nCodigos);
    void AgregarLineaSecuencia(std::string linea);
    void EstablecerCodigosYBases();
    void OrdenarCodigosYBases();
    bool VerificarCodigosValidos();

  protected:
    std::string descripcion;
    std::vector< std::string > lineas_secuencia;
    int numbases;
    int numcodigos;
    std::vector< char > codigos;

};

#endif // __SISTEMA__H__

// eof - Sistema.h
