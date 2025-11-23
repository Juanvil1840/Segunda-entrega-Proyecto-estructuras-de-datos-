#ifndef __GRAFO_H__
#define __GRAFO_H__
#include <vector>

class Grafo {
    protected:
	std::vector<Base> vertices;
	double** aristas;
    public:
	Grafo();
	Grafo(std::vector<Base> &verts, double** aris);
	~Grafo();
	std::vector<Base>& obtenerVertices();
	double** obtenerAristas();
	Base obtenerBase(int indice);
	void fijarVertices(std::vector<Base>& nverts);
	bool fijarAristas(double** naris);
	
	//Operaciones sobre vertices
	int cantidadVertices();
	bool insertarVertice(Base dato);
	int buscarVertice(Base dato);
	int buscarVertice(int i, int j);
	bool eliminarVertice(Base dato);
	
	//Operaciones sobre aristas
	int cantidadAristas();
	bool insertarArista(int indiceOrigen, int indiceDestino, double peso);
	double buscarArista(Base origen, Base destino); 
	bool eliminarArista(Base origen, Base destino);

	//Recorridos
	void recorridoPlano(); 
	bool recorridoEnProfundidad(Base vertice, std::vector<Base>& visitados);
	bool recorridoEnAnchura(Base vInicial, std::vector<Base>& visitados); 

	// Rutas
	std::vector<int> rutaMasCorta(int inicio, int destino);
	double Grafo::costoRuta(std::vector<int> ruta);
};

#endif