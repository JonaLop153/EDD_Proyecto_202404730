#include "../include/MatrizDispersa.h"
#include <iostream>

NodoMatriz::NodoMatriz(string val, int f, int c) {
    valor = val;
    fila = f;
    columna = c;
    arriba = nullptr;
    abajo = nullptr;
    izquierda = nullptr;
    derecha = nullptr;
}

NodoEncabezado::NodoEncabezado(string identificador, int pos) {
    id = identificador;
    posicion = pos;
    siguiente = nullptr;
    anterior = nullptr;
    acceso = nullptr;
}

MatrizDispersa::MatrizDispersa() {
    filasEncabezado = nullptr;
    columnasEncabezado = nullptr;
}

NodoEncabezado* MatrizDispersa::buscarCrearEncabezadoFila(string idPiloto, int& pos) {
    if (filasEncabezado == nullptr) {
        pos = 0;
        filasEncabezado = new NodoEncabezado(idPiloto, pos);
        return filasEncabezado;
    }
    
    NodoEncabezado* actual = filasEncabezado;
    NodoEncabezado* anterior = nullptr;
    pos = 0;
    
    while (actual != nullptr) {
        if (actual->id == idPiloto) {
            return actual;
        }
        if (actual->id > idPiloto) {
            break;
        }
        anterior = actual;
        actual = actual->siguiente;
        pos++;
    }
    
    NodoEncabezado* nuevo = new NodoEncabezado(idPiloto, pos);
    
    if (anterior == nullptr) {
        nuevo->siguiente = filasEncabezado;
        if (filasEncabezado) filasEncabezado->anterior = nuevo;
        filasEncabezado = nuevo;
        
        // Actualizar posiciones
        actual = nuevo->siguiente;
        int p = 1;
        while (actual != nullptr) {
            actual->posicion = p++;
            actual = actual->siguiente;
        }
    } else {
        nuevo->siguiente = anterior->siguiente;
        nuevo->anterior = anterior;
        if (anterior->siguiente) anterior->siguiente->anterior = nuevo;
        anterior->siguiente = nuevo;
        
        // Actualizar posiciones
        actual = nuevo->siguiente;
        int p = pos + 1;
        while (actual != nullptr) {
            actual->posicion = p++;
            actual = actual->siguiente;
        }
    }
    
    return nuevo;
}

NodoEncabezado* MatrizDispersa::buscarCrearEncabezadoColumna(string ciudad, int& pos) {
    if (columnasEncabezado == nullptr) {
        pos = 0;
        columnasEncabezado = new NodoEncabezado(ciudad, pos);
        return columnasEncabezado;
    }
    
    NodoEncabezado* actual = columnasEncabezado;
    NodoEncabezado* anterior = nullptr;
    pos = 0;
    
    while (actual != nullptr) {
        if (actual->id == ciudad) {
            return actual;
        }
        if (actual->id > ciudad) {
            break;
        }
        anterior = actual;
        actual = actual->siguiente;
        pos++;
    }
    
    NodoEncabezado* nuevo = new NodoEncabezado(ciudad, pos);
    
    if (anterior == nullptr) {
        nuevo->siguiente = columnasEncabezado;
        if (columnasEncabezado) columnasEncabezado->anterior = nuevo;
        columnasEncabezado = nuevo;
        
        // Actualizar posiciones
        actual = nuevo->siguiente;
        int p = 1;
        while (actual != nullptr) {
            actual->posicion = p++;
            actual = actual->siguiente;
        }
    } else {
        nuevo->siguiente = anterior->siguiente;
        nuevo->anterior = anterior;
        if (anterior->siguiente) anterior->siguiente->anterior = nuevo;
        anterior->siguiente = nuevo;
        
        // Actualizar posiciones
        actual = nuevo->siguiente;
        int p = pos + 1;
        while (actual != nullptr) {
            actual->posicion = p++;
            actual = actual->siguiente;
        }
    }
    
    return nuevo;
}

void MatrizDispersa::insertar(string idPiloto, string ciudad, string vuelo) {
    int posFila, posColumna;
    NodoEncabezado* encabezadoFila = buscarCrearEncabezadoFila(idPiloto, posFila);
    NodoEncabezado* encabezadoColumna = buscarCrearEncabezadoColumna(ciudad, posColumna);
    
    // Buscar si ya existe el nodo
    NodoMatriz* actual = encabezadoFila->acceso;
    while (actual != nullptr) {
        if (actual->columna == posColumna) {
            actual->valor = vuelo; // Actualizar
            return;
        }
        actual = actual->derecha;
    }
    
    NodoMatriz* nuevo = new NodoMatriz(vuelo, posFila, posColumna);
    
    insertarNodoEnFila(nuevo, encabezadoFila);
    insertarNodoEnColumna(nuevo, encabezadoColumna);
}

