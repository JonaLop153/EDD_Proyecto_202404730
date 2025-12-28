#include "../include/MatrizDispersa.h"
#include <iostream>

using namespace std;

/* ================== NODOS ================== */

NodoMatriz::NodoMatriz(string val, NodoEncabezado* f, NodoEncabezado* c) {
    valor = val;
    fila = f;
    columna = c;
    arriba = abajo = izquierda = derecha = nullptr;
}

NodoEncabezado::NodoEncabezado(string identificador) {
    id = identificador;
    siguiente = anterior = nullptr;
    acceso = nullptr;
}

/* ================== MATRIZ ================== */

MatrizDispersa::MatrizDispersa() {
    filasEncabezado = nullptr;
    columnasEncabezado = nullptr;
}

/* ================== BUSCAR / CREAR ================== */

NodoEncabezado* MatrizDispersa::buscarCrearFila(string idPiloto) {
    if (!filasEncabezado) {
        filasEncabezado = new NodoEncabezado(idPiloto);
        return filasEncabezado;
    }

    NodoEncabezado* actual = filasEncabezado;
    NodoEncabezado* anterior = nullptr;

    while (actual && actual->id < idPiloto) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual && actual->id == idPiloto)
        return actual;

    NodoEncabezado* nuevo = new NodoEncabezado(idPiloto);
    nuevo->siguiente = actual;
    nuevo->anterior = anterior;

    if (actual) actual->anterior = nuevo;
    if (anterior) anterior->siguiente = nuevo;
    else filasEncabezado = nuevo;

    return nuevo;
}

NodoEncabezado* MatrizDispersa::buscarCrearColumna(string ciudad) {
    if (!columnasEncabezado) {
        columnasEncabezado = new NodoEncabezado(ciudad);
        return columnasEncabezado;
    }

    NodoEncabezado* actual = columnasEncabezado;
    NodoEncabezado* anterior = nullptr;

    while (actual && actual->id < ciudad) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual && actual->id == ciudad)
        return actual;

    NodoEncabezado* nuevo = new NodoEncabezado(ciudad);
    nuevo->siguiente = actual;
    nuevo->anterior = anterior;

    if (actual) actual->anterior = nuevo;
    if (anterior) anterior->siguiente = nuevo;
    else columnasEncabezado = nuevo;

    return nuevo;
}

/* ================== INSERTAR ================== */

void MatrizDispersa::insertar(string idPiloto, string ciudad, string vuelo) {
    NodoEncabezado* fila = buscarCrearFila(idPiloto);
    NodoEncabezado* columna = buscarCrearColumna(ciudad);

    NodoMatriz* actual = fila->acceso;
    while (actual) {
        if (actual->columna == columna) {
            actual->valor = vuelo;
            return;
        }
        actual = actual->derecha;
    }

    NodoMatriz* nuevo = new NodoMatriz(vuelo, fila, columna);
    insertarEnFila(nuevo);
    insertarEnColumna(nuevo);
}

void MatrizDispersa::insertarEnFila(NodoMatriz* nuevo) {
    NodoMatriz* actual = nuevo->fila->acceso;

    if (!actual || actual->columna->id > nuevo->columna->id) {
        nuevo->derecha = actual;
        if (actual) actual->izquierda = nuevo;
        nuevo->fila->acceso = nuevo;
        return;
    }

    while (actual->derecha &&
           actual->derecha->columna->id < nuevo->columna->id) {
        actual = actual->derecha;
    }

    nuevo->derecha = actual->derecha;
    if (actual->derecha) actual->derecha->izquierda = nuevo;
    actual->derecha = nuevo;
    nuevo->izquierda = actual;
}

void MatrizDispersa::insertarEnColumna(NodoMatriz* nuevo) {
    NodoMatriz* actual = nuevo->columna->acceso;

    if (!actual || actual->fila->id > nuevo->fila->id) {
        nuevo->abajo = actual;
        if (actual) actual->arriba = nuevo;
        nuevo->columna->acceso = nuevo;
        return;
    }

    while (actual->abajo &&
           actual->abajo->fila->id < nuevo->fila->id) {
        actual = actual->abajo;
    }

    nuevo->abajo = actual->abajo;
    if (actual->abajo) actual->abajo->arriba = nuevo;
    actual->abajo = nuevo;
    nuevo->arriba = actual;
}

/* ================== ELIMINAR ================== */

void MatrizDispersa::eliminarPiloto(string idPiloto) {
    NodoEncabezado* fila = filasEncabezado;

    while (fila && fila->id != idPiloto)
        fila = fila->siguiente;

    if (!fila) return;

    NodoMatriz* nodo = fila->acceso;
    while (nodo) {
        if (nodo->arriba) nodo->arriba->abajo = nodo->abajo;
        else nodo->columna->acceso = nodo->abajo;

        if (nodo->abajo) nodo->abajo->arriba = nodo->arriba;

        NodoMatriz* tmp = nodo;
        nodo = nodo->derecha;
        delete tmp;
    }

    if (fila->anterior) fila->anterior->siguiente = fila->siguiente;
    else filasEncabezado = fila->siguiente;

    if (fila->siguiente) fila->siguiente->anterior = fila->anterior;

    delete fila;
}

/* ================== MOSTRAR ================== */

void MatrizDispersa::mostrar() {
    cout << "\n=== Matriz Dispersa (Pilotos x Ciudades) ===" << endl;

    if (!filasEncabezado) {
        cout << "Matriz vacia" << endl;
        return;
    }

    NodoEncabezado* fila = filasEncabezado;
    while (fila) {
        cout << fila->id << ": ";
        NodoMatriz* nodo = fila->acceso;
        while (nodo) {
            cout << "[" << nodo->columna->id
                 << ": " << nodo->valor << "] ";
            nodo = nodo->derecha;
        }
        cout << endl;
        fila = fila->siguiente;
    }
}

/* ================== GRAPHVIZ ================== */

void MatrizDispersa::generarReporte(string nombreArchivo) {
    ofstream archivo("reportes/" + nombreArchivo + ".dot");
    archivo << "digraph Matriz {\nrankdir=TB;\nnode [shape=box];\n";
    generarDotMatriz(archivo);
    archivo << "}\n";
    archivo.close();

    system(("dot -Tpng reportes/" + nombreArchivo +
            ".dot -o reportes/" + nombreArchivo + ".png").c_str());
}

void MatrizDispersa::generarDotMatriz(ofstream& archivo) {
    NodoEncabezado* fila = filasEncabezado;
    while (fila) {
        NodoMatriz* nodo = fila->acceso;
        while (nodo) {
            archivo << "\"" << fila->id << "\" -> \""
                    << nodo->columna->id << "\\n"
                    << nodo->valor << "\";\n";
            nodo = nodo->derecha;
        }
        fila = fila->siguiente;
    }
}
