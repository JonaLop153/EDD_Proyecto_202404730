#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

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
void construirMatriz();

/* ===================== ESTRUCTURAS GLOBALES ===================== */

ArbolB* arbolAvionesDisponibles = new ArbolB(5);
ListaCircular* listaMantenimiento = new ListaCircular();
ArbolBST* arbolPilotos = new ArbolBST();
TablaHash* tablaPilotos = new TablaHash(19);
Grafo* grafoRutas = new Grafo();
MatrizDispersa* matrizVuelos = new MatrizDispersa();

/* ===================== UTILIDADES ===================== */

void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausa() {
    cout << "\nPresione ENTER para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

/* ===================== CARGAS ===================== */

void cargarAviones() {
    string ruta;
    cout << "Ingrese la ruta del JSON de aviones: ";
    cin >> ruta;

    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo." << endl;
        pausa();
        return;
    }

    json datos;
    archivo >> datos;
    archivo.close();

    int cont = 0;
    for (auto& a : datos) {
        Avion* avion = new Avion(
            a["vuelo"],
            a["numero_de_registro"],
            a["modelo"],
            a["capacidad"],
            a["aerolinea"],
            a["ciudad_destino"],
            a["estado"]
        );

        if (avion->getEstado() == "Disponible")
            arbolAvionesDisponibles->insertar(avion->getNumeroRegistro(), avion);
        else
            listaMantenimiento->insertar(avion);

        cont++;
    }

    cout << "Se cargaron " << cont << " aviones." << endl;
    pausa();
}

void cargarPilotos() {
    string rutaArchivo;
    cout << "Ingrese la ruta del JSON de pilotos: ";
    cin >> rutaArchivo;

    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo." << endl;
        pausa();
        return;
    }

    json datos;
    try {
        archivo >> datos;
    } catch (json::exception& e) {
        cout << "Error al parsear JSON: " << e.what() << endl;
        pausa();
        return;
    }

    archivo.close();

    if (!datos.is_array()) {
        cout << "Error: El JSON de pilotos debe ser un arreglo." << endl;
        pausa();
        return;
    }

    int contador = 0;

    for (auto& item : datos) {

        // ===== LECTURA SEGURA =====
        string nombre       = item.value("nombre", "Desconocido");
        string nacionalidad = item.value("nacionalidad", "Desconocida");
        string numero_id    = item.value("numero_de_id", "");
        string vuelo        = item.value("vuelo", "No asignado");
        int horas_vuelo     = item.value("horas_de_vuelo", 0);
        string licencia     = item.value("tipo_de_licencia", "N/A");

        // ===== VALIDACIÓN MÍNIMA =====
        if (numero_id.empty()) {
            cout << "Piloto ignorado: numero_de_id vacío." << endl;
            continue;
        }

        // ===== CREAR PILOTO =====
        Piloto* piloto = new Piloto(
            nombre,
            nacionalidad,
            numero_id,
            vuelo,
            horas_vuelo,
            licencia
        );

        // ===== INSERTAR EN ESTRUCTURAS =====
        arbolPilotos->insertar(piloto);
        tablaPilotos->insertar(piloto);

        contador++;
    }

    cout << "Se cargaron " << contador << " pilotos correctamente." << endl;
    pausa();
}



void cargarRutas() {
    string rutaArchivo;
    cout << "Ingrese la ruta del TXT de rutas: ";
    cin >> rutaArchivo;

    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo." << endl;
        pausa();
        return;
    }

    string linea;
    int contador = 0;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        string origen, destino, distanciaStr;
        stringstream ss(linea);

        if (getline(ss, origen, '/') &&
            getline(ss, destino, '/') &&
            getline(ss, distanciaStr, ';')) {

            // Limpiar espacios
            distanciaStr.erase(
                remove_if(distanciaStr.begin(), distanciaStr.end(), ::isspace),
                distanciaStr.end()
            );

            try {
                int distancia = stoi(distanciaStr);
                grafoRutas->agregarRuta(origen, destino, distancia);
                contador++;
            } catch (...) {
                cout << "Ruta invalida ignorada: " << linea << endl;
            }
        }
    }

    archivo.close();
    cout << "Se cargaron " << contador << " rutas exitosamente." << endl;
    pausa();
}


