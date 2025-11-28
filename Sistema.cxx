#include<fstream>
#include<string>
#include<vector>
#include<list>
#include <bitset>
#include "Sistema.h"
#include "ArbolCod.h"
#include <cstdint> 
#include <cmath>
#include <algorithm>
#include <iostream>

const std::vector<Comando>& Sistema :: ObtenerComandos() const{
    return( comandos );
}

std::list< Secuencia >& Sistema:: ObtenerSecuencias(){
    return( secuencias );
}

void Sistema::FijarComandos(std::vector<Comando> ncomandos) {
  this->comandos = ncomandos;
}

void Sistema:: FijarSecuencias (std::list< Secuencia > secs){
    this->secuencias = secs;
}

//COMANDO CARGAR
void Sistema :: cargar(std:: string nombre_archivo){

    std::list<Secuencia> lista;
    bool invalido = false;
    
    std::ifstream file(nombre_archivo);
    // en caso de que el archivo no se abra
    if (!file.is_open()) {
        std::cout << nombre_archivo << " no se encuentra o no puede leerse." << std::endl;
        file.close();
    }
    else{
        std::string linea;
        Secuencia actual;

        while (std::getline(file, linea)) {
            if (!linea.empty() && linea[0] == '>') {

		if(!actual.ObtenerDescripcion().empty() && !invalido){
		    lista.push_back(actual);
		}

                // Si ya había una secuencia en construcción
                if (!actual.ObtenerDescripcion().empty()) {
                    actual = Secuencia(); // reiniciamos
		    invalido = false;
                }
                linea = linea.substr(1); // quitar '>'

	        actual.FijarDescripcion(linea.erase(linea.find_last_not_of(" \n\r\t")+1)); //quitar espacios y saltos de linea
            } else if (!linea.empty()) {
		if(invalido){
		    continue;
		}
		
	        if (!actual.ObtenerDescripcion().empty() && !invalido) {
		    std::vector<char> invalidos = verificarCodigosValidos(linea);
		    if(invalidos.size() != 0){
		        std :: cout << "La secuencia " << actual.ObtenerDescripcion() 
            	        	    << " no puede ser cargada en el sistema porque contiene codigos invalidos para una secuencia genetica como: ";

	
		        for(size_t i = 0; i < invalidos.size(); i++){
	    		    std :: cout << '\'' << invalidos[i] << '\''; 
		        }
		    
			std :: cout << std::endl;
		        invalido = true;
		    }else{
		     actual.AgregarLineaSecuencia(linea.erase(linea.find_last_not_of(" \n\r\t")+1));
	 	    }       
		}
	    }
        }  

        // Guardar la última secuencia
        if (!invalido) {
            lista.push_back(actual);
        }

        file.close();

	this->FijarSecuencias(lista);

        // mostrar en pantalla cuantas cadenas fueron validas y se cargaron correctamente
        if (this->ObtenerSecuencias().empty()) {
            std::cout << nombre_archivo 
                      << " no contiene ninguna secuencia." << std::endl;
        } else if (this->ObtenerSecuencias().size() == 1) {
            std::cout << "1 secuencia cargada correctamente desde " 
                      << nombre_archivo << "." << std::endl;
        } else {
            std::cout << lista.size() 
                      << " secuencias cargadas correctamente desde " 
                      << nombre_archivo << "." << std::endl;
        }

        //Establecer codigos y frecuencias en cada secuencia recien cargada

	std::list<Secuencia>::iterator itS;
        for(itS = this->ObtenerSecuencias().begin(); itS != this->ObtenerSecuencias().end(); itS ++){
	    itS -> EstablecerCodigosYFrecuencias();
        }
    }
}

//COMANDO LISTAR_SECUENCIAS
void Sistema :: listar_secuencias(){
    std::list<Secuencia>& secuencias = this->ObtenerSecuencias();

    if(secuencias.empty()){
        std:: cout << "No hay secuencias cargadas en memoria" << std::endl;
    }else{	
	std::cout << "Hay " << this->ObtenerSecuencias().size() << " secuencias cargadas en memoria" << std::endl;
	//Imprimir cuantas bases tiene cada secuencia
	std::list<Secuencia>::iterator itS;
	for(itS = this->ObtenerSecuencias().begin(); itS != this->ObtenerSecuencias().end(); itS ++){
	    int contBases = 0;
	    std::map<char,int,orden>::const_iterator itM;
	    for(itM = itS->ObtenerFrecuencias().begin();itM != itS->ObtenerFrecuencias().end(); itM++){
		if(itM -> first != '-'){
		    contBases += itM -> second;
		} 
	    }
	    
	    std:: cout << "Secuencia " << itS->ObtenerDescripcion() << " contiene ";
	    if(!(itS->ObtenerFrecuencias().find('-') != itS->ObtenerFrecuencias().end())){
		std:: cout << contBases << " bases." << std::endl;
	    }else{
		std:: cout << "al menos " << contBases << " bases" << std::endl;
	    }
	}
    }
}


