#include "Grafo.h"
#include <algorithm>
#include <queue>

Grafo::Grafo() {
    vertices.clear();
    aristas = nullptr;
}

Grafo::Grafo(std::vector<Base> &verts, double** aris){
    vertices = verts; 

    //Como el grafo es un par ordenado, es importante verificar que exista el conjunto de vertices para poder crear el conjunto de aristas
    if(!vertices.empty()){
        std::size_t n = vertices.size();
	aristas = new double*[n];
        
        for (std::size_t i = 0; i < n; ++i) {
            aristas[i] = new double[n];
            for (std::size_t j = 0; j < n; ++j) {
                aristas[i][j] = aris[i][j];
            }
        }
    }else{
	aristas = nullptr;
    }
}

Grafo::~Grafo() {
    if (aristas != nullptr) {
        std::size_t n = vertices.size();
        for (std::size_t i = 0; i < n; ++i) {
            delete[] aristas[i];
        }
        delete[] aristas;
        aristas = nullptr;
    }

    vertices.clear();
}

std::vector<Base>& Grafo::obtenerVertices(){
    return vertices;
}

double** Grafo::obtenerAristas(){
    return aristas;
}

void Grafo::fijarVertices(std::vector<Base>& nvertices){
    if (aristas != nullptr) {
        std::size_t n = vertices.size();
        for (std::size_t i = 0; i < n; ++i) {
            delete[] aristas[i];
        }
        delete[] aristas;
        aristas = nullptr;
    }

    vertices = nvertices;
}

bool Grafo::fijarAristas(double** naris) {
    if (vertices.empty()) return false;

    std::size_t n = vertices.size();

    if (aristas != nullptr) {
        for (std::size_t i = 0; i < n; ++i) {
            delete[] aristas[i];
        }
        delete[] aristas;
    }

    aristas = new double*[n];
    for (std::size_t i = 0; i < n; ++i) {
        aristas[i] = new double[n];
        for (std::size_t j = 0; j < n; ++j) {
            aristas[i][j] = naris[i][j];
        }
    }

    return true;
}


//Operaciones sobre vertices
int Grafo::cantidadVertices(){
    return vertices.size();
}

bool Grafo::insertarVertice(Base dato){
    if(buscarVertice(dato) != -1) return false;

    vertices.push_back(dato);

    double** nueva = new doible*[vertices.size()];
    for (std::size_t i = 0; i < vertices.size(); ++i) {
        nueva[i] = new double[vertices.size()];
    }

    if (aristas != nullptr && vertices.size()-1 > 0) {
        for (std::size_t i = 0; i < vertices.size()-1; ++i) {
            for (std::size_t j = 0; j < vertices.size()-1; ++j) {
                nueva[i][j] = aristas[i][j];
            }
        }
    }

    for (std::size_t i = 0; i < vertices.size(); ++i) {
        nueva[i][vertices.size() - 1] = 0;       
        nueva[vertices.size() - 1][i] = 0;       
    }

    if (aristas != nullptr && vertices.size()-1 > 0) {
        for (std::size_t i = 0; i < vertices.size()-1; ++i) {
            delete[] aristas[i];
        }
        delete[] aristas;
    }

    aristas = nueva;

    return true;    
}

int Grafo::buscarVertice(Base dato){
    for(std::size_t i = 0; i < vertices.size(); i++){
	if(vertices[i] == dato) return (int)i;
    }   

    return -1; 
}

int Grafo::buscarVertice(int i, int j){
    for(std::size_t i = 0; i < vertices.size(); i++){
	if((vertices[i].obteneri() == i) && (vertices[i].obtenerj() == j)) return (int)i;
    }   

    return -1; 
}

bool Grafo::eliminarVertice(Base dato){
    int indice = buscarVertice(dato);
    if(indice == -1) return false;

    double** nueva = nullptr;

    if (vertices.size()-1 > 0) {
        nueva = new double*[vertices.size()-1];
        for (std::size_t i = 0; i < vertices.size()-1; ++i) {
            nueva[i] = new double[vertices.size()-1];
        }
    }

    if (aristas != nullptr && vertices.size()-1 > 0) {
        for (std::size_t i = 0, ni = 0; i < vertices.size(); ++i) {
            if (i == indice) continue;

            for (std::size_t j = 0, nj = 0; j < vertices.size(); ++j) {
                if (j == indice) continue;

                nueva[ni][nj] = aristas[i][j];
                nj++;
            }
            ni++;
        }
    }

    if (aristas != nullptr) {
        for (std::size_t i = 0; i < vertices.size(); ++i) {
            delete[] aristas[i];
        }
        delete[] aristas;
    }

    aristas = nueva;

    typename std::vector<Base>::iterator itv = vertices.begin() + indice;
    vertices.erase(itv);

    return true;
}

