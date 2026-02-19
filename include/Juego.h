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
        int cartasAcumuladas;
        void generarCartas(int numMazos); 
        void repartirCartasIniciales();
        void limpiarPantalla();
        void reponerMazo();
        void aplicarCartaEspecial(Carta* cartaJugada, bool teniaOpcionDeCarta);
        void mostrarMesa(Jugador* actual, Carta* cartaTopeVisual);
        TipoCarta tipoAcumulado;
        Color pedirColorUsuario();
    public:
        Juego();
        ~Juego();
        void inicializar(int numJugadores, ReglasFlags configuracion);
        void flujoPrincipal();
        void gestionarTurno();
        Jugador* verificarGanador();
        int getCartasEnMazo();
        int getCartasEnDescarte();
        int getCantidadJugadores();
        bool getLadoFlipActivo();
        ListaCircular<Jugador*>* getJugadores();
};

#endif