//COMANDO HISTOGRAMA
void Sistema :: histograma(std::string descripcion_secuencia){
    std::list<Secuencia>& secuencias = this->ObtenerSecuencias();

    if (secuencias.empty()) {
      std::cout << "No hay secuencias cargadas en memoria.\n";
        return;
    }

    bool encontrado = false;

    //buscar descripcion_secuencia
    std::list<Secuencia>::iterator itS;
    for(itS = secuencias.begin(); itS != secuencias.end(); itS ++){
	descripcion_secuencia = descripcion_secuencia.erase(descripcion_secuencia.find_last_not_of(" \n\r\t")+1);
	if( descripcion_secuencia == itS->ObtenerDescripcion()){
	    encontrado = true;
	    std::map<char,int,orden>::const_iterator itM;
	    //Imprimir histograma
	    for(itM = itS->ObtenerFrecuencias().begin();itM != itS->ObtenerFrecuencias().end(); itM++){
		std :: cout << itM -> first << " : " << itM -> second << std::endl;
	    }
	}
    }

    if(!encontrado) std::cout<< "Secuencia invalida" << std::endl;
}



// COMANDO ES_SUBSECUENCIA
void Sistema::es_subsecuencia(std::string subsecuencia) {
    std::list<Secuencia>& secs = this->ObtenerSecuencias();
    if (secs.empty()) {
        std::cout << "No hay secuencias cargadas en memoria.\n";
        return;
    }
    if (subsecuencia.empty()) {
        std::cout << "La subsecuencia dada no existe dentro de las secuencias cargadas en memoria.\n";
        return;
    }

    long long total = 0;

    for (std::list<Secuencia>::iterator it = secs.begin(); it != secs.end(); ++it) {
        Secuencia& s = *it;
        const std::vector<std::string>& lineas = s.ObtenerLineasSecuencia();

        // 1) Concatenar SIN whitespace (cruza líneas)
        std::string concat;
        // Estimación de capacidad
        size_t cap = 0;
        for (size_t i = 0; i < lineas.size(); ++i) cap += lineas[i].size();
        concat.reserve(cap);

        for (size_t i = 0; i < lineas.size(); ++i) {
            const std::string& L = lineas[i];
            for (size_t j = 0; j < L.size(); ++j) {
                char ch = L[j];
                // Ignorar CR/LF/TAB/espacio por robustez
                if (ch == '\r' || ch == '\n' || ch == '\t' || ch == ' ') continue;
                concat.push_back(ch);
            }
        }

        // 2) Buscar con solapamiento
        size_t pos = 0;
        while (true) {
            pos = concat.find(subsecuencia, pos);
            if (pos == std::string::npos) break;
            ++total;
            ++pos; // solapamientos
        }
    }

    if (total == 0) {
        std::cout << "La subsecuencia dada no existe dentro de las secuencias cargadas en memoria.\n";
    } else {
        std::cout << "La subsecuencia dada se repite " << total
                  << " veces dentro de las secuencias cargadas en memoria.\n";
    }
}

//COMANDO ENMASCARAR
void Sistema::enmascarar(std::string subsecuencia) {
    std::list<Secuencia>& secs = this->ObtenerSecuencias();
    if (secs.empty()) {
        std::cout << "No hay secuencias cargadas en memoria.\n";
        return;
    }
    if (subsecuencia.empty()) {
        std::cout << "La subsecuencia dada no existe dentro de las secuencias cargadas en memoria, por tanto no se enmascara nada.\n";
        return;
    }

    long long total_enmascaradas = 0;

    for (std::list<Secuencia>::iterator it = secs.begin(); it != secs.end(); ++it) {
        Secuencia& s = *it;
        std::vector<std::string>& lineas = s.ObtenerLineasSecuencia();
        if (lineas.empty()) continue;

        // 1) Guardar anchos "visibles" por línea (sin whitespace)
        std::vector<size_t> largos; largos.reserve(lineas.size());
        size_t len_total = 0;

        // 2) Concatenar SIN whitespace
        std::string concat;
        size_t cap = 0;
        for (size_t i = 0; i < lineas.size(); ++i) cap += lineas[i].size();
        concat.reserve(cap);

        for (size_t i = 0; i < lineas.size(); ++i) {
            const std::string& L = lineas[i];
            size_t cuenta = 0;
            for (size_t j = 0; j < L.size(); ++j) {
                char ch = L[j];
                if (ch == '\r' || ch == '\n' || ch == '\t' || ch == ' ') continue;
                concat.push_back(ch);
                ++cuenta;
            }
            largos.push_back(cuenta);
            len_total += cuenta;
        }

        // 3) Buscar y enmascarar con solapamiento
        size_t pos = 0;
        long long en_esta = 0;
        while (true) {
            pos = concat.find(subsecuencia, pos);
            if (pos == std::string::npos) break;
            for (size_t k = 0; k < subsecuencia.size(); ++k) {
                concat[pos + k] = 'X';
            }
            ++en_esta;
            ++total_enmascaradas;
            ++pos; // solapamientos
        }

        // 4) Si hubo cambios, reconstruir con mismos anchos
        if (en_esta > 0) {
            std::vector<std::string> nuevas;
            nuevas.reserve(largos.size());
            size_t idx = 0;
            for (size_t i = 0; i < largos.size(); ++i) {
                size_t L = largos[i];
                if (L == 0) {
                    nuevas.push_back(std::string()); // línea vacía
                } else {
                    nuevas.push_back(concat.substr(idx, L));
                    idx += L;
                }
            }
            s.FijarLineasSecuencia(nuevas);
            s.EstablecerCodigosYFrecuencias();
        }
    }

    if (total_enmascaradas == 0) {
        std::cout << "La subsecuencia dada no existe dentro de las secuencias cargadas en memoria, por tanto no se enmascara nada.\n";
    } else {
        std::cout << total_enmascaradas
                  << " subsecuencias han sido enmascaradas dentro de las secuencias cargadas en memoria.\n";
    }
}


