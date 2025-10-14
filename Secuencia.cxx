#include "Secuencia.h"
#include <cctype>
#include <algorithm>

// Constructor / Destructor
Secuencia::Secuencia()
    : descripcion(), lineas_secuencia(), frecuenciaCodigos(),
      codigos(), numbases(0), numcodigos(0) {}

Secuencia::~Secuencia() {}

// Descripción
std::string Secuencia::ObtenerDescripcion() const {
    return descripcion;
}

void Secuencia::FijarDescripcion(const std::string &ndescripcion) {
    descripcion = ndescripcion;
}

// Lineas de secuencia
std::vector<std::string>& Secuencia::ObtenerLineasSecuencia() {
    return lineas_secuencia;
}

void Secuencia::FijarLineasSecuencia(const std::vector<std::string> &n_lineas_secuencia) {
    lineas_secuencia = n_lineas_secuencia;
    // recalcular frecuencias y campos derivados
    EstablecerCodigosYBases();
}

// Agregar linea (actualiza frecuencias incrementalmente)
void Secuencia::AgregarLineaSecuencia(const std::string &linea) {
    lineas_secuencia.push_back(linea);
    // actualizar mapa de frecuencias
    for (char c : linea) {
        frecuenciaCodigos[c] += 1;
        ++numbases;
    }
    // reconstruir vector codigos y numcodigos
    codigos.clear();
    for (const auto &p : frecuenciaCodigos) codigos.push_back(p.first);
    numcodigos = static_cast<int>(codigos.size());
}

// --- Interfaz antigua (compatibilidad con código existente) ---

int Secuencia::ObtenerNumbases() const {
    return numbases;
}

int Secuencia::ObtenerNumcodigos() const {
    return numcodigos;
}

std::vector<char>& Secuencia::ObtenerCodigos() {
    return codigos;
}

void Secuencia::FijarNumbases(int n_numbases) {
    numbases = n_numbases;
}

void Secuencia::FijarNumcodigos(int n_numcodigos) {
    numcodigos = n_numcodigos;
}

void Secuencia::FijarCodigos(const std::vector<char> &nCodigos) {
    codigos = nCodigos;
    numcodigos = static_cast<int>(codigos.size());
    // actualizar el mapa de frecuencias para claves nuevas si no existen (inicializar a 0)
    for (char c : codigos) {
        if (frecuenciaCodigos.find(c) == frecuenciaCodigos.end()) {
            frecuenciaCodigos[c] = 0;
        }
    }
}

// --- Métodos de soporte ---

// Rellena frecuenciaCodigos, codigos, numbases y numcodigos a partir de lineas_secuencia.
// Este método reemplaza la lógica previa: siempre recalcula desde cero.
void Secuencia::EstablecerCodigosYBases() {
    frecuenciaCodigos.clear();
    numbases = 0;
    codigos.clear();

    for (const auto &line : lineas_secuencia) {
        for (char c : line) {
            frecuenciaCodigos[c] += 1;
            ++numbases;
        }
    }

    // llenar vector de codigos (orden por clave, porque std::map está ordenado)
    for (const auto &p : frecuenciaCodigos) {
        codigos.push_back(p.first);
    }
    numcodigos = static_cast<int>(codigos.size());
}

// Ordena codigos y (si se desea) mantiene correspondencia con frecuenciaCodigos.
// Aquí simplemente ordenamos lexicográficamente el vector codigos (A..Z).
void Secuencia::OrdenarCodigosYBases() {
    std::sort(codigos.begin(), codigos.end());
    // numcodigos y frecuenciaCodigos ya están consistentes
}

// Imprime las frecuencias (firma const exigida)
void Secuencia::MostrarFrecuencias() const {
    std::cout << "Frecuencias (codigo : frecuencia)\n";
    // Recorremos las claves del mapa para asegurar orden
    for (const auto &p : frecuenciaCodigos) {
        std::cout << "'" << p.first << "' : " << p.second << "\n";
    }
}

// Verificación simple: no contar espacios en blanco y frecuencias > 0
bool Secuencia::VerificarCodigosValidos() const {
    if (lineas_secuencia.empty()) return false;
    for (const auto &p : frecuenciaCodigos) {
        char c = p.first;
        int freq = p.second;
        if (freq <= 0) return false;
        if (std::isspace(static_cast<unsigned char>(c))) return false;
    }
    return true;
}
