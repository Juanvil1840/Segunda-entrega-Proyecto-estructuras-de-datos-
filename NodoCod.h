#ifndef __NODOCOD_H__
#define __NODOCOD_H__

#include <string>

class NodoCod {
    protected:
	char dato;
	int frecuencia;
	NodoCod* hijoIzq;
	NodoCod* hijoDer;
    public:
	NodoCod();
	NodoCod(char dato, int frecuencia);
	~NodoCod();
	char& obtenerDato();
	void fijarDato(char val);
	int& obtenerFrecuencia();
	void fijarFrecuencia(int freq);
	NodoCod* obtenerHijoIzq();
	NodoCod* obtenerHijoDer();
	void fijarHijoIzq(NodoCod* izq);
	void fijarHijoDer(NodoCod* der);
	bool esHoja();
	bool codigoCar(char c, std::string& fraseCod);
};

#endif

