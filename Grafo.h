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
	void fijarVertices(std::vector<Base>& nverts);
	bool fijarAristas(double** naris);
	
	//Operaciones sobre vertices
	int cantidadVertices();
	bool insertarVertice(Base dato);
	int buscarVertice(Base dato);
	bool eliminarVertice(Base dato);
	
	//Operaciones sobre aristas
	int cantidadAristas();
	bool insertarArista(Base origen, Base destino, double peso);
	P buscarArista(Base origen, Base destino); 
	bool eliminarArista(Base origen, Base destino);

	//Recorridos
	void recorridoPlano(); 
	bool recorridoEnProfundidad(Base vertice, std::vector<Base>& visitados);
	bool recorridoEnAnchura(Base vInicial, std::vector<Base>& visitados); 
};

#endif