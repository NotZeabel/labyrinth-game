#ifndef GAME_H
#define GAME_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>

#include "Laberinto.h"
#include "LaberintoProfundidad.h"
#include "DLinkedList.h"
#include "LaberintoExpansionPrim.h"
#include "LaberintoExpansionKruskal.h"
#include "Jugador.h"


class Game{


private:
    int width;
    int height;
    float CELL_SIZE;
    sf::RenderWindow* window;
    Laberinto* laberinto;
    Jugador* jugador;
    sf::RectangleShape salida;
    DLinkedList<sf::CircleShape>* manzanas;
    sf::Clock clock;
    sf::Time tiempoJuego;
    sf::Font font;
    sf::Text textoTiempo;
    sf::Text textoManzanas;
    sf::Text textoLaberinto;
    sf::Text textoNivel;


public:
    Game(int width, int height,float cellSize, int laberintoTipo, sf::Time tiempo,int manzanasCantidad, int animacion, int nivel, sf::RenderWindow* window){
        this->width = width;
        this->height = height;
        this->window = window;
        CELL_SIZE = cellSize;
        tiempoJuego = tiempo;

        switch(laberintoTipo){
            case 0:
                laberinto = new LaberintoProfundidad(height,width);
                break;
            case 1:
                laberinto = new LaberintoExpansionPrim(height,width);
                break;
            case 2:
                laberinto = new LaberintoExpansionKruskal(height,width);
                break;
            default:
                laberinto = new LaberintoExpansionKruskal(height,width);
                break;

        }

        salida = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        salida.setFillColor(sf::Color::Cyan);
        salida.setPosition(CELL_SIZE*(width-1)*2, CELL_SIZE*(height-1)*2);

        manzanas = new DLinkedList<sf::CircleShape>;
        for(int i=0; i<manzanasCantidad; i++){
            sf::CircleShape manzana = sf::CircleShape(CELL_SIZE/2);
            manzana.setFillColor(sf::Color::Red);
            manzana.setPosition(CELL_SIZE*(rand()%(width-1))*2, CELL_SIZE*(rand()%(height-1))*2);
            manzanas->append(manzana);
        }

        jugador = new Jugador(0, 0, CELL_SIZE, laberinto);

        if (!font.loadFromFile("fuente/04B_30__.ttf")){
            throw new runtime_error("Archivo fuente no encontrado");
        }
        textoTiempo.setPosition(16, 416);
        textoTiempo.setFont(font);
        textoTiempo.setCharacterSize(24);
        textoTiempo.setFillColor(sf::Color::Red);

        textoManzanas.setPosition(512, 416);
        textoManzanas.setFont(font);
        textoManzanas.setCharacterSize(24);
        textoManzanas.setFillColor(sf::Color::Red);

        textoNivel.setPosition(264, 416);
        textoNivel.setFont(font);
        textoNivel.setCharacterSize(24);
        textoNivel.setFillColor(sf::Color::Red);
        textoNivel.setString("LEVEL " + to_string(nivel));

        textoLaberinto.setPosition(-300, (height/2)*CELL_SIZE*2);
        textoLaberinto.setFont(font);
        textoLaberinto.setCharacterSize(36);
        textoLaberinto.setFillColor(sf::Color::Red);
        switch(animacion){
            case 1:
                textoLaberinto.setString("APPLES++");
                break;
            case 2:
                textoLaberinto.setString("TIME--");
                break;
            default:
                textoLaberinto.setString("LABYRINTH!");
                break;
        }
    }
    virtual ~Game() {
        delete laberinto;
        delete jugador;
        delete manzanas;
    }


