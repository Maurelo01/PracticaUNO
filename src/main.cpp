#include <iostream>
#include "../include/Carta.h"
#include "../include/Pila.h"
#include "../include/ListaDoble.h"
#include "../include/ListaCircular.h"

using namespace std;

int main() {
    cout << "=== FIN DIA 2: PRUEBA DE ESTRUCTURAS ===" << endl;

    // 1. PRUEBA PILA (MAZO)
    Pila<Carta*> mazo;
    mazo.apilar(new Carta(ROJO, NUMERO, 1));
    mazo.apilar(new Carta(AZUL, MAS_DOS, -1));
    cout << "Pila OK. Tope: " << mazo.verTope()->toString() << endl;

    // 2. PRUEBA LISTA DOBLE (MANO)
    ListaDoble<Carta*> mano;
    mano.insertarAlFinal(new Carta(VERDE, SALTO, -1));
    mano.insertarAlFinal(new Carta(AMARILLO, NUMERO, 8));
    cout << "Lista Doble OK. Carta indice 1: " << mano.obtenerPorIndice(1)->toString() << endl;
    
    // Prueba destructora en mano
    mano.eliminarPorTipo(SALTO);
    cout << "Despues de eliminar SALTO, tamano mano: " << mano.getTamaño() << " (Debe ser 1)" << endl;

    // 3. PRUEBA LISTA CIRCULAR (JUGADORES - Simulados con int por ahora)
    ListaCircular<int*> mesa;
    int j1 = 1, j2 = 2, j3 = 3;
    mesa.insertar(&j1);
    mesa.insertar(&j2);
    mesa.insertar(&j3);

    cout << "Lista Circular OK. Turno actual: Jugador " << *mesa.obtenerActual() << endl;
    mesa.siguiente();
    cout << "Avanzamos turno (siguiente): Jugador " << *mesa.obtenerActual() << endl;
    mesa.anterior(); // Simulando REVERSA
    mesa.anterior();
    cout << "Retrocedemos 2 veces (anterior): Jugador " << *mesa.obtenerActual() << " (Debe ser 3)" << endl;

    return 0;
}