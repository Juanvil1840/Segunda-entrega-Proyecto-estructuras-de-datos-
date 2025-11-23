#ifndef __BASE_H__
#define __BASE_H__

class Base {
    protected:
	char codigo
	int i;
	int j;
    public:
	Base();
	Base(char ncodigo, int ni, int nj);
	char obtenerCodigo();
	int obteneri();
	int obtenerj();
	void fijarCodigo(char ncodigo);
	void fijari(int ni);
	void fijarj(int nj);
	//operador ==
	//operador <
};

#endif