//COMANDO GUARDAR
void Sistema :: guardar(std::string nombre_archivo){
 if(!secuencias.empty()){
     //Crear/abrir archivo 
     std::ofstream archivo(nombre_archivo+".fa");
     if(!archivo){
       std::cout<<"Error guardando en "<<nombre_archivo<<".\n";
     }
     
     std::vector< std::string >::iterator itCod; //iterador para cada linea 
     std::list< Secuencia >::iterator itSec; //iterador que me recorrera la lista de secuencias
     for(itSec = secuencias.begin(); itSec != secuencias.end();  ++itSec){
        //primero guarda la descripción
        archivo << ">" << itSec->ObtenerDescripcion() <<"\n";
        //luego recorre linea por linea el vector que contiene la linea de secuencia
        for(itCod = itSec->ObtenerLineasSecuencia().begin(); itCod != itSec->ObtenerLineasSecuencia().end(); ++itCod){
        archivo << *itCod << "\n";
        }
     }
     archivo.close();
     std::cout<<"Las secuencias han sido guardadas en "<<nombre_archivo<<"\n";


    }else{
     std::cout<<"No hay secuencias cargadas en memoria. \n";
    }
}



/*COMANDO CODIFICAR
void Sistema :: codificar(std::string nombre_archivo){

 if(secuencias.empty()){
    std::cout << "No hay secuencias cargadas en memoria.\n";
    return;
 };

  std::map<char, int> frecuencias; //asocia cada codigo a una frecuencia
    std::list<Secuencia>::iterator itS; //para recorrer lista de secuencias
    for (itS = secuencias.begin(); itS != secuencias.end(); ++itS) { //recorre lista de secuencias
        std::vector<std::string>& lineas = itS->ObtenerLineasSecuencia(); //obtiene las lineas por cada elemento de la lista secuencias
        for (int i = 0; i < (int)lineas.size(); i++) {
            std::string linea = lineas[i]; //guarda en una variable temporal la linea leida
            for (int j = 0; j < (int)linea.size(); j++) {  //recorre cada caracterer 
                char c = linea[j];
                frecuencias[c]++; // suma la aparición de cada base
            }
        }
    }

//arbol de Huffman

ArbolCod arbol;
arbol.Huffman(frecuencias);

//archivo binario
std::ofstream archivo(nombre_archivo.c_str(), std::ios::binary);
if(!archivo.is_open()){
    std::cout<<"No se pueden guardar las secuencias cargadas en "<<nombre_archivo<<"\n";
    return;
}

//estructura archivo fabin
//n: cantidad de bases diferentes
short n = (short)frecuencias.size(); // cuenta el numero de "frecuencias"
archivo.write((char*)&n, sizeof(short)); // escribe en binario con la cantidad de bytes (2 bytes)

// c: base. f: frecuencia asociada a la base 
std::map<char, int>::iterator itF;
for(itF = frecuencias.begin(); itF != frecuencias.end(); ++itF){
    char c = itF->first;
    uint64_t f = static_cast<uint64_t>(itF->second);
    archivo.write(reinterpret_cast<char*>(&c), sizeof(char)); // escribe en binario c (1 byte)
    archivo.write(reinterpret_cast<char*>(&f), sizeof(uint64_t)); // escribe en binario f (8 bytes)
}

// ns: cantidad de secuencias 
uint32_t ns = static_cast<uint32_t>(secuencias.size());  //cantidad de secuencias cargadas en 
archivo.write(reinterpret_cast<char*>(&ns), sizeof(uint32_t)); //pasa a binario 


// l y s

for (itS = secuencias.begin(); itS != secuencias.end(); ++itS) {
    std::string nombre = itS->ObtenerDescripcion(); //obtiene el nombre de la secuencia
    if(!nombre.empty() && nombre[0] == '>'){
        nombre = nombre.substr(1); //quita >
    }



short l = (short)nombre.size(); //mide cuantos caracteres tiene el nombre
archivo.write((char*)&l, sizeof(short));
archivo.write(nombre.c_str(), l);


// concatenar las lineas de la secuencia
std::vector<std::string>& lineas = itS->ObtenerLineasSecuencia();
std::string bases;
for (int i = 0; i < (int)lineas.size(); i++) {
    bases += lineas[i];
}

// Codificar las bases con Huffman (obtener bits primero)
std::string bits = arbol.codificacion(bases);

// wi: Longitud de la secuencia EN BITS (antes de padding)
uint64_t w = static_cast<uint64_t>(bits.size());

// Rellenar hasta múltiplo de 8 bits para escribir bytes
while (bits.size() % 8 != 0) {
    bits += '0';
}

// Escribir w (uint64_t)
archivo.write(reinterpret_cast<char*>(&w), sizeof(uint64_t));

// x: Ancho de linea (uint16_t / short)
short x = 0;
if (!lineas.empty()) {
    x = static_cast<short>(lineas[0].size()); // ancho real de línea
}
archivo.write(reinterpret_cast<char*>(&x), sizeof(short));

// binary_code: escribir los bits como bytes
for (int j = 0; j < (int)bits.size(); j += 8) {
    std::string byteStr = bits.substr(j, 8);
    std::bitset<8> byte(byteStr);
    unsigned char c = static_cast<unsigned char>(byte.to_ulong());
    archivo.write(reinterpret_cast<char*>(&c), 1);
    std::bitset<8> bitsMostrados(c);
    //std::cout << bitsMostrados;
}


 }

  archivo.close();
  std::cout << "\nSecuencias codificadas y almacenadas en " << nombre_archivo << std::endl;
}
*/

