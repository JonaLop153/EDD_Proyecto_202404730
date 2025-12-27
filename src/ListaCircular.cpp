#include "../include/ListaCircular.h"
#include <iostream>

NodoCircular::NodoCircular(Avion* a) {
    avion = a;
    siguiente = nullptr;
    anterior = nullptr;
}

ListaCircular::ListaCircular() {
    inicio = nullptr;
    tamanio = 0;
}

void ListaCircular::insertar(Avion* avion) {
    NodoCircular* nuevo = new NodoCircular(avion);

    if (inicio == nullptr) {
        inicio = nuevo;
        nuevo->siguiente = nuevo;
        nuevo->anterior = nuevo;
    } else {
        NodoCircular* ultimo = inicio->anterior;

        nuevo->siguiente = inicio;
        nuevo->anterior = ultimo;
        ultimo->siguiente = nuevo;
        inicio->anterior = nuevo;
    }
    tamanio++;
}

Avion* ListaCircular::buscar(string numeroRegistro) {
    if (inicio == nullptr) return nullptr;

    NodoCircular* actual = inicio;
    do {
        if (actual->avion->getNumeroRegistro() == numeroRegistro) {
            return actual->avion;
        }
        actual = actual->siguiente;
    } while (actual != inicio);

    return nullptr;
}

void ListaCircular::eliminar(string numeroRegistro) {
    if (inicio == nullptr) return;

    NodoCircular* actual = inicio;
    do {
        if (actual->avion->getNumeroRegistro() == numeroRegistro) {
            if (actual->siguiente == actual) {
                inicio = nullptr;
            } else {
                actual->anterior->siguiente = actual->siguiente;
                actual->siguiente->anterior = actual->anterior;

                if (actual == inicio) {
                    inicio = actual->siguiente;
                }
            }
            delete actual;
            tamanio--;
            return;
        }
        actual = actual->siguiente;
    } while (actual != inicio);
}

void ListaCircular::mostrar() {
    if (inicio == nullptr) {
        cout << "Lista vacia" << endl;
        return;
    }

    cout << "\n=== Aviones en Mantenimiento ===" << endl;
    NodoCircular* actual = inicio;
    int contador = 1;
    do {
        cout << contador++ << ". ";
        actual->avion->mostrar();
        actual = actual->siguiente;
    } while (actual != inicio);
}

void ListaCircular::generarReporte(string nombreArchivo) {
    ofstream archivo("reportes/" + nombreArchivo + ".dot");
    archivo << "digraph ListaCircular {\n";
    archivo << "rankdir=LR;\n";
    archivo << "node [shape=record];\n";

    generarDotLista(archivo);

    archivo << "}\n";
    archivo.close();

    string comando = "dot -Tpng reportes/" + nombreArchivo +
                     ".dot -o reportes/" + nombreArchivo + ".png";
    system(comando.c_str());
}

void ListaCircular::generarDotLista(ofstream& archivo) {
    if (inicio == nullptr) {
        archivo << "vacio [label=\"Lista Vacia\"];\n";
        return;
    }

    NodoCircular* actual = inicio;
    do {
        archivo << "\""
                << actual->avion->getNumeroRegistro()
                << "\" [label=\"{"
                << actual->avion->getNumeroRegistro() << "|"
                << actual->avion->getModelo() << "|"
                << actual->avion->getEstado()
                << "}\"];\n";
        actual = actual->siguiente;
    } while (actual != inicio);

    actual = inicio;
    do {
        archivo << "\""
                << actual->avion->getNumeroRegistro()
                << "\" -> \""
                << actual->siguiente->avion->getNumeroRegistro()
                << "\";\n";

        archivo << "\""
                << actual->avion->getNumeroRegistro()
                << "\" -> \""
                << actual->anterior->avion->getNumeroRegistro()
                << "\" [constraint=false];\n";

        actual = actual->siguiente;
    } while (actual != inicio);
}

bool ListaCircular::estaVacia() {
    return inicio == nullptr;
}
