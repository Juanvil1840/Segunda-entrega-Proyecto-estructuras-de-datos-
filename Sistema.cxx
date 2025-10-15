#include<fstream>
#include<string>
#include<vector>
#include<list>
#include <bitset>
#include "Sistema.h"
#include "ArbolCod.h" // para poder usar la implementación del arbol de codificación
#include "NodoCod.h"
#include <cstdint> 
#include <cmath>


// obtener comandos
const std::vector<Comando>& Sistema :: ObtenerComandos() const{
    return( comandos );
}

// fijar comandos
void Sistema::FijarComandos(std::vector<Comando> ncomandos) {
  this->comandos = ncomandos;
}

// obtener secuencias
std::list< Secuencia >& Sistema:: ObtenerSecuencias(){
    return( secuencias );
}

// fijar secuencias
void Sistema:: FijarSecuencias (std::list< Secuencia > secs){
    this->secuencias = secs;
}

//COMANDO CARGAR
void Sistema :: cargar(std:: string nombre_archivo){

   std::list<Secuencia> lista;
    
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
                // Si ya había una secuencia en construcción
                if (!actual.ObtenerDescripcion().empty()) {
                    lista.push_back(actual);
                    actual = Secuencia(); // reiniciamos
                }
                linea = linea.substr(1); // quitar '>'

		// Metodo para quitar caracteres de saltos de linea y espacios obtenido con IA generativa

		actual.FijarDescripcion(linea.erase(linea.find_last_not_of(" \n\r\t")+1)); //quitar espacios y saltos de linea
            } else if (!linea.empty()) {
            actual.AgregarLineaSecuencia(linea.erase(linea.find_last_not_of(" \n\r\t")+1)); 
        }
    }    

        // Guardar la última secuencia
        if (!actual.ObtenerDescripcion().empty()) {
            lista.push_back(actual);
        }

        file.close();

	// Verificar que las secuencias tengan unicamente codigos validos
	std::list<Secuencia> listaAux;
	std::list<Secuencia>::iterator itS;
	for(itS = lista.begin(); itS != lista.end(); itS ++){
	    if(itS -> VerificarCodigosValidos()){
		listaAux.push_back(*itS);
	    }else{
		std :: cout << std:: endl;
	    }
	}

	this->FijarSecuencias(listaAux);

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

        //Establecer codigos y bases en cada secuencia recien cargada
        for(itS = this->ObtenerSecuencias().begin(); itS != this->ObtenerSecuencias().end(); itS ++){
	    itS -> EstablecerCodigosYBases();
        }

	   //Ordenar los codigos y bases segun la especificacion de la tabla 1 para cada una de las secuencias
        for(itS = this->ObtenerSecuencias().begin(); itS != this->ObtenerSecuencias().end(); itS ++){
	    itS -> OrdenarCodigosYBases();
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
	    std:: cout << "Secuencia " << itS->ObtenerDescripcion() << " contiene ";
	    if(itS->ObtenerNumcodigos() == itS->ObtenerNumbases()){
		std:: cout << itS->ObtenerNumbases() << " bases." << std::endl;
	    }else{
		std:: cout << "al menos " << itS->ObtenerNumbases() << " bases" << std::endl;
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
	    int* contadorCodigos = new int[itS->ObtenerCodigos().size()]();
	  
	    //Contar cuantos codigos hay de cada uno
	    std::vector< std::string >::iterator itL;
	    // Recorre el vector de lineas de secuencia
	    for(itL = itS->ObtenerLineasSecuencia().begin(); itL != itS->ObtenerLineasSecuencia().end(); itL ++){
	    // Recorre la cadena de caracteres
		for (int j = 0; j < itL->size(); j++) {
		    char c = (*itL)[j];
		    // Recorre el vector de codigos
		    for(int i = 0 ; i < itS -> ObtenerCodigos().size() ; i++){
			if( c == itS -> ObtenerCodigos()[i] ){
			    contadorCodigos[i]++;
			}
		    }
		} 
            }

	    //Imprimir histograma
	    for(int i = 0 ; i < itS -> ObtenerCodigos().size() ; i++){
		std :: cout << itS -> ObtenerCodigos()[i] << " : " << contadorCodigos[i] << std::endl;
	    }

	    delete[] contadorCodigos;
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
            s.EstablecerCodigosYBases();
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

//COMANDO CODIFICAR
//COMANDO CODIFICAR
void Sistema :: codificar(std::string nombre_archivo){

    if(secuencias.empty()){
        std::cout << "No hay secuencias cargadas en memoria.\n";
        return;
    }

    // 1) Calcular frecuencias (map<char,int>)
    std::map<char, int> frecuencias;
    for (auto itS = secuencias.begin(); itS != secuencias.end(); ++itS) {
        const std::vector<std::string>& lineas = itS->ObtenerLineasSecuencia();
        for (size_t i = 0; i < lineas.size(); ++i) {
            const std::string &linea = lineas[i];
            for (size_t j = 0; j < linea.size(); ++j) {
                char c = linea[j];
                frecuencias[c]++;
            }
        }
    }

    // 2) Construir árbol de Huffman (usa map<char,int>)
    ArbolCod arbol;
    arbol.Huffman(frecuencias);

    // 3) Abrir archivo binario para escritura
    std::ofstream archivo(nombre_archivo.c_str(), std::ios::binary);
    if(!archivo.is_open()){
        std::cout << "No se pueden guardar las secuencias cargadas en " << nombre_archivo << "\n";
        return;
    }

    // 4) Escribir n (uint16_t)
    uint16_t n = static_cast<uint16_t>(frecuencias.size());
    archivo.write(reinterpret_cast<char*>(&n), sizeof(uint16_t));

    // 5) Escribir (ci, fi) para cada frecuencia: ci (1 byte), fi (uint64_t)
    for (auto itF = frecuencias.begin(); itF != frecuencias.end(); ++itF) {
        char ci = itF->first;
        uint64_t fi = static_cast<uint64_t>(itF->second);
        archivo.write(reinterpret_cast<char*>(&ci), sizeof(char));
        archivo.write(reinterpret_cast<char*>(&fi), sizeof(uint64_t));
    }

    // 6) Escribir ns (uint32_t): número de secuencias
    uint32_t ns = static_cast<uint32_t>(secuencias.size());
    archivo.write(reinterpret_cast<char*>(&ns), sizeof(uint32_t));

    // 7) Para cada secuencia escribir: li (uint16_t) nombre, wi (uint64_t) bits, xi (uint16_t), binary_code
    for (auto itS = secuencias.begin(); itS != secuencias.end(); ++itS) {
        // nombre y li
        std::string nombre = itS->ObtenerDescripcion();
        if(!nombre.empty() && nombre[0] == '>') nombre = nombre.substr(1);
        uint16_t li = static_cast<uint16_t>(nombre.size());
        archivo.write(reinterpret_cast<char*>(&li), sizeof(uint16_t));
        if (li > 0) archivo.write(nombre.c_str(), li);

        // concatenar las lineas en 'bases'
        const std::vector<std::string>& lineasVec = itS->ObtenerLineasSecuencia();
        std::string bases;
        bases.reserve( (size_t)std::max<size_t>(1, lineasVec.size()) * 80 ); // heurística
        for (size_t i = 0; i < lineasVec.size(); ++i) bases += lineasVec[i];

        // codificar con Huffman -> bits (string de '0'/'1')
        std::string bits = arbol.codificacion(bases);

        // wi: longitud en bits (antes de padding que agregamos para bytes completos)
        uint64_t wi = static_cast<uint64_t>(bits.size());

        // rellenar hasta múltiplo de 8
        while (bits.size() % 8 != 0) bits.push_back('0');

        // escribir wi (uint64_t)
        archivo.write(reinterpret_cast<char*>(&wi), sizeof(uint64_t));

        // xi: justificación (anchura de línea) -> uint16_t
        uint16_t xi = 0;
        if (!lineasVec.empty()) xi = static_cast<uint16_t>(lineasVec[0].size());
        archivo.write(reinterpret_cast<char*>(&xi), sizeof(uint16_t));

        // escribir binary_code: convertir cada 8 bits en un byte y escribir
        for (size_t j = 0; j < bits.size(); j += 8) {
            std::string byteStr = bits.substr(j, 8);
            std::bitset<8> byte(byteStr);
            unsigned char bb = static_cast<unsigned char>(byte.to_ulong());
            archivo.write(reinterpret_cast<char*>(&bb), 1);
            std::bitset<8> bitsMostrados(bb);
        }
    }

    archivo.close();
    std::cout << "\nSecuencias codificadas y almacenadas en " << nombre_archivo << std::endl;
}



//COMANDO DECODIFICAR
void Sistema:: decodificar(std::string nombre_archivo) {
    // Intentar abrir el archivo .fabin en binario
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

        // Leer ci (1 byte) y fi (uint64_t) n veces -> construir tabla de frecuencias
        std::map<char, uint64_t> freq64;
        for (uint16_t i = 0; i < n; ++i) {
            char ci;
            uint64_t fi = 0;
            fin.read(&ci, 1);
            fin.read(reinterpret_cast<char*>(&fi), sizeof(uint64_t));
            if (!fin) throw std::runtime_error("Formato invalido (ci/fi).");
            freq64[ci] = fi;
        }

        // Convertir a map<char,int> si ArbolCod lo requiere (Huffman firma: map<char,int>&)
        std::map<char,int> freq_for_tree;
        for (const auto &p : freq64) {
            // cast puede truncar si la frecuencia > INT_MAX; ajustar según necesidad
            freq_for_tree[p.first] = static_cast<int>(p.second);
        }

        // Leer ns (uint32_t): número de secuencias
        uint32_t ns = 0;
        fin.read(reinterpret_cast<char*>(&ns), sizeof(uint32_t));
        if (!fin) throw std::runtime_error("Formato invalido (ns).");

        // Crear árbol de Huffman a partir de las frecuencias
        ArbolCod arbol;
        arbol.Huffman(freq_for_tree);

        // Preparar nueva lista de secuencias que cargaremos (sobrescribe las existentes al final)
        std::list<Secuencia> nuevas;

        for (uint32_t s = 0; s < ns; ++s) {
            // li (uint16_t) -> tamaño del nombre
            uint16_t li = 0;
            fin.read(reinterpret_cast<char*>(&li), sizeof(uint16_t));
            if (!fin) throw std::runtime_error("Formato invalido (li).");

            // leer nombre (li bytes)
            std::string nombre;
            if (li > 0) {
                nombre.resize(li);
                fin.read(&nombre[0], li);
                if (!fin) throw std::runtime_error("Formato invalido (nombre).");
            }

            // wi (uint64_t) -> longitud de la secuencia (en bits)
            uint64_t wi = 0;
            fin.read(reinterpret_cast<char*>(&wi), sizeof(uint64_t));
            if (!fin) throw std::runtime_error("Formato invalido (wi).");

            // xi (uint16_t) -> justificación (ancho de línea)
            uint16_t xi = 0;
            fin.read(reinterpret_cast<char*>(&xi), sizeof(uint16_t));
            if (!fin) throw std::runtime_error("Formato invalido (xi).");

            // Leer la secuencia binaria: wi bits => ceil(wi/8) bytes
            uint64_t bytesNecesarios = (wi + 7) / 8;
            std::vector<char> raw(bytesNecesarios);
            if (bytesNecesarios > 0) {
                fin.read(reinterpret_cast<char*>(raw.data()), bytesNecesarios);
                if (!fin) throw std::runtime_error("Formato invalido (binary_data).");
            }

            // Convertir bytes a string de '0'/'1' y recortar a wi bits exactos
            std::string bits;
            bits.reserve(static_cast<size_t>(bytesNecesarios * 8));
            for (uint64_t b = 0; b < bytesNecesarios; ++b) {
                unsigned char byte = static_cast<unsigned char>(raw[b]);
                for (int bit = 7; bit >= 0; --bit) {
                    bits.push_back( ((byte >> bit) & 1) ? '1' : '0' );
                }
            }
            if (bits.size() > wi) bits.resize(static_cast<size_t>(wi));

            // Decodificar la secuencia de bits usando el árbol (devuelve la secuencia de bases)
            std::string decod = arbol.decodificacion(bits);
            // ArbolCod::decodificacion puede devolver mensajes de error como "Codigo incompleto"
            if (decod == "Codigo incompleto") {
                throw std::runtime_error("Error decodificacion: codigo incompleto.");
            }

            // Reconstruir las lineas con justificacion xi
            std::vector<std::string> lineas;
            if (xi == 0) {
                // Por seguridad: si xi==0, colocamos todo en una sola línea
                lineas.push_back(decod);
            } else {
                for (size_t pos = 0; pos < decod.size(); pos += xi) {
                    lineas.push_back(decod.substr(pos, xi));
                }
            }

            // Crear objeto Secuencia y fijar sus datos
            Secuencia sec;
            sec.FijarDescripcion(nombre);
            sec.FijarLineasSecuencia(lineas);
            // Las demás propiedades (numbases, codigos, etc.) serán calculadas en EstablecerCodigosYBases()

            // Agregar a la lista temporal
            nuevas.push_back(sec);
        }

        // Cerrar archivo
        fin.close();

        // Sobrescribir las secuencias en memoria
        this->FijarSecuencias(nuevas);

        // Establecer códigos y bases y ordenar para cada secuencia (igual que en cargar)
        std::list<Secuencia>::iterator itS;
        for (itS = this->ObtenerSecuencias().begin(); itS != this->ObtenerSecuencias().end(); ++itS) {
            itS->EstablecerCodigosYBases();
        }
        for (itS = this->ObtenerSecuencias().begin(); itS != this->ObtenerSecuencias().end(); ++itS) {
            itS->OrdenarCodigosYBases();
        }

        // Mensaje de éxito (coincide con la especificación)
        std::cout << "Secuencias decodificadas desde " << nombre_archivo << " y cargadas en memoria." << std::endl;
    }
    catch (const std::exception &e) {
        // Si hubo cualquier error, informar el mensaje de error según el enunciado
        fin.close();
        std::cout << "No se pueden cargar las secuencias desde " << nombre_archivo << "." << std::endl;
        // (Opcional) imprimir info para debug:
        // std::cerr << "Detalle: " << e.what() << std::endl;
        return;
    }
}

//COMANDO RUTA_MAS_CORTA
void Sistema :: ruta_mas_corta(std::string descripcion_secuencia, int i, int j, int x, int y){

    std::cout<<"Exito ruta_mas_corta"<<descripcion_secuencia<<" i= "<<i<<" j= "<<j<<" x= "<<x<<" y= "<<y<<"\n";

}

//COMANDO BASE REMOTA
void Sistema :: base_remota(std::string base_remota, int i, int j){

    std::cout<<"Exito base_remota"<<base_remota<<" i= "<<i<<" j= "<<j<<"\n";

}
