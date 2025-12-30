#include "../include/TablaHash.h"
#include <iostream>

NodoHash::NodoHash(Piloto* p) {
    piloto = p;
    siguiente = nullptr;
}

TablaHash::TablaHash(int tam) {
    tamanio = tam;
    tabla.resize(tamanio, nullptr);
}

int TablaHash::funcionHash(string id) {
    long long suma = 0;
    for (char c : id) {
        suma = (suma * 31 + (int)c) % 19;  // Mejor dispersión
    }
    return suma % 19;  // ✅ Siempre módulo 19
}

void TablaHash::insertar(Piloto* piloto) {
    int indice = funcionHash(piloto->numero_de_id);
    
    NodoHash* nuevo = new NodoHash(piloto);
    
    if (tabla[indice] == nullptr) {
        tabla[indice] = nuevo;
    } else {
        NodoHash* actual = tabla[indice];
        while (actual->siguiente != nullptr) {
            if (actual->piloto->numero_de_id == piloto->numero_de_id) {
                // Ya existe, actualizar
                actual->piloto = piloto;
                delete nuevo;
                return;
            }
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
}

Piloto* TablaHash::buscar(string id) {
    int indice = funcionHash(id);
    
    NodoHash* actual = tabla[indice];
    while (actual != nullptr) {
        if (actual->piloto->numero_de_id == id) {
            return actual->piloto;
        }
        actual = actual->siguiente;
    }
    
    return nullptr;
}

void TablaHash::eliminar(string id) {
    int indice = funcionHash(id);
    
    NodoHash* actual = tabla[indice];
    NodoHash* anterior = nullptr;
    
    while (actual != nullptr) {
        if (actual->piloto->numero_de_id == id) {
            if (anterior == nullptr) {
                tabla[indice] = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            delete actual;
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
}

void TablaHash::mostrar() {
    cout << "\n=== Tabla Hash de Pilotos ===" << endl;
    for (int i = 0; i < tamanio; i++) {
        cout << "Indice " << i << ": ";
        NodoHash* actual = tabla[i];
        if (actual == nullptr) {
            cout << "vacio";
        } else {
            while (actual != nullptr) {
                cout << "[" << actual->piloto->numero_de_id << "]";
                if (actual->siguiente != nullptr) cout << " -> ";
                actual = actual->siguiente;
            }
        }
        cout << endl;
    }
}

void TablaHash::generarReporte(string nombreArchivo) {
    ofstream archivo("reportes/" + nombreArchivo + ".dot");
    archivo << "digraph TablaHash {" << endl;
    archivo << "    rankdir=LR;" << endl;
    archivo << "    node [shape=record];" << endl;
    
    generarDotTabla(archivo);
    
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

void TablaHash::generarDotTabla(ofstream& archivo) {
    archivo << "    tabla [label=\"";
    for (int i = 0; i < tamanio; i++) {
        if (i > 0) archivo << "|";
        archivo << "<f" << i << "> " << i;
    }
    archivo << "\"];" << endl;
    
    for (int i = 0; i < tamanio; i++) {
        NodoHash* actual = tabla[i];
        if (actual != nullptr) {
            int contador = 0;
            while (actual != nullptr) {
                string nombreNodo = "nodo_" + to_string(i) + "_" + to_string(contador);
                archivo << "    " << nombreNodo << " [label=\"{" 
                       << actual->piloto->numero_de_id << "|" 
                       << actual->piloto->nombre << "|" 
                       << actual->piloto->horas_de_vuelo << " hrs}\"];" << endl;
                
                if (contador == 0) {
                    archivo << "    tabla:f" << i << " -> " << nombreNodo << ";" << endl;
                } else {
                    string nombreAnterior = "nodo_" + to_string(i) + "_" + to_string(contador - 1);
                    archivo << "    " << nombreAnterior << " -> " << nombreNodo << ";" << endl;
                }
                
                actual = actual->siguiente;
                contador++;
            }
        }
    }
}