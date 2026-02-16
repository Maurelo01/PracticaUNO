#ifndef JUEGO_H
#define JUEGO_H
#include "Pila.h"
#include "ListaCircular.h"
#include "Jugador.h"
#include "Carta.h"

struct ReglasFlags
{
    bool acumulacion = false;
    bool retoMasCuatro = false;
    bool roboHastaJugar = false;
    bool gritoUno = false;
    bool ganarConNegra = false;
    bool modoFlip = false;
};

class Juego
{
    private:
        Pila<Carta*>* mazo;
        Pila<Carta*>* descarte;
        ListaCircular<Jugador*>* jugadores;
        ReglasFlags reglas;
        bool direccionDerecha;        
        bool ladoFlipActivo;
        void generarCartas(int numMazos); 
        void repartirCartasIniciales();
    public:
        Juego();
        ~Juego();
        void inicializar(int numJugadores, ReglasFlags configuracion);
        int getCartasEnMazo();
        int getCartasEnDescarte();
        int getCantidadJugadores();
        bool getLadoFlipActivo();
        ListaCircular<Jugador*>* getJugadores();
};

#endif