void MatrizDispersa::insertarNodoEnFila(NodoMatriz* nuevo, NodoEncabezado* encabezadoFila) {
    if (encabezadoFila->acceso == nullptr) {
        encabezadoFila->acceso = nuevo;
        return;
    }
    
    if (nuevo->columna < encabezadoFila->acceso->columna) {
        nuevo->derecha = encabezadoFila->acceso;
        encabezadoFila->acceso->izquierda = nuevo;
        encabezadoFila->acceso = nuevo;
        return;
    }
    
    NodoMatriz* actual = encabezadoFila->acceso;
    while (actual->derecha != nullptr && actual->derecha->columna < nuevo->columna) {
        actual = actual->derecha;
    }
    
    nuevo->derecha = actual->derecha;
    if (actual->derecha != nullptr) {
        actual->derecha->izquierda = nuevo;
    }
    actual->derecha = nuevo;
    nuevo->izquierda = actual;
}

void MatrizDispersa::insertarNodoEnColumna(NodoMatriz* nuevo, NodoEncabezado* encabezadoColumna) {
    if (encabezadoColumna->acceso == nullptr) {
        encabezadoColumna->acceso = nuevo;
        return;
    }
    
    if (nuevo->fila < encabezadoColumna->acceso->fila) {
        nuevo->abajo = encabezadoColumna->acceso;
        encabezadoColumna->acceso->arriba = nuevo;
        encabezadoColumna->acceso = nuevo;
        return;
    }
    
    NodoMatriz* actual = encabezadoColumna->acceso;
    while (actual->abajo != nullptr && actual->abajo->fila < nuevo->fila) {
        actual = actual->abajo;
    }
    
    nuevo->abajo = actual->abajo;
    if (actual->abajo != nullptr) {
        actual->abajo->arriba = nuevo;
    }
    actual->abajo = nuevo;
    nuevo->arriba = actual;
}

void MatrizDispersa::eliminarPiloto(string idPiloto) {
    NodoEncabezado* encabezadoFila = filasEncabezado;
    
    while (encabezadoFila != nullptr && encabezadoFila->id != idPiloto) {
        encabezadoFila = encabezadoFila->siguiente;
    }
    
    if (encabezadoFila == nullptr) return;
    
    // Eliminar todos los nodos de esta fila
    NodoMatriz* nodo = encabezadoFila->acceso;
    while (nodo != nullptr) {
        NodoMatriz* siguiente = nodo->derecha;
        
        // Desconectar de la columna
        if (nodo->arriba != nullptr) {
            nodo->arriba->abajo = nodo->abajo;
        } else {
            // Es el primer nodo de la columna
            NodoEncabezado* encCol = columnasEncabezado;
            while (encCol != nullptr && encCol->posicion != nodo->columna) {
                encCol = encCol->siguiente;
            }
            if (encCol != nullptr) {
                encCol->acceso = nodo->abajo;
            }
        }
        
        if (nodo->abajo != nullptr) {
            nodo->abajo->arriba = nodo->arriba;
        }
        
        delete nodo;
        nodo = siguiente;
    }
    
    // Eliminar encabezado de fila
    if (encabezadoFila->anterior != nullptr) {
        encabezadoFila->anterior->siguiente = encabezadoFila->siguiente;
    } else {
        filasEncabezado = encabezadoFila->siguiente;
    }
    
    if (encabezadoFila->siguiente != nullptr) {
        encabezadoFila->siguiente->anterior = encabezadoFila->anterior;
    }
    
    delete encabezadoFila;
    
    // Actualizar posiciones de filas
    NodoEncabezado* actual = filasEncabezado;
    int pos = 0;
    while (actual != nullptr) {
        actual->posicion = pos++;
        actual = actual->siguiente;
    }
    
    // Limpiar columnas sin conexiones
    NodoEncabezado* col = columnasEncabezado;
    while (col != nullptr) {
        NodoEncabezado* siguiente = col->siguiente;
        if (col->acceso == nullptr) {
            limpiarColumnaSinConexiones(col);
        }
        col = siguiente;
    }
}

void MatrizDispersa::limpiarColumnaSinConexiones(NodoEncabezado* encabezado) {
    if (encabezado->anterior != nullptr) {
        encabezado->anterior->siguiente = encabezado->siguiente;
    } else {
        columnasEncabezado = encabezado->siguiente;
    }
    
    if (encabezado->siguiente != nullptr) {
        encabezado->siguiente->anterior = encabezado->anterior;
    }
    
    delete encabezado;
    
    // Actualizar posiciones
    NodoEncabezado* actual = columnasEncabezado;
    int pos = 0;
    while (actual != nullptr) {
        actual->posicion = pos++;
        actual = actual->siguiente;
    }
}

