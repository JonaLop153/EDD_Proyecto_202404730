#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../include/json.hpp"
#include "../include/Avion.h"
#include "../include/Piloto.h"
#include "../include/ArbolB.h"
#include "../include/ArbolBST.h"
#include "../include/TablaHash.h"
#include "../include/Grafo.h"
#include "../include/MatrizDispersa.h"
#include "../include/ListaCircular.h"

using json = nlohmann::json;
using namespace std;

/* ================== ESTRUCTURAS GLOBALES ================== */
ArbolB* arbolAvionesDisponibles = new ArbolB(5);
ListaCircular* listaMantenimiento = new ListaCircular();
ArbolBST* arbolPilotos = new ArbolBST();
TablaHash* tablaPilotos = new TablaHash(19);
Grafo* grafoRutas = new Grafo();
MatrizDispersa* matrizVuelos = new MatrizDispersa();

/* ================== UTILIDADES ================== */
void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausa() {
    cout << "\nPresione ENTER para continuar...";
    cin.ignore();
    cin.get();
}

/* ================== CARGA DE AVIONES ================== */
void cargarAviones() {
    string ruta;
    cout << "Ingrese la ruta del archivo JSON de aviones: ";
    cin >> ruta;

    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "Error al abrir archivo." << endl;
        pausa();
        return;
    }

    json datos;
    archivo >> datos;
    archivo.close();

    int contador = 0;
    for (auto& item : datos) {
        Avion* avion = new Avion(
            item.value("vuelo", ""),
            item.value("numero_de_registro", ""),
            item.value("modelo", ""),
            item.value("capacidad", 0),
            item.value("aerolinea", ""),
            item.value("ciudad_destino", ""),
            item.value("estado", "")
        );

        if (avion->getEstado() == "Disponible")
            arbolAvionesDisponibles->insertar(avion->getNumeroRegistro(), avion);
        else
            listaMantenimiento->insertar(avion);

        contador++;
    }

    cout << "Se cargaron " << contador << " aviones exitosamente." << endl;
    pausa();
}

/* ================== CARGA DE PILOTOS ================== */
void cargarPilotos() {
    string ruta;
    cout << "Ingrese la ruta del archivo JSON de pilotos: ";
    cin >> ruta;

    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "Error al abrir archivo." << endl;
        pausa();
        return;
    }

    json datos;
    archivo >> datos;
    archivo.close();

    int contador = 0;
    for (auto& item : datos) {
        Piloto* p = new Piloto(
            item.value("nombre", ""),
            "Desconocida",
            item.value("id", ""),
            "No asignado",
            item.value("horas_vuelo", 0),
            item.value("licencia", "")
        );

        arbolPilotos->insertar(p);
        tablaPilotos->insertar(p);
        contador++;
    }

    cout << "Se cargaron " << contador << " pilotos exitosamente." << endl;
    pausa();
}

/* ================== CARGA DE RUTAS ================== */
void cargarRutas() {
    string ruta;
    cout << "Ingrese la ruta del archivo TXT de rutas: ";
    cin >> ruta;

    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "Error al abrir archivo." << endl;
        pausa();
        return;
    }

    string linea;
    int contador = 0;
    while (getline(archivo, linea)) {
        string origen, destino, dist;
        stringstream ss(linea);

        getline(ss, origen, '/');
        getline(ss, destino, '/');
        getline(ss, dist, ';');

        grafoRutas->agregarRuta(origen, destino, stoi(dist));
        contador++;
    }

    archivo.close();
    cout << "Se cargaron " << contador << " rutas exitosamente." << endl;
    pausa();
}

/* ================== CARGA DE ASIGNACIONES (TXT) ================== */
void cargarAsignaciones() {
    string ruta;
    cout << "Ingrese la ruta del archivo de asignaciones (TXT): ";
    cin >> ruta;

    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "Error al abrir archivo." << endl;
        pausa();
        return;
    }

    string linea;
    int contador = 0;

    while (getline(archivo, linea)) {
        string id, vuelo, ciudad;
        stringstream ss(linea);

        getline(ss, id, ',');
        getline(ss, vuelo, ',');
        getline(ss, ciudad);

        matrizVuelos->insertar(id, ciudad, vuelo);
        contador++;
    }

    archivo.close();
    cout << "Se cargaron " << contador << " asignaciones exitosamente." << endl;
    pausa();
}

