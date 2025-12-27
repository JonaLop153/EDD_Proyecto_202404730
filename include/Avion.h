#ifndef AVION_H
#define AVION_H

#include <string>
#include <iostream>

using namespace std;

class Avion {
private:
    string vuelo;
    string numeroRegistro;
    string modelo;
    int capacidad;
    string aerolinea;
    string ciudadDestino;
    string estado;

public:
    // Constructor
    Avion(string vuelo,
          string numeroRegistro,
          string modelo,
          int capacidad,
          string aerolinea,
          string ciudadDestino,
          string estado);

    // Getters
    string getVuelo();
    string getNumeroRegistro();
    string getModelo();
    int getCapacidad();
    string getAerolinea();
    string getCiudadDestino();
    string getEstado();

    // Setters
    void setEstado(string nuevoEstado);

    // Métodos
    void mostrar();
};

#endif
