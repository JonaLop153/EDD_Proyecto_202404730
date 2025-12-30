#ifndef MATRIZDISPERSA_H
#define MATRIZDISPERSA_H

#include <string>
#include <fstream>
using namespace std;

class NodoEncabezado;

class NodoMatriz {
public:
    string valor; // ID del piloto
    NodoEncabezado* fila;    // Encabezado de vuelo
    NodoEncabezado* columna; // Encabezado de ciudad

    NodoMatriz* arriba;
    NodoMatriz* abajo;
    NodoMatriz* izquierda;
    NodoMatriz* derecha;

    NodoMatriz(string val, NodoEncabezado* f, NodoEncabezado* c);
};

class NodoEncabezado {
public:
    string id; // Puede ser número de vuelo o nombre de ciudad
    NodoEncabezado* siguiente;
    NodoEncabezado* anterior;
    NodoMatriz* acceso;

    NodoEncabezado(string identificador);
};

class MatrizDispersa {
private:
    NodoEncabezado* filasEncabezado;    // Vuelos
    NodoEncabezado* columnasEncabezado; // Ciudades

    NodoEncabezado* buscarCrearFila(string vuelo);
    NodoEncabezado* buscarCrearColumna(string ciudad);

    void insertarEnFila(NodoMatriz* nuevo);
    void insertarEnColumna(NodoMatriz* nuevo);

    void generarDotMatriz(ofstream& archivo);

public:
    MatrizDispersa();

    // Cambiar parámetros: vuelo, ciudad, idPiloto
   void insertar(string vuelo, string ciudad, string idPiloto);
    void eliminarPiloto(string idPiloto);

    void mostrar();
    void generarReporte(const string& nombreArchivo);
};

#endif