//COMANDO CODIFICAR
void Sistema :: codificar(std::string nombre_archivo){

    if(secuencias.empty()){
        std::cout << "No hay secuencias cargadas en memoria.\n";
        return;
    };

    // La frecuencia total de todas las bases se sigue almacenando en un mapa <char, int> 
    std::map<char, int> frecuencias; 
    std::list<Secuencia>::iterator itS; 
    
    //  Calcular frecuencias totales de todas las secuencias
    for (itS = secuencias.begin(); itS != secuencias.end(); ++itS) { 
        std::vector<std::string>& lineas = itS->ObtenerLineasSecuencia(); 
        for (int i = 0; i < (int)lineas.size(); i++) {
            std::string linea = lineas[i];
            for (int j = 0; j < (int)linea.size(); j++) { 
                char c = linea[j];
                frecuencias[c]++;
            }
        }
    }

    //  Creación del árbol de Huffman
    ArbolCod arbol;
    arbol.Huffman(frecuencias);

    //  Apertura y validación del archivo binario
    std::ofstream archivo(nombre_archivo.c_str(), std::ios::binary);
    if(!archivo.is_open()){
        std::cout<<"No se pueden guardar las secuencias cargadas en "<<nombre_archivo<<"\n";
        return;
    }

    // ESTRUCTURA DEL ARCHIVO .FABIN (Orden de escritura)
    
    // a. n: cantidad de bases diferentes (short)
    short n = (short)frecuencias.size();
    archivo.write((char*)&n, sizeof(short));

    // b. Tabla de Frecuencias (char c, int f)
    std::map<char, int>::iterator itF;
    for(itF = frecuencias.begin(); itF != frecuencias.end(); ++itF){
        char c = itF->first;
        // CORRECCIÓN CLAVE: Escribir la frecuencia como 'int' (4 bytes)
        // para asegurar que el decodificador lea 4 bytes.
        int f = itF->second; 
        archivo.write(reinterpret_cast<char*>(&c), sizeof(char));
        archivo.write(reinterpret_cast<char*>(&f), sizeof(int)); // **¡CAMBIO AQUÍ!**
    }

    // c. ns: cantidad de secuencias (uint32_t)
    uint32_t ns = static_cast<uint32_t>(secuencias.size());
    archivo.write(reinterpret_cast<char*>(&ns), sizeof(uint32_t));

    //  Datos de cada secuencia
    for (itS = secuencias.begin(); itS != secuencias.end(); ++itS) {
        
        //  l y s (Longitud y Nombre de la secuencia)
        std::string nombre = itS->ObtenerDescripcion();
        if(!nombre.empty() && nombre[0] == '>'){
            nombre = nombre.substr(1);
        }

        short l = (short)nombre.size();
        archivo.write((char*)&l, sizeof(short));
        archivo.write(nombre.c_str(), l);

        // . Concatenar las bases
        std::vector<std::string>& lineas = itS->ObtenerLineasSecuencia();
        std::string bases;
        for (int i = 0; i < (int)lineas.size(); i++) {
            bases += lineas[i];
        }

        //  Codificar las bases con Huffman
        std::string bits = arbol.codificacion(bases);

        //  wi: Longitud de la secuencia EN BITS (antes de padding, uint64_t)
        uint64_t w = static_cast<uint64_t>(bits.size());

        // Rellenar hasta múltiplo de 8 bits para escribir bytes
        while (bits.size() % 8 != 0) {
            bits += '0';
        }

        archivo.write(reinterpret_cast<char*>(&w), sizeof(uint64_t));

        //  x: Ancho de linea (short)
        short x = 0;
        if (!lineas.empty()) {
            x = static_cast<short>(lineas[0].size());
        }
        archivo.write(reinterpret_cast<char*>(&x), sizeof(short));

        //  binary_code: escribir los bits como bytes
        for (int j = 0; j < (int)bits.size(); j += 8) {
            std::string byteStr = bits.substr(j, 8);
            std::bitset<8> byte(byteStr);
            unsigned char c = static_cast<unsigned char>(byte.to_ulong());
            archivo.write(reinterpret_cast<char*>(&c), 1);
        }
    }

    archivo.close();
    std::cout << "\nSecuencias codificadas y almacenadas en " << nombre_archivo << std::endl;
}

