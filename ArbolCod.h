#ifndef __ARBOLCOD_H__
#define __ARBOLCOD_H__

#include "NodoCod.h"
#include <map>

class ArbolCod{
    protected:
	NodoCod* raiz;
    public:
	ArbolCod();
	~ArbolCod();
	bool esVacio();
	void Huffman(std::map<char,int>& codigos);
	std::string codificacion(std::string frase);
 	std::string decodificacion(std::string fraseCod);
};

#endif