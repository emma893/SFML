#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>

using namespace std;

class Game {

    public:

        Game();
        void run();

    private:

        void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
        void processEvents();
        void update();
        void generacion_mapa();
        bool movimiento_valido(int x, int y);
        void render();

    private:

        sf::RenderWindow mWindow;
        sf::CircleShape mPlayer;
        sf::CircleShape mPlayerObj;
        sf::RectangleShape mPlayerObs;
        sf::Font mFont;
        sf::Text mText;
        sf::Text mText2;
        sf::Vector2f movement;

        char posicion[40];

        bool tabla_de_movimientos [5][7];

        int posicion_jugable_x;
        int posicion_jugable_y;

        int posicion_obs_x;
        int posicion_obs_y;

        int posicion_obj_x;
        int posicion_obj_y;

        string texto;

        bool mIsMovingUp, mIsMovingRight, mIsMovingLeft, mIsMovingDown;
        bool mIsMovingUpRel, mIsMovingRightRel, mIsMovingLeftRel, mIsMovingDownRel;
};

    Game::Game(): mWindow(sf::VideoMode(700, 500), "Atrapame!"), mPlayer() {

        // Generacion de mapa
            Game::generacion_mapa();

        // Texto de coordenadas.
            mFont.loadFromFile("arial.ttf");

            mText.setFont(mFont);
            mText.setCharacterSize(22);
            mText.setStyle(sf::Text::Bold);
            mText.setColor(sf::Color::Red);
            mText.setPosition(350, 350);

            texto = "";

            mText2.setFont(mFont);
            mText2.setCharacterSize(22);
            mText2.setStyle(sf::Text::Bold);
            mText2.setColor(sf::Color::Red);
            mText2.setPosition(150, 350);

        // Circulo Objetivo
            mPlayerObj.setRadius(50.f);
            mPlayerObj.setFillColor(sf::Color::Green);

        // Circulo jugable
            mPlayer.setRadius(50.f);
            mPlayer.setPosition(100, 100);
            mPlayer.setFillColor(sf::Color::Cyan);

        // Movimiento del objeto mPlayer
            sf::Vector2f movement(0.f, 0.f);

        // Cuadrado de obstaculo
            sf::Vector2f mPlayerObs_size(100, 100);
            mPlayerObs.setSize(mPlayerObs_size);
            mPlayerObs.setFillColor(sf::Color::White);

        // Variables de movimiento
            mIsMovingDown = mIsMovingLeft = mIsMovingRight = mIsMovingUp = 0;
            mIsMovingDownRel = mIsMovingLeftRel = mIsMovingRightRel = mIsMovingUpRel = 0;

        // Posicion inicial del objetivo
            posicion_obj_x = mPlayerObj.getPosition().x;
            posicion_obj_y = mPlayerObj.getPosition().y;
    }

    void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {

        if (key == sf::Keyboard::W)
        mIsMovingUp = isPressed;
        else if (key == sf::Keyboard::S)
        mIsMovingDown = isPressed;
        else if (key == sf::Keyboard::A)
        mIsMovingLeft = isPressed;
        else if (key == sf::Keyboard::D)
        mIsMovingRight = isPressed;

        if (key == sf::Keyboard::W) {

            this->mIsMovingUpRel = isPressed;
        }

        if (key == sf::Keyboard::S) {

            this->mIsMovingDownRel = isPressed;
        }

        if (key == sf::Keyboard::A) {

            this->mIsMovingLeftRel = isPressed;

        }

        if (key == sf::Keyboard::D) {

            this->mIsMovingRightRel = isPressed;
        }
    }

    void Game::processEvents() {

        sf::Event event;

        while (mWindow.pollEvent(event)) {

            switch (event.type) {
                case sf::Event::KeyPressed:handlePlayerInput(event.key.code, true);
                break;

                case sf::Event::KeyReleased:handlePlayerInput(event.key.code, false);
                break;

                case sf::Event::Closed:mWindow.close();
                break;
            }
        }
    }

    void Game::update() {

            posicion_jugable_x = mPlayer.getPosition().x;
            posicion_jugable_y = mPlayer.getPosition().y;

            this->movement.y = 0.f;
            this->movement.x = 0.f;

            if (this->mIsMovingUp & this->mIsMovingUpRel & (posicion_jugable_y > 0) & movimiento_valido(0, -1)) {

                this->movement.y -= 100.f;
                this->mIsMovingUpRel = 0;
            }

            if (this->mIsMovingDown &  this->mIsMovingDownRel & (posicion_jugable_y < 380) & movimiento_valido(0, 1)) {

                this->movement.y += 100.f;
                this->mIsMovingDownRel = 0;
            }

            if (this->mIsMovingLeft & this->mIsMovingLeftRel & (posicion_jugable_x  > 0) & movimiento_valido(-1, 0)) {

                this->movement.x -= 100.f;
                this->mIsMovingLeftRel = 0;

            }

            if (this->mIsMovingRight & this->mIsMovingRightRel & (posicion_jugable_x < 540) & movimiento_valido(1, 0)) {

                this->movement.x += 100.f;
                this->mIsMovingRightRel = 0;
            }

            if(mPlayer.getPosition() == mPlayerObj.getPosition()) {
                Game::generacion_mapa();
                mPlayerObj.setPosition((rand() % 6) * 100, (rand() % 4) * 100);
            }

//            sprintf(posicion, "%d, %d", Game::posicion_jugable_x / 100, Game::posicion_jugable_y / 100);
//            mText.setString(posicion);

            mPlayer.move(this->movement);

    }

    void Game::generacion_mapa(){

       srand(time(0));
       Game::texto = "";
       for ( int i = 0; i < 5; i++ ) {

              for ( int j = 0; j < 7; j++ ) {

                    Game::tabla_de_movimientos[i][j] = rand() % 2;

                    if(Game::tabla_de_movimientos[i][j] == 0){

                        Game::mPlayerObs.setPosition(i * 100, j * 100);
                    }
                    sprintf(Game::posicion, "%d", Game::tabla_de_movimientos[i][j]);

                    Game::texto += Game::posicion;
                    mPlayerObs.setPosition(i * 100, j * 100);
                    mPlayerObs.setSize(sf::Vector2f());
              }
              Game::texto += "\n";
        }
        mText.setString(Game::texto);
    }

    bool Game::movimiento_valido(int x, int y){
        Game::texto = "";
        sprintf(Game::posicion, "%d, %d, %d",
        Game::posicion_jugable_y / 100,
        Game::posicion_jugable_x / 100,
        Game::tabla_de_movimientos[(Game::posicion_jugable_y / 100)][(Game::posicion_jugable_x / 100)]);

        if(Game::tabla_de_movimientos[(Game::posicion_jugable_y / 100) + y][(Game::posicion_jugable_x / 100) + x] == 1){

                texto = texto + posicion;
                mText2.setString(Game::texto);
            return true;
        }else{

                texto = texto + posicion;
                mText2.setString(Game::texto);
            return false;
        }
    }

    void Game::render() {

        mWindow.clear();
        mWindow.draw(mText);
        mWindow.draw(mText2);
        mWindow.draw(mPlayer);
        mWindow.draw(mPlayerObj);
        mWindow.draw(mPlayerObs);
        mWindow.display();
    }

    void Game::run() {

        while(mWindow.isOpen()) {

            processEvents();
            update();
            render();
        }
    }

int main() {

    Game game;
    game.run();
}