/*COMANDO DECODIFICAR
void Sistema::decodificar(std::string nombre_archivo) {
    std::ifstream fin(nombre_archivo, std::ios::binary);
    if (!fin.is_open()) {
        std::cout << "No se pueden cargar las secuencias desde " << nombre_archivo << "." << std::endl;
        return;
    }

    try {
        // Leer n (uint16_t)
        uint16_t n = 0;
        fin.read(reinterpret_cast<char*>(&n), sizeof(uint16_t));
        if (!fin) throw std::runtime_error("Formato invalido (n).");

        // Leer tabla de frecuencias
        std::map<char, uint64_t> freq64;
        for (uint16_t i = 0; i < n; ++i) {
            char ci;
            uint64_t fi = 0;
            fin.read(&ci, 1);
            fin.read(reinterpret_cast<char*>(&fi), sizeof(uint64_t));
            if (!fin) throw std::runtime_error("Formato invalido (ci/fi).");
            freq64[ci] = fi;
        }

        // Convertir a map<char,int>
        std::map<char, int> freq_for_tree;
        std::map<char, uint64_t>::iterator itFreq;
        for (itFreq = freq64.begin(); itFreq != freq64.end(); ++itFreq) {
            freq_for_tree[itFreq->first] = static_cast<int>(itFreq->second);
        }

        // Leer ns (uint32_t)
        uint32_t ns = 0;
        fin.read(reinterpret_cast<char*>(&ns), sizeof(uint32_t));
        if (!fin) throw std::runtime_error("Formato invalido (ns).");

        // Crear árbol de Huffman
        ArbolCod arbol;
        arbol.Huffman(freq_for_tree);

        // Lista de nuevas secuencias
        std::list<Secuencia> nuevas;

        for (uint32_t s = 0; s < ns; ++s) {
            uint16_t li = 0;
            fin.read(reinterpret_cast<char*>(&li), sizeof(uint16_t));
            if (!fin) throw std::runtime_error("Formato invalido (li).");

            std::string nombre;
            if (li > 0) {
                nombre.resize(li);
                fin.read(&nombre[0], li);
                if (!fin) throw std::runtime_error("Formato invalido (nombre).");
            }

            uint64_t wi = 0;
            fin.read(reinterpret_cast<char*>(&wi), sizeof(uint64_t));
            if (!fin) throw std::runtime_error("Formato invalido (wi).");

            uint16_t xi = 0;
            fin.read(reinterpret_cast<char*>(&xi), sizeof(uint16_t));
            if (!fin) throw std::runtime_error("Formato invalido (xi).");

            uint64_t bytesNecesarios = (wi + 7) / 8;
            std::vector<char> raw(bytesNecesarios);
            if (bytesNecesarios > 0) {
                fin.read(reinterpret_cast<char*>(raw.data()), bytesNecesarios);
                if (!fin) throw std::runtime_error("Formato invalido (binary_data).");
            }

            std::string bits;
            bits.reserve(static_cast<size_t>(bytesNecesarios * 8));
            for (uint64_t b = 0; b < bytesNecesarios; ++b) {
                unsigned char byte = static_cast<unsigned char>(raw[b]);
                for (int bit = 7; bit >= 0; --bit) {
                    bits.push_back(((byte >> bit) & 1) ? '1' : '0');
                }
            }
            if (bits.size() > wi) bits.resize(static_cast<size_t>(wi));

            std::string decod = arbol.decodificacion(bits);
            if (decod == "Codigo incompleto") {
                throw std::runtime_error("Error decodificacion: codigo incompleto.");
            }

            std::vector<std::string> lineas;
            if (xi == 0) {
                lineas.push_back(decod);
            } else {
                for (size_t pos = 0; pos < decod.size(); pos += xi) {
                    lineas.push_back(decod.substr(pos, xi));
                }
            }

            Secuencia sec;
            sec.FijarDescripcion(nombre);
            sec.FijarLineasSecuencia(lineas);

            nuevas.push_back(sec);
        }

        fin.close();

        // Sobrescribir las secuencias
        this->FijarSecuencias(nuevas);

        // Establecer codigos y frecuencias
        std::list<Secuencia>::iterator itS;
        for (itS = this->ObtenerSecuencias().begin(); itS != this->ObtenerSecuencias().end(); ++itS) {
            itS->EstablecerCodigosYFrecuencias();
        }

        std::cout << "Secuencias decodificadas desde " << nombre_archivo << " y cargadas en memoria." << std::endl;
    }
    catch (const std::exception &e) {
        fin.close();
        std::cout << "No se pueden cargar las secuencias desde " << nombre_archivo << "." << std::endl;
        return;
    }
}
*/

