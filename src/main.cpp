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
        if(numJugadores < 2)
        {
            cout << "El minimo de jugadores es 2. Intenta de nuevo." << endl;
        }
        if(cin.fail())
        {
            cin.clear(); limpiarBufferMain();
        }
    }
    while (numJugadores < 2);
    cout << " REGLAS FLAG " << endl;
    cout << "1. ¿Activar UNO FLIP? (s/n): ";
    cin >> opc; 
    reglas.modoFlip = (opc == 's' || opc == 'S');
    if (opc == 's' || opc == 'S')
    {
        cout << "2. ¿Permitir acumulacion? (Lado Claro: +1 sobre +1/+2, +2 sobre +2 | Lado Oscuro (Flip): +3 sobre +3/+6, +6 sobre +6)(s/n): ";
        cin >> opc;
        reglas.acumulacion = (opc == 's' || opc == 'S');
    }
    else if (opc == 'n' || opc == 'N')
    {
        cout << "2. ¿Permitir acumulacion? (+2 sobre +2/+4, +4 sobre +4)(s/n): ";
        cin >> opc;
        reglas.acumulacion = (opc == 's' || opc == 'S');
        cout << "3. ¿Activar el Reto del +4? (s/n): ";
        cin >> opc;
        reglas.retoMasCuatro = (opc == 's' || opc == 'S');
    }
    cout << "4. ¿Robar cartas hasta poder jugar? (s/n): ";
    cin >> opc;
    reglas.roboHastaJugar = (opc == 's' || opc == 'S');
    cout << "5. ¿Castigo de tomar 2 cartas por no gritar UNO? (s/n): ";
    cin >> opc;
    reglas.gritoUno = (opc == 's' || opc == 'S');
    cout << "6. ¿Permitir ganar lanzando carta negra(Comodín) al final? (s/n): ";
    cin >> opc;
    reglas.ganarConNegra = (opc == 's' || opc == 'S');
    limpiarBufferMain();
    miJuego.inicializar(numJugadores, reglas);
    miJuego.flujoPrincipal();
    return 0;
}