#ifndef LABERINTOPROFUNDIDAD_H
#define LABERINTOPROFUNDIDAD_H

#include <utility>
#include <time.h>
#include "DLinkedList.h"
#include "LinkedStack.h"
#include "Laberinto.h"

using namespace std;

class LaberintoProfundidad : public Laberinto{


public:
    LaberintoProfundidad(int rows, int cols){

        srand(time(0));
        this->rows = rows;
        this->cols = cols;
        this->size = rows*cols;
        laberinto = new DLinkedList<int>[size];
        DLinkedList<int>* grafoInicial = new DLinkedList<int>[size];
        inicializarArcos(grafoInicial);
        generarLaberinto(grafoInicial);
        agregarArcosExtra(grafoInicial);
        delete [] grafoInicial;
    }

    virtual ~LaberintoProfundidad() {
        delete [] laberinto;
    }


    void generarLaberinto(DLinkedList<int>* grafo){
        //Metodo que genera un arbol de expansion a partir de un grafo inicial
        //Usa el algoritmo de busqueda a profundidad y escoge los nodos que visita de forma aleatoria.
        //Utiliza pilas
        LinkedStack<int>* pila = new LinkedStack<int>();
        bool visitados[size];
        for(int i=0; i<size; i++){
            visitados[i]=false;
        }

        int actual = 0;

        do{
            visitados[actual]=true;

            if(grafo[actual].isEmpty()){
                actual = pila->pop();
                continue;
            }

            int vecino;
            int n = grafo[actual].getSize();
            int pos = rand()%n;
            do{
                grafo[actual].goToPos(pos);
                vecino = grafo[actual].getElement();
                pos = (pos+1)%grafo[actual].getSize();
                n--;
            }while(visitados[vecino] && n>0);


            if(visitados[vecino]){
                actual = pila->pop();
                continue;
            }else{
                grafo[actual].remove();
                grafo[vecino].goToStart();
                while(/*!grafo[vecino].isEmpty() &&*/ !grafo[vecino].atEnd()){
                    if(grafo[vecino].getElement() == actual){
                        grafo[vecino].remove();
                        break;
                    }
                    grafo[vecino].next();
                }


                laberinto[actual].append(vecino);
                laberinto[vecino].append(actual);
                pila->push(actual);
                actual = vecino;
            }

        }while(!pila->isEmpty());

        delete pila;
    }



};

#endif // LABERINTOPROFUNDIDAD_H
