#include "Secuencia.h"
#include <algorithm>

//CONSTRUCTOR
Secuencia:: Secuencia(){
descripcion = "";
lineas_secuencia.clear();
};

//LIBERAR MEMORIA
Secuencia:: ~Secuencia(){
};

// obtener descripcion
std::string& Secuencia:: ObtenerDescripcion(){
    return( descripcion );
}

// obtener lineas_secuencia
std::vector< std::string >& Secuencia:: ObtenerLineasSecuencia(){
    return( lineas_secuencia );
}

// obtener numbases
int Secuencia:: ObtenerNumbases(){
    return( numbases );
}

// obtener numcodigos
int Secuencia:: ObtenerNumcodigos(){
    return( numcodigos );
}

// obtener codigos
std::vector< char >& Secuencia:: ObtenerCodigos(){
    return( codigos );
}

// fijar descripcion
void Secuencia:: FijarDescripcion(std::string ndescripcion){
    this->descripcion = ndescripcion;
}

// fijar lineas_secuencia
void Secuencia:: FijarLineasSecuencia(std::vector< std::string > n_lineas_secuencia ){
    this->lineas_secuencia = n_lineas_secuencia;
}

// fijar numbases
void Secuencia:: FijarNumbases(int n_numbases ){
    this->numbases = n_numbases;
}

// fijar numcodigos
void Secuencia:: FijarNumcodigos(int n_numcodigos ){
    this->numcodigos = n_numcodigos;
}

// fijar codigos
void Secuencia:: FijarCodigos(std::vector< char > nCodigos){
    this->codigos = nCodigos;
}

// agregar una linea a lineas_secuencia
void Secuencia:: AgregarLineaSecuencia(std::string linea){
    std::vector<std::string> temp = this->ObtenerLineasSecuencia();
    temp.push_back(linea); 
    this->FijarLineasSecuencia(temp); 
}

// establecer codigos, numcodigos y numbases
void Secuencia:: EstablecerCodigosYBases(){
    int bases = 0, ncodigos = 0;
    std::vector< char > codigos;
    //Recorrer el vector de lineas
    std::vector< std::string >::iterator itL;
    for(itL = this->ObtenerLineasSecuencia().begin(); itL != this->ObtenerLineasSecuencia().end(); itL ++){
        //Recorrer la cadena de caracteres para contar las bases y codigos
        for(char c: *itL){
	    if(c >= 'A' && c <= 'Z') bases ++;
	    if((c >= 'A' && c <= 'Z') || (c == '-')){
	        ncodigos++;
	        if(std::find(codigos.begin(), codigos.end(), c) == codigos.end()){
 		    codigos.push_back(c);
	        }
	    }
  
        } 
    }

    this->FijarNumbases(bases);
    this->FijarNumcodigos(ncodigos);
    this->FijarCodigos(codigos);
}

void Secuencia:: OrdenarCodigosYBases(){
    std:: string orden = "ACGTURYKMSWBDHVNX-"; //cadena de caracteres que define el orden
    int* ordenCodigos = new int[this->ObtenerCodigos().size()];

    // Recorre el vector de codigos
    for(int i = 0 ; i < this -> ObtenerCodigos().size() ; i++){
	//Recorre el arreglo de caracteres con los ordenes
	for(int j = 0; j < orden.size() ; j++){
	    //Establece el orden
	    if(this -> ObtenerCodigos()[i] == orden[j]){
		ordenCodigos[i] = j;
	    }
	}
    }

    //Ordenar según el orden anteriormente establecido
    std::vector< char > codigos = this -> ObtenerCodigos();
    for(int i = 0; i < this -> ObtenerCodigos().size() ; i++){
	for (int j = 0; j < this -> ObtenerCodigos().size() - 1; j++){
	    if (ordenCodigos[j] > ordenCodigos[j + 1]) {
		int tempOrden = ordenCodigos[j];
		char tempCodigos = codigos[j];

		ordenCodigos[j] = ordenCodigos[j+1];
		codigos[j] = codigos[j+1];

		ordenCodigos[j+1] = tempOrden;
		codigos[j+1] = tempCodigos;
	    }
	}
    }

    this -> FijarCodigos(codigos);

    delete[] ordenCodigos;
}

bool Secuencia:: VerificarCodigosValidos(){
    bool SecValida = true;
    std :: string permitidos = "ACGTURYKMSWBDHVNX-"; //cadena de caracteres que define los codigos permitidos
    std :: vector<char> invalidos; //vector que guarda todos los caracteres invalidos encontrados

    //Recorrer las lineas de secuencias
    std::vector< std::string >::iterator itL;
    for(itL = this->ObtenerLineasSecuencia().begin(); itL != this->ObtenerLineasSecuencia().end(); itL ++){
        //Recorrer la cadena de caracteres
        for(char c: *itL){

	    //Las siguientes comparaciones fueron hechas con IA generativa

	    if(permitidos.find(c) == std::string::npos){
		// Revisar que el caracter no este ya en el vector de invalidos
		if (std::find(invalidos.begin(), invalidos.end(), c) == invalidos.end()) {
                    invalidos.push_back(c);
		}
	    }
	}
    }

    if(!invalidos.empty()){
	std :: cout << "La secuencia " << this->ObtenerDescripcion() 
            << " no puede ser cargada en el sistema porque contiene codigos invalidos para una secuencia genetica como: ";

	
	for(int i = 0; i < invalidos.size(); i++){
	    std :: cout << '\'' << invalidos[i] << '\''; 
	}
	return false;
    }

    return true;
}
