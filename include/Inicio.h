#ifndef INICIO_H
#define INICIO_H


class Inicio{
    private:
    sf::RenderWindow* window;
    sf::Font font;
    sf::Text textoTitulo;
    sf::Text textoJugar;
    sf::Text textoSalir;
    int selected;



public:
    Inicio(sf::RenderWindow* window){
        selected = 0;
        this->window = window;


        if (!font.loadFromFile("fuente/04B_30__.ttf")){
            throw new runtime_error("Archivo fuente no encontrado");
        }
        textoTitulo.setPosition(90, 90);
        textoTitulo.setFont(font);
        textoTitulo.setCharacterSize(64);
        textoTitulo.setFillColor(sf::Color::Blue);
        textoTitulo.setString("LABYRINTH");

        textoJugar.setPosition(265, 250);
        textoJugar.setFont(font);
        textoJugar.setCharacterSize(36);
        textoJugar.setFillColor(sf::Color::Green);
        textoJugar.setString("START");

        textoSalir.setPosition(285, 300);
        textoSalir.setFont(font);
        textoSalir.setCharacterSize(36);
        textoSalir.setFillColor(sf::Color::Blue);
        textoSalir.setString("EXIT");
    }
    virtual ~Inicio() {
    }


    int runInicio(){
        //Funcion que actualiza y dibuja en pantalla el menu de Inicio
        //Retorna la opcion del menu de inicio que el usuario escoja

        while(window->isOpen()){
            bool continuar = true;
            //Eventos
            ////////////
            sf::Event event;

            while(window->pollEvent(event)){
                if(event.type == sf::Event::Closed){
                    window->close();
                }
                if (event.type == sf::Event::KeyPressed){

                    if(event.key.scancode == sf::Keyboard::W){
                        selected = (selected-1);
                        if(selected<0){
                            selected=1;
                        }
                    }
                    if(event.key.scancode == sf::Keyboard::S){
                        selected = (selected+1)%2;
                    }
                    if(event.key.scancode == sf::Keyboard::Scan::Enter){
                        continuar = false;
                    }
                }
            }

            if(selected == 0){
                textoJugar.setFillColor(sf::Color::Green);
                textoSalir.setFillColor(sf::Color::Blue);
            }
            if(selected == 1){
                textoJugar.setFillColor(sf::Color::Blue);
                textoSalir.setFillColor(sf::Color::Green);
            }
            if(!continuar) break;


            //Draw
            /////////
            window->clear(sf::Color::Black);
            window->draw(textoTitulo);
            window->draw(textoJugar);
            window->draw(textoSalir);
            window->display();

        }
        return selected;
    }


};

#endif // INICIO_H
