#include "../include/ArbolB.h"
#include <iostream>
#include <algorithm>

using namespace std;

NodoB::NodoB(bool hoja) {
    esHoja = hoja;
    n = 0;
}

ArbolB::ArbolB(int ord) {
    raiz = nullptr;
    orden = ord;
}

void ArbolB::insertar(string clave, Avion* avion) {
    if (raiz == nullptr) {
        raiz = new NodoB(true);
        raiz->claves.push_back(clave);
        raiz->aviones.push_back(avion);
        raiz->n = 1;
    } else {
        if (raiz->n == 2 * orden - 1) {
            NodoB* nuevoNodo = new NodoB(false);
            nuevoNodo->hijos.push_back(raiz);
            dividirHijo(nuevoNodo, 0, raiz);
            
            int i = 0;
            if (nuevoNodo->claves[0] < clave) {
                i++;
            }
            insertarNoLleno(nuevoNodo->hijos[i], clave, avion);
            raiz = nuevoNodo;
        } else {
            insertarNoLleno(raiz, clave, avion);
        }
    }
}

void ArbolB::dividirHijo(NodoB* padre, int i, NodoB* hijo) {
    int t = orden;
    NodoB* nuevoNodo = new NodoB(hijo->esHoja);
    nuevoNodo->n = t - 1;

    for (int j = 0; j < t - 1; j++) {
        nuevoNodo->claves.push_back(hijo->claves[j + t]);
        nuevoNodo->aviones.push_back(hijo->aviones[j + t]);
    }

    if (!hijo->esHoja) {
        for (int j = 0; j < t; j++) {
            nuevoNodo->hijos.push_back(hijo->hijos[j + t]);
        }
    }

    hijo->n = t - 1;

    padre->hijos.insert(padre->hijos.begin() + i + 1, nuevoNodo);
    padre->claves.insert(padre->claves.begin() + i, hijo->claves[t - 1]);
    padre->aviones.insert(padre->aviones.begin() + i, hijo->aviones[t - 1]);
    padre->n++;

    hijo->claves.resize(t - 1);
    hijo->aviones.resize(t - 1);
    if (!hijo->esHoja) {
        hijo->hijos.resize(t);
    }
}

void ArbolB::insertarNoLleno(NodoB* nodo, string clave, Avion* avion) {
    int i = nodo->n - 1;

    if (nodo->esHoja) {
        nodo->claves.push_back("");
        nodo->aviones.push_back(nullptr);
        
        while (i >= 0 && clave < nodo->claves[i]) {
            nodo->claves[i + 1] = nodo->claves[i];
            nodo->aviones[i + 1] = nodo->aviones[i];
            i--;
        }
        
        nodo->claves[i + 1] = clave;
        nodo->aviones[i + 1] = avion;
        nodo->n++;
    } else {
        while (i >= 0 && clave < nodo->claves[i]) {
            i--;
        }
        i++;
        
        if (nodo->hijos[i]->n == 2 * orden - 1) {
            dividirHijo(nodo, i, nodo->hijos[i]);
            if (clave > nodo->claves[i]) {
                i++;
            }
        }
        insertarNoLleno(nodo->hijos[i], clave, avion);
    }
}

Avion* ArbolB::buscar(string clave) {
    if (raiz == nullptr) return nullptr;
    return buscarRecursivo(raiz, clave);
}

Avion* ArbolB::buscarRecursivo(NodoB* nodo, string clave) {
    int i = 0;
    while (i < nodo->n && clave > nodo->claves[i]) {
        i++;
    }

    if (i < nodo->n && clave == nodo->claves[i]) {
        return nodo->aviones[i];
    }

    if (nodo->esHoja) {
        return nullptr;
    }

    return buscarRecursivo(nodo->hijos[i], clave);
}

void ArbolB::eliminar(string clave) {
    if (raiz == nullptr) return;
    
    eliminarRecursivo(raiz, clave);
    
    if (raiz->n == 0) {
        NodoB* temp = raiz;
        if (raiz->esHoja) {
            raiz = nullptr;
        } else {
            raiz = raiz->hijos[0];
        }
        delete temp;
    }
}

void ArbolB::eliminarRecursivo(NodoB* nodo, string clave) {
    int idx = 0;
    while (idx < nodo->n && nodo->claves[idx] < clave) {
        idx++;
    }

    if (idx < nodo->n && nodo->claves[idx] == clave) {
        if (nodo->esHoja) {
            nodo->claves.erase(nodo->claves.begin() + idx);
            nodo->aviones.erase(nodo->aviones.begin() + idx);
            nodo->n--;
        } else {
            if (nodo->hijos[idx]->n >= orden) {
                string predClave;
                Avion* predAvion;
                obtenerPredecesor(nodo, idx, predClave, predAvion);
                nodo->claves[idx] = predClave;
                nodo->aviones[idx] = predAvion;
                eliminarRecursivo(nodo->hijos[idx], predClave);
            } else if (nodo->hijos[idx + 1]->n >= orden) {
                string sucClave;
                Avion* sucAvion;
                obtenerSucesor(nodo, idx, sucClave, sucAvion);
                nodo->claves[idx] = sucClave;
                nodo->aviones[idx] = sucAvion;
                eliminarRecursivo(nodo->hijos[idx + 1], sucClave);
            } else {
                fusionar(nodo, idx);
                eliminarRecursivo(nodo->hijos[idx], clave);
            }
        }
    } else {
        if (nodo->esHoja) {
            return;
        }

        bool enUltimoHijo = (idx == nodo->n);

        if (nodo->hijos[idx]->n < orden) {
            llenar(nodo, idx);
        }

        if (enUltimoHijo && idx > nodo->n) {
            eliminarRecursivo(nodo->hijos[idx - 1], clave);
        } else {
            eliminarRecursivo(nodo->hijos[idx], clave);
        }
    }
}

