#ifndef TABLAHASH_H
#define TABLAHASH_H

#include "Piloto.h"
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class NodoHash {
public:
    Piloto* piloto;
    NodoHash* siguiente;
    
    NodoHash(Piloto* p);
};

class TablaHash {
private:
    vector<NodoHash*> tabla;
    int tamanio;
    
    int funcionHash(string id);
    void generarDotTabla(ofstream& archivo);

public:
    TablaHash(int tam);
    void insertar(Piloto* piloto);
    Piloto* buscar(string id);
    void eliminar(string id);
    void generarReporte(string nombreArchivo);
    void mostrar();
};

#endif