/* ===================== MOVIMIENTOS ===================== */

void procesarMovimientos() {
    string ruta;
    cout << "Ingrese la ruta del archivo de movimientos: ";
    cin >> ruta;

    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo." << endl;
        pausa();
        return;
    }

    string linea;
    int cont = 0;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        // ================= MANTENIMIENTO AVIONES =================
        if (linea.find("MantenimientoAviones") != string::npos) {

            // Ejemplo:
            // MantenimientoAviones,Ingreso,N70014;
            string tipo, accion, registro;

            stringstream ss(linea);
            getline(ss, tipo, ',');
            getline(ss, accion, ',');
            getline(ss, registro, ';');

            if (accion == "Ingreso") {
                Avion* a = arbolAvionesDisponibles->buscar(registro);
                if (a) {
                    arbolAvionesDisponibles->eliminar(registro);
                    a->setEstado("Mantenimiento");
                    listaMantenimiento->insertar(a);
                    cont++;
                }
            }
            else if (accion == "Salida") {
                Avion* a = listaMantenimiento->buscar(registro);
                if (a) {
                    listaMantenimiento->eliminar(registro);
                    a->setEstado("Disponible");
                    arbolAvionesDisponibles->insertar(registro, a);
                    cont++;
                }
            }
        }

        // ================= DAR DE BAJA PILOTO =================
        else if (linea.find("DarDeBaja") != string::npos) {

            size_t ini = linea.find("(");
            size_t fin = linea.find(")");

            if (ini == string::npos || fin == string::npos) continue;

            string idPiloto = linea.substr(ini + 1, fin - ini - 1);

            arbolPilotos->eliminar(idPiloto);
            tablaPilotos->eliminar(idPiloto);
            matrizVuelos->eliminarPiloto(idPiloto);

            cont++;
        }
    }

    archivo.close();
    cout << "Se procesaron " << cont << " movimientos." << endl;
    pausa();
}


/* ===================== CONSULTAS ===================== */

void menuConsultas() {
    int op;
    do {
        limpiarPantalla();
        cout << "\n=== MENU DE CONSULTAS ===\n";
        cout << "1. Aviones disponibles\n";
        cout << "2. Aviones en mantenimiento\n";
        cout << "3. Pilotos (Hash)\n";
        cout << "4. Rutas\n";
        cout << "5. Matriz de vuelos\n";  // <-- OPCIÓN 5
        cout << "6. Volver\n";
        cout << "Opcion: ";
        cin >> op;

        switch (op) {
            case 1:
                arbolAvionesDisponibles->generarReporte("consulta_disponibles");
                pausa(); break;
            case 2:
                listaMantenimiento->mostrar();
                pausa(); break;
            case 3:
                tablaPilotos->mostrar();
                pausa(); break;
            case 4:
                grafoRutas->mostrar();
                pausa(); break;
            case 5:
                construirMatriz();  // <-- ESTA LÍNEA
                pausa(); break;
        }
    } while (op != 6);
}

/* ===================== RECORRIDOS BST ===================== */

void recorridosArbol() {
    int op;
    do {
        limpiarPantalla();
        cout << "\n1. Preorden\n2. Inorden\n3. Postorden\n4. Volver\nOpcion: ";
        cin >> op;

        if (!arbolPilotos->estaVacio()) {
            if (op == 1) arbolPilotos->preorden();
            if (op == 2) arbolPilotos->inorden();
            if (op == 3) arbolPilotos->postorden();
        }
        if (op != 4) pausa();
    } while (op != 4);
}

/* ===================== RUTA MAS CORTA ===================== */

void recomendarRuta() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string origen, destino;

    cout << "Ciudad origen: ";
    getline(cin, origen);
    cout << "Ciudad destino: ";
    getline(cin, destino);

    int distancia;
    vector<string> ruta = grafoRutas->rutaMasCorta(origen, destino, distancia);

    if (ruta.empty()) {
        cout << "No existe ruta.\n";
    } else {
        cout << "Ruta: ";
        for (auto& c : ruta) cout << c << " ";
        cout << "\nDistancia: " << distancia << " km\n";
    }
    pausa();
}