//Operaciones sobre aristas
int Grafo::cantidadAristas(){
    if (aristas == nullptr) return 0;
    int cantArist = 0;
    
    for (std::size_t i = 0; i < vertices.size(); ++i){
	for (std::size_t j = 0; j < vertices.size(); ++j){
	    if(aristas[i][j] != 0) cantArist++;
	}
    }

    return cantArist;
}

bool Grafo::insertarArista(int indiceOrigen, int indiceDestino, double peso){
    if((aristas == nullptr) || (indiceOrigen == -1) || (indiceDestino == -1)) return false;

    aristas[indiceOrigen][indiceDestino] = peso;
     
    return true;
}

double Grafo::buscarArista(Base origen, Base destino){
    int indiceOrigen = buscarVertice(origen);
    int indiceDestino = buscarVertice(destino);

    if((aristas == nullptr) || (indiceOrigen == -1) || (indiceDestino == -1)) return 0;

    return aristas[indiceOrigen][indiceDestino];
}

bool Grafo::eliminarArista(Base origen, Base destino){
    int indiceOrigen = buscarVertice(origen);
    int indiceDestino = buscarVertice(destino);

    if((aristas == nullptr) || (indiceOrigen == -1) || (indiceDestino == -1)) return false;

    aristas[indiceOrigen][indiceDestino] = 0;

    return true;
}

//Recorridos
void Grafo::recorridoPlano(){
    for(std::size_t i = 0; i < vertices.size(); i++){
	std::cout << vertices[i] << " ";
    }

    std::cout << std::endl;
}

bool Grafo::recorridoEnProfundidad(Base vertice, std::vector<Base>& visitados){
    int indice = buscarVertice(vertice);
    if(indice == -1) return false;

    if(std::find(visitados.begin(),visitados.end(), vertice) != visitados.end()) return true;

    visitados.push_back(vertices[indice]);
    if (aristas == nullptr) return true;

    std::size_t n = vertices.size();
    std::vector<int> vecinos;
    for (std::size_t j = 0; j < n; ++j) {
        if (aristas[indice][j] != 0) {
            vecinos.push_back((int)j);
        }
    }

    //Ordenar la lista de vecinos de menor a mayor
    std::sort(vecinos.begin(), vecinos.end(),
              [this](int a, int b){
                  return vertices[a] < vertices[b];
              });

    // Recorrer los vecinos no visitados
    for (int idxVecino : vecinos) {
        T vecino = vertices[idxVecino];
        if (std::find(visitados.begin(), visitados.end(), vecino) == visitados.end()) {
            if (!recorridoEnProfundidad(vecino, visitados))
                return false;
        }
    }

    return true;
}

bool Grafo::recorridoEnAnchura(Base vInicial, std::vector<Base>& visitados){
    int indice = buscarVertice(vInicial);
    if(indice == -1) return false;

    visitados.push_back(vertices[indice]);

    if (aristas == nullptr) return true;

    std::queue<int> colaVertices;
    colaVertices.push(indice);

    while(!colaVertices.empty()){
	int vecino = colaVertices.front();
	colaVertices.pop();

	std::vector<int> vecinos;
        for (std::size_t i = 0; i < vertices.size(); ++i) {
            if (aristas[vecino][i] != 0) {
                vecinos.push_back((int)i);
            }
        }

	//Ordenar la lista de vecinos inmediatos de menor a mayor
    std::sort(vecinos.begin(), vecinos.end(),
              [this](int a, int b){
                  return vertices[a] < vertices[b];
              });

        for (std::size_t i = 0; i < vecinos.size(); ++i) {
            int indiceVecino = vecinos[i];
            T datoVecino = vertices[indiceVecino];

            if (std::find(visitados.begin(), visitados.end(), datoVecino) == visitados.end()) {
                visitados.push_back(datoVecino);
                colaVertices.push(indiceVecino);
            }
        }	
    }

    return true;
} 