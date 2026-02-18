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
        void limpiarPantalla();
        void reponerMazo();
        void aplicarCartaEspecial(Carta* cartaJugada);
        void mostrarMesa(Jugador* actual, Carta* cartaTopeVisual);
        Color pedirColorUsuario();
    public:
        Juego();
        ~Juego();
        void inicializar(int numJugadores, ReglasFlags configuracion);
        void flujoPrincipal();
        void gestionarTurno();
        bool verificarGanador();
        int getCartasEnMazo();
        int getCartasEnDescarte();
        int getCantidadJugadores();
        bool getLadoFlipActivo();
        ListaCircular<Jugador*>* getJugadores();
};

#endif