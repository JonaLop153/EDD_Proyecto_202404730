#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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

ArbolB* arbolAvionesDisponibles = new ArbolB(5);
ListaCircular* listaMantenimiento = new ListaCircular();
ArbolBST* arbolPilotos = new ArbolBST();
TablaHash* tablaPilotos = new TablaHash(19);
Grafo* grafoRutas = new Grafo();
MatrizDispersa* matrizVuelos = new MatrizDispersa();

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

void cargarAviones() {
    string rutaArchivo;
    cout << "Ingrese la ruta del archivo JSON de aviones: ";
    cin >> rutaArchivo;
    
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo." << endl;
        return;
    }
    
    json datos;
    try {
        archivo >> datos;
    } catch (json::exception& e) {
        cout << "Error al parsear JSON: " << e.what() << endl;
        archivo.close();
        return;
    }
    archivo.close();
    
    int contador = 0;
    for (auto& item : datos) {
        string vuelo = item["vuelo"];
        string numero_registro = item["numero_de_registro"];
        string modelo = item["modelo"];
        int capacidad = item["capacidad"];
        string aerolinea = item["aerolinea"];
        string ciudad_destino = item["ciudad_destino"];
        string estado = item["estado"];
        
        Avion* avion = new Avion(vuelo, numero_registro, modelo, capacidad, 
                                 aerolinea, ciudad_destino, estado);
        
        if (estado == "Disponible") {
            arbolAvionesDisponibles->insertar(numero_registro, avion);
        } else if (estado == "Mantenimiento") {
            listaMantenimiento->insertar(avion);
        }
        contador++;
    }
    
    cout << "Se cargaron " << contador << " aviones exitosamente." << endl;
    pausa();
}

void cargarPilotos() {
    string rutaArchivo;
    cout << "Ingrese la ruta del archivo JSON de pilotos: ";
    cin >> rutaArchivo;
    
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo." << endl;
        return;
    }
    
    json datos;
    try {
        archivo >> datos;
    } catch (json::exception& e) {
        cout << "Error al parsear JSON: " << e.what() << endl;
        archivo.close();
        return;
    }
    archivo.close();
    
    int contador = 0;
    for (auto& item : datos) {
        string nombre = item["nombre"];
        string nacionalidad = item["nacionalidad"];
        string numero_id = item["numero_de_id"];
        string vuelo = item["vuelo"];
        int horas_vuelo = item["horas_de_vuelo"];
        string licencia = item["tipo_de_licencia"];
        
        Piloto* piloto = new Piloto(nombre, nacionalidad, numero_id, 
                                    vuelo, horas_vuelo, licencia);
        
        arbolPilotos->insertar(piloto);
        tablaPilotos->insertar(piloto);
        contador++;
    }
    
    cout << "Se cargaron " << contador << " pilotos exitosamente." << endl;
    pausa();
}

void cargarRutas() {
    string rutaArchivo;
    cout << "Ingrese la ruta del archivo TXT de rutas: ";
    cin >> rutaArchivo;
    
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo." << endl;
        return;
    }
    
    string linea;
    int contador = 0;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string origen, destino, distanciaStr;
        
        if (getline(ss, origen, '/') && 
            getline(ss, destino, '/') && 
            getline(ss, distanciaStr, ';')) {
            
            int distancia = stoi(distanciaStr);
            grafoRutas->agregarRuta(origen, destino, distancia);
            contador++;
        }
    }
    archivo.close();
    
    cout << "Se cargaron " << contador << " rutas exitosamente." << endl;
    pausa();
}

void cargarAsignaciones() {
    string rutaArchivo;
    cout << "Ingrese la ruta del archivo JSON de asignaciones: ";
    cin >> rutaArchivo;
    
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo." << endl;
        return;
    }
    
    json datos;
    try {
        archivo >> datos;
    } catch (json::exception& e) {
        cout << "Error al parsear JSON: " << e.what() << endl;
        archivo.close();
        return;
    }
    archivo.close();
    
    int contador = 0;
    for (auto& item : datos) {
        string id_piloto = item["id_piloto"];
        string vuelo = item["vuelo"];
        string ciudad_destino = item["ciudad_destino"];
        
        matrizVuelos->insertar(id_piloto, ciudad_destino, vuelo);
        contador++;
    }
    
    cout << "Se cargaron " << contador << " asignaciones exitosamente." << endl;
    pausa();
}