/* ================== PROCESAR MOVIMIENTOS ================== */
void procesarMovimientos() {
    string ruta;
    cout << "Ingrese la ruta del archivo de movimientos: ";
    cin >> ruta;

    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "Error al abrir archivo." << endl;
        pausa();
        return;
    }

    string linea;
    int contador = 0;

    while (getline(archivo, linea)) {
        if (linea.find("MantenimientoAviones") == 0) {
            string _, estado, registro;
            stringstream ss(linea);
            ss >> _ >> estado >> registro;

            if (estado == "Disponible") {
                Avion* a = listaMantenimiento->buscar(registro);
                if (a) {
                    a->setEstado("Disponible");
                    listaMantenimiento->eliminar(registro);
                    arbolAvionesDisponibles->insertar(registro, a);
                }
            } else {
                Avion* a = arbolAvionesDisponibles->buscar(registro);
                if (a) {
                    a->setEstado("Mantenimiento");
                    arbolAvionesDisponibles->eliminar(registro);
                    listaMantenimiento->insertar(a);
                }
            }
            contador++;
        }
        else if (linea.find("DarDeBaja") == 0) {
            size_t i = linea.find("(");
            size_t f = linea.find(")");
            string id = linea.substr(i + 1, f - i - 1);

            arbolPilotos->eliminar(id);
            tablaPilotos->eliminar(id);
            matrizVuelos->eliminarPiloto(id);
            contador++;
        }
    }

    archivo.close();
    cout << "Se procesaron " << contador << " movimientos." << endl;
    pausa();
}

/* ================== MENÚS ================== */
void menuConsultas() {
    int op;
    do {
        limpiarPantalla();
        cout << "\n=== CONSULTAS ===" << endl;
        cout << "1. Aviones disponibles (reporte)" << endl;
        cout << "2. Aviones en mantenimiento (reporte)" << endl;
        cout << "3. Pilotos (tabla hash)" << endl;
        cout << "4. Grafo de rutas" << endl;
        cout << "5. Matriz de vuelos" << endl;
        cout << "6. Volver" << endl;
        cout << "Seleccione: ";
        cin >> op;

        switch (op) {
            case 1: arbolAvionesDisponibles->generarReporte("consulta_disponibles"); pausa(); break;
            case 2: listaMantenimiento->generarReporte("consulta_mantenimiento"); pausa(); break;
            case 3: tablaPilotos->mostrar(); pausa(); break;
            case 4: grafoRutas->mostrar(); pausa(); break;
            case 5: matrizVuelos->mostrar(); pausa(); break;
        }
    } while (op != 6);
}

void menuReportes() {
    arbolAvionesDisponibles->generarReporte("arbol_b");
    listaMantenimiento->generarReporte("lista_mantenimiento");
    arbolPilotos->generarReporte("bst_pilotos");
    tablaPilotos->generarReporte("tabla_hash");
    grafoRutas->generarReporte("grafo");
    matrizVuelos->generarReporte("matriz");
    pausa();
}

/* ================== MAIN ================== */
int main() {
#ifdef _WIN32
    system("mkdir reportes 2>nul");
#else
    system("mkdir -p reportes");
#endif

    int op;
    do {
        limpiarPantalla();
        cout << "\n===== SISTEMA DE GESTION DE AEROPUERTO =====" << endl;
        cout << "1. Cargar aviones" << endl;
        cout << "2. Cargar pilotos" << endl;
        cout << "3. Cargar rutas" << endl;
        cout << "4. Cargar asignaciones" << endl;
        cout << "5. Consultas" << endl;
        cout << "6. Reportes" << endl;
        cout << "7. Procesar movimientos" << endl;
        cout << "8. Salir" << endl;
        cout << "Seleccione: ";
        cin >> op;

        switch (op) {
            case 1: cargarAviones(); break;
            case 2: cargarPilotos(); break;
            case 3: cargarRutas(); break;
            case 4: cargarAsignaciones(); break;
            case 5: menuConsultas(); break;
            case 6: menuReportes(); break;
            case 7: procesarMovimientos(); break;
        }
    } while (op != 8);

    cout << "Gracias por usar el sistema." << endl;
    return 0;
}