void ArbolB::obtenerPredecesor(NodoB* nodo, int idx, string& clave, Avion*& avion) {
    NodoB* actual = nodo->hijos[idx];
    while (!actual->esHoja) {
        actual = actual->hijos[actual->n];
    }
    clave = actual->claves[actual->n - 1];
    avion = actual->aviones[actual->n - 1];
}

void ArbolB::obtenerSucesor(NodoB* nodo, int idx, string& clave, Avion*& avion) {
    NodoB* actual = nodo->hijos[idx + 1];
    while (!actual->esHoja) {
        actual = actual->hijos[0];
    }
    clave = actual->claves[0];
    avion = actual->aviones[0];
}

void ArbolB::llenar(NodoB* nodo, int idx) {
    if (idx != 0 && nodo->hijos[idx - 1]->n >= orden) {
        prestarDelAnterior(nodo, idx);
    } else if (idx != nodo->n && nodo->hijos[idx + 1]->n >= orden) {
        prestarDelSiguiente(nodo, idx);
    } else {
        if (idx != nodo->n) {
            fusionar(nodo, idx);
        } else {
            fusionar(nodo, idx - 1);
        }
    }
}

void ArbolB::prestarDelAnterior(NodoB* nodo, int idx) {
    NodoB* hijo = nodo->hijos[idx];
    NodoB* hermano = nodo->hijos[idx - 1];

    hijo->claves.insert(hijo->claves.begin(), nodo->claves[idx - 1]);
    hijo->aviones.insert(hijo->aviones.begin(), nodo->aviones[idx - 1]);

    if (!hijo->esHoja) {
        hijo->hijos.insert(hijo->hijos.begin(), hermano->hijos[hermano->n]);
    }

    nodo->claves[idx - 1] = hermano->claves[hermano->n - 1];
    nodo->aviones[idx - 1] = hermano->aviones[hermano->n - 1];

    hermano->claves.pop_back();
    hermano->aviones.pop_back();
    if (!hermano->esHoja) {
        hermano->hijos.pop_back();
    }

    hijo->n++;
    hermano->n--;
}

void ArbolB::prestarDelSiguiente(NodoB* nodo, int idx) {
    NodoB* hijo = nodo->hijos[idx];
    NodoB* hermano = nodo->hijos[idx + 1];

    hijo->claves.push_back(nodo->claves[idx]);
    hijo->aviones.push_back(nodo->aviones[idx]);

    if (!hijo->esHoja) {
        hijo->hijos.push_back(hermano->hijos[0]);
    }

    nodo->claves[idx] = hermano->claves[0];
    nodo->aviones[idx] = hermano->aviones[0];

    hermano->claves.erase(hermano->claves.begin());
    hermano->aviones.erase(hermano->aviones.begin());
    if (!hermano->esHoja) {
        hermano->hijos.erase(hermano->hijos.begin());
    }

    hijo->n++;
    hermano->n--;
}

void ArbolB::fusionar(NodoB* nodo, int idx) {
    NodoB* hijo = nodo->hijos[idx];
    NodoB* hermano = nodo->hijos[idx + 1];

    hijo->claves.push_back(nodo->claves[idx]);
    hijo->aviones.push_back(nodo->aviones[idx]);

    for (int i = 0; i < hermano->n; i++) {
        hijo->claves.push_back(hermano->claves[i]);
        hijo->aviones.push_back(hermano->aviones[i]);
    }

    if (!hijo->esHoja) {
        for (int i = 0; i <= hermano->n; i++) {
            hijo->hijos.push_back(hermano->hijos[i]);
        }
    }

    nodo->claves.erase(nodo->claves.begin() + idx);
    nodo->aviones.erase(nodo->aviones.begin() + idx);
    nodo->hijos.erase(nodo->hijos.begin() + idx + 1);

    hijo->n += hermano->n + 1;
    nodo->n--;

    delete hermano;
}

void ArbolB::generarReporte(string nombreArchivo) {
    ofstream archivo("reportes/" + nombreArchivo + ".dot");
    archivo << "digraph ArbolB {" << endl;
    archivo << "    node [shape=record];" << endl;
    
    if (raiz != nullptr) {
        int contador = 0;
        generarDotRecursivo(raiz, archivo, contador);
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

void ArbolB::generarDotRecursivo(NodoB* nodo, ofstream& archivo, int& contador) {
    int idActual = contador++;
    
    archivo << "    nodo" << idActual << " [label=\"";
    for (int i = 0; i < nodo->n; i++) {
        if (i > 0) archivo << "|";
        archivo << "<f" << i << "> " << nodo->claves[i];
    }
    archivo << "\"];" << endl;
    
    if (!nodo->esHoja) {
        for (int i = 0; i <= nodo->n; i++) {
            int idHijo = contador;
            generarDotRecursivo(nodo->hijos[i], archivo, contador);
            archivo << "    nodo" << idActual << ":f" << (i == 0 ? 0 : i-1) 
                   << " -> nodo" << idHijo << ";" << endl;
        }
    }
}

bool ArbolB::estaVacio() {
    return raiz == nullptr;
}