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
    cartasAcumuladas = 0;           
    tipoAcumulado = NUMERO;
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
        Jugador* ganador = verificarGanador();
        if (ganador != nullptr)
        {
            limpiarPantalla();
            cout << " ¡¡¡" << ganador->getNombre() << " es el Ganador!!! " << endl;
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

Jugador* Juego::verificarGanador()
{
    Nodo<Jugador*>* actual = jugadores->getNodoActual();
    if (actual == nullptr) return nullptr;
    Nodo<Jugador*>* inicio = actual;
    do 
    {
        if (actual->dato->cantidadCartas() == 0)
        {
            return actual->dato;
        }
        actual = actual->siguiente;
    }
    while (actual != inicio);
    return nullptr;
}

Color Juego::pedirColorUsuario()
{
    int opcion = 0;
    while (true) 
    {
        if (!ladoFlipActivo) 
        {
            cout << "-> Elige el nuevo color: 1.ROJO  2.AZUL  3.VERDE  4.AMARILLO: ";
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
    cout << "-----------------------------------------------------" << endl;
    cout << " |MAZO|: " << mazo->getTamaño() << " CARTAS " << "|DESCARTE: " << topeVisual->toString() << "|" << endl;
    cout << "-----------------------------------------------------" << endl;
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
    string pausa = "";
    limpiarPantalla();
    cout << "----------------------------------------------" << endl << endl << endl;
    cout << "            TURNO DE: " << actual->getNombre() << endl << endl << endl;
    cout << "----------------------------------------------" << endl << endl;
    cout << "Asegurate de que los demas no miren la pantalla" << endl;
    cout << " -> Presiona ENTER para ver tus cartas.";
    getline(cin, pausa);
    while (!turnoTerminado)
    {
        actual->ordenarMano(ladoFlipActivo);
        Carta* topeFisico = descarte->verTope();
        Carta* topeVisual = ladoFlipActivo ? topeFisico->getLadoOscuro() : topeFisico;
        limpiarPantalla();
        mostrarMesa(actual, topeVisual);
        int opcion = 0;
        if (cartasAcumuladas > 0)
        {
            cout << " Tienes " << cartasAcumuladas << " cartas acumuladas para robar del mazo." << endl;
            cout << " Elige tirar una carta igual o mayor para acumular más, o escribe (-1) para aceptar tu triste destino." << endl;
        }
        cout << "-> Elige una carta por su indice o escribe (-1) para robar: ";
        if (!(cin >> opcion))
        {
            cin.clear();
            string sueltos; 
            getline(cin, sueltos);
            continue;
        }
        if (opcion == -1)
        {
            if (cartasAcumuladas > 0)
            {
                cout << " Has aceptado tu destino. Tomas " << cartasAcumuladas << " cartas." << endl;
                for(int i=0; i<cartasAcumuladas; i++)
                {
                    if (mazo->estaVacia()) reponerMazo();
                    if (!mazo->estaVacia()) actual->robarCarta(mazo->desapilar());
                }
                cartasAcumuladas = 0;
                tipoAcumulado = NUMERO;
                turnoTerminado = true;
                continue;
            }
            bool cartaEncontrada = false;
            while (!cartaEncontrada)
            {
                if (mazo->estaVacia()) reponerMazo();
                if (mazo->estaVacia())
                {
                    cout << " !!!El mazo esta vacio¡¡¡ No se puede reponer, pasas turno." << endl;
                    turnoTerminado = true;
                    break;
                }
                Carta* robada = mazo->desapilar();
                actual->robarCarta(robada);
                Carta* robadaVisual = ladoFlipActivo ? robada->getLadoOscuro() : robada;
                cout << " Tomaste una carta: " << robadaVisual->toString() << endl;
                if (robadaVisual->esCompatible(topeVisual))
                {
                    int indiceRecienRobada = actual->cantidadCartas() - 1;
                    actual->jugarCarta(indiceRecienRobada);
                    descarte->apilar(robada);
                    cout << "-> Jugaste la carta compatible: " << robadaVisual->toString() << endl;
                    if (reglas.gritoUno && actual->cantidadCartas() == 1)
                    {
                        cout << "¡¡¡TE QUEDA 1 CARTA!!! Escribe 'UNO' para evitar el castigo: ";
                        string grito;
                        cin >> grito;
                        if (grito != "UNO")
                        {
                            cout << " No gritaste UNO correctamente: " << grito << ". Robas +2 de castigo." << endl;
                            for(int i=0; i<2; i++)
                            {
                                if(mazo->estaVacia()) reponerMazo();
                                if(!mazo->estaVacia()) actual->robarCarta(mazo->desapilar());
                            }
                        }
                        else
                        {
                            cout << " ¡Gritaste UNO!" << endl;
                        }
                    }
                    if (robadaVisual->getColor() == NEGRO)
                    {
                        Color nuevoColor = pedirColorUsuario();
                        robada->setColor(nuevoColor); 
                        if(ladoFlipActivo) robadaVisual->setColor(nuevoColor);
                        cout << "-> COLOR CAMBIADO A: " << nombreColor(nuevoColor) << endl;
                    }
                    aplicarCartaEspecial(robadaVisual, false);
                    cartaEncontrada = true;
                }
                else
                {
                    if (reglas.roboHastaJugar)
                    {
                        cout << " Esta carta no se puede jugar, toma otra." << endl;
                        int opcion = 0;
                        while (opcion != -1)
                        {
                            cout << " -> Escribe (-1) para robar otra: ";
                            if (!(cin >> opcion))
                            {
                                cin.clear();
                                string sueltos;
                                getline(cin, sueltos);
                            }
                        }
                    }
                    else
                    {
                        cartaEncontrada = true; 
                    }
                }
            }
            turnoTerminado = true;
        }
        else if (opcion >= 0 && opcion < actual->cantidadCartas())
        {
            Carta* cartaFisica = actual->getMano()->obtenerPorIndice(opcion);
            Carta* cartaVisual = ladoFlipActivo ? cartaFisica->getLadoOscuro() : cartaFisica;
            if (!reglas.ganarConNegra && actual->cantidadCartas() == 1 && cartaVisual->getColor() == NEGRO) 
            {
                cout << " !!!No puedes ganar lanzando un comodin como ultima carta." << endl;
                cout << " Presiona Enter."; cin.ignore(); cin.get();
                continue;
            }
            if (cartasAcumuladas > 0)
            {
                int valorNueva = 0;
                if (cartaVisual->getTipo() == MAS_UNO) valorNueva = 1;
                else if (cartaVisual->getTipo() == MAS_DOS) valorNueva = 2;
                else if (cartaVisual->getTipo() == MAS_TRES) valorNueva = 3;
                else if (cartaVisual->getTipo() == MAS_CUATRO) valorNueva = 4;
                else if (cartaVisual->getTipo() == MAS_SEIS) valorNueva = 6;
                int valorActual = 0;
                if (tipoAcumulado == MAS_UNO) valorActual = 1;
                else if (tipoAcumulado == MAS_DOS) valorActual = 2;
                else if (tipoAcumulado == MAS_TRES) valorActual = 3;
                else if (tipoAcumulado == MAS_CUATRO) valorActual = 4;
                else if (tipoAcumulado == MAS_SEIS) valorActual = 6;
                if (valorNueva == 0 || valorNueva < valorActual)
                {
                    cout << " !!!Movimiento Invalido. Para acumular debes tirar una carta de castigo IGUAL o MAYOR a +" << valorActual << endl;
                    cout << "Presiona Enter.";
                    cin.ignore();
                    cin.get();
                    continue;
                }
                // Si la carta es válida, se juega
                actual->jugarCarta(opcion);
                descarte->apilar(cartaFisica);
                if (reglas.gritoUno && actual->cantidadCartas() == 1)
                {
                    cout << "¡¡¡TE QUEDA 1 CARTA!!! Escribe 'UNO' para evitar el castigo: ";
                    string grito;
                    cin >> grito;
                    if (grito != "UNO")
                    {
                        cout << " No gritaste UNO correctamente: " << grito << ". Robas +2 de castigo." << endl;
                        for(int i=0; i<2; i++)
                        {
                            if(mazo->estaVacia()) reponerMazo();
                            if(!mazo->estaVacia()) actual->robarCarta(mazo->desapilar());
                        }
                    }
                    else cout << " ¡Gritaste UNO!" << endl;
                }
                if (cartaVisual->getColor() == NEGRO)
                {
                    Color nuevoColor = pedirColorUsuario();
                    cartaFisica->setColor(nuevoColor);
                    if(ladoFlipActivo) cartaVisual->setColor(nuevoColor);
                    cout << "-> COLOR CAMBIADO A: " << nombreColor(nuevoColor) << endl;
                }
                aplicarCartaEspecial(cartaVisual, false);
                turnoTerminado = true;
            }
            else
            {
                if (cartaVisual->esCompatible(topeVisual))
                {
                    bool teniaOpcion = false;
                    for(int i = 0; i < actual->cantidadCartas(); i++)
                    {
                        if (i == opcion) continue;
                        Carta* cartaEnMano = actual->getMano()->obtenerPorIndice(i);
                        Carta* cartaVisible = ladoFlipActivo ? cartaEnMano->getLadoOscuro() : cartaEnMano;
                        if (cartaVisible->getColor() == topeVisual->getColor() || (cartaVisible->getTipo() == NUMERO && topeVisual->getTipo() == NUMERO && cartaVisible->getValor() == topeVisual->getValor()))
                        {
                            teniaOpcion = true;
                            break;
                        }
                    }
                    actual->jugarCarta(opcion);
                    descarte->apilar(cartaFisica);
                    cout << "-> Jugaste: " << cartaVisual->toString() << endl;
                    if (reglas.gritoUno && actual->cantidadCartas() == 1)
                    {
                        cout << "¡¡¡TE QUEDA 1 CARTA!!! Escribe 'UNO' para evitar el castigo: ";
                        string grito;
                        cin >> grito;
                        if (grito != "UNO")
                        {
                            cout << " No gritaste UNO correctamente: " << grito << ". Robas +2 de castigo." << endl;
                            for(int i=0; i<2; i++)
                            {
                                if(mazo->estaVacia()) reponerMazo();
                                if(!mazo->estaVacia()) actual->robarCarta(mazo->desapilar());
                            }
                        }
                        else cout << " ¡Gritaste UNO!" << endl;
                    }
                    if (cartaVisual->getColor() == NEGRO)
                    {
                        Color nuevoColor = pedirColorUsuario();
                        cartaFisica->setColor(nuevoColor);
                        if(ladoFlipActivo) cartaVisual->setColor(nuevoColor);
                        cout << "-> COLOR CAMBIADO A: " << nombreColor(nuevoColor) << endl;
                    }
                    aplicarCartaEspecial(cartaVisual, teniaOpcion);
                    turnoTerminado = true;
                }
                else
                {
                    cout << " !!!Movimiento Invalido. No coincide con " << topeVisual->toString() << endl;
                    cout << "Presiona Enter.";
                    cin.ignore(); cin.get();
                }
            }
        }
        else
        {
            cout << "Indice incorrecto." << endl;
        }
    }
}

void Juego::aplicarCartaEspecial(Carta* cartaJugada, bool teniaOpcionDeCarta)
{
    TipoCarta tipo = cartaJugada->getTipo();
    if (tipo == REVERSA)
    {
        direccionDerecha = !direccionDerecha;
        cout << " ¡Cambio de sentido! Ahora el sentido es hacia la " << (direccionDerecha ? "DERECHA" : "IZQUIERDA") << endl;
        if (jugadores->getTamaño() == 2)
        {
            cout << " ¡Repites turno!" << endl;
            if (direccionDerecha) jugadores->siguiente();
            else jugadores->anterior();
        }
    }
    else if (tipo == SALTO)
    {
        cout << " ¡Salto de turno!" << endl;
        if (direccionDerecha) jugadores->siguiente();
        else jugadores->anterior();
        if (jugadores->getTamaño() == 2)
        {
            cout << " ¡Repites turno!" << endl;
        }
    }
    else if (tipo == SALTO_TODOS)
    {
        cout << " ¡SALTO A TODOS! Repites turno." << endl;
        if (direccionDerecha) jugadores->anterior();
        else jugadores->siguiente();
    }
    else if (tipo == FLIP)
    {
        ladoFlipActivo = !ladoFlipActivo;
        cout << " ¡¡¡CAMBIO DE LADO!!! CAMBIANDO AL " << (ladoFlipActivo ? "LADO OSCURO" : "LADO CLARO") << endl;
    }
    else if (tipo == MAS_UNO || tipo == MAS_DOS || tipo == MAS_CUATRO || tipo == MAS_TRES || tipo == MAS_SEIS)
    {
        int cantidad = 0;
        if (tipo == MAS_UNO) cantidad = 1;
        else if (tipo == MAS_DOS) cantidad = 2;
        else if (tipo == MAS_TRES) cantidad = 3;
        else if (tipo == MAS_CUATRO) cantidad = 4;
        else if (tipo == MAS_SEIS) cantidad = 6;
        if ((tipo == MAS_CUATRO || tipo == MAS_SEIS) && reglas.retoMasCuatro && cartasAcumuladas == 0) 
        {
            Jugador* victima = (direccionDerecha) ? jugadores->getNodoActual()->siguiente->dato : jugadores->getNodoActual()->anterior->dato;
            cout << victima->getNombre() << " te han lanzado un +" << cantidad << "." << endl;
            cout << " ¿Deseas RETAR al rival? (Si pierdes tendras que tomar +2 cartas extra) (s/n): ";
            char resp;
            cin >> resp;
            if (resp == 's' || resp == 'S') 
            {
                if (teniaOpcionDeCarta)
                {
                    cout << " ¡RETO GANADO! El rival tenia cartas jugables, ahora el toma el castigo y roba " << cantidad << " cartas." << endl;
                    Jugador* lanzador = jugadores->obtenerActual();
                    for(int i=0; i<cantidad; i++)
                    {
                        if(!mazo->estaVacia()) lanzador->robarCarta(mazo->desapilar());
                    }
                    return;
                }
                else
                {
                    cout << " ¡RETO PERDIDO! El rival tiro legal. " << victima->getNombre() << " roba " << cantidad + 2 << " y pierde turno." << endl;
                    if (direccionDerecha) jugadores->siguiente();
                    else jugadores->anterior();
                    for(int i=0; i<cantidad+2; i++)
                    {
                        if(!mazo->estaVacia()) victima->robarCarta(mazo->desapilar());
                    }
                    return;
                }
            }
        }
        
        if (reglas.acumulacion && (tipo == MAS_DOS || tipo == MAS_CUATRO || tipo == MAS_SEIS || tipo == MAS_TRES || tipo == MAS_UNO)) 
        {
            cartasAcumuladas += cantidad;
            tipoAcumulado = tipo;
            cout << " ¡Se han acumulado " << cartasAcumuladas << " cartas para el siguiente jugador!" << endl;
        }
        else 
        {
            if (direccionDerecha) jugadores->siguiente(); else jugadores->anterior();
            Jugador* victima = jugadores->obtenerActual();
            cout << victima->getNombre() << " ¡¡¡PIERDE TURNO Y ROBA!!! Total: " << cantidad << endl;
            for(int i = 0; i < cantidad; i++)
            {
                if (mazo->estaVacia()) reponerMazo();
                if (!mazo->estaVacia()) victima->robarCarta(mazo->desapilar());
            }
        }
    }
    else if (tipo == DESTRUCTORA)
    {
        Jugador* actual = jugadores->obtenerActual();
        if (actual->cantidadCartas() == 0)
        {
            cout << "¡¡¡DESTRUCTORA!!! Pero ya no tienes mas cartas en tu mano para destruir." << endl;
            return;
        }
        cout << "¡¡¡DESTRUCTORA USADA!!!" << endl;
        cout << "Elige el indice de una carta de tu mano para destruir." << endl;
        cout << "Se destruira de tu mano y todas las copias exactas de las manos de los demas jugadores." << endl;
        int indiceSacrificio;
        Carta* cartaSacrificada = nullptr;
        while (true)
        {
            cout << " -> Indice a destruir: ";
            if (!(cin >> indiceSacrificio)) 
            {
                cin.clear();
                string sueltas;
                getline(cin, sueltas);
                continue;
            }
            if (indiceSacrificio >= 0 && indiceSacrificio < actual->cantidadCartas())
            {
                cartaSacrificada = actual->getMano()->obtenerPorIndice(indiceSacrificio);
                break;
            }
            else
            {
                cout << " Indice invalido, revisa tu mano." << endl;
            }
        }
        Color colObjetivo = cartaSacrificada->getColor();
        TipoCarta tipObjetivo = cartaSacrificada->getTipo();
        int valObjetivo = cartaSacrificada->getValor();
        Carta* visualSacrificada = ladoFlipActivo ? cartaSacrificada->getLadoOscuro() : cartaSacrificada;
        cout << " ¡Has destruido " << visualSacrificada->toString() << "! Destruyendo copias en todos los mazos..." << endl;
        Nodo<Jugador*>* inicio = jugadores->getNodoActual();
        Nodo<Jugador*>* jugadorRevisado = inicio;
        do
        {
            int destruidas = jugadorRevisado->dato->aplicarDestruccionExacta(colObjetivo, tipObjetivo, valObjetivo);
            if (destruidas > 0)
            {
                cout << "  -> ¡" << jugadorRevisado->dato->getNombre() << " ha perdido " << destruidas << " cartas!" << endl;
            }
            jugadorRevisado = jugadorRevisado->siguiente;
        }
        while (jugadorRevisado != inicio);
    }
    else if (tipo == PASAR_EXTREMO)
    {
        cout << "¡¡¡PASAR EXTREMO!!! Todos deben pasar su ultima carta al siguiente jugador." << endl;
        int n = jugadores->getTamaño();
        Carta** cartasTemp = new Carta*[n];
        Nodo<Jugador*>** ordenJugadores = new Nodo<Jugador*>*[n];
        Nodo<Jugador*>* jugadorRevisado = jugadores->getNodoActual();
        for (int i = 0; i < n; i++) 
        {
            ordenJugadores[i] = jugadorRevisado;
            int cantidad = jugadorRevisado->dato->cantidadCartas();
            if (cantidad > 0)
            {
                cartasTemp[i] = jugadorRevisado->dato->jugarCarta(cantidad - 1);
            }
            else
            {
                cartasTemp[i] = nullptr;
            }
            if (direccionDerecha) jugadorRevisado = jugadorRevisado->siguiente;
            else jugadorRevisado = jugadorRevisado->anterior;
        }
        for (int i = 0; i < n; i++) 
        {
            if (cartasTemp[i] != nullptr) 
            {
                int indiceReceptor = (i + 1) % n; 
                ordenJugadores[indiceReceptor]->dato->robarCarta(cartasTemp[i]);
                Carta* cartaVisual = ladoFlipActivo ? cartasTemp[i]->getLadoOscuro() : cartasTemp[i];
                cout << "  -> " << ordenJugadores[i]->dato->getNombre() << " le paso la ultima carta " << cartaVisual->toString() << " a " << ordenJugadores[indiceReceptor]->dato->getNombre() << "." << endl;
            }
        }
        delete[] cartasTemp;
        delete[] ordenJugadores;
    }
    else if (tipo == COLOR_ETERNO)
    {
        if (direccionDerecha) jugadores->siguiente();
        else jugadores->anterior();
        Jugador* victima = jugadores->obtenerActual();
        Color objetivo = cartaJugada->getColor(); 
        cout << " ¡¡¡COLOR ETERNO!!! " << victima->getNombre() << " roba hasta encontrar una carta color: " << nombreColor(objetivo) << "." << endl;
        bool encontrado = false;
        while (!encontrado)
        {
            if (mazo->estaVacia()) reponerMazo();
            if (mazo->estaVacia()) break; 
            Carta* c = mazo->desapilar();
            victima->robarCarta(c);
            Carta* visualC = ladoFlipActivo ? c->getLadoOscuro() : c;
            cout << " -> Robo: " << visualC->toString() << endl;
            if (visualC->getColor() == objetivo)
            {
                encontrado = true;
                cout << " ¡Encontrado! Deja de tomar cartas." << endl;
            }
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
    cout << "       INICIANDO JUEGO" << endl;
    cout << " Jugadores: " << numJugadores << " | Mazos generados: " << numMazos << endl;
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
        Carta* primeraCarta = mazo->desapilar();
        descarte->apilar(primeraCarta);
        
        Carta* visual = ladoFlipActivo ? primeraCarta->getLadoOscuro() : primeraCarta;
        if (visual->getColor() == NEGRO)
        {
            cout << "-> Primera carta es un comodin, " << jugadores->obtenerActual()->getNombre() << " tiene el privilegio de elegir el color inicial." << endl;
            Color nuevoColor = pedirColorUsuario();
            primeraCarta->setColor(nuevoColor);
            if (ladoFlipActivo) visual->setColor(nuevoColor);
            string sueltos;
            getline(cin, sueltos);
        }
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
                tempCaraPrincipal[contador] = new Carta(NEGRO, MAS_DOS, -1);
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
        for(int i = 0; i < 7; i++)
        {
            if(!mazo->estaVacia())
            {
                Carta* c = mazo->desapilar();
                actual->dato->robarCarta(c);
            }
        }
        actual = actual->siguiente;
    }
    while (actual != inicio);
}

int Juego::getCartasEnMazo() { return mazo->getTamaño(); }
int Juego::getCartasEnDescarte() { return descarte->getTamaño(); }
int Juego::getCantidadJugadores() { return jugadores->getTamaño(); }
bool Juego::getLadoFlipActivo() { return ladoFlipActivo; }
ListaCircular<Jugador*>* Juego::getJugadores() { return jugadores; }