void MatrizDispersa::mostrar() {
    cout << "\n=== Matriz Dispersa (Pilotos x Ciudades) ===" << endl;
    
    if (filasEncabezado == nullptr || columnasEncabezado == nullptr) {
        cout << "Matriz vacia" << endl;
        return;
    }
    
    NodoEncabezado* fila = filasEncabezado;
    while (fila != nullptr) {
        cout << fila->id << ": ";
        NodoMatriz* nodo = fila->acceso;
        while (nodo != nullptr) {
            NodoEncabezado* col = columnasEncabezado;
            while (col != nullptr && col->posicion != nodo->columna) {
                col = col->siguiente;
            }
            if (col != nullptr) {
                cout << "[" << col->id << ": " << nodo->valor << "] ";
            }
            nodo = nodo->derecha;
        }
        cout << endl;
        fila = fila->siguiente;
    }
}

void MatrizDispersa::generarReporte(string nombreArchivo) {
    ofstream archivo("reportes/" + nombreArchivo + ".dot");
    archivo << "digraph MatrizDispersa {" << endl;
    archivo << "    rankdir=TB;" << endl;
    archivo << "    node [shape=box];" << endl;
    
    generarDotMatriz(archivo);
    
    archivo << "}" << endl;
    archivo.close();
    
    string comando = "dot -Tpng reportes/" + nombreArchivo + ".dot -o reportes/" + nombreArchivo + ".png";
    system(comando.c_str());
    
#ifdef _WIN32
    system(("start reportes/" + nombreArchivo + ".png").c_str());
#elif __APPLE__
    system(("open reportes/" + nombreArchivo + ".png").c_str());
#else
    system(("xdg-open reportes/" + nombreArchivo + ".png").c_str());
#endif
}

void MatrizDispersa::generarDotMatriz(ofstream& archivo) {
    archivo << "    raiz [label=\"M\", group=0];" << endl;
    
    // Encabezados de columnas
    NodoEncabezado* col = columnasEncabezado;
    while (col != nullptr) {
        archivo << "    col_" << col->posicion << " [label=\"" << col->id 
               << "\", group=" << (col->posicion + 1) << "];" << endl;
        col = col->siguiente;
    }
    
    // Encabezados de filas
    NodoEncabezado* fila = filasEncabezado;
    while (fila != nullptr) {
        archivo << "    fila_" << fila->posicion << " [label=\"" << fila->id 
               << "\", group=0];" << endl;
        fila = fila->siguiente;
    }
    
    // Conectar raiz con columnas
    col = columnasEncabezado;
    if (col != nullptr) {
        archivo << "    raiz -> col_" << col->posicion << ";" << endl;
        while (col->siguiente != nullptr) {
            archivo << "    col_" << col->posicion << " -> col_" 
                   << col->siguiente->posicion << ";" << endl;
            col = col->siguiente;
        }
    }
    
    // Conectar raiz con filas
    fila = filasEncabezado;
    if (fila != nullptr) {
        archivo << "    raiz -> fila_" << fila->posicion << " [constraint=false];" << endl;
        while (fila->siguiente != nullptr) {
            archivo << "    fila_" << fila->posicion << " -> fila_" 
                   << fila->siguiente->posicion << " [constraint=false];" << endl;
            fila = fila->siguiente;
        }
    }
    
    // Nodos internos
    fila = filasEncabezado;
    while (fila != nullptr) {
        NodoMatriz* nodo = fila->acceso;
        while (nodo != nullptr) {
            string nombreNodo = "nodo_" + to_string(nodo->fila) + "_" + to_string(nodo->columna);
            archivo << "    " << nombreNodo << " [label=\"" << nodo->valor 
                   << "\", group=" << (nodo->columna + 1) << "];" << endl;
            
            // Conectar desde fila
            if (nodo->izquierda == nullptr) {
                archivo << "    fila_" << nodo->fila << " -> " << nombreNodo 
                       << " [constraint=false];" << endl;
            } else {
                string nombreIzq = "nodo_" + to_string(nodo->izquierda->fila) + "_" 
                                 + to_string(nodo->izquierda->columna);
                archivo << "    " << nombreIzq << " -> " << nombreNodo 
                       << " [constraint=false];" << endl;
            }
            
            // Conectar desde columna
            if (nodo->arriba == nullptr) {
                archivo << "    col_" << nodo->columna << " -> " << nombreNodo << ";" << endl;
            } else {
                string nombreArriba = "nodo_" + to_string(nodo->arriba->fila) + "_" 
                                    + to_string(nodo->arriba->columna);
                archivo << "    " << nombreArriba << " -> " << nombreNodo << ";" << endl;
            }
            
            nodo = nodo->derecha;
        }
        fila = fila->siguiente;
    }
}