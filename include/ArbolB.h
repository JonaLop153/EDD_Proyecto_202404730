#ifndef ARBOLB_H
#define ARBOLB_H

#include <vector>
#include <string>
#include <fstream>
#include "Avion.h"

using namespace std;

/* ===== NODO DEL ÁRBOL B ===== */
class NodoB {
public:
    bool esHoja;
    int n;
    vector<string> claves;
    vector<Avion*> aviones;
    vector<NodoB*> hijos;

    NodoB(bool hoja);
};

/* ===== ÁRBOL B ===== */
class ArbolB {
private:
    NodoB* raiz;
    int orden;

    void insertarNoLleno(NodoB* nodo, string clave, Avion* avion);
    void dividirHijo(NodoB* padre, int i, NodoB* hijo);

    Avion* buscarRecursivo(NodoB* nodo, string clave);

    void eliminarRecursivo(NodoB* nodo, string clave);
    void obtenerPredecesor(NodoB* nodo, int idx, string& clave, Avion*& avion);
    void obtenerSucesor(NodoB* nodo, int idx, string& clave, Avion*& avion);
    void llenar(NodoB* nodo, int idx);
    void prestarDelAnterior(NodoB* nodo, int idx);
    void prestarDelSiguiente(NodoB* nodo, int idx);
    void fusionar(NodoB* nodo, int idx);

    void generarDotRecursivo(NodoB* nodo, ofstream& archivo, int& contador);

public:
    ArbolB(int orden);

    void insertar(string clave, Avion* avion);
    Avion* buscar(string clave);
    void eliminar(string clave);

    void generarReporte(string nombreArchivo);
    bool estaVacio();
};

#endif