    sf::Time runGame(){
        //Funcion que actualiza y dibuja el juego en pantalla
        //Recibe y controla eventos de teclado
        //Retorna la cantidad de tiempo restante cuando el jugador termina el nivel
        DLinkedList<int>* dijkstra = new DLinkedList<int>();
        bool dFlag = false;

        while(window->isOpen()){

            //Eventos
            ////////////
            sf::Event event;

            while(window->pollEvent(event)){
                if(event.type == sf::Event::Closed){
                    window->close();
                }
                if (event.type == sf::Event::KeyPressed){
                    dFlag = false;
                    if(event.key.scancode == sf::Keyboard::W){
                        jugador->moveUp();
                    }
                    if(event.key.scancode == sf::Keyboard::A){
                        jugador->moveLeft();
                    }
                    if(event.key.scancode == sf::Keyboard::S){
                        jugador->moveDown();
                    }
                    if(event.key.scancode == sf::Keyboard::D){
                        jugador->moveRight();
                    }
                    if(event.key.scancode == sf::Keyboard::M) {
                        int pos = laberinto->getPos(jugador->getY(), jugador->getX());
                        dijkstra = laberinto->dijkstra(pos);
                        dFlag=true;
                        tiempoJuego -= sf::seconds((tiempoJuego.asSeconds() * 0.1));
                    }
                }
            }

            //Toco manzana
            if(!manzanas->isEmpty()){
                manzanas->goToStart();
                while(!manzanas->atEnd()){
                    sf::CircleShape r = manzanas->getElement();
                    if(r.getGlobalBounds().intersects(jugador->getApariencia().getGlobalBounds())){
                        manzanas->remove();
                        break;
                    }
                    manzanas->next();
                }
            }else{
                salida.setFillColor(sf::Color::Green);
            }

            //Actualizar texto
            int i = round(tiempoJuego.asSeconds()-clock.getElapsedTime().asSeconds());
            textoTiempo.setString("TIME: " + to_string(i));
            textoManzanas.setString("APPLES: " + to_string(manzanas->getSize()));
            if(clock.getElapsedTime().asSeconds()<10) updateAnimacion();


            //Draw
            /////////
            window->clear(sf::Color::Black);
            dibujarNodos();
            dibujarArcos();
            if(dFlag){
                dibujarDijkstra(window, dijkstra);
            }
            dibujarManzanas();
            window->draw(salida);
            window->draw(jugador->getApariencia());
            window->draw(textoTiempo);
            window->draw(textoManzanas);
            window->draw(textoLaberinto);
            window->draw(textoNivel);
            window->display();


            //Final
            //Legó al final
            if(manzanas->isEmpty() && jugador->getX() == width-1 && jugador->getY() == height-1){
                break;
            }
            //Se acabó el tiempo
            if(tiempoJuego.asSeconds()-clock.getElapsedTime().asSeconds() < 0){
                break;
            }
        }
        return tiempoJuego-clock.getElapsedTime();
    }



private:

    void dibujarArco(int row1, int col1, int row2, int col2, sf::Color color){
        //Metodo que dibuja un arco del laberinto en pantalla
        //Recibe la posicion del arco y el color con el que se dibuja
        sf::RectangleShape rect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        rect.setFillColor(color);
        int r = 0;
        int c = 0;

        if(row2<row1){
            r=-CELL_SIZE;
        }
        if(row2>row1){
            r=CELL_SIZE;
        }
        if(col2<col1){
            c =-CELL_SIZE;
        }
        if(col2>col1){
            c= +CELL_SIZE;
        }
        rect.setPosition(CELL_SIZE*col1*2+c, CELL_SIZE*row1*2+r);
        window->draw(rect);
    }



    void dibujarNodo(int row, int col, sf::Color color){
        //Metodo que dibuja un nodo del laberinto en pantalla
        //Recibe la posicion del nodo y el color con el que se dibuja
        sf::RectangleShape rect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        rect.setFillColor(color);
        rect.setPosition(CELL_SIZE*col*2, CELL_SIZE*row*2);
        window->draw(rect);
    }


    void dibujarNodos(){
        //Metodo que dibuja los nodos del laberinto
        //Se basa en la cantidad de filas y columnas del laberinto
        for(int r=0; r<laberinto->getRows(); r++){
            for(int c=0; c<laberinto->getCols(); c++){
                dibujarNodo(r, c, sf::Color::Blue);
            }
        }
    }

    void dibujarDijkstra(sf::RenderWindow* window, DLinkedList<int>* ruta){
        //Metodo que dibuja en pantalla los  nodos y arcos que forman parte de
        // la lista de nodos recibida por parametro.
        ruta->goToStart();
        while(!ruta->atEnd()){
            int nodo = ruta->getElement();
            int nodoRow = laberinto->nodoToRow(nodo);
            int nodoCol = laberinto->nodoToCol(nodo);
            dibujarNodo(nodoRow, nodoCol, sf::Color::Yellow);
            if(!ruta->atStart()){
                ruta->previous();
                int nodo2 = ruta->getElement();
                int nodoRow2 = laberinto->nodoToRow(nodo2);
                int nodoCol2 = laberinto->nodoToCol(nodo2);
                dibujarArco(nodoRow2, nodoCol2, nodoRow, nodoCol, sf::Color::Yellow);
                ruta->next();
            }
            ruta->next();
        }
    }

    void dibujarArcos(){
        //Metodo que dibuja en pantalla los arcos del laberinto
        for(int r=0; r<laberinto->getRows(); r++){
            for(int c=0; c<laberinto->getCols(); c++){
                DLinkedList<int>* lista = laberinto->getNodo(r,c);
                lista->goToStart();
                while(!lista->atEnd()){
                    int nodo = lista->getElement();
                    int nodoRow = laberinto->nodoToRow(nodo);
                    int nodoCol = laberinto->nodoToCol(nodo);
                    dibujarArco(r, c,nodoRow, nodoCol, sf::Color::Blue);
                    lista->next();
                }
            }
        }
    }


    void dibujarManzanas(){
        //Metodo que dibuja en pantalla las manzanas
        manzanas->goToStart();
        while(!manzanas->atEnd()){
            sf::CircleShape r = manzanas->getElement();
            window->draw(r);
            manzanas->next();
        }
    }

    void updateAnimacion(){
        //Metodo que actualiza la posicion del textoLaberinto
        textoLaberinto.setPosition(textoLaberinto.getPosition().x+5, (height/2)*CELL_SIZE*2);
    }
};

#endif // GAME_H
