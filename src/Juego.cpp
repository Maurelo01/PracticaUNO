#include "../include/Juego.h"
#include <cstdlib>
#include <ctime>

Juego::Juego()
{
    mazo = new Pila<Carta*>();
    descarte = new Pila<Carta*>();
    jugadores = new ListaCircular<Jugador*>();
    direccionDerecha = true;
    ladoFlipActivo = false;
    srand(time(0));
}

Juego::~Juego()
{
    delete mazo;
    delete descarte;
    delete jugadores;
}

void Juego::inicializar(int numJugadores, ReglasFlags configuracion)
{
    this->reglas = configuracion;
    int numMazos = ((numJugadores - 1) / 6) + 1;
    cout << " INICIANDO JUEGO" << endl;
    cout << "Jugadores: " << numJugadores << " | Mazos generados: " << numMazos << endl;
    generarCartas(numMazos);
    mazo->barajar();
    for(int i = 1; i <= numJugadores; i++)
    {
        string nombre = "Jugador " + to_string(i);
        jugadores->insertar(new Jugador(nombre));
    }
    repartirCartasIniciales();
    if (!mazo->estaVacia())
    {
        descarte->apilar(mazo->desapilar());
    }
}

void Juego::generarCartas(int numMazos)
{
    int capacidadAprox = 150 * numMazos; 
    Carta** tempCaraPrincipal = new Carta*[capacidadAprox];
    Carta** tempFlip = new Carta*[capacidadAprox];
    int contador = 0;
    Color coloresCaraPrincipal[] = {ROJO, AZUL, VERDE, AMARILLO};
    Color coloresFlip[] = {NARANJA, ROSA, TURQUESA, VIOLETA};
    if (reglas.modoFlip)
    {
        // Generar Cartas para el modo Flip vinculando cara principal con reverso
        for (int m = 0; m < numMazos; m++)
        {
            for (int k = 0; k < 4; k++) 
            {
                Color cCaraPrincipal = coloresCaraPrincipal[k];
                Color cFlip = coloresFlip[k];
                tempCaraPrincipal[contador] = new Carta(cCaraPrincipal, NUMERO, 0);
                tempFlip[contador] = new Carta(cFlip, NUMERO, 0);
                contador++;
                for (int i = 1; i <= 9; i++)
                {
                    for (int j = 0; j < 2; j++)
                    {
                        tempCaraPrincipal[contador] = new Carta(cCaraPrincipal, NUMERO, i);
                        tempFlip[contador] = new Carta(cFlip, NUMERO, i);
                        contador++;
                    }
                }
                for (int j = 0; j < 2; j++)
                {
                    tempCaraPrincipal[contador] = new Carta(cCaraPrincipal, SALTO, -1);
                    tempFlip[contador] = new Carta(cFlip, SALTO_TODOS, -1);
                    contador++;
                    tempCaraPrincipal[contador] = new Carta(cCaraPrincipal, REVERSA, -1);
                    tempFlip[contador] = new Carta(cFlip, REVERSA, -1);
                    contador++;
                    tempCaraPrincipal[contador] = new Carta(cCaraPrincipal, MAS_UNO, -1);
                    tempFlip[contador] = new Carta(cFlip, MAS_TRES, -1);
                    contador++;
                    tempCaraPrincipal[contador] = new Carta(cCaraPrincipal, FLIP, -1);
                    tempFlip[contador] = new Carta(cFlip, FLIP, -1);
                    contador++;
                }
            }
            for (int j = 0; j < 4; j++) 
            {
                tempCaraPrincipal[contador] = new Carta(NEGRO, COMODIN_COLOR, -1);
                tempFlip[contador] = new Carta(NEGRO, COLOR_ETERNO, -1);
                contador++;
                tempCaraPrincipal[contador] = new Carta(NEGRO, MAS_CUATRO, -1);
                tempFlip[contador] = new Carta(NEGRO, MAS_SEIS, -1);
                contador++;
                tempCaraPrincipal[contador] = new Carta(NEGRO, MAS_DOS, -1);
                tempFlip[contador] = new Carta(NEGRO, COLOR_ETERNO, -1); 
                contador++;
            }
            for (int j = 0; j < 2; j++)
            {
                tempCaraPrincipal[contador] = new Carta(NEGRO, DESTRUCTORA, -1);
                tempFlip[contador] = new Carta(NEGRO, DESTRUCTORA, -1);
                contador++;
                tempCaraPrincipal[contador] = new Carta(NEGRO, PASAR_EXTREMO, -1);
                tempFlip[contador] = new Carta(NEGRO, PASAR_EXTREMO, -1);
                contador++;
            }
        }

        // Mezcla flip
        for (int i = contador - 1; i > 0; i--) 
        {
            int j = rand() % (i + 1);
            Carta* temp = tempFlip[i];
            tempFlip[i] = tempFlip[j];
            tempFlip[j] = temp;
        }

        for (int i = 0; i < contador; i++)
        {
            tempCaraPrincipal[i]->setLadoOscuro(tempFlip[i]);
            mazo->apilar(tempCaraPrincipal[i]);
        }
    }
    else
    {
        // Generación en modo normal
        for (int m = 0; m < numMazos; m++)
        {
            for (int k = 0; k < 4; k++) 
            {
                Color cCaraPrincipal = coloresCaraPrincipal[k];
                
                tempCaraPrincipal[contador] = new Carta(cCaraPrincipal, NUMERO, 0);
                contador++;
                
                for (int i = 1; i <= 9; i++)
                {
                    for (int j = 0; j < 2; j++)
                    {
                        tempCaraPrincipal[contador] = new Carta(cCaraPrincipal, NUMERO, i);
                        contador++;
                    }
                }
                for (int j = 0; j < 2; j++)
                {
                    tempCaraPrincipal[contador] = new Carta(cCaraPrincipal, SALTO, -1);
                    contador++;
                    tempCaraPrincipal[contador] = new Carta(cCaraPrincipal, REVERSA, -1);
                    contador++;
                    tempCaraPrincipal[contador] = new Carta(cCaraPrincipal, MAS_DOS, -1);
                    contador++;
                }
            }
            for (int j = 0; j < 4; j++) 
            {
                tempCaraPrincipal[contador] = new Carta(NEGRO, COMODIN_COLOR, -1);
                contador++;
                tempCaraPrincipal[contador] = new Carta(NEGRO, MAS_CUATRO, -1);
                contador++;
            }
            for (int j = 0; j < 2; j++)
            {
                tempCaraPrincipal[contador] = new Carta(NEGRO, DESTRUCTORA, -1);
                contador++;
                tempCaraPrincipal[contador] = new Carta(NEGRO, PASAR_EXTREMO, -1);
                contador++;
            }
        }

        // Mezcla de cartas modo normal
        for (int i = 0; i < contador; i++)
        {
            tempCaraPrincipal[i]->setLadoOscuro(nullptr);
            mazo->apilar(tempCaraPrincipal[i]);
        }
    }
    delete[] tempCaraPrincipal;
    delete[] tempFlip;
}

void Juego::repartirCartasIniciales()
{
    cout << "Repartiendo 7 cartas a cada jugador" << endl;
    Nodo<Jugador*>* actual = jugadores->getNodoActual();
    if (!actual) return;
    Nodo<Jugador*>* inicio = actual;
    do 
    {
        for(int i=0; i<7; i++)
        {
            if(!mazo->estaVacia())
            {
                Carta* c = mazo->desapilar();
                actual->dato->robarCarta(c);
            }
        }
        actual = actual->siguiente;
    } while (actual != inicio);
}

int Juego::getCartasEnMazo() { return mazo->getTamaño(); }
int Juego::getCartasEnDescarte() { return descarte->getTamaño(); }
int Juego::getCantidadJugadores() { return jugadores->getTamaño(); }
bool Juego::getLadoFlipActivo() { return ladoFlipActivo; }
ListaCircular<Jugador*>* Juego::getJugadores() { return jugadores; }