void procesarMovimientos() {
    string rutaArchivo;
    cout << "Ingrese la ruta del archivo de movimientos: ";
    cin >> rutaArchivo;
    
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo." << endl;
        return;
    }
    
    string linea;
    int contador = 0;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string comando;
        ss >> comando;
        
        if (comando == "MantenimientoAviones") {
            string estado, numeroRegistro;
            ss >> estado >> numeroRegistro;
            
            if (estado == "Disponible") {
                Avion* avion = listaMantenimiento->buscar(numeroRegistro);
                if (avion != nullptr) {
                    avion->setEstado("Disponible");
                    arbolAvionesDisponibles->insertar(numeroRegistro, avion);
                    listaMantenimiento->eliminar(numeroRegistro);
                    cout << "Avion " << numeroRegistro << " movido a Disponible" << endl;
                }
            } else if (estado == "Mantenimiento") {
                Avion* avion = arbolAvionesDisponibles->buscar(numeroRegistro);
                if (avion != nullptr) {
                    avion->setEstado("Mantenimiento");
                    listaMantenimiento->insertar(avion);
                    arbolAvionesDisponibles->eliminar(numeroRegistro);
                    cout << "Avion " << numeroRegistro << " movido a Mantenimiento" << endl;
                }
            }
            contador++;
        } else if (comando == "DarDeBaja") {
            string idPiloto;
            getline(ss, idPiloto, '(');
            getline(ss, idPiloto, ')');
            
            arbolPilotos->eliminar(idPiloto);
            tablaPilotos->eliminar(idPiloto);
            matrizVuelos->eliminarPiloto(idPiloto);
            cout << "Piloto " << idPiloto << " dado de baja" << endl;
            contador++;
        }
    }
    archivo.close();
    
    cout << "Se procesaron " << contador << " movimientos." << endl;
    pausa();
}

void recorridosArbol() {
    int opcion;
    do {
        limpiarPantalla();
        cout << "\n=== RECORRIDOS DEL ARBOL BST ===" << endl;
        cout << "1. Preorden" << endl;
        cout << "2. Inorden" << endl;
        cout << "3. Postorden" << endl;
        cout << "4. Volver" << endl;
        cout << "Seleccione: ";
        cin >> opcion;
        
        switch(opcion) {
            case 1:
                arbolPilotos->preorden();
                pausa();
                break;
            case 2:
                arbolPilotos->inorden();
                pausa();
                break;
            case 3:
                arbolPilotos->postorden();
                pausa();
                break;
            case 4:
                break;
            default:
                cout << "Opcion invalida" << endl;
                pausa();
        }
    } while(opcion != 4);
}

void recomendarRuta() {
    string origen, destino;
    cin.ignore();
    cout << "Ingrese ciudad origen: ";
    getline(cin, origen);
    cout << "Ingrese ciudad destino: ";
    getline(cin, destino);
    
    int distanciaTotal;
    vector<string> ruta = grafoRutas->rutaMasCorta(origen, destino, distanciaTotal);
    
    if (distanciaTotal == -1) {
        cout << "\nNo existe ruta entre " << origen << " y " << destino << endl;
    } else {
        cout << "\n=== RUTA MAS CORTA ===" << endl;
        cout << "Distancia total: " << distanciaTotal << " km" << endl;
        cout << "Ruta: ";
        for (size_t i = 0; i < ruta.size(); i++) {
            cout << ruta[i];
            if (i < ruta.size() - 1) cout << " -> ";
        }
        cout << endl;
    }
    pausa();
}

