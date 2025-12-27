#include "../include/Piloto.h"
#include <iostream>

using namespace std;

Piloto::Piloto(string nom, string nac, string id, string v, int horas, string licencia) {
    nombre = nom;
    nacionalidad = nac;
    numero_de_id = id;
    vuelo = v;
    horas_de_vuelo = horas;
    tipo_de_licencia = licencia;
}

void Piloto::mostrar() {
    cout << "Nombre: " << nombre 
         << " | ID: " << numero_de_id 
         << " | Horas: " << horas_de_vuelo 
         << " | Vuelo: " << vuelo 
         << " | Licencia: " << tipo_de_licencia 
         << " | Nacionalidad: " << nacionalidad << endl;
}