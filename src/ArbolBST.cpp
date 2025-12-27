#include "../include/ArbolBST.h"
#include <iostream>
#include <algorithm>

NodoBST::NodoBST(Piloto* p) {
    piloto = p;
    izquierdo = nullptr;
    derecho = nullptr;
    altura = 1;
}

ArbolBST::ArbolBST() {
    raiz = nullptr;
}

int ArbolBST::obtenerAltura(NodoBST* nodo) {
    if (nodo == nullptr) return 0;
    return nodo->altura;
}

int ArbolBST::obtenerBalance(NodoBST* nodo) {
    if (nodo == nullptr) return 0;
    return obtenerAltura(nodo->izquierdo) - obtenerAltura(nodo->derecho);
}

NodoBST* ArbolBST::rotarDerecha(NodoBST* y) {
    NodoBST* x = y->izquierdo;
    NodoBST* T2 = x->derecho;

    x->derecho = y;
    y->izquierdo = T2;

    y->altura = max(obtenerAltura(y->izquierdo), obtenerAltura(y->derecho)) + 1;
    x->altura = max(obtenerAltura(x->izquierdo), obtenerAltura(x->derecho)) + 1;

    return x;
}

NodoBST* ArbolBST::rotarIzquierda(NodoBST* x) {
    NodoBST* y = x->derecho;
    NodoBST* T2 = y->izquierdo;

    y->izquierdo = x;
    x->derecho = T2;

    x->altura = max(obtenerAltura(x->izquierdo), obtenerAltura(x->derecho)) + 1;
    y->altura = max(obtenerAltura(y->izquierdo), obtenerAltura(y->derecho)) + 1;

    return y;
}

void ArbolBST::insertar(Piloto* piloto) {
    raiz = insertarRecursivo(raiz, piloto);
}

