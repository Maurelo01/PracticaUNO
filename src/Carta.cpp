#include "../include/Carta.h"

Carta::Carta(Color color, TipoCarta tipo, int valor) 
{
    this->color = color;
    this->tipo = tipo;
    this->valor = valor;
    this->ladoOscuro = nullptr;
    this->esLadoOscuro = false;
}

Color Carta::getColor() { return color; }
TipoCarta Carta::getTipo() { return tipo; }
int Carta::getValor() { return valor; }
Carta* Carta::getLadoOscuro() { return ladoOscuro; }
bool Carta::getEsLadoOscuro() { return esLadoOscuro; }

void Carta::setLadoOscuro(Carta* reverso) 
{
    this->ladoOscuro = reverso;
    if (reverso != nullptr)
    {
        reverso->esLadoOscuro = true;
    }
}

void Carta::voltear() 
{
    esLadoOscuro = !esLadoOscuro;
}

bool Carta::esCompatible(Carta* otra) 
{
    if (this->color == NEGRO || this->tipo == COMODIN_COLOR || this->tipo == MAS_CUATRO || this->tipo == DESTRUCTORA || this->tipo == PASAR_EXTREMO)
    {
        return true;
    }
    if (this->color == otra->getColor())
    {
        return true;
    }
    if (this->tipo == otra->getTipo())
    {
        if (this->tipo == NUMERO)
        {
            return (this->valor == otra->getValor());
        }
        return true;
    }
    return false;
}

void Carta::setColor(Color nuevoColor) 
{
    this->color = nuevoColor;
}

string Carta::toString()
{
    string nomColor;
    switch(color) 
    {
        case ROJO: nomColor = "ROJO"; break;
        case AZUL: nomColor = "AZUL"; break;
        case VERDE: nomColor = "VERDE"; break;
        case AMARILLO: nomColor = "AMARILLO"; break;
        case NEGRO: nomColor = "NEGRO (WILD)"; break; 
        case ROSA: nomColor = "ROSA(F)"; break;
        case TURQUESA: nomColor = "TURQUESA(F)"; break;
        case NARANJA: nomColor = "NARANJA(F)"; break;
        case VIOLETA: nomColor = "VIOLETA(F)"; break;
        default: nomColor = "???";
    }
    string nomTipo;
    switch(tipo)
    {
        case NUMERO: nomTipo = to_string(valor); break;
        case SALTO: nomTipo = "SALTO"; break;
        case REVERSA: nomTipo = "REVERSA"; break;
        case MAS_UNO: nomTipo = "+1"; break; 
        case MAS_DOS:
            if (color == NEGRO) nomTipo = "+2 (WILD)"; 
            else nomTipo = "+2"; 
            break;
        case MAS_CUATRO: nomTipo = "+4"; break;
        case COMODIN_COLOR: nomTipo = "CAMBIO COLOR"; break;
        case SALTO_TODOS: nomTipo = "SALTO TODOS"; break;
        case MAS_TRES: nomTipo = "+3"; break; 
        case MAS_SEIS: nomTipo = "+6"; break; 
        case COLOR_ETERNO: nomTipo = "COLOR ETERNO"; break;
        case DESTRUCTORA: nomTipo = "¡¡DESTRUCTORA!!"; break;
        case PASAR_EXTREMO: nomTipo = "PASAR EXTREMO >>"; break;
        default: nomTipo = "ACCION";
    }
    return "[" + nomColor + " | " + nomTipo + "]";
}