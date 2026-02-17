#include <iostream>
#include <limits>
#include "../include/Juego.h"

using namespace std;

void limpiarBufferMain()
{
    string sueltos; 
    getline(cin, sueltos);
}

int main() 
{
    Juego miJuego;
    ReglasFlags reglas;
    int numJugadores;
    char opc;
    cout << " ______________________________________" << endl;
    cout << "|___________UNO CLASICO/FLIP___________|" << endl;
    do 
    {
        cout << "Ingresa la cantidad de jugadores (Minimo 2): ";
        cin >> numJugadores;
        if(cin.fail())
        {
            cin.clear(); limpiarBufferMain();
        }
    }
    while (numJugadores < 2);
    cout << "Reglas FLAG: " << endl;
    cout << "¿Activar UNO FLIP? (s/n): ";
    cin >> opc;
    reglas.modoFlip = (opc == 's' || opc == 'S');

    if (!reglas.modoFlip)
    {
        cout << "¿Permitir acumulacion +2 sobre +2? (s/n): ";
        cin >> opc;
        reglas.acumulacion = (opc == 's' || opc == 'S');
    }
    miJuego.inicializar(numJugadores, reglas);
    miJuego.flujoPrincipal();
    return 0;
}