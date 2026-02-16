#ifndef LISTADOBLE_H
#define LISTADOBLE_H
#include "Nodo.h"
#include "Carta.h"
using namespace std;

template <class T>
class ListaDoble 
{
    private:
        Nodo<T>* cabeza;
        Nodo<T>* cola;
        int tamaño;

    public:
        ListaDoble()
        {
            cabeza = nullptr;
            cola = nullptr;
            tamaño = 0;
        }

        int getTamaño()
        {
            return tamaño;
        }

        void insertarAlFinal(T dato) 
        {
            Nodo<T>* nuevo = new Nodo<T>(dato);
            if (cabeza == nullptr) 
            {
                cabeza = cola = nuevo;
            }
            else
            {
                cola->siguiente = nuevo;
                nuevo->anterior = cola;
                cola = nuevo;
            }
            tamaño++;
        }

        void eliminar(T dato)
        {
            if (cabeza == nullptr) return;
            Nodo<T>* actual = cabeza;
            while (actual != nullptr)
            {
                if (actual->dato == dato)
                {
                    if (actual == cabeza && actual == cola)
                    {
                        cabeza = cola = nullptr;
                    }
                    else if (actual == cabeza)
                    {
                        cabeza = cabeza->siguiente;
                        cabeza->anterior = nullptr;
                    }
                    else if (actual == cola)
                    {
                        cola = cola->anterior;
                        cola->siguiente = nullptr;
                    }
                    else
                    {
                        actual->anterior->siguiente = actual->siguiente;
                        actual->siguiente->anterior = actual->anterior;
                    }
                    delete actual;
                    tamaño--;
                    return;
                }
                actual = actual->siguiente;
            }
        }

        T obtenerPorIndice(int indice)
        {
            if (indice < 0 || indice >= tamaño) return nullptr;
            Nodo<T>* actual = cabeza;
            for (int i = 0; i < indice; i++)
            {
                actual = actual->siguiente;
            }
            return actual->dato;
        }

        void eliminarPorTipo(TipoCarta tipoObjetivo)
        {
            Nodo<T>* actual = cabeza;
            while (actual != nullptr)
            {
                Nodo<T>* siguienteTemp = actual->siguiente;
                if (actual->dato->getTipo() == tipoObjetivo)
                {
                    T cartaABorrar = actual->dato;
                    delete cartaABorrar; 
                    eliminar(cartaABorrar); 
                    cout << "   [Mano] Carta eliminada." << endl;
                }
                actual = siguienteTemp;
            }
        }
};

#endif