#ifndef LABERINTOEXPANSIONKRUSKAL_H
#define LABERINTOEXPANSIONKRUSKAL_H

#include <utility>
#include <time.h>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>
#include "DLinkedList.h"
#include "LinkedStack.h"
#include "Laberinto.h"
#include "KVPair.h"

class LaberintoExpansionKruskal : public Laberinto{
private:
    void randomize(DLinkedList<KVPair<int, int>>* lista) {
        if (size < 2) {
            return;
        }

        vector<KVPair<int, int>> pares;
        lista->goToStart();
        for (int i = 0; i < lista->getSize(); i++) {
            pares.push_back(lista->getElement());
            lista->next();
        }

        shuffle(pares.begin(), pares.end(), default_random_engine(time(0)));

        lista->clear();
        lista->goToStart();
        for (KVPair<int, int> par : pares) {
            lista->append(par);
        }
    }
public:
    LaberintoExpansionKruskal(int rows, int cols) {
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

    virtual ~LaberintoExpansionKruskal() {
        delete [] laberinto;
    }

    void generarLaberinto(DLinkedList<int>* grafo) {
        // Este metodo genera un laberinto por medio de una version
        // modificada del algoritmo de kruskal
        // Toma un grafo con todos los arcos y nodos para generar el laberinto
        DLinkedList<KVPair<int, int>>* arcos = new DLinkedList<KVPair<int, int>>();
        DLinkedList<int>* conjuntos = new DLinkedList<int>[size];
        //Añadir todos los arcos a la lista
        //Crear conjuntos
        for (int i = 0; i < size; i++) {
            grafo[i].goToStart();
            for (int j = 0; j < grafo[i].getSize(); j++) {
                KVPair<int, int> arco(i, grafo[i].getElement());
                arcos->append(arco);
                grafo[i].next();
            }
            conjuntos[i].append(i);
        }
        //Randomizar lista de arcos
        randomize(arcos);

        //Algoritmo miedo
        arcos->goToStart();
        while (!arcos->isEmpty()) {
            KVPair<int, int> arco = arcos->getElement();
            int nodoPartida = arco.getKey();
            int nodoLlegada = arco.getValue();

            //Revisar conjuntos
            int conjuntoPartida = -1;
            int conjuntoLlegada = -1;
            for (int i = 0; i < size; i++) {
                if (conjuntos[i].contains(nodoPartida)) {
                    conjuntoPartida = i;
                }
                if (conjuntos[i].contains(nodoLlegada)) {
                    conjuntoLlegada = i;
                }
            }

            if (conjuntoPartida != conjuntoLlegada) {
                //Dejar el arco en el grafo del laberinto
                laberinto[nodoPartida].append(arco.getValue());
                laberinto[nodoLlegada].append(arco.getKey());
                //Unir los conjuntos de los nodos
                conjuntos[conjuntoLlegada].goToStart();
                for (int i = 0; i < conjuntos[conjuntoLlegada].getSize(); i++){
                    conjuntos[conjuntoPartida].append(conjuntos[conjuntoLlegada].getElement());
                    conjuntos[conjuntoLlegada].next();
                }
                //Hubiera usado variables mas leibles
                //pero no se me ocurrieron nombres
                //._.xd
                conjuntos[conjuntoLlegada].clear();

                grafo[nodoPartida].goToStart();
                for (int i = 0; i < grafo[nodoPartida].getSize(); i++) {
                    if (grafo[nodoPartida].getElement() == nodoLlegada)
                        grafo[nodoPartida].remove();
                    grafo[nodoPartida].next();
                }

                grafo[nodoLlegada].goToStart();
                for (int i = 0; i < grafo[nodoLlegada].getSize(); i++) {
                    if (grafo[nodoLlegada].getElement() == nodoPartida)
                        grafo[nodoLlegada].remove();
                    grafo[nodoLlegada].next();
                }
            }
            arcos->remove();
        }

        delete arcos;
        delete [] conjuntos;
    }
};

#endif // LABERINTOEXPANSIONKRUSKAL_H
