#ifndef PILA_H
#define PILA_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Nodo.h"
#include "Carta.h"

using namespace std;

template <class T>
class Pila
{
    private:
        Nodo<T>* tope;
        int tamaño;

    public:
        Pila()
        {
            tope = nullptr;
            tamaño = 0;
            srand(time(0));
        }

        ~Pila()
        {
            while (!estaVacia())
            {
                desapilar();
            }
        }

        bool estaVacia()
        {
            return tope == nullptr;
        }

        int getTamaño()
        {
            return tamaño;
        }

        void apilar(T dato) 
        {
            Nodo<T>* nuevo = new Nodo<T>(dato);
            nuevo->siguiente = tope;
            tope = nuevo;
            tamaño++;
        }

        T desapilar()
        {
            if (estaVacia())
            {
                return nullptr;
            }
            Nodo<T>* temp = tope;
            T dato = temp->dato;
            tope = tope->siguiente;
            delete temp;
            tamaño--;
            return dato;
        }

        T verTope()
        {
            if (estaVacia()) return nullptr;
            return tope->dato;
        }

        void barajar()
        {
            if (tamaño < 2) return;
            T* arr = new T[tamaño];
            Nodo<T>* actual = tope;
            int count = 0;
            while(actual != nullptr)
            {
                arr[count++] = actual->dato;
                actual = actual->siguiente;
            }
            for (int i = tamaño - 1; i > 0; i--)
            {
                int j = rand() % (i + 1);
                T temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
            actual = tope;
            count = 0;
            while(actual != nullptr)
            {
                actual->dato = arr[count++];
                actual = actual->siguiente;
            }
            delete[] arr;
        } 
        
        void eliminarPorTipo(TipoCarta tipoObjetivo) 
        {
            if (estaVacia()) return;
            while (tope != nullptr && tope->dato->getTipo() == tipoObjetivo)
            {
                Nodo<T>* aBorrar = tope;
                tope = tope->siguiente;
                delete aBorrar->dato; 
                delete aBorrar;
                tamaño--;
            }
            if (tope == nullptr) return;
            Nodo<T>* actual = tope;
            while (actual->siguiente != nullptr) 
            {
                if (actual->siguiente->dato->getTipo() == tipoObjetivo)
                {
                    Nodo<T>* aBorrar = actual->siguiente;
                    actual->siguiente = aBorrar->siguiente;
                    delete aBorrar->dato;
                    delete aBorrar;
                    tamaño--;
                }
                else 
                {
                    actual = actual->siguiente;
                }
            }
        }
    };

#endif