#ifndef CARTA_H
#define CARTA_H
#include <string>
#include <iostream>
using namespace std;

enum Color 
{
    ROJO, AZUL, VERDE, AMARILLO, NEGRO, // Cara principal
    ROSA, TURQUESA, NARANJA, VIOLETA // Cara flip
};

enum TipoCarta 
{
    NUMERO, SALTO, MAS_UNO,  // Cartas comodin simples
    COMODIN_COLOR, MAS_DOS, MAS_CUATRO, FLIP, // Comodines cara principal
    SALTO_TODOS, MAS_TRES, MAS_SEIS, COLOR_ETERNO, // Cartas comodin cara flip
    REVERSA, // Carta comodin para ambas caras
    DESTRUCTORA, PASAR_EXTREMO // Mis cartas personalizadas
};

class Carta
{
    private:
        Color color;
        TipoCarta tipo;
        int valor; // 0-9 para cartas numéricas, -1 para especiales
        Carta* ladoOscuro; // Puntero al reverso flip
        bool esLadoOscuro;
    public:
        Carta(Color color, TipoCarta tipo, int valor);
        Color getColor();
        TipoCarta getTipo();
        int getValor();
        Carta* getLadoOscuro();
        bool getEsLadoOscuro();
        void setLadoOscuro(Carta* reverso);
        void voltear(); // Cambia la carta al lado opuesto visualmente
        bool esCompatible(Carta* otra); // Valida si se puede jugar sobre otra
        string toString();
};

#endif