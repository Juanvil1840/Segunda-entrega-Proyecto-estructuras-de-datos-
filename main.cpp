#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>
#include "Sistema.h"

bool isInteger(const std::string &s) {
    if (s.empty()) return false;
    size_t pos = (s[0] == '+' || s[0] == '-') ? 1 : 0;
    for (; pos < s.size(); ++pos) {
        if (!std::isdigit(s[pos])) return false;
    }
    return true;
}

template <typename T>
const T* findByName(const std::vector<T> &vec, const std::string &name) {
    typename std::vector<T>::const_iterator it = vec.begin();
    for (; it != vec.end(); ++it) {
        if (it->ObtenerNombre() == name) return &(*it);
    }
    return NULL;
}

template <typename T>
void printCommands(const std::vector<T> &vec) {
    typename std::vector<T>::const_iterator it = vec.begin();
    for (; it != vec.end(); ++it) {
        std::cout << "  " << it->ObtenerUso() << "\n";
    }
}

Comando CrearComando(std::string nombre, int entrada, std::string uso) {
    Comando c;
    c.FijarNombre(nombre);
    c.FijarEntrada(entrada);
    c.FijarUso(uso);
    return c;
}

int main() {
    Sistema sistema;

    std::vector<Comando> commands = {
        CrearComando("cargar", 1, "cargar <nombre_archivo>"),
        CrearComando("listar_secuencias", 0, "listar_secuencias"),
        CrearComando("histograma", 1, "histograma <descripcion_secuencia>"),
        CrearComando("es_subsecuencia", 1, "es_subsecuencia <subsecuencia>"),
        CrearComando("enmascarar", 1, "enmascarar <subsecuencia>"),
        CrearComando("guardar", 1, "guardar <nombre_archivo>"),
        CrearComando("salir", 0, "salir"),
        CrearComando("codificar", 1, "codificar <nombre_archivo.fabin>"),
        CrearComando("decodificar", 1, "decodificar <nombre_archivo.fabin>"),
        CrearComando("ruta_mas_corta", 5, "ruta_mas_corta <descripcion_secuencia> <i> <j> <x> <y>"),
        CrearComando("base_remota", 3, "base_remota <descripcion_secuencia> <i> <j>"),
        CrearComando("ayuda", 0, "ayuda [<comando>]")
    };

    sistema.FijarComandos(commands);


    std::string line;
    while (true) {
        std::cout << "$ ";
        if (!std::getline(std::cin, line)) break;

        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string tok;
        while (iss >> tok) tokens.push_back(tok);
        if (tokens.empty()) continue;

        const std::string cmd = tokens[0];
        // Comando 'ayuda'
        if (cmd == "ayuda") {
            if (tokens.size() == 1) {
                std::cout << "Comandos disponibles:\n";
                printCommands(sistema.ObtenerComandos());
            } else if (tokens.size() == 2) {
                const Comando *c = findByName(sistema.ObtenerComandos(), tokens[1]);
                if (c != NULL) std::cout << "Uso: " << c->ObtenerUso() << "\n";
                else           std::cout << "Error: comando desconocido para ayuda\n";
            } else {
                std::cout << "Error: uso incorrecto de ayuda. Uso: ayuda [<comando>]\n";
            }
            continue;
        }

       
        const Comando *command = findByName(sistema.ObtenerComandos(), cmd);
        if (command == NULL) {
            std::cout << "Error: comando desconocido\n";
            continue;
        }

        int provided = static_cast<int>(tokens.size()) - 1;
        if (provided != command->ObtenerEntrada()) {
            std::cout << "Error: numero de argumentos incorrecto. Uso: "
                      << command->ObtenerUso() << "\n";
            continue;
        }

        bool valid = true;
        if (cmd == "ruta_mas_corta") {
            for (int i = 2; i < static_cast<int>(tokens.size()); ++i) {
                if (!isInteger(tokens[i])) {
                    std::cout << "Error: argumentos invalidos. Para ruta_mas_corta <descripcion_secuencia> <i> <j> <x> <y>. i, j, x y y deben ser enteros.\n";
                    valid = false;
                    break;
                }
            }
        } else if (cmd == "base_remota") {
            for (int i = 2; i < static_cast<int>(tokens.size()); ++i) {
                if (!isInteger(tokens[i])) {
                    std::cout << "Error: argumentos invalidos. Para base_remota <descripcion_secuencia> <i> <j>. i y j deben ser enteros.\n";
                    valid = false;
                    break;
                }
            }
        }
        if (!valid) continue;

	if(cmd=="cargar"){
	   sistema.cargar(tokens[1]); 	
	}else if(cmd=="listar_secuencias"){
	   sistema.listar_secuencias();
	}else if(cmd=="histograma"){
	   sistema.histograma(tokens[1]);
	}else if(cmd=="es_subsecuencia"){
	   sistema.es_subsecuencia(tokens[1]);
	}else if(cmd=="enmascarar"){
           sistema.enmascarar(tokens[1]);
	} else if(cmd=="guardar"){
            sistema.guardar(tokens[1]);
        }else if(cmd=="codificar"){
            sistema.codificar(tokens[1]);
        }else if(cmd=="decodificar"){
            sistema.decodificar(tokens[1]);
        }else if(cmd=="ruta_mas_corta"){
            sistema.ruta_mas_corta(tokens[1], std::stoi(tokens[2]), std::stoi(tokens[3]), std::stoi(tokens[4]),std::stoi(tokens[5]));
        }else if (tokens[0] == "base_remota") {
            sistema.base_remota(tokens[1], std::stoi(tokens[2]), std::stoi(tokens[3]));
        }

	if (cmd == "salir"){ 
	    std::cout << "Exito salir \n"; 
	    break;
	}
    }
    return 0;
}