NodoBST* ArbolBST::insertarRecursivo(NodoBST* nodo, Piloto* piloto) {
    if (nodo == nullptr) {
        return new NodoBST(piloto);
    }

    if (piloto->horas_de_vuelo < nodo->piloto->horas_de_vuelo) {
        nodo->izquierdo = insertarRecursivo(nodo->izquierdo, piloto);
    } else if (piloto->horas_de_vuelo > nodo->piloto->horas_de_vuelo) {
        nodo->derecho = insertarRecursivo(nodo->derecho, piloto);
    } else {
        // Si tienen las mismas horas, comparar por ID
        if (piloto->numero_de_id < nodo->piloto->numero_de_id) {
            nodo->izquierdo = insertarRecursivo(nodo->izquierdo, piloto);
        } else {
            nodo->derecho = insertarRecursivo(nodo->derecho, piloto);
        }
    }

    nodo->altura = 1 + max(obtenerAltura(nodo->izquierdo), obtenerAltura(nodo->derecho));

    int balance = obtenerBalance(nodo);

    // Caso Left-Left
    if (balance > 1 && piloto->horas_de_vuelo < nodo->izquierdo->piloto->horas_de_vuelo) {
        return rotarDerecha(nodo);
    }

    // Caso Right-Right
    if (balance < -1 && piloto->horas_de_vuelo > nodo->derecho->piloto->horas_de_vuelo) {
        return rotarIzquierda(nodo);
    }

    // Caso Left-Right
    if (balance > 1 && piloto->horas_de_vuelo > nodo->izquierdo->piloto->horas_de_vuelo) {
        nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
        return rotarDerecha(nodo);
    }

    // Caso Right-Left
    if (balance < -1 && piloto->horas_de_vuelo < nodo->derecho->piloto->horas_de_vuelo) {
        nodo->derecho = rotarDerecha(nodo->derecho);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

void ArbolBST::eliminar(string id) {
    raiz = eliminarRecursivo(raiz, id);
}

NodoBST* ArbolBST::eliminarRecursivo(NodoBST* nodo, string id) {
    if (nodo == nullptr) return nodo;

    if (id < nodo->piloto->numero_de_id) {
        nodo->izquierdo = eliminarRecursivo(nodo->izquierdo, id);
    } else if (id > nodo->piloto->numero_de_id) {
        nodo->derecho = eliminarRecursivo(nodo->derecho, id);
    } else {
        if (nodo->izquierdo == nullptr || nodo->derecho == nullptr) {
            NodoBST* temp = nodo->izquierdo ? nodo->izquierdo : nodo->derecho;

            if (temp == nullptr) {
                temp = nodo;
                nodo = nullptr;
            } else {
                *nodo = *temp;
            }
            delete temp;
        } else {
            NodoBST* temp = encontrarMinimo(nodo->derecho);
            nodo->piloto = temp->piloto;
            nodo->derecho = eliminarRecursivo(nodo->derecho, temp->piloto->numero_de_id);
        }
    }

    if (nodo == nullptr) return nodo;

    nodo->altura = 1 + max(obtenerAltura(nodo->izquierdo), obtenerAltura(nodo->derecho));

    int balance = obtenerBalance(nodo);

    // Caso Left-Left
    if (balance > 1 && obtenerBalance(nodo->izquierdo) >= 0) {
        return rotarDerecha(nodo);
    }

    // Caso Left-Right
    if (balance > 1 && obtenerBalance(nodo->izquierdo) < 0) {
        nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
        return rotarDerecha(nodo);
    }

    // Caso Right-Right
    if (balance < -1 && obtenerBalance(nodo->derecho) <= 0) {
        return rotarIzquierda(nodo);
    }

    // Caso Right-Left
    if (balance < -1 && obtenerBalance(nodo->derecho) > 0) {
        nodo->derecho = rotarDerecha(nodo->derecho);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

NodoBST* ArbolBST::encontrarMinimo(NodoBST* nodo) {
    NodoBST* actual = nodo;
    while (actual->izquierdo != nullptr) {
        actual = actual->izquierdo;
    }
    return actual;
}

Piloto* ArbolBST::buscar(string id) {
    return buscarRecursivo(raiz, id);
}

Piloto* ArbolBST::buscarRecursivo(NodoBST* nodo, string id) {
    if (nodo == nullptr) return nullptr;
    
    if (id == nodo->piloto->numero_de_id) {
        return nodo->piloto;
    }
    
    if (id < nodo->piloto->numero_de_id) {
        return buscarRecursivo(nodo->izquierdo, id);
    }
    
    return buscarRecursivo(nodo->derecho, id);
}

void ArbolBST::preorden() {
    cout << "\n=== Recorrido Preorden ===" << endl;
    preordenRecursivo(raiz);
    cout << endl;
}

void ArbolBST::preordenRecursivo(NodoBST* nodo) {
    if (nodo != nullptr) {
        nodo->piloto->mostrar();
        preordenRecursivo(nodo->izquierdo);
        preordenRecursivo(nodo->derecho);
    }
}

void ArbolBST::inorden() {
    cout << "\n=== Recorrido Inorden ===" << endl;
    inordenRecursivo(raiz);
    cout << endl;
}

void ArbolBST::inordenRecursivo(NodoBST* nodo) {
    if (nodo != nullptr) {
        inordenRecursivo(nodo->izquierdo);
        nodo->piloto->mostrar();
        inordenRecursivo(nodo->derecho);
    }
}

void ArbolBST::postorden() {
    cout << "\n=== Recorrido Postorden ===" << endl;
    postordenRecursivo(raiz);
    cout << endl;
}

void ArbolBST::postordenRecursivo(NodoBST* nodo) {
    if (nodo != nullptr) {
        postordenRecursivo(nodo->izquierdo);
        postordenRecursivo(nodo->derecho);
        nodo->piloto->mostrar();
    }
}

void ArbolBST::generarReporte(string nombreArchivo) {
    ofstream archivo("reportes/" + nombreArchivo + ".dot");
    archivo << "digraph ArbolBST {" << endl;
    archivo << "    node [shape=circle];" << endl;
    
    if (raiz != nullptr) {
        generarDotRecursivo(raiz, archivo);
    }
    
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

void ArbolBST::generarDotRecursivo(NodoBST* nodo, ofstream& archivo) {
    if (nodo != nullptr) {
        archivo << "    \"" << nodo->piloto->numero_de_id << "\\n" 
                << nodo->piloto->horas_de_vuelo << " hrs\";" << endl;
        
        if (nodo->izquierdo != nullptr) {
            archivo << "    \"" << nodo->piloto->numero_de_id << "\\n" 
                    << nodo->piloto->horas_de_vuelo << " hrs\" -> \"" 
                    << nodo->izquierdo->piloto->numero_de_id << "\\n" 
                    << nodo->izquierdo->piloto->horas_de_vuelo << " hrs\";" << endl;
            generarDotRecursivo(nodo->izquierdo, archivo);
        }
        
        if (nodo->derecho != nullptr) {
            archivo << "    \"" << nodo->piloto->numero_de_id << "\\n" 
                    << nodo->piloto->horas_de_vuelo << " hrs\" -> \"" 
                    << nodo->derecho->piloto->numero_de_id << "\\n" 
                    << nodo->derecho->piloto->horas_de_vuelo << " hrs\";" << endl;
            generarDotRecursivo(nodo->derecho, archivo);
        }
    }
}

bool ArbolBST::estaVacio() {
    return raiz == nullptr;
}