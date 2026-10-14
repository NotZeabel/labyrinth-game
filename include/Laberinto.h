#ifndef LABERINTO_H
#define LABERINTO_H

#include <DLinkedList.h>
#include <vector>


class Laberinto{
private:
    void operator= (const Laberinto &other){}
    Laberinto(const Laberinto &other){}

    int distanciaMinima(const vector<int>& distancias, const vector<bool>& visitados) {
        // Este método toma la lista con las dinstancias y nodos visitados
        // en base a esto calcula la distancia minima hacia un nodo
        // no visitado
        int minimo = INT_MAX;
        int indice;

        for (int i = 0; i < size; i++) {
            if (!visitados[i] && distancias[i] <= minimo) {
                minimo = distancias[i];
                indice = i;
            }
        }

        return indice;
}


protected:
    DLinkedList<int>* laberinto;
    int rows;
    int cols;
    int size;

public:
    Laberinto(){}
    virtual ~Laberinto(){}
    virtual void generarLaberinto(DLinkedList<int>* grafo) =0;


    DLinkedList<int>* getNodo(int row, int column){
        //Metodo que retorna el nodo que se encuentra en
        //la posicion indicada en los parametros
        return &laberinto[row*cols+column];
    }

    int getPos(int row, int column) {
        return row*cols+column;
    }

    DLinkedList<int>* getNodo(int nodo){
        return &laberinto[nodo];
    }

    void inicializarArcos(DLinkedList<int>* grafo){
        //Metodo que añade arcos al grafo que recibe por parametro
        //Los arcos son creados de modo que se forma una rejilla con los nodos

        for(int i=0; i<size; i++){
            //Vecino Izquierdo
            if(0 <= i-1 && (i-1)/cols == i/cols){
                grafo[i].append(i-1);
            }
            //Vecino Derecho
            if(i+1 < size && (i+1)/cols == i/cols){
                grafo[i].append(i+1);
            }
            //Vecino Arriba
            if(0 <= i-cols){
                grafo[i].append(i-cols);
            }
            //Vecino Abajo
            if(i+cols < size){
                grafo[i].append(i+cols);
            }
        }
    }

     void agregarArcosExtra(DLinkedList<int>* grafo){
        //Metodo que añade arcos extra al laberinto partir de un grafo que recibe por parametro
        // El grafo que recibe por parametro debe contener los arcos que no se encuentren en el laberinto
        //Añade un 5% de arcos extra segun el tamaño del grafo
        for(int i=0; i<size*0.05; i++){
            int nodo1 = rand()%size;
            if(grafo[nodo1].isEmpty()){
                i--;
                continue;
            }
            grafo[nodo1].goToPos(rand()%grafo[nodo1].getSize());
            int nodo2 = grafo[nodo1].getElement();
            laberinto[nodo1].append(nodo2);
            laberinto[nodo2].append(nodo1);
        }
    }

    DLinkedList<int>* dijkstra(int nodoActual) {
        // Este método toma la casilla actual del jugador y aplica
        // el algoritmo de Dijkstra para encontrar la ruta
        // más corta a la salida desde la posicion actual
        vector<int> distancia(size, INT_MAX);
        vector<bool> definitivos(size, false);
        vector<int> rutas(size, -1);

        distancia[nodoActual] = 0;

        for (int contador = 0; contador < size - 1; contador++) {
            int indice = distanciaMinima(distancia, definitivos);
            definitivos[indice] = true;

            laberinto[indice].goToStart();
            for (int i = 0; i < laberinto[indice].getSize(); i++) {
                int vecino = laberinto[indice].getElement();
                int weight = 1;

                if (!definitivos[vecino] && distancia[indice] + weight < distancia[vecino]) {
                    distancia[vecino] = distancia[indice] + weight;
                    rutas[vecino] = indice;
                }

                laberinto[indice].next();
            }
        }

        vector<int> ruta;
        int current = size - 1;
        while (current != -1) {
            ruta.push_back(current);
            current = rutas[current];
        }

        DLinkedList<int>* resultado = new DLinkedList<int>();

        for (int nodo : ruta) {
            resultado->append(nodo);
        }

        return resultado;
}


    int getRows(){
        return rows;
    }

    int getCols(){
        return cols;
    }

    int getSize(){
        return size;
    }

    int nodoToRow(int nodo){
        //Metodo que recibe la posicion de un nodo en la lista y
        //Retorna la fila en la que se encuentra.
        return nodo/cols;
    }

    int nodoToCol(int nodo){
        //Metodo que recibe la posicion de un nodo en la lista y
        //Retorna la columna en la que se encuentra.
        return nodo%cols;
    }
};

#endif // LABERINTO_H