//COMANDO DECODIFICAR
void Sistema :: decodificar(std::string nombre_archivo){
    // 1. Apertura y validación del archivo binario
    std::ifstream archivo(nombre_archivo.c_str(), std::ios::binary);

    if(!archivo.is_open()){
        std::cout << "No se puede abrir el archivo " << nombre_archivo << ".\n";
        return;
    }

    // LISTA TEMPORAL PARA LAS SECUENCIAS DECODIFICADAS
    std::list<Secuencia> secuencias_decodificadas;
    
    // VARIABLES DE METADATOS GLOBALES
    short n = 0; // Cantidad de símbolos (bases) diferentes
    uint32_t ns = 0; // Cantidad de secuencias
    std::map<char, int> frecuencias; // Mapa para reconstruir el árbol

    // --- LECTURA DE METADATOS GLOBALES ---

    // Leer n (short, 2 bytes)
    if (!archivo.read(reinterpret_cast<char*>(&n), sizeof(short)) || n <= 0) {
        std::cout << "Error: Archivo binario inválido o dañado (al leer n).\n";
        archivo.close();
        return;
    }

    //  Leer Tabla de Frecuencias (n veces: char c, int f)
    for (int i = 0; i < n; ++i) {
        char c;
        int f; // Se lee como int (4 bytes)
        
        if (!archivo.read(reinterpret_cast<char*>(&c), sizeof(char))) {
             std::cout << "Error: Archivo binario inválido o dañado (al leer código).\n";
             archivo.close();
             return;
        }
        
        //  Leer la frecuencia como 'int' (4 bytes)
        if (!archivo.read(reinterpret_cast<char*>(&f), sizeof(int))) { 
            std::cout << "Error: Archivo binario inválido o dañado (al leer frecuencia).\n";
            archivo.close();
            return;
        }
        
        frecuencias[c] = f;
    }

    //  Leer ns (cantidad de secuencias, uint32_t, 4 bytes)
    if (!archivo.read(reinterpret_cast<char*>(&ns), sizeof(uint32_t))) {
        std::cout << "Error: Archivo binario inválido o dañado (al leer ns).\n";
        archivo.close();
        return;
    }
    
    //  Reconstrucción del árbol de Huffman
    ArbolCod arbol;
    arbol.Huffman(frecuencias);
    
    // --- LECTURA Y DECODIFICACIÓN DE CADA SECUENCIA ---

    for (uint32_t i = 0; i < ns; ++i) {
        short l = 0;       // Longitud del nombre (short)
        uint64_t w = 0;    // Longitud de la secuencia EN BITS (uint64_t)
        short x = 0;       // Ancho de línea (short)

        //  Leer l (longitud del nombre)
        if (!archivo.read(reinterpret_cast<char*>(&l), sizeof(short)) || l <= 0) {
            std::cout << "Error: Archivo incompleto o dañado (al leer longitud del nombre).\n";
            archivo.close();
            return;
        }

        //  Leer s (Nombre de la secuencia)
        std::string nombre(l, ' ');
        if (!archivo.read(&nombre[0], l)) {
            std::cout << "Error: Archivo incompleto o dañado (al leer nombre).\n";
            archivo.close();
            return;
        }
        std::string descripcion = ">" + nombre;

        //  Leer w (Longitud en bits)
        if (!archivo.read(reinterpret_cast<char*>(&w), sizeof(uint64_t)) || w == 0) {
            std::cout << "Error: Archivo incompleto o dañado (al leer longitud en bits).\n";
            archivo.close();
            return;
        }
        
        //  Leer x (Ancho de línea)
        if (!archivo.read(reinterpret_cast<char*>(&x), sizeof(short))) {
            std::cout << "Error: Archivo incompleto o dañado (al leer ancho de línea).\n";
            archivo.close();
            return;
        }

        //  Leer y decodificar el Código Binario (binary_code)
        uint64_t bytes_a_leer = (w + 7) / 8; // Número de bytes que contiene los w bits
        std::string bits_codificados;
        
        for (uint64_t j = 0; j < bytes_a_leer; ++j) {
            unsigned char byte_leido;
            if (!archivo.read(reinterpret_cast<char*>(&byte_leido), 1)) {
                std::cout << "Error: Archivo incompleto o dañado (al leer código binario).\n";
                archivo.close();
                return;
            }
            std::bitset<8> bits_byte(byte_leido);
            bits_codificados += bits_byte.to_string();
        }
        
        // Truncar los bits al tamaño original w (eliminar padding)
        bits_codificados.resize(w); 

        // Decodificación
        std::string bases_decodificadas = arbol.decodificacion(bits_codificados);

        // Reconstruir la Secuencia
        Secuencia nueva_secuencia;
        nueva_secuencia.FijarDescripcion(descripcion);

        // Reconstruir las líneas de secuencia usando el ancho x
        std::vector<std::string> lineas_reconstruidas;
        if (x > 0) {
            for (size_t k = 0; k < bases_decodificadas.length(); k += x) {
                lineas_reconstruidas.push_back(bases_decodificadas.substr(k, x));
            }
        } else {
            // Caso borde: si x=0 (aunque se espera x>0), se pone toda la secuencia en una línea
            lineas_reconstruidas.push_back(bases_decodificadas);
        }
        
        nueva_secuencia.FijarLineasSecuencia(lineas_reconstruidas);
        secuencias_decodificadas.push_back(nueva_secuencia);
    }

    archivo.close();
    
    //  Carga en memoria y Actualización final
    this->FijarSecuencias(secuencias_decodificadas);
    
    std::list<Secuencia>::iterator itS;
    for (itS = secuencias.begin(); itS != secuencias.end(); ++itS) {
        itS->EstablecerCodigosYFrecuencias();
    }

    std::cout << "Decodificación completada. Se cargaron " << ns << " secuencias en memoria.\n";
}





