#ifndef GRAFO_H
#define GRAFO_H

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <limits>

using namespace std;

class Arista {
public:
    string destino;
    int distancia;
    
    Arista(string dest, int dist);
};

class Grafo {
private:
    map<string, vector<Arista>> listaAdyacencia;
    
    void generarDotGrafo(ofstream& archivo);

public:
    void agregarRuta(string origen, string destino, int distancia);
    void mostrar();
    void generarReporte(string nombreArchivo);
    vector<string> rutaMasCorta(string origen, string destino, int& distanciaTotal);
};

#endif