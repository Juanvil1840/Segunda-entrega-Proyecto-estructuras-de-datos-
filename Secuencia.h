#ifndef __SECUENCIA__H__
#define __SECUENCIA__H__

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cstddef>

struct orden{
    bool operator()(char a, char b) const {
	std::string orden = "ACGTURYKMSWBDHVNX-";
	std::size_t posicion_a = orden.find(a);
	std::size_t posicion_b = orden.find(b);
	return posicion_a < posicion_b;
    }
};

class Secuencia {
public:
    Secuencia();
    virtual ~Secuencia();
    std::string ObtenerDescripcion() const;
    std::vector<std::string>& ObtenerLineasSecuencia();
    const std::map<char, int, orden>& ObtenerFrecuencias() const;
    void FijarDescripcion(const std::string &ndescripcion);
    void FijarLineasSecuencia(const std::vector<std::string> &n_lineas_secuencia);
    void AgregarLineaSecuencia(const std::string &linea);
    void EstablecerCodigosYFrecuencias();

private:
    std::string descripcion;
    std::vector<std::string> lineas_secuencia;
    std::map<char, int,orden> frecuenciaCodigos;
};

#endif
