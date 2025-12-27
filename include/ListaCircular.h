#ifndef LISTACIRCULAR_H
#define LISTACIRCULAR_H

#include "Avion.h"
#include <fstream>

class NodoCircular {
public:
    Avion* avion;
    NodoCircular* siguiente;
    NodoCircular* anterior;
    
    NodoCircular(Avion* a);
};

class ListaCircular {
private:
    NodoCircular* inicio;
    int tamanio;
    
    void generarDotLista(ofstream& archivo);

public:
    ListaCircular();
    void insertar(Avion* avion);
    Avion* buscar(string numeroRegistro);
    void eliminar(string numeroRegistro);
    void mostrar();
    void generarReporte(string nombreArchivo);
    bool estaVacia();
};

#endif