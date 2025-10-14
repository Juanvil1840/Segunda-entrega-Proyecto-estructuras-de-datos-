#ifndef __SECUENCIA__H__
#define __SECUENCIA__H__

#include <string>
#include <vector>
#include <map>
#include <iostream>
  
class Secuencia {
public:
    Secuencia();
    virtual ~Secuencia();

    // Descripción
    std::string ObtenerDescripcion() const;
    void FijarDescripcion(const std::string &ndescripcion);

    // Lineas de secuencia
    std::vector<std::string>& ObtenerLineasSecuencia();
    void FijarLineasSecuencia(const std::vector<std::string> &n_lineas_secuencia);
    void AgregarLineaSecuencia(const std::string &linea);

    // Interfaz antigua 
    int ObtenerNumbases() const;          // total de bases (suma de frecuencias)
    int ObtenerNumcodigos() const;        // cantidad de códigos distintos
    std::vector<char>& ObtenerCodigos();  // lista de códigos (orden variable hasta ordenar)

    void FijarNumbases(int n_numbases);
    void FijarNumcodigos(int n_numcodigos);
    void FijarCodigos(const std::vector<char> &nCodigos);

    // Metodos nuevos 
    void EstablecerCodigosYBases();   // llena frecuenciaCodigos, codigos, numbases, numcodigos
    void OrdenarCodigosYBases();      // ordena codigos (según orden lexicográfico por defecto)
    void MostrarFrecuencias() const;  // imprime frecuencias (firma const: requerida por proyecto)
    bool VerificarCodigosValidos() const;

protected:
    std::string descripcion;
    std::vector<std::string> lineas_secuencia;

    // Estructura principal de frecuencias (k -> frecuencia)
    std::map<char, int> frecuenciaCodigos;

    // Campos mantenidos por compatibilidad con la interfaz previa
    std::vector<char> codigos; // lista de claves (copia de las claves del map)
    int numbases;              // suma de todas las frecuencias
    int numcodigos;            // codigos.size()
};

#endif // __SECUENCIA__H__
