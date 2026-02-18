#include "../include/Juego.h"
#include <cstdlib>
#include <ctime>
#include <string>

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

void Juego::flujoPrincipal() 
{
    bool juegoTerminado = false;
    
    while (!juegoTerminado)
    {
        gestionarTurno();

        if (verificarGanador())
        {
            limpiarPantalla();
            cout << " ¡¡¡" << jugadores->obtenerActual()->getNombre() << " es el Ganador!!! " << endl;
            juegoTerminado = true;
        }
        else
        {
            if (direccionDerecha) jugadores->siguiente();
            else jugadores->anterior();
            cout << "Presiona ENTER para terminar tu turno.";
            string sueltos; 
            getline(cin, sueltos);
            cin.get(); 
        }
    }
}

bool Juego::verificarGanador()
{
    return jugadores->obtenerActual()->cantidadCartas() == 0;
}

Color Juego::pedirColorUsuario()
{
    int opcion = 0;
    while (true) 
    {
        if (!ladoFlipActivo) 
        {
            cout << "Elige el nuevo color: 1.ROJO  2.AZUL  3.VERDE  4.AMARILLO: ";
            if (!(cin >> opcion))
            {
                cin.clear();
                string sueltos;
                getline(cin, sueltos);
                continue;
            }
            switch(opcion)
            {
                case 1: return ROJO;
                case 2: return AZUL;
                case 3: return VERDE;
                case 4: return AMARILLO;
                default: cout << "Opcion invalida." << endl;
            }
        }
        else 
        {
            cout << "Elige el nuevo color: 1.ROSA  2.TURQUESA  3.NARANJA  4.VIOLETA: ";
            if (!(cin >> opcion))
            {
                cin.clear();
                string sueltos;
                getline(cin, sueltos);
                continue;
            }
            switch(opcion)
            {
                case 1: return ROSA;
                case 2: return TURQUESA;
                case 3: return NARANJA;
                case 4: return VIOLETA;
                default: cout << "Opcion invalida." << endl;
            }
        }
    }
}

void Juego::mostrarMesa(Jugador* actual, Carta* topeVisual)
{
    cout << " MODO: " << (ladoFlipActivo ? "LADO OSCURO (FLIP)" : "LADO CLARO") << endl;
    cout << " ES TURNO DE: " << actual->getNombre() << endl;
    cout << "------------------------------------------" << endl;
    cout << " |MAZO|       |DESCARTE: " << topeVisual->toString() << "|" << endl;
    cout << "------------------------------------------" << endl;
    cout << " TU MANO: " << endl;
    ListaDoble<Carta*>* mano = actual->getMano();
    for(int i = 0; i < mano->getTamaño(); i++)
    {
        Carta* fisica = mano->obtenerPorIndice(i);
        Carta* visual = ladoFlipActivo ? fisica->getLadoOscuro() : fisica;
        if (visual == nullptr) cout << "  |" << i << "| ERROR (Carta Vacia)" << endl;
        else cout << "  |" << i << "| " << visual->toString() << endl;
    }
    cout << "------------------------------------------" << endl;
}

void Juego::limpiarPantalla()
{
    cout << "\033[2J"; // ANSI para limpiar pantalla
}

string nombreColor(Color c)
{
    switch(c)
    {
        case ROJO: return "ROJO";
        case AZUL: return "AZUL";
        case VERDE: return "VERDE";
        case AMARILLO: return "AMARILLO";
        case NEGRO: return "NEGRO";
        case ROSA: return "ROSA";
        case TURQUESA: return "TURQUESA";
        case NARANJA: return "NARANJA";
        case VIOLETA: return "VIOLETA";
        default: return "DESCONOCIDO";
    }
}

