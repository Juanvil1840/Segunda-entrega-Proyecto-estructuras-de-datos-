#include "Secuencia.h"
#include <cctype>
#include <algorithm>

Secuencia::Secuencia() : descripcion(""), lineas_secuencia(), frecuenciaCodigos() {}
Secuencia::~Secuencia() {}

//  DESCRIPCIÓN

std::string Secuencia::ObtenerDescripcion() const {
    return descripcion;
}

void Secuencia::FijarDescripcion(const std::string &ndescripcion) {
    descripcion = ndescripcion;
}

//  LÍNEAS DE SECUENCIA 

std::vector<std::string>& Secuencia::ObtenerLineasSecuencia() {
    return lineas_secuencia;
}

void Secuencia::FijarLineasSecuencia(const std::vector<std::string> &n_lineas_secuencia) {
    lineas_secuencia = n_lineas_secuencia;
    EstablecerCodigosYBases();
}

void Secuencia::AgregarLineaSecuencia(const std::string &linea) {
    lineas_secuencia.push_back(linea);

    // Actualizar frecuencias carácter por carácter
    for (size_t i = 0; i < linea.size(); ++i) {
        char c = linea[i];
        frecuenciaCodigos[c] = frecuenciaCodigos[c] + 1;
    }
}

//  MANEJO DE FRECUENCIAS

// Recalcula todas las frecuencias desde las líneas
void Secuencia::EstablecerCodigosYBases() {
    frecuenciaCodigos.clear();

    for (size_t i = 0; i < lineas_secuencia.size(); ++i) {
        const std::string &linea = lineas_secuencia[i];
        for (size_t j = 0; j < linea.size(); ++j) {
            char c = linea[j];
            frecuenciaCodigos[c] = frecuenciaCodigos[c] + 1;
        }
    }
}

// Devuelve el total de bases (suma de frecuencias)
int Secuencia::ObtenerNumbases() const {
    int total = 0;

    std::map<char, int>::const_iterator it;
    for (it = frecuenciaCodigos.begin(); it != frecuenciaCodigos.end(); ++it) {
        total += it->second;
    }
    return total;
}

// Devuelve el número de códigos distintos
int Secuencia::ObtenerNumcodigos() const {
    return static_cast<int>(frecuenciaCodigos.size());
}

// Devuelve un vector con los códigos únicos (claves del mapa)
std::vector<char> Secuencia::ObtenerCodigos() const {
    std::vector<char> v;
    v.reserve(frecuenciaCodigos.size());

    std::map<char, int>::const_iterator it;
    for (it = frecuenciaCodigos.begin(); it != frecuenciaCodigos.end(); ++it) {
        v.push_back(it->first);
    }
    return v;
}

// Devuelve referencia al mapa de frecuencias
const std::map<char, int>& Secuencia::ObtenerFrecuencias() const {
    return frecuenciaCodigos;
}

// Imprime las frecuencias de todos los códigos
void Secuencia::MostrarFrecuencias() const {
    std::cout << "\nFrecuencias de bases:\n";

    std::map<char, int>::const_iterator it;
    for (it = frecuenciaCodigos.begin(); it != frecuenciaCodigos.end(); ++it) {
        std::cout << " " << it->first << " : " << it->second << "\n";
    }
}

// Verifica que todos los códigos sean válidos (solo letras o '-')
bool Secuencia::VerificarCodigosValidos() const {
    std::map<char, int>::const_iterator it;
    for (it = frecuenciaCodigos.begin(); it != frecuenciaCodigos.end(); ++it) {
        char c = it->first;
        if (!std::isalpha(static_cast<unsigned char>(c)) && c != '-') {
            return false;
        }
    }
    return true;
}
