#ifndef NODO_H
#define NODO_H

template <class T>
class Nodo 
{
    public:
        T dato; // Carta* o Jugador*
        Nodo<T>* siguiente;
        Nodo<T>* anterior;
        Nodo(T valor)
        {
            this->dato = valor;
            this->siguiente = nullptr;
            this->anterior = nullptr;
        }
};
#endif