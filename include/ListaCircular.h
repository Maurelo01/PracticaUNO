#ifndef LISTACIRCULAR_H
#define LISTACIRCULAR_H

#include "Nodo.h"

template <class T>
class ListaCircular
{
    private:
        Nodo<T>* actual;
        int tamaño;

    public:
        ListaCircular()
        {
            actual = nullptr;
            tamaño = 0;
        }

        int getTamaño() 
        { 
            return tamaño;
        }
        
        void insertar(T dato)
        {
            Nodo<T>* nuevo = new Nodo<T>(dato);
            if (actual == nullptr)
            {
                actual = nuevo;
                actual->siguiente = actual;
                actual->anterior = actual;
            }
            else
            {
                Nodo<T>* ultimo = actual->anterior;
                ultimo->siguiente = nuevo;
                nuevo->anterior = ultimo;
                nuevo->siguiente = actual;
                actual->anterior = nuevo;
            }
            tamaño++;
        }

        void siguiente()
        {
            if (actual != nullptr)
            {
                actual = actual->siguiente;
            }
        }

        void anterior()
        {
            if (actual != nullptr)
            {
                actual = actual->anterior;
            }
        }

        T obtenerActual()
        {
            if (actual == nullptr) return nullptr;
            return actual->dato;
        }
        
        Nodo<T>* getNodoActual()
        {
            return actual;
        }
};

#endif