void Juego::gestionarTurno()
{
    Jugador* actual = jugadores->obtenerActual();
    bool turnoTerminado = false;
    Carta* topeFisico = descarte->verTope();
    Carta* topeVisual = ladoFlipActivo ? topeFisico->getLadoOscuro() : topeFisico;
    while (!turnoTerminado)
    {
        limpiarPantalla();
        mostrarMesa(actual, topeVisual);
        int opcion;
        cout << "-> Elige una carta por su indice o escribe (-1') para robar: ";
        if (!(cin >> opcion))
        {
            cin.clear();
            string sueltos; 
            getline(cin, sueltos);
            continue;
        }
        if (opcion == -1)
        {
            if (mazo->estaVacia())
            {
                reponerMazo();
            }
            if (mazo->estaVacia())
            {
                cout << " !!!El mazo esta vacio¡¡¡ No se puede reponer, pasas turno." << endl;
                turnoTerminado = true;
            }
            else
            {
                Carta* robada = mazo->desapilar();
                actual->robarCarta(robada);
                Carta* robadaVisual = ladoFlipActivo ? robada->getLadoOscuro() : robada;
                cout << " Tomaste una carta: " << robadaVisual->toString() << endl;
                if (robadaVisual->esCompatible(topeVisual))
                {
                    cout << " Puedes jugar la carta robada ¿Quieres jugarla de una vez? (s/n): ";
                    char resp;
                    cin >> resp;
                    if (resp == 's' || resp == 'S')
                    {
                        int indiceRecienRobada = actual->cantidadCartas() - 1;
                        actual->jugarCarta(indiceRecienRobada);
                        descarte->apilar(robada);
                        cout << "-> Jugaste la carta robada: " << robadaVisual->toString() << endl;
                        if (robadaVisual->getColor() == NEGRO)
                        {
                            Color nuevoColor = pedirColorUsuario();
                            robada->setColor(nuevoColor); 
                            if(ladoFlipActivo) robadaVisual->setColor(nuevoColor);
                            cout << "-> COLOR CAMBIADO A: " << nombreColor(nuevoColor) << endl;
                        }
                        if (robadaVisual->getTipo() == FLIP)
                        {
                            ladoFlipActivo = !ladoFlipActivo;
                            cout << "¡¡¡MODO FLIP!!! CAMBIANDO DE LADO " << (ladoFlipActivo ? "OSCURO" : "CLARO") << endl;
                        }
                        if (robadaVisual->getTipo() == REVERSA)
                        {
                            direccionDerecha = !direccionDerecha;
                            cout << "-> Sentido del juego invertido." << endl;
                        }
                    }
                }
                turnoTerminado = true;
            }
        }
        else if (opcion >= 0 && opcion < actual->cantidadCartas())
        {
            Carta* cartaFisica = actual->getMano()->obtenerPorIndice(opcion);
            Carta* cartaVisual = ladoFlipActivo ? cartaFisica->getLadoOscuro() : cartaFisica;
            if (cartaVisual->esCompatible(topeVisual))
            {
                actual->jugarCarta(opcion);
                descarte->apilar(cartaFisica);
                cout << "-> Jugaste: " << cartaVisual->toString() << endl;
                TipoCarta tipoJugado = cartaVisual->getTipo();
                if (cartaVisual->getColor() == NEGRO)
                {
                    Color nuevoColor = pedirColorUsuario();
                    cartaFisica->setColor(nuevoColor);
                    if(ladoFlipActivo) cartaVisual->setColor(nuevoColor);
                    cout << "-> COLOR CAMBIADO A: " << nombreColor(nuevoColor) << endl;
                }
                if (tipoJugado == FLIP)
                {
                    ladoFlipActivo = !ladoFlipActivo;
                    cout << "¡¡¡MODO FLIP!!! CAMBIANDO DE LADO " << (ladoFlipActivo ? "OSCURO" : "CLARO") << endl;
                }
                if (tipoJugado == REVERSA)
                {
                    direccionDerecha = !direccionDerecha;
                    cout << "-> Sentido de juego invertido." << endl;
                }
                turnoTerminado = true;
            }
            else
            {
                cout << " !!!Movimiento Invalido. No coincide con " << topeVisual->toString() << endl;
                cout << "Presiona Enter.";
                cin.ignore(); cin.get();
            }
        }
        else
        {
            cout << "Indice incorrecto." << endl;
        }
    }
}

void Juego::reponerMazo()
{
    if (descarte->getTamaño() <= 1) 
    {
        cout << " No hay suficientes cartas para reponer el mazo!" << endl;
        return;
    }

    cout << " ¡¡¡SE ACABO EL MAZO!!! Barajando descarte para reponer" << endl;
    Carta* topeVisible = descarte->desapilar();
    while (!descarte->estaVacia())
    {
        mazo->apilar(descarte->desapilar());
    }
    descarte->apilar(topeVisible);
    mazo->barajar();
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