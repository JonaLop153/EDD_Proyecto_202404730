#include "../include/Avion.h"

Avion::Avion(string vuelo,
             string numeroRegistro,
             string modelo,
             int capacidad,
             string aerolinea,
             string ciudadDestino,
             string estado) {
    this->vuelo = vuelo;
    this->numeroRegistro = numeroRegistro;
    this->modelo = modelo;
    this->capacidad = capacidad;
    this->aerolinea = aerolinea;
    this->ciudadDestino = ciudadDestino;
    this->estado = estado;
}

string Avion::getVuelo() {
    return vuelo;
}

string Avion::getNumeroRegistro() {
    return numeroRegistro;
}

string Avion::getModelo() {
    return modelo;
}

int Avion::getCapacidad() {
    return capacidad;
}

string Avion::getAerolinea() {
    return aerolinea;
}

string Avion::getCiudadDestino() {
    return ciudadDestino;
}

string Avion::getEstado() {
    return estado;
}

void Avion::setEstado(string nuevoEstado) {
    estado = nuevoEstado;
}

void Avion::mostrar() {
    cout << "Registro: " << numeroRegistro
         << " | Modelo: " << modelo
         << " | Estado: " << estado << endl;
}