/* ===================== REPORTES ===================== */

void menuReportes() {
    int op;
    do {
        limpiarPantalla();
        cout << "\n1. Arbol B\n2. Lista\n3. BST\n4. Hash\n5. Grafo\n6. Matriz\n7. Volver\nOpcion: ";
        cin >> op;

        if (op == 1) arbolAvionesDisponibles->generarReporte("arbol_b");
        if (op == 2) listaMantenimiento->generarReporte("lista");
        if (op == 3) arbolPilotos->generarReporte("bst");
        if (op == 4) tablaPilotos->generarReporte("hash");
        if (op == 5) grafoRutas->generarReporte("grafo");
        if (op == 6) matrizVuelos->generarReporte("matriz");

        if (op != 7) pausa();
    } while (op != 7);
}

/* ===================== CONSTRUIR MATRIZ ===================== */

void construirMatriz() {
    cout << "\n=== CONSTRUYENDO MATRIZ DISPERSA ===" << endl;
    
    // Limpiar matriz anterior
    delete matrizVuelos;
    matrizVuelos = new MatrizDispersa();
    
    int contador = 0;
    
    // Verificar que tenemos datos
    if (arbolAvionesDisponibles->estaVacio()) {
        cout << "ERROR: No hay aviones cargados." << endl;
        return;
    }
    
    if (arbolPilotos->estaVacio()) {
        cout << "ERROR: No hay pilotos cargados." << endl;
        return;
    }
    
    cout << "Asignando pilotos a vuelos..." << endl;
    
    // Recorrer TODOS los pilotos
    arbolPilotos->recorrerInorden([&](Piloto* piloto) {
        // Cada piloto tiene un campo "vuelo"
        if (!piloto->vuelo.empty()) {
            // Buscar el avión por su número de vuelo
            Avion* avion = arbolAvionesDisponibles->buscarPorVuelo(piloto->vuelo);
            
            if (avion && avion->getEstado() == "Disponible") {
                // Insertar en la matriz: vuelo, ciudad, piloto
                matrizVuelos->insertar(
                    avion->getVuelo(),          // Fila: Número de vuelo
                    avion->getCiudadDestino(),  // Columna: Ciudad destino
                    piloto->numero_de_id        // Valor: ID del piloto
                );
                contador++;
                cout << "  " << piloto->numero_de_id << " -> " 
                     << avion->getVuelo() << " -> " 
                     << avion->getCiudadDestino() << endl;
            } else {
                cout << "  Aviso: No se encontró avión disponible para vuelo " 
                     << piloto->vuelo << endl;
            }
        } else {
            cout << "  Aviso: Piloto " << piloto->nombre 
                 << " no tiene vuelo asignado" << endl;
        }
    });
    
    cout << "\nMatriz construida con " << contador << " asignaciones de vuelo." << endl;
    
    // Mostrar la matriz
    cout << "\n=== MATRIZ DISPERSA ===" << endl;
    if (contador > 0) {
        matrizVuelos->mostrar();
    } else {
        cout << "Matriz vacía - No hay asignaciones" << endl;
    }
}

/* ===================== MAIN ===================== */

int main() {
#ifdef _WIN32
    system("if not exist reportes mkdir reportes");
#else
    system("mkdir -p reportes");
#endif

    int op;
    do {
        limpiarPantalla();
        cout << "\n1.Cargar Aviones\n2.Cargar Pilotos\n3.Cargar Rutas\n";
        cout << "5.Consultas\n6.Recorridos\n7.Reportes\n8.Ruta mas corta\n9.Movimientos\n10.Salir\nOpcion: ";
        cin >> op;

        switch (op) {
            case 1: cargarAviones(); break;
            case 2: cargarPilotos(); break;
            case 3: cargarRutas(); break;
            case 5: menuConsultas(); break;
            case 6: recorridosArbol(); break;
            case 7: menuReportes(); break;
            case 8: recomendarRuta(); break;
            case 9: procesarMovimientos(); break;
        }
    } while (op != 10);

    return 0;
}