//COMANDO RUTA_MAS_CORTA
void Sistema :: ruta_mas_corta(std::string descripcion_secuencia, int i, int j, int x, int y){
    Secuencia seq;

    //Verificar que la secuencia con la descripcion dada exista
    bool encontrado = false;
    std::list<Secuencia>::iterator itS;
    for(itS = this->ObtenerSecuencias().begin(); itS != this->ObtenerSecuencias().end(); itS ++){
	if(itS->ObtenerDescripcion() == descripcion_secuencia){
	    encontrado = true;
	    seq = *itS;
	    break;
	}
    }

    if(!encontrado){
	std::cout << "La secuencia " << descripcion_secuencia << " no existe" << std::endl;
        return;
    }

    std::vector<std::string> matrizSecuencia = seq.ObtenerLineasSecuencia();   

    // Verificar que la posicion de la base origen sea valida
    if((i >= (int)matrizSecuencia.size()) || (j >= (int)matrizSecuencia[i].size())){
	std::cout << "La base en la posicion [" << i << "," << j << "] no existe" << std::endl;
        return;
    }

    // Verificar que la posicion de la base destino sea valida
    if((x >= (int)matrizSecuencia.size()) || (y >= (int)matrizSecuencia[x].size())){
	std::cout << "La base en la posicion [" << x << "," << y << "] no existe" << std::endl;
        return;
    }

    // Obtener Grafo de la secuencia
    Grafo grafoSecuencia = this->obtenerGrafoSecuencia(matrizSecuencia);

    //Obtener la ruta mas corta entre la base origen y la base destino
    int indiceBaseOrigen = grafoSecuencia.buscarVertice(i,j);
    int indiceBaseDestino = grafoSecuencia.buscarVertice(x,y);
    std::vector<int> ruta = grafoSecuencia.rutaMasCorta(indiceBaseOrigen,indiceBaseDestino);
    double costoRuta = grafoSecuencia.costoRuta(ruta);

    std::cout << "Para la secuencia " << descripcion_secuencia << ", la ruta mas corta entre la base "<< matrizSecuencia[i][j] << " en [" << i << "," << j << "] y en la base "<< matrizSecuencia[x][y] << " en [" << x << "," << y << "] es:" << std::endl;

    for(std::size_t k = 0; k < ruta.size(); ++k){
	std::cout << grafoSecuencia.obtenerBase(ruta[k]) << " ";
    }
 
    std::cout << std::endl;

    std::cout << "El costo total de la ruta es: " << costoRuta << std::endl;
}

//COMANDO BASE REMOTA
void Sistema :: base_remota(std::string descripcion_secuencia, int i, int j){
    Secuencia seq;

    //Verificar que la secuencia con la descripcion dada exista
    bool encontrado = false;
    std::list<Secuencia>::iterator itS;
    for(itS = this->ObtenerSecuencias().begin(); itS != this->ObtenerSecuencias().end(); itS ++){
	if(itS->ObtenerDescripcion() == descripcion_secuencia){
	    encontrado = true;
	    seq = *itS;
	    break;
	}
    }

    if(!encontrado){
	std::cout << "La secuencia " << descripcion_secuencia << " no existe" << std::endl;
        return;
    }

    std::vector<std::string> matrizSecuencia = seq.ObtenerLineasSecuencia();   

    // Verificar que la posicion de la base sea valida
    if((i >= (int)matrizSecuencia.size()) || (j >= (int)matrizSecuencia[i].size())){
	std::cout << "La base en la posicion [" << i << "," << j << "] no existe" << std::endl;
        return;
    }

    // Obtener Grafo de la secuencia
    Grafo grafoSecuencia = this->obtenerGrafoSecuencia(matrizSecuencia);

    // Obtener los indices de todas las bases con la misma letra
    int indiceBase = grafoSecuencia.buscarVertice(i,j);
    std::vector<Base> vertices = grafoSecuencia.obtenerVertices();
    char letra = vertices[indiceBase].obtenerCodigo();
    std::vector<int> indicesConMismaLetra;
    std::vector<double> pesosRutas;

    for(std::size_t k = 0; k < vertices.size(); ++k){
	if(((int)k != indiceBase) && (vertices[k].obtenerCodigo() == letra)){
	    indicesConMismaLetra.push_back(k);

	    std::vector<int> ruta = grafoSecuencia.rutaMasCorta(indiceBase,k);
    	    double costo = grafoSecuencia.costoRuta(ruta);
	    pesosRutas.push_back(costo);
	}
    }

    // Verificar que se encontro al menos una base con la misma letra
    if(indicesConMismaLetra.empty()){
	std::cout << "No se encontro otra base " << letra << " diferente a la ubicada en la posicion (" << i << "," << j << ")"<< std::endl;
        return;
    }

    // Buscar el costo maximo entre todas las rutas 
    double mayor = pesosRutas[0];
    for(std::size_t k = 1; k < pesosRutas.size(); ++k){
        if(pesosRutas[k] > mayor){
            mayor = pesosRutas[k];
        }
    }

    //Encontrar todas las rutas que tengan es costo maximo
    const double EPS = 1e-9;
    std::vector<int> indicesRemotos;
    for(std::size_t k = 0; k < pesosRutas.size(); ++k){
        if (std::fabs(pesosRutas[k] - mayor) < EPS) {
            indicesRemotos.push_back(indicesConMismaLetra[k]);
        }
    }

    // Imprimir resultados
    if(indicesRemotos.size() == 1){
        int idxRemoto = indicesRemotos[0];
        std::vector<int> rutaBaseRemota = grafoSecuencia.rutaMasCorta(indiceBase, idxRemoto);

        std::cout << "En la secuencia " << descripcion_secuencia 
		  << " para la base ubicada en la posicion ["
		  << i << "," << j << "]"
                  << ", su base remota esta ubicada en [" 
                  << vertices[idxRemoto].obteneri() << "," 
                  << vertices[idxRemoto].obtenerj() << "] y la ruta entre la base [" 
                  << i << "," << j << "] y la base remota en [" 
                  << vertices[idxRemoto].obteneri() << "," 
                  << vertices[idxRemoto].obtenerj() << "] es:" << std::endl;

        for(std::size_t k = 0; k < rutaBaseRemota.size(); ++k){
            std::cout << grafoSecuencia.obtenerBase(rutaBaseRemota[k]) << " ";
        }
        std::cout << std::endl;
        std::cout << "El costo total de la ruta es: " 
                  << grafoSecuencia.costoRuta(rutaBaseRemota) << std::endl;
    } else {
        std::cout << "En la secuencia " << descripcion_secuencia 
		  << " para la base ubicada en la posicion ["
		  << i << "," << j << "]"
                  << ", se encontraron " << indicesRemotos.size() 
                  << " bases remotas a una distancia de "
                  << mayor << std::endl;

        for (std::size_t r = 0; r < indicesRemotos.size(); ++r) {
            int idxRemoto = indicesRemotos[r];
            std::vector<int> rutaBaseRemota = grafoSecuencia.rutaMasCorta(indiceBase, idxRemoto);

            std::cout << (r + 1) << ". Base " 
                      << vertices[idxRemoto].obtenerCodigo() 
                      << " en [" << vertices[idxRemoto].obteneri()
                      << "," << vertices[idxRemoto].obtenerj() << "]" 
                      << std::endl;

            std::cout << "   Ruta: ";
            for (std::size_t k = 0; k < rutaBaseRemota.size(); ++k) {
                std::cout << grafoSecuencia.obtenerBase(rutaBaseRemota[k]) << " ";
            }
            std::cout << std::endl;
        }
    }
}
std :: vector<char> Sistema ::verificarCodigosValidos(const std::string& linea){
    const std :: string permitidos = "ACGTURYKMSWBDHVNX-"; //cadena de caracteres que define los codigos permitidos
    std :: vector<char> invalidos; //vector que guarda todos los caracteres invalidos encontrados

    invalidos.clear();

    for(char c: linea){
	if(permitidos.find(c) == std::string::npos){
	    // Revisar que el caracter no este ya en el vector de invalidos
	    if (std::find(invalidos.begin(), invalidos.end(), c) == invalidos.end()) {
                invalidos.push_back(c);
	    }
	}
    }

    return invalidos;
}

