#ifndef MATRIZDISPERSA_H
#define MATRIZDISPERSA_H

#include <string>
#include <fstream>
using namespace std;

class NodoEncabezado;

class NodoMatriz {
public:
    string valor; // vuelo
    NodoEncabezado* fila;
    NodoEncabezado* columna;

    NodoMatriz* arriba;
    NodoMatriz* abajo;
    NodoMatriz* izquierda;
    NodoMatriz* derecha;

    NodoMatriz(string val, NodoEncabezado* f, NodoEncabezado* c);
};

class NodoEncabezado {
public:
    string id;
    NodoEncabezado* siguiente;
    NodoEncabezado* anterior;
    NodoMatriz* acceso;

    NodoEncabezado(string identificador);
};

class MatrizDispersa {
private:
    NodoEncabezado* filasEncabezado;
    NodoEncabezado* columnasEncabezado;

    NodoEncabezado* buscarCrearFila(string idPiloto);
    NodoEncabezado* buscarCrearColumna(string ciudad);

    void insertarEnFila(NodoMatriz* nuevo);
    void insertarEnColumna(NodoMatriz* nuevo);

    void generarDotMatriz(ofstream& archivo);

public:
    MatrizDispersa();

    void insertar(string idPiloto, string ciudad, string vuelo);
    void eliminarPiloto(string idPiloto);

    void mostrar();
    void generarReporte(const string& nombreArchivo);
};

#endif
