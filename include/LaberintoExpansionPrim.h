#ifndef LABERINTOEXPANSIONPRIM_H
#define LABERINTOEXPANSIONPRIM_H

#include <utility>
#include <time.h>
#include "DLinkedList.h"
#include "LinkedStack.h"
#include "Laberinto.h"
#include "KVPair.h"

class LaberintoExpansionPrim : public Laberinto{
public:
    LaberintoExpansionPrim(int rows, int cols) {
        //Constructor de la clase
        //Toma la cantidad de filas y columnas
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

    ~LaberintoExpansionPrim() {
        // Destructor de la clase
        delete [] laberinto;
    }

    void generarLaberinto(DLinkedList<int>* grafo) {
        // Este metodo genera un laberinto por medio de una version
        // modificada del algoritmo de prim
        // Toma un grafo con todos los arcos y nodos para generar el laberinto
        int nodoInicial = rand() % size;
        bool* enLab = new bool[size];
        DLinkedList<KVPair<int, int>>* arcos = new DLinkedList<KVPair<int, int>>();

        //Inicializar arrlego
        for (int i = 0; i < size; i++) {
            enLab[i] = false;
        }
        //Primer nodo
        enLab[nodoInicial] = true;
        grafo[nodoInicial].goToStart();
        for (int i = 0; i < grafo[nodoInicial].getSize(); i++) {
            KVPair<int, int> arco(nodoInicial, grafo[nodoInicial].getElement());
            arcos->append(arco);
        }

        //Algoritmo ahora sí
        while (!arcos->isEmpty()) {
            //Arco aleatorio
            arcos->goToPos(rand() % arcos->getSize());
            KVPair<int, int> arcoActual = arcos->getElement();

            int nodoActual = arcoActual.getKey();

            if (!enLab[arcoActual.getValue()]) {
                //Añadir arco si no está en el laberinto
                laberinto[nodoActual].append(arcoActual.getValue());
                laberinto[arcoActual.getValue()].append(nodoActual);
                enLab[arcoActual.getValue()] = true;

                //Añadir arcos del nuevo nodo a la lista de arcos
                grafo[arcoActual.getValue()].goToStart();
                for (int i = 0; i < grafo[arcoActual.getValue()].getSize(); i++) {
                    int arcoNuevo = grafo[arcoActual.getValue()].getElement();
                    KVPair<int, int> nodoNuevo(arcoActual.getValue(), arcoNuevo);
                    arcos->append(nodoNuevo);
                    grafo[arcoActual.getValue()].next();
                }

                grafo[nodoActual].goToStart();
                for (int i = 0; i < grafo[nodoActual].getSize(); i++){
                    if (grafo[nodoActual].getElement() == arcoActual.getValue())
                        grafo[nodoActual].remove();
                    grafo[nodoActual].next();
                }
                grafo[arcoActual.getValue()].goToStart();
                for (int i = 0; i < grafo[arcoActual.getValue()].getSize(); i++){
                    if (grafo[arcoActual.getValue()].getElement() == nodoActual)
                        grafo[arcoActual.getValue()].remove();
                    grafo[arcoActual.getValue()].next();
                }
            } else {
                arcos->remove();
            }
        }

        delete [] enLab;
        delete arcos;
    }
};

#endif // LABERINTOEXPANSIONPRIM_H
