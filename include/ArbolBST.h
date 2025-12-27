#ifndef ARBOLBST_H
#define ARBOLBST_H

#include "Piloto.h"
#include <fstream>

class NodoBST {
public:
    Piloto* piloto;
    NodoBST* izquierdo;
    NodoBST* derecho;
    int altura;

    NodoBST(Piloto* p);
};

class ArbolBST {
private:
    NodoBST* raiz;
    
    int obtenerAltura(NodoBST* nodo);
    int obtenerBalance(NodoBST* nodo);
    NodoBST* rotarDerecha(NodoBST* y);
    NodoBST* rotarIzquierda(NodoBST* x);
    NodoBST* insertarRecursivo(NodoBST* nodo, Piloto* piloto);
    NodoBST* eliminarRecursivo(NodoBST* nodo, string id);
    NodoBST* encontrarMinimo(NodoBST* nodo);
    Piloto* buscarRecursivo(NodoBST* nodo, string id);
    void preordenRecursivo(NodoBST* nodo);
    void inordenRecursivo(NodoBST* nodo);
    void postordenRecursivo(NodoBST* nodo);
    void generarDotRecursivo(NodoBST* nodo, ofstream& archivo);

public:
    ArbolBST();
    void insertar(Piloto* piloto);
    void eliminar(string id);
    Piloto* buscar(string id);
    void preorden();
    void inorden();
    void postorden();
    void generarReporte(string nombreArchivo);
    bool estaVacio();
};

#endif