void menuReportes() {
    int opcion;
    do {
        limpiarPantalla();
        cout << "\n=== MENU DE REPORTES ===" << endl;
        cout << "1. Arbol B (Aviones Disponibles)" << endl;
        cout << "2. Lista Circular (Aviones en Mantenimiento)" << endl;
        cout << "3. Arbol BST (Pilotos por horas de vuelo)" << endl;
        cout << "4. Tabla Hash (Pilotos)" << endl;
        cout << "5. Grafo (Rutas)" << endl;
        cout << "6. Matriz Dispersa (Vuelos y Ciudades)" << endl;
        cout << "7. Volver" << endl;
        cout << "Seleccione: ";
        cin >> opcion;
        
        switch(opcion) {
            case 1:
                if (arbolAvionesDisponibles->estaVacio()) {
                    cout << "El arbol esta vacio" << endl;
                    pausa();
                } else {
                    arbolAvionesDisponibles->generarReporte("arbol_b_disponibles");
                    cout << "Reporte generado y abierto exitosamente" << endl;
                    pausa();
                }
                break;
            case 2:
                if (listaMantenimiento->estaVacia()) {
                    cout << "La lista esta vacia" << endl;
                    pausa();
                } else {
                    listaMantenimiento->generarReporte("lista_mantenimiento");
                    cout << "Reporte generado y abierto exitosamente" << endl;
                    pausa();
                }
                break;
            case 3:
                if (arbolPilotos->estaVacio()) {
                    cout << "El arbol esta vacio" << endl;
                    pausa();
                } else {
                    arbolPilotos->generarReporte("arbol_bst_pilotos");
                    cout << "Reporte generado y abierto exitosamente" << endl;
                    pausa();
                }
                break;
            case 4:
                tablaPilotos->generarReporte("tabla_hash_pilotos");
                cout << "Reporte generado y abierto exitosamente" << endl;
                pausa();
                break;
            case 5:
                grafoRutas->generarReporte("grafo_rutas");
                cout << "Reporte generado y abierto exitosamente" << endl;
                pausa();
                break;
            case 6:
                matrizVuelos->generarReporte("matriz_dispersa");
                cout << "Reporte generado y abierto exitosamente" << endl;
                pausa();
                break;
            case 7:
                break;
            default:
                cout << "Opcion invalida" << endl;
                pausa();
        }
    } while(opcion != 7);
}

void menuConsultas() {
    int opcion;
    do {
        limpiarPantalla();
        cout << "\n=== MENU DE CONSULTAS ===" << endl;
        cout << "1. Mostrar aviones disponibles" << endl;
        cout << "2. Mostrar aviones en mantenimiento" << endl;
        cout << "3. Mostrar pilotos (Tabla Hash)" << endl;
        cout << "4. Mostrar rutas" << endl;
        cout << "5. Mostrar matriz de vuelos" << endl;
        cout << "6. Volver" << endl;
        cout << "Seleccione: ";
        cin >> opcion;
        
        switch(opcion) {
            case 1:
                cout << "\nGenerando reporte de aviones disponibles..." << endl;
                arbolAvionesDisponibles->generarReporte("consulta_disponibles");
                pausa();
                break;
            case 2:
                listaMantenimiento->mostrar();
                pausa();
                break;
            case 3:
                tablaPilotos->mostrar();
                pausa();
                break;
            case 4:
                grafoRutas->mostrar();
                pausa();
                break;
            case 5:
                matrizVuelos->mostrar();
                pausa();
                break;
            case 6:
                break;
            default:
                cout << "Opcion invalida" << endl;
                pausa();
        }
    } while(opcion != 6);
}

int main() {
    int opcion;
    
#ifdef _WIN32
    system("mkdir reportes 2>nul");
#else
    system("mkdir -p reportes");
#endif
    
    do {
        limpiarPantalla();
        cout << "\n=====================================" << endl;
        cout << "  SISTEMA DE GESTION DE AEROPUERTO  " << endl;
        cout << "=====================================" << endl;
        cout << "1. Carga de aviones" << endl;
        cout << "2. Carga de pilotos" << endl;
        cout << "3. Carga de rutas" << endl;
        cout << "4. Carga de asignaciones (Matriz)" << endl;
        cout << "5. Consultas" << endl;
        cout << "6. Recorridos del arbol de pilotos" << endl;
        cout << "7. Visualizar reportes" << endl;
        cout << "8. Recomendar ruta mas corta" << endl;
        cout << "9. Procesar movimientos" << endl;
        cout << "10. Salir" << endl;
        cout << "=====================================" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        
        switch(opcion) {
            case 1:
                cargarAviones();
                break;
            case 2:
                cargarPilotos();
                break;
            case 3:
                cargarRutas();
                break;
            case 4:
                cargarAsignaciones();
                break;
            case 5:
                menuConsultas();
                break;
            case 6:
                recorridosArbol();
                break;
            case 7:
                menuReportes();
                break;
            case 8:
                recomendarRuta();
                break;
            case 9:
                procesarMovimientos();
                break;
            case 10:
                cout << "\nGracias por usar el sistema. Adios!" << endl;
                break;
            default:
                cout << "\nOpcion invalida. Intente nuevamente." << endl;
                pausa();
        }
    } while(opcion != 10);
    
    return 0;
}