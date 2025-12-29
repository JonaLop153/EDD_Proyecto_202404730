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

void MatrizDispersa::generarReporte(const string& nombreArchivo) {
    ofstream archivo("reportes/" + nombreArchivo + ".dot");
    archivo << "digraph Matriz {\n";
    archivo << "rankdir=TB;\n";
    archivo << "node [shape=box];\n";
    generarDotMatriz(archivo);
    archivo << "}\n";
    archivo.close();

    system(("dot -Tpng reportes/" + nombreArchivo +
            ".dot -o reportes/" + nombreArchivo + ".png").c_str());
}

/* ================== GRAPHVIZ ================== */

void MatrizDispersa::generarDotMatriz(ofstream& archivo) {

    // ===== ROOT =====
    archivo << "ROOT [label=\"\", shape=box, style=filled, fillcolor=gray, group=0];\n";

    /* ================== COLUMNAS (AVIONES) ================== */
    archivo << "{ rank=same; ROOT; ";

    int groupCol = 1;
    NodoEncabezado* col = columnasEncabezado;
    while (col) {
        archivo << "\"" << col->id << "\" ";
        col = col->siguiente;
    }
    archivo << "}\n";

    // Definir columnas con group
    groupCol = 1;
    col = columnasEncabezado;
    while (col) {
        archivo << "\"" << col->id
                << "\" [shape=box, style=filled, fillcolor=lightblue, group="
                << groupCol << "];\n";
        col = col->siguiente;
        groupCol++;
    }

    // Enlaces horizontales de columnas
    col = columnasEncabezado;
    while (col && col->siguiente) {
        archivo << "\"" << col->id << "\" -> \""
                << col->siguiente->id << "\";\n";
        col = col->siguiente;
    }

    if (columnasEncabezado)
        archivo << "ROOT -> \"" << columnasEncabezado->id << "\";\n";

    /* ================== FILAS (PILOTOS) ================== */
    NodoEncabezado* fila = filasEncabezado;

    // Enlace vertical ROOT -> primera fila
    if (fila)
        archivo << "ROOT -> \"" << fila->id << "\";\n";

    while (fila) {
        archivo << "\"" << fila->id
                << "\" [shape=box, style=filled, fillcolor=lightyellow, group=0];\n";

        if (fila->siguiente)
            archivo << "\"" << fila->id << "\" -> \""
                    << fila->siguiente->id << "\";\n";

        /* ===== CELDAS ===== */
        NodoMatriz* nodo = fila->acceso;
        while (nodo) {
            // Obtener group correcto de la columna
            int g = 1;
            NodoEncabezado* tmp = columnasEncabezado;
            while (tmp && tmp != nodo->columna) {
                g++;
                tmp = tmp->siguiente;
            }

            string idCelda = fila->id + "_" + nodo->columna->id;

            archivo << idCelda
                    << " [label=\"" << nodo->valor
                    << "\", shape=ellipse, group=" << g << "];\n";

            archivo << "\"" << fila->id << "\" -> " << idCelda << ";\n";
            archivo << "\"" << nodo->columna->id << "\" -> " << idCelda << ";\n";

            nodo = nodo->derecha;
        }

        fila = fila->siguiente;
    }
}




