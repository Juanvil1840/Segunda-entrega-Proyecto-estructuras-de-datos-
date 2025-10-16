#include "Secuencia.h"
#include <cctype>
#include <algorithm>

// Constructor / Destructor
Secuencia::Secuencia() : descripcion(""), lineas_secuencia(), frecuenciaCodigos() {}
Secuencia::~Secuencia() {}

// Descripción
std::string Secuencia::ObtenerDescripcion() const {
    return descripcion;
}

void Secuencia::FijarDescripcion(const std::string &ndescripcion) {
    descripcion = ndescripcion;
}

// Lineas
std::vector<std::string>& Secuencia::ObtenerLineasSecuencia() {
    return lineas_secuencia;
}

void Secuencia::FijarLineasSecuencia(const std::vector<std::string> &n_lineas_secuencia) {
    lineas_secuencia = n_lineas_secuencia;
    EstablecerCodigosYBases();
}

void Secuencia::AgregarLineaSecuencia(const std::string &linea) {
    lineas_secuencia.push_back(linea);
    for (char c : linea) {
        frecuenciaCodigos[c]++;
    }
}

// ==================== NUEVO MAP ====================

void Secuencia::EstablecerCodigosYBases() {
    frecuenciaCodigos.clear();
    for (const auto &line : lineas_secuencia) {
        for (char c : line) {
            frecuenciaCodigos[c]++;
        }
    }
}

int Secuencia::ObtenerNumbases() const {
    int total = 0;
    for (const auto &p : frecuenciaCodigos)
        total += p.second;
    return total;
}

int Secuencia::ObtenerNumcodigos() const {
    return static_cast<int>(frecuenciaCodigos.size());
}

std::vector<char> Secuencia::ObtenerCodigos() const {
    std::vector<char> v;
    v.reserve(frecuenciaCodigos.size());
    for (const auto &p : frecuenciaCodigos)
        v.push_back(p.first);
    return v;
}

const std::map<char, int>& Secuencia::ObtenerFrecuencias() const {
    return frecuenciaCodigos;
}

void Secuencia::MostrarFrecuencias() const {
    std::cout << "\nFrecuencias de bases:\n";
    for (const auto &p : frecuenciaCodigos)
        std::cout << " " << p.first << " : " << p.second << "\n";
}

bool Secuencia::VerificarCodigosValidos() const {
    for (const auto &p : frecuenciaCodigos) {
        char c = p.first;
        if (!std::isalpha(static_cast<unsigned char>(c)) && c != '-') return false;
    }
    return true;
}
