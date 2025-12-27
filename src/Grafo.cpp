#include "../include/Grafo.h"
#include <iostream>
#include <queue>
#include <set>

Arista::Arista(string dest, int dist) {
    destino = dest;
    distancia = dist;
}

void Grafo::agregarRuta(string origen, string destino, int distancia) {
    listaAdyacencia[origen].push_back(Arista(destino, distancia));
    
    // Asegurar que el destino existe en el mapa aunque no tenga aristas salientes
    if (listaAdyacencia.find(destino) == listaAdyacencia.end()) {
        listaAdyacencia[destino] = vector<Arista>();
    }
}

void Grafo::mostrar() {
    cout << "\n=== Grafo de Rutas ===" << endl;
    for (auto& par : listaAdyacencia) {
        cout << par.first << " -> ";
        for (size_t i = 0; i < par.second.size(); i++) {
            cout << par.second[i].destino << "(" << par.second[i].distancia << "km)";
            if (i < par.second.size() - 1) cout << ", ";
        }
        if (par.second.empty()) {
            cout << "Sin rutas salientes";
        }
        cout << endl;
    }
}

vector<string> Grafo::rutaMasCorta(string origen, string destino, int& distanciaTotal) {
    map<string, int> distancias;
    map<string, string> predecesores;
    set<string> visitados;
    
    // Inicializar distancias
    for (auto& par : listaAdyacencia) {
        distancias[par.first] = numeric_limits<int>::max();
    }
    distancias[origen] = 0;
    
    // Cola de prioridad: pair<distancia, nodo>
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
    pq.push(make_pair(0, origen));
    
    while (!pq.empty()) {
        string actual = pq.top().second;
        pq.pop();
        
        if (visitados.find(actual) != visitados.end()) {
            continue;
        }
        visitados.insert(actual);
        
        if (actual == destino) {
            break;
        }
        
        // Revisar vecinos
        for (auto& arista : listaAdyacencia[actual]) {
            string vecino = arista.destino;
            int pesoArista = arista.distancia;
            int nuevaDistancia = distancias[actual] + pesoArista;
            
            if (nuevaDistancia < distancias[vecino]) {
                distancias[vecino] = nuevaDistancia;
                predecesores[vecino] = actual;
                pq.push(make_pair(nuevaDistancia, vecino));
            }
        }
    }
    
    // Reconstruir ruta
    vector<string> ruta;
    if (distancias[destino] == numeric_limits<int>::max()) {
        distanciaTotal = -1;
        return ruta; // No hay ruta
    }
    
    string actual = destino;
    while (actual != origen) {
        ruta.insert(ruta.begin(), actual);
        actual = predecesores[actual];
    }
    ruta.insert(ruta.begin(), origen);
    
    distanciaTotal = distancias[destino];
    return ruta;
}

void Grafo::generarReporte(string nombreArchivo) {
    ofstream archivo("reportes/" + nombreArchivo + ".dot");
    archivo << "digraph Grafo {" << endl;
    archivo << "    rankdir=LR;" << endl;
    archivo << "    node [shape=circle];" << endl;
    
    generarDotGrafo(archivo);
    
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

void Grafo::generarDotGrafo(ofstream& archivo) {
    for (auto& par : listaAdyacencia) {
        archivo << "    \"" << par.first << "\";" << endl;
    }
    
    for (auto& par : listaAdyacencia) {
        for (auto& arista : par.second) {
            archivo << "    \"" << par.first << "\" -> \"" << arista.destino 
                   << "\" [label=\"" << arista.distancia << " km\"];" << endl;
        }
    }
}