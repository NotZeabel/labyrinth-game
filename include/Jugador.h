#ifndef JUGADOR_H
#define JUGADOR_H

#include <SFML/Graphics.hpp>
#include "Laberinto.h"
#include "DLinkedList.h"


class Jugador{

private:
    int posX;
    int posY;
    int cellSize;
    Laberinto* laberinto;
    sf::CircleShape apariencia;

    void updatePos(){
        apariencia.setPosition(cellSize*posX*2, cellSize*posY*2);
    }


public:
    Jugador(int posX, int posY, int cellSize, Laberinto* laberinto) {
        this->posX = posX;
        this->posY = posY;
        this->cellSize = cellSize;
        apariencia = sf::CircleShape(cellSize/2);
        apariencia.setFillColor(sf::Color(100, 250, 50));
        apariencia.setPosition(cellSize*posX*2, cellSize*posY*2);
        this->laberinto = laberinto;
    }
    virtual ~Jugador() {}


    sf::CircleShape getApariencia(){
        return apariencia;
    }

    int getX(){
        return posX;
    }

    int getY(){
        return posY;
    }


    void moveRight(){
        //Metodo que mueve el jugador hacia la derecha
        //Verifica que el laberinto tenga un arco en esa direccion
        DLinkedList<int>* arcos = laberinto->getNodo(posY, posX);
        arcos->goToStart();
        while(!arcos->atEnd()){
            int nodo = arcos->getElement();
            int nodoRow = laberinto->nodoToRow(nodo);
            int nodoCol = laberinto->nodoToCol(nodo);
            if(nodoRow==posY && nodoCol == posX+1){
                posX++;
                updatePos();
                break;
            }
            arcos->next();
        }
    }

    void moveLeft(){
        //Metodo que mueve el jugador hacia la izquierda
        //Verifica que el laberinto tenga un arco en esa direccion
        DLinkedList<int>* arcos = laberinto->getNodo(posY, posX);
        arcos->goToStart();
        while(!arcos->atEnd()){
            int nodo = arcos->getElement();
            int nodoRow = laberinto->nodoToRow(nodo);
            int nodoCol = laberinto->nodoToCol(nodo);
            if(nodoRow==posY && nodoCol == posX-1){
                posX--;
                updatePos();
                break;
            }
            arcos->next();
        }
    }

    void moveUp(){
        //Metodo que mueve el jugador hacia arriba
        //Verifica que el laberinto tenga un arco en esa direccion
        DLinkedList<int>* arcos = laberinto->getNodo(posY, posX);
        arcos->goToStart();
        while(!arcos->atEnd()){
            int nodo = arcos->getElement();
            int nodoRow = laberinto->nodoToRow(nodo);
            int nodoCol = laberinto->nodoToCol(nodo);
            if(nodoRow==posY-1 && nodoCol==posX){
                posY--;
                updatePos();
                break;
            }
            arcos->next();
        }
    }

    void moveDown(){
        //Metodo que mueve el jugador hacia abajo
        //Verifica que el laberinto tenga un arco en esa direccion
        DLinkedList<int>* arcos = laberinto->getNodo(posY, posX);
        arcos->goToStart();
        while(!arcos->atEnd()){
            int nodo = arcos->getElement();
            int nodoRow = laberinto->nodoToRow(nodo);
            int nodoCol = laberinto->nodoToCol(nodo);
            if(nodoRow==posY+1 && nodoCol==posX){
                posY++;
                updatePos();
                break;
            }
            arcos->next();
        }
    }

};

#endif // JUGADOR_H
