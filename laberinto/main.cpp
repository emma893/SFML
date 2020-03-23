#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>

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
        sf::RectangleShape getObstaculo(int x, int y);
        void cambiar_posicion_objetos();
        bool mapa_valido(int y_actual, int x_acutal);
        int pasos_entre_objetos;
        void render();

        sf::RenderWindow mWindow;
        sf::CircleShape mPlayer;
        sf::CircleShape mPlayerObj;
        sf::RectangleShape mPlayerObs;
        sf::Vector2f movement;

        const static int windowDimentionX = 500; // window dimention Y
        const static int windowDimentionY = 700; // window dimention X

        // size map
        const static int sizeBlock = 20;
        
        const static int fieldDimentionX = ((windowDimentionX / sizeBlock) * 2) + 1;
        const static int fieldDimentionY = ((windowDimentionY / sizeBlock) * 2) + 2;

        // large: 100. medium: 50, little: 10.
        const static int sizeField = sizeBlock / 2;
        float movementDistance = sizeField;

        bool tabla_de_comprobacion [fieldDimentionX][fieldDimentionY];
        bool tabla_de_movimientos [fieldDimentionX][fieldDimentionY];

        int posicion_jugable_x;
        int posicion_jugable_y;

        int posicion_objetivo_x;
        int posicion_objetivo_y;

        bool mIsMovingUp, mIsMovingRight, mIsMovingLeft, mIsMovingDown;
        bool mIsMovingUpRel, mIsMovingRightRel, mIsMovingLeftRel, mIsMovingDownRel;
        bool mIsMapGenerate;
        bool mapIsChecked;
};

    Game::Game(): mWindow(sf::VideoMode(windowDimentionY, windowDimentionX), "Atrapame!"), mPlayer() {

            mapIsChecked = false;
            mIsMapGenerate = true;
            pasos_entre_objetos = 0;

        // Circulo Objetivo
            mPlayerObj.setRadius(movementDistance / 2.f);
            mPlayerObj.setFillColor(sf::Color::Cyan);

        // Circulo jugable
            mPlayer.setRadius(movementDistance / 2.f);
            mPlayer.setFillColor(sf::Color::Cyan);

        // Movimiento del objeto mPlayer
            sf::Vector2f movement(0.f, 0.f);

        // Variables de movimiento
            mIsMovingDown = mIsMovingLeft = mIsMovingRight = mIsMovingUp = 0;
            mIsMovingDownRel = mIsMovingLeftRel = mIsMovingRightRel = mIsMovingUpRel = 0;

        // Posicion inicial del objetivo
            posicion_objetivo_x = mPlayerObj.getPosition().x;
            posicion_objetivo_y = mPlayerObj.getPosition().y;
    }

    void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {

        if (key == sf::Keyboard::Up) mIsMovingUp = isPressed;
        else if (key == sf::Keyboard::Down) mIsMovingDown = isPressed;
        else if (key == sf::Keyboard::Left) mIsMovingLeft = isPressed;
        else if (key == sf::Keyboard::Right) mIsMovingRight = isPressed;

        this->mIsMovingUpRel    = (key == sf::Keyboard::Up) ?: isPressed;        
        this->mIsMovingDownRel  = (key == sf::Keyboard::Down) ?: isPressed;
        this->mIsMovingLeftRel  = (key == sf::Keyboard::Left) ?: isPressed;
        this->mIsMovingRightRel = (key == sf::Keyboard::Right) ?: isPressed;

        if (key == sf::Keyboard::J) {
            this->mIsMapGenerate = isPressed;
            this->tabla_de_movimientos;
            Game::generacion_mapa();
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

        if (this->mIsMovingUp && this->mIsMovingUpRel && (posicion_jugable_y > 0) && movimiento_valido(0, -1)) {
            this->movement.y -= movementDistance;
            this->mIsMovingUpRel = 0;
        }

        if (this->mIsMovingDown &&  this->mIsMovingDownRel && (posicion_jugable_y < (windowDimentionX - sizeField)) && movimiento_valido(0, 1)) {
            this->movement.y += movementDistance;
            this->mIsMovingDownRel = 0;
        }

        if (this->mIsMovingLeft && this->mIsMovingLeftRel && (posicion_jugable_x  > 0) && movimiento_valido(-1, 0)) {
            this->movement.x -= movementDistance;
            this->mIsMovingLeftRel = 0;
        }

        if (this->mIsMovingRight && this->mIsMovingRightRel && (posicion_jugable_x < (windowDimentionY - sizeField)) && movimiento_valido(1, 0)) {
            this->movement.x += movementDistance;
            this->mIsMovingRightRel = 0;
        }

        if(mPlayer.getPosition() == mPlayerObj.getPosition()) {
            this->mIsMapGenerate = true;
            Game::generacion_mapa();
        }

        mPlayer.move(this->movement);
    }

    void Game::cambiar_posicion_objetos(){

        int x_player;
        int y_player;
        int x_obj;
        int y_obj;

        while(true){

            x_player = rand() % (fieldDimentionY - 2);
            y_player = rand() % (fieldDimentionX - 1);

            x_obj = rand() % (fieldDimentionY - 2);
            y_obj = rand() % (fieldDimentionX - 1);

            if(Game::tabla_de_movimientos[y_obj][x_obj] && Game::tabla_de_movimientos[y_player][x_player]){
                break;
            }
        }

        mPlayerObj.setPosition(x_obj * sizeField, y_obj * sizeField);
        mPlayer.setPosition(x_player * sizeField, y_player * sizeField);
    }


    sf::RectangleShape Game::getObstaculo(int x, int y){

        sf::RectangleShape nuevo;

        // Cuadrado de obstaculo
        sf::Vector2f mPlayerObs_size(sizeField, sizeField);
        nuevo.setSize(mPlayerObs_size);
        nuevo.setFillColor(sf::Color::White);
        nuevo.setPosition(y, x);

        return nuevo;
    }

    void Game::generacion_mapa(){

       srand(time(0));
       for (int i = 0; i < (fieldDimentionX - 1); i++) {
            for (int j = 0; j < (fieldDimentionY - 2); j++) {

                if(this->mIsMapGenerate){
                    Game::tabla_de_movimientos[i][j] = rand() % 2;
                    cambiar_posicion_objetos();
                    Game::tabla_de_comprobacion[i][j] = false;
                    this->mapIsChecked = false;
                }

                if(Game::tabla_de_movimientos[i][j] == 0) {
                    Game::mWindow.draw(Game::getObstaculo(i * sizeField, j * sizeField));
                }
            }
        }
        this->mIsMapGenerate = false;
    }

    bool Game::movimiento_valido(int x, int y){

        int nextPositionY = (Game::posicion_jugable_y / sizeField) + y;
        int nextPositionX = (Game::posicion_jugable_x / sizeField) + x;

        bool isAValidPlay = Game::tabla_de_movimientos[nextPositionY][nextPositionX] == 1;

        return isAValidPlay;
    }

    bool Game::mapa_valido(int y_actual, int x_actual){

        if(Game::tabla_de_comprobacion[y_actual][x_actual] == true){

          this->pasos_entre_objetos --;
          return false;
        }

        if(y_actual > (fieldDimentionX - 1) || y_actual < 0){

            this->pasos_entre_objetos --;
            return false;
        }

        if(x_actual > (fieldDimentionY - 2) || x_actual < 0){

            this->pasos_entre_objetos --;
            return false;
        }

        Game::tabla_de_comprobacion[y_actual][x_actual] = true;

        if(Game::tabla_de_movimientos[y_actual][x_actual] == 0){

            this->pasos_entre_objetos --;
            return false;
        }

        int positionPlayerX = Game::mPlayerObj.getPosition().x / sizeField;
        int positionPlayerY = Game::mPlayerObj.getPosition().y / sizeField;

        if(positionPlayerX == x_actual && positionPlayerY == y_actual) {

            return true;
        }

        this->pasos_entre_objetos ++;

        if(Game::mapa_valido(y_actual - 1, x_actual))return true;
        else
        if(Game::mapa_valido(y_actual, x_actual - 1))return true;
        else
        if(Game::mapa_valido(y_actual + 1, x_actual))return true;
        else
        if(Game::mapa_valido(y_actual, x_actual + 1))return true;
        else
        return false;
    }

    void Game::render() {

        mWindow.clear();

        Game::generacion_mapa();

        while(!mapIsChecked) {

            int positionPlayerX = Game::mPlayer.getPosition().x / sizeField;
            int positionPlayerY = Game::mPlayer.getPosition().y / sizeField;

            bool isMapaValido = Game::mapa_valido(positionPlayerY, positionPlayerX);

            if(isMapaValido){

                this->mapIsChecked = true;
            }else{

                this->pasos_entre_objetos = 0;
                this->mIsMapGenerate = true;
                Game::generacion_mapa();
            }
        }

        mWindow.draw(mPlayer);
        mWindow.draw(mPlayerObj);
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

