#include "../include/Jugador.h"

Jugador::Jugador(string nombre)
{
    this->nombre = nombre;
    this->mano = new ListaDoble<Carta*>();
}

Jugador::~Jugador()
{
    delete mano; 
}

string Jugador::getNombre()
{
    return this->nombre;
}

ListaDoble<Carta*>* Jugador::getMano()
{
    return this->mano;
}

int Jugador::cantidadCartas()
{
    return mano->getTamaño();
}

void Jugador::robarCarta(Carta* c)
{
    if (c != nullptr)
    {
        mano->insertarAlFinal(c);
    }
}

Carta* Jugador::jugarCarta(int indice)
{
    Carta* c = mano->obtenerPorIndice(indice);
    if (c != nullptr)
    {
        mano->eliminar(c);
        return c;
    }
    return nullptr;
}

bool Jugador::tieneCarta(TipoCarta tipo)
{
    for(int i = 0; i < mano->getTamaño(); i++)
    {
        Carta* c = mano->obtenerPorIndice(i);
        if(c->getTipo() == tipo)
        {
            return true;
        }
    }
    return false;
}

void Jugador::aplicarDestruccion(TipoCarta tipo)
{
    mano->eliminarPorTipo(tipo);
}