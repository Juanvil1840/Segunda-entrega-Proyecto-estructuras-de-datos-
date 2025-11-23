#ifndef __BASE_H__
#define __BASE_H__

#include <ostream> 

class Base {
    protected:
	char codigo;
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
	bool operator==(const Base& b) const;
	bool operator<(const Base& b) const;
	double pesoConexion(Base b);

	friend std::ostream& operator<<(std::ostream& os, const Base& b){
    	    os << b.codigo << ": (" << b.i << "," << b.j << ")";
    	    return os;
	}
};

#endif