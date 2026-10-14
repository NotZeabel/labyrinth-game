#include "Game.h"
#include "Inicio.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <time.h>




int main(){
    srand(time(0));

    sf::RenderWindow window(sf::VideoMode(720,480), "Labyrinth");
    window.setFramerateLimit(60);

    Inicio* inicio = new Inicio(&window);

    int opcion = inicio->runInicio();





    while(opcion==0){
        //Loop Principal del Juego
        //Controla el incremento de dificultad entre niveles
        Game* game;
        sf::Time tiempoJuego = sf::seconds(45);
        sf::Time tiempoRestante = tiempoJuego;
        int dificultad = 0;
        int manzanas = 1;
        int nivel = 0;
        int tipo = rand()%3;
        int width = 12;
        int height = width/2+1;
        float cellSize = 720/((width)*2);


        while(tiempoRestante.asSeconds()>0){
            switch(dificultad){
                case 1:
                    manzanas++;
                    break;
                case 2:
                    tiempoJuego -= sf::seconds(5);
                    break;
                default:
                    break;
            }

            game = new Game(width, height, cellSize, tipo, tiempoRestante, manzanas, dificultad, nivel, &window);
            tiempoRestante = game->runGame();
            if(tiempoRestante<sf::seconds(0)) break;


            tiempoRestante += tiempoJuego;
            if(rand()%4 != 0){
                dificultad = 2;
            }else{
                dificultad = 1;
            }
            nivel++;
            tipo = (tipo+1)%3;
            width++;
            height = width/2+1;
            cellSize = 720/((width)*2);
        }
        delete game;
        inicio = new Inicio(&window);
        opcion = inicio->runInicio();
    }

    return 0;
}
