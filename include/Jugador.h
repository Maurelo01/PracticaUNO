#ifndef JUGADOR_H
#define JUGADOR_H
#include <string>
#include "ListaDoble.h"
#include "Carta.h"
using namespace std;

class Jugador 
{
    private:
        string nombre;
        ListaDoble<Carta*>* mano;
    public:
        Jugador(string nombre);
        ~Jugador();
        string getNombre();
        ListaDoble<Carta*>* getMano();
        void robarCarta(Carta* c);
        Carta* jugarCarta(int indice);
        bool tieneCarta(TipoCarta tipo);
        void aplicarDestruccion(TipoCarta tipo);
        void ordenarMano(bool ladoFlipActivo);
        int aplicarDestruccionExacta(Color col, TipoCarta tip, int val);
        int cantidadCartas();
};

#endif