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

int obtenerValorColor(Color c, bool ladoFlipActivo) 
{
    if (!ladoFlipActivo) 
    {
        switch(c)
        {
            case ROJO: return 0;
            case AZUL: return 1;
            case VERDE: return 2;
            case AMARILLO: return 3;
            case NEGRO: return 4;
            default: return 100;
        }
    } 
    else 
    {
        switch(c)
        {
            case ROSA: return 0;
            case TURQUESA: return 1;
            case NARANJA: return 2;
            case VIOLETA: return 3;
            case NEGRO: return 4;
            default: return 100;
        }
    }
}

int obtenerPrioridadTipo(Carta* c) 
{
    if (c->getTipo() == NUMERO) return c->getValor();
    switch(c->getTipo()) 
    {
        case SALTO: return 10;
        case REVERSA: return 11;
        case FLIP: return 12;
        case SALTO_TODOS: return 13;
        case COMODIN_COLOR: return 14;
        case MAS_UNO: return 15;
        case MAS_DOS: return 16;
        case MAS_TRES: return 17;
        case MAS_CUATRO: return 18;
        case MAS_SEIS: return 19;
        case COLOR_ETERNO: return 20;
        case DESTRUCTORA: return 21;
        case PASAR_EXTREMO: return 22;
        default: return 100;
    }
}

void Jugador::ordenarMano(bool ladoFlipActivo) 
{
    int n = mano->getTamaño();
    if (n <= 1) return; 
    Carta** arr = new Carta*[n];
    for (int i = 0; i < n; i++)
    {
        arr[i] = mano->obtenerPorIndice(i);
    }
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            Carta* aFisica = arr[j];
            Carta* bFisica = arr[j+1];
            Carta* aVisual = ladoFlipActivo ? aFisica->getLadoOscuro() : aFisica;
            Carta* bVisual = ladoFlipActivo ? bFisica->getLadoOscuro() : bFisica;
            int colorA = obtenerValorColor(aVisual->getColor(), ladoFlipActivo);
            int colorB = obtenerValorColor(bVisual->getColor(), ladoFlipActivo);
            bool intercambiar = false;
            if (colorA > colorB)
            {
                intercambiar = true;
            } 
            else if (colorA == colorB)
            {
                if (obtenerPrioridadTipo(aVisual) > obtenerPrioridadTipo(bVisual))
                {
                    intercambiar = true;
                }
            }
            if (intercambiar)
            {
                arr[j] = bFisica;
                arr[j+1] = aFisica;
            }
        }
    }
    for (int i = 0; i < n; i++)
    {
        mano->eliminar(arr[i]); 
    }
    for (int i = 0; i < n; i++)
    {
        mano->insertarAlFinal(arr[i]); 
    }
    delete[] arr;
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

int Jugador::aplicarDestruccionExacta(Color col, TipoCarta tip, int val)
{
    int i = 0;
    int destruidas = 0;
    while (i < mano->getTamaño())
    {
        Carta* carta = mano->obtenerPorIndice(i);
        if (carta->getColor() == col && carta->getTipo() == tip && carta->getValor() == val)
        {
            mano->eliminar(carta);
            if (carta->getLadoOscuro() != nullptr)
            {
                delete carta->getLadoOscuro();
            }
            delete carta; 
            destruidas++;
        }
        else
        {
            i++;
        }
    }
    return destruidas;
}