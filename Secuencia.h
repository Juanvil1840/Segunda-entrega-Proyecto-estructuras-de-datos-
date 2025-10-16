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

    // Información general
    std::string ObtenerDescripcion() const;
    void FijarDescripcion(const std::string &ndescripcion);

    // Manejo de las líneas de la secuencia
    std::vector<std::string>& ObtenerLineasSecuencia();
    void FijarLineasSecuencia(const std::vector<std::string> &n_lineas_secuencia);
    void AgregarLineaSecuencia(const std::string &linea);

    // Métodos relacionados con las bases
    void EstablecerCodigosYBases();  // Calcula frecuencias desde las líneas
    void MostrarFrecuencias() const; // Imprime las frecuencias
    bool VerificarCodigosValidos() const;

    // === NUEVA interfaz basada en map ===
    int ObtenerNumbases() const;                  // Suma total de frecuencias
    int ObtenerNumcodigos() const;                // Cantidad de claves únicas
    std::vector<char> ObtenerCodigos() const;     // Devuelve las claves
    const std::map<char, int>& ObtenerFrecuencias() const; // Acceso al mapa completo

private:
    std::string descripcion;
    std::vector<std::string> lineas_secuencia;
    std::map<char, int> frecuenciaCodigos;  // Base principal de datos
};

#endif
