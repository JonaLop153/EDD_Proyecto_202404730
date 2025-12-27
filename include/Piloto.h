#ifndef PILOTO_H
#define PILOTO_H

#include <string>
using namespace std;

class Piloto {
public:
    string nombre;
    string nacionalidad;
    string numero_de_id;
    string vuelo;
    int horas_de_vuelo;
    string tipo_de_licencia;

    Piloto(string nom, string nac, string id, string v, int horas, string licencia);
    void mostrar();
};

#endif