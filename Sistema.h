#ifndef __SISTEMA__H__
#define __SISTEMA__H__
#include<iostream>
#include <list>
#include <vector>
#include "Comando.h"
#include "Secuencia.h"

class Sistema {
  public:
    const std::vector<Comando>& ObtenerComandos() const;
    void FijarComandos(std::vector<Comando> ncomandos);
    std::list< Secuencia > & ObtenerSecuencias();
    void FijarSecuencias (std::list< Secuencia > secs);
    void cargar(std::string nombre_archivo);
    void listar_secuencias();
    void histograma(std::string descripcion_secuencia);
    void es_subsecuencia(std::string subsecuencia);
    void enmascarar(std::string subsecuencia);
    void guardar(std:: string nombre_archivo);
    void codificar(std:: string nombre_archivo);
    void decodificar(std::string nombre_archivo);
    void ruta_mas_corta(std::string descripcion_secuencia, int i, int j, int x, int y);
    void base_remota(std::string descripcion_secuencia, int i, int j); 
    std :: vector<char> verificarCodigosValidos(const std::string& linea);
  protected:
    std::list< Secuencia > secuencias;
    std::vector<Comando> comandos;
};

#endif // __SISTEMA__H__

// eof - Sistema.h
