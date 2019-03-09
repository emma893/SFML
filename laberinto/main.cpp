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
        sf::RectangleShape getObstaculo(int x, int y);
        void cambiar_posicion_objectos();
        bool mapa_valido(int y_actual, int x_acutal);
        int pasos_entre_objetos;
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
        bool tabla_de_comprobacion [11][16];
        bool tabla_de_movimientos [11][16];

        int posicion_jugable_x;
        int posicion_jugable_y;

        int posicion_obs_x;
        int posicion_obs_y;

        int posicion_obj_x;
        int posicion_obj_y;

        string texto;

        bool mIsMovingUp, mIsMovingRight, mIsMovingLeft, mIsMovingDown;
        bool mIsMovingUpRel, mIsMovingRightRel, mIsMovingLeftRel, mIsMovingDownRel;
        bool mIsMapGenerate;
        bool MapIsChecked;
};

    Game::Game(): mWindow(sf::VideoMode(700, 500), "Atrapame!"), mPlayer() {


            MapIsChecked = false;
            mIsMapGenerate = true;
            pasos_entre_objetos = 0;
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
            mPlayerObj.setRadius(25.f);
            mPlayerObj.setFillColor(sf::Color::Cyan);

        // Circulo jugable
            mPlayer.setRadius(25.f);
            mPlayer.setFillColor(sf::Color::Cyan);

        // Movimiento del objeto mPlayer
            sf::Vector2f movement(0.f, 0.f);

        // Variables de movimiento
            mIsMovingDown = mIsMovingLeft = mIsMovingRight = mIsMovingUp = 0;
            mIsMovingDownRel = mIsMovingLeftRel = mIsMovingRightRel = mIsMovingUpRel = 0;

        // Posicion inicial del objetivo
            posicion_obj_x = mPlayerObj.getPosition().x;
            posicion_obj_y = mPlayerObj.getPosition().y;

    }

    void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {

        if (key == sf::Keyboard::Up)
        mIsMovingUp = isPressed;
        else if (key == sf::Keyboard::Down)
        mIsMovingDown = isPressed;
        else if (key == sf::Keyboard::Left)
        mIsMovingLeft = isPressed;
        else if (key == sf::Keyboard::Right)
        mIsMovingRight = isPressed;

        if (key == sf::Keyboard::Up) {

            this->mIsMovingUpRel = isPressed;
        }

        if (key == sf::Keyboard::Down) {

            this->mIsMovingDownRel = isPressed;
        }

        if (key == sf::Keyboard::Left) {

            this->mIsMovingLeftRel = isPressed;

        }

        if (key == sf::Keyboard::Right) {

            this->mIsMovingRightRel = isPressed;
        }

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

            if (this->mIsMovingUp & this->mIsMovingUpRel & (posicion_jugable_y > 0) & movimiento_valido(0, -1)) {

                this->movement.y -= 50.f;
                this->mIsMovingUpRel = 0;
            }

            if (this->mIsMovingDown &  this->mIsMovingDownRel & (posicion_jugable_y < 450) & movimiento_valido(0, 1)) {

                this->movement.y += 50.f;
                this->mIsMovingDownRel = 0;
            }

            if (this->mIsMovingLeft & this->mIsMovingLeftRel & (posicion_jugable_x  > 0) & movimiento_valido(-1, 0)) {

                this->movement.x -= 50.f;
                this->mIsMovingLeftRel = 0;

            }

            if (this->mIsMovingRight & this->mIsMovingRightRel & (posicion_jugable_x < 650) & movimiento_valido(1, 0)) {

                this->movement.x += 50.f;
                this->mIsMovingRightRel = 0;
            }

            if(mPlayer.getPosition() == mPlayerObj.getPosition()) {

                this->mIsMapGenerate = true;
                Game::generacion_mapa();
            }

            mPlayer.move(this->movement);

    }

    void Game::cambiar_posicion_objectos(){

        int x_player;
        int y_player;
        int x_obj;
        int y_obj;

        while(true){

            x_player = rand() % 14;
            y_player = rand() % 10;

            x_obj = rand() % 14;
            y_obj = rand() % 10;

            if(Game::tabla_de_movimientos[y_obj][x_obj] == true & Game::tabla_de_movimientos[y_player][x_player] == true){
                break;
            }
        }

        mPlayerObj.setPosition(x_obj * 50, y_obj * 50);
        mPlayer.setPosition(x_player * 50, y_player * 50);
    }


    sf::RectangleShape Game::getObstaculo(int x, int y){

        sf::RectangleShape nuevo;

        // Cuadrado de obstaculo
            sf::Vector2f mPlayerObs_size(50, 50);
            nuevo.setSize(mPlayerObs_size);
            nuevo.setFillColor(sf::Color::White);
            nuevo.setPosition(y, x);

        return nuevo;
    }

    void Game::generacion_mapa(){

       srand(time(0));
//       Game::texto = "";
       for ( int i = 0; i < 10; i++ ) {

              for ( int j = 0; j < 14; j++ ) {

                    if(this->mIsMapGenerate){

                        Game::tabla_de_movimientos[i][j] = rand() % 2;
                        cambiar_posicion_objectos();
                        Game::tabla_de_comprobacion[i][j] = false;
                        this->MapIsChecked = false;

                    }

                    if(Game::tabla_de_movimientos[i][j] == 0){

//                        Game::texto += Game::posicion;

                        Game::mWindow.draw(Game::getObstaculo(i * 50, j * 50));
                    }
//                    sprintf(Game::posicion, "%d", Game::tabla_de_movimientos[i][j]);

              }
//              Game::texto += "\n";
        }
//        Game::tabla_de_movimientos[0][0] = 0;
//        Game::tabla_de_movimientos[0][1] = 0;
//        Game::tabla_de_movimientos[0][2] = 0;
//        Game::tabla_de_movimientos[0][4] = 0;
//        Game::tabla_de_movimientos[1][0] = 0;
//        Game::tabla_de_movimientos[2][3] = 0;
//        Game::tabla_de_movimientos[3][0] = 0;
//        Game::tabla_de_movimientos[3][2] = 0;
//        Game::tabla_de_movimientos[4][2] = 0;
//        Game::tabla_de_movimientos[4][4] = 0;
//        Game::tabla_de_movimientos[5][0] = 0;
//        Game::tabla_de_movimientos[6][0] = 0;
//        Game::tabla_de_movimientos[6][2] = 0;
//        Game::tabla_de_movimientos[1][4] = 0;
//        mText.setString(Game::texto);

//        if(this->mIsMapGenerate){
//
//
//        }
//        Game::tabla_de_movimientos[2][2] = 1;
//        Game::tabla_de_movimientos[4][4] = 1;
//
        this->mIsMapGenerate = false;
    }

    bool Game::movimiento_valido(int x, int y){

//        Game::texto = "";
//        sprintf(Game::posicion, "%d, %d, %d",
//        Game::posicion_jugable_y ,
//        Game::pasos_entre_objetos,
//        Game::tabla_de_movimientos[(Game::posicion_jugable_y / 50)][(Game::posicion_jugable_x / 50)]);

        if(Game::tabla_de_movimientos[(Game::posicion_jugable_y / 50) + y][(Game::posicion_jugable_x / 50) + x] == 1){
//
//                texto = texto + posicion;
//                mText2.setString(Game::texto);
            return true;
        }else{

//                texto = texto + posicion;
//                mText2.setString(Game::texto);
            return false;
        }
    }

    bool Game::mapa_valido(int y_actual, int x_actual){

//        int temp ;
//
//        temp = x_actual;
//
//        x_actual = Game::mPlayerObj.getPosition().x/100;
//
//        x_actual = temp;
//
//        temp = y_actual;
//
//        y_actual = Game::mPlayerObj.getPosition().y/100;
//
//        y_actual = temp;

        if(Game::tabla_de_comprobacion[y_actual][x_actual] == true){

          this->pasos_entre_objetos --;
          return false;
        }


        if(y_actual > 10 || y_actual < 0){

            this->pasos_entre_objetos --;
            return false;
        }

        if(x_actual > 14 || x_actual < 0){

            this->pasos_entre_objetos --;
            return false;
        }

        Game::tabla_de_comprobacion[y_actual][x_actual] = true;

        if(Game::tabla_de_movimientos[y_actual][x_actual] == 0){

            this->pasos_entre_objetos --;
            return false;
        }

        if((Game::mPlayerObj.getPosition().x/50) == x_actual && ((Game::mPlayerObj.getPosition().y/50) == y_actual)) {

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
//
        while(!MapIsChecked){

            if(Game::mapa_valido(Game::mPlayer.getPosition().y / 50, Game::mPlayer.getPosition().x / 50)){

                this->MapIsChecked = true;

            }else{

                this->pasos_entre_objetos = 0;
                this->mIsMapGenerate = true;
                Game::generacion_mapa();
//                Game::cambiar_posicion_objectos();
            }
        }
//        mWindow.draw(mText);
//        mWindow.draw(mText2);
        mWindow.draw(mPlayer);
        mWindow.draw(mPlayerObj);
//        mWindow.draw(mPlayerObs);
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

