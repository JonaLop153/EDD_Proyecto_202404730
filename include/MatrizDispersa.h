#ifndef MATRIZDISPERSA_H
#define MATRIZDISPERSA_H

#include <string>
#include <fstream>
using namespace std;

class NodoMatriz {
public:
    string valor;
    int fila;
    int columna;
    NodoMatriz* arriba;
    NodoMatriz* abajo;
    NodoMatriz* izquierda;
    NodoMatriz* derecha;
    
    NodoMatriz(string val, int f, int c);
};

class NodoEncabezado {
public:
    string id;
    int posicion;
    NodoEncabezado* siguiente;
    NodoEncabezado* anterior;
    NodoMatriz* acceso;
    
    NodoEncabezado(string identificador, int pos);
};

class MatrizDispersa {
private:
    NodoEncabezado* filasEncabezado;
    NodoEncabezado* columnasEncabezado;
    
    NodoEncabezado* buscarCrearEncabezadoFila(string idPiloto, int& pos);
    NodoEncabezado* buscarCrearEncabezadoColumna(string ciudad, int& pos);
    void insertarNodoEnFila(NodoMatriz* nuevo, NodoEncabezado* encabezadoFila);
    void insertarNodoEnColumna(NodoMatriz* nuevo, NodoEncabezado* encabezadoColumna);
    void generarDotMatriz(ofstream& archivo);
    void limpiarFilaSinConexiones(NodoEncabezado* encabezado);
    void limpiarColumnaSinConexiones(NodoEncabezado* encabezado);

public:
    MatrizDispersa();
    void insertar(string idPiloto, string ciudad, string vuelo);
    void eliminarPiloto(string idPiloto);
    void mostrar();
    void generarReporte(string nombreArchivo);
};

#endif