Grafo Sistema::obtenerGrafoSecuencia(std::vector<std::string> matriz){
    Grafo grafoSecuencia;
    
    //Insertar vertices
    for(std::size_t i = 0; i < matriz.size(); i++){
	std::string fila = matriz[i];
	for(std::size_t j = 0; j < fila.size(); j++){
	    Base base(fila[j], i, j);
	    grafoSecuencia.insertarVertice(base);
	}
    }

    //Insertar aristas
    std::vector<Base> vertices = grafoSecuencia.obtenerVertices();
    for(int k = 0; k < grafoSecuencia.cantidadVertices(); k++){
	int fila = vertices[k].obteneri();
	int columna = vertices[k].obtenerj();

	//Vecino superior
	if(fila > 0){
	    int indiceVecino = grafoSecuencia.buscarVertice(fila-1, columna);
	
	    double peso = vertices[k].pesoConexion(vertices[indiceVecino]);
	    grafoSecuencia.insertarArista(k,indiceVecino, peso); 
	    grafoSecuencia.insertarArista(indiceVecino,k, peso); 
	}

	//Vecino inferior
	if(fila < (int)(matriz.size() - 1)){
	    int indiceVecino = grafoSecuencia.buscarVertice(fila+1, columna);
	
	    double peso = vertices[k].pesoConexion(vertices[indiceVecino]);
	    grafoSecuencia.insertarArista(k,indiceVecino, peso); 
	    grafoSecuencia.insertarArista(indiceVecino,k, peso); 
	}

	//Vecino izquierdo
	if(columna > 0){
	    int indiceVecino = grafoSecuencia.buscarVertice(fila, columna-1);
	
	    double peso = vertices[k].pesoConexion(vertices[indiceVecino]);
	    grafoSecuencia.insertarArista(k,indiceVecino, peso); 
	    grafoSecuencia.insertarArista(indiceVecino,k, peso); 
	}

	//Vecino derecho
	if(columna < (int)(matriz[fila].size() - 1)){
	    int indiceVecino = grafoSecuencia.buscarVertice(fila, columna+1);
	
	    double peso = vertices[k].pesoConexion(vertices[indiceVecino]);
	    grafoSecuencia.insertarArista(k,indiceVecino, peso); 
	    grafoSecuencia.insertarArista(indiceVecino,k, peso); 
	}
    }

    return grafoSecuencia;
}
