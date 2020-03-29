#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>

class Game {

    public:

        Game();
        void run();

    private:

        // MAIN FUNCTIONS
        void updateGame();
        void createNewMapAndObjects();
        void renderMapAndObjects();
        void processEvents();

        // GENERAL FUNCIONS
        void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
        void createNewMap();
        bool checkMovement(int x, int y);
        void setObjectPositions();
        bool checkMap(int y_actual, int x_acutal);
        
        int distanceBetweenObjects;
        sf::RectangleShape getWall(int x, int y, sf::Color wallColor);
        sf::RenderWindow gameWindow;
        sf::CircleShape playerObject;
        sf::CircleShape objetiveObject;
        sf::Vector2f movement;

        const static int windowDimentionX = 500; // window dimention Y
        const static int windowDimentionY = 700; // window dimention X

        // large: 100. medium: 50, little: 10.
        const static int sizeBlock = 100;
        
        const static int fieldDimentionX = ((windowDimentionX / sizeBlock) * 2) + 1;
        const static int fieldDimentionY = ((windowDimentionY / sizeBlock) * 2) + 2;

        const static int sizeField = sizeBlock / 2;
        float movementDistance = sizeField;

        bool arrayToCheck [fieldDimentionX][fieldDimentionY];
        bool arrayToMap [fieldDimentionX][fieldDimentionY];

        int playerPositionX;
        int playerPositionY;

        int objectivePositionX;
        int objectivePositionY;

        bool mIsMovingUp, mIsMovingRight, mIsMovingLeft, mIsMovingDown;
        bool mIsMovingUpRel, mIsMovingRightRel, mIsMovingLeftRel, mIsMovingDownRel;
        bool isMapChecked;
        bool isMapGenerated;
};

    Game::Game(): gameWindow(sf::VideoMode(windowDimentionY, windowDimentionX), "Atrapame!"), playerObject() {

            isMapChecked = false;
            distanceBetweenObjects = 0;
            isMapGenerated = false;

        // Circulo Objetivo
            objetiveObject.setRadius(movementDistance / 2.f);
            objetiveObject.setFillColor(sf::Color::Cyan);

        // Circulo jugable
            playerObject.setRadius(movementDistance / 2.f);
            playerObject.setFillColor(sf::Color::Green);

        // Movimiento del objeto playerObject
            sf::Vector2f movement(0.f, 0.f);

        // Variables de movimiento
            mIsMovingDown = mIsMovingLeft = mIsMovingRight = mIsMovingUp = 0;
            mIsMovingDownRel = mIsMovingLeftRel = mIsMovingRightRel = mIsMovingUpRel = 0;
    }

    void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {

        if (key == sf::Keyboard::Up) mIsMovingUp = isPressed;
        else if (key == sf::Keyboard::Down) mIsMovingDown = isPressed;
        else if (key == sf::Keyboard::Left) mIsMovingLeft = isPressed;
        else if (key == sf::Keyboard::Right) mIsMovingRight = isPressed;

        if (key == sf::Keyboard::J) {
            this->isMapGenerated = false;
        }

        if (isPressed) {
            Game::updateGame();
        }
    }

    void Game::processEvents() {

        sf::Event event;

        while (gameWindow.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::KeyPressed:handlePlayerInput(event.key.code, true);
                break;

                case sf::Event::KeyReleased:handlePlayerInput(event.key.code, false);
                break;

                case sf::Event::Closed:gameWindow.close();
                break;
            }
        }
    }

    void Game::updateGame() {

        int playerPositionXState = playerObject.getPosition().x;
        int playerPositionYState = playerObject.getPosition().y;

        this->movement.y = 0.f;
        this->movement.x = 0.f;

        if (this->mIsMovingUp && (playerPositionYState > 0) && checkMovement(0, -1)) {
            this->movement.y -= movementDistance;
            this->mIsMovingUp = false;
        } else {
            if (this->mIsMovingDown && (playerPositionYState < (windowDimentionX - sizeField)) && checkMovement(0, 1)) {
                this->movement.y += movementDistance;
                this->mIsMovingDown = false;
            } else {
                if (this->mIsMovingLeft && (playerPositionXState  > 0) && checkMovement(-1, 0)) {
                    this->movement.x -= movementDistance;
                    this->mIsMovingLeft= false;
                } else {
                    if (this->mIsMovingRight && (playerPositionXState < (windowDimentionY - sizeField)) && checkMovement(1, 0)) {
                        this->movement.x += movementDistance;
                        this->mIsMovingRight = false;
                    }
                }
            }
        }

        playerObject.move(this->movement);

        if(playerObject.getPosition() != objetiveObject.getPosition()) {
            gameWindow.draw(getWall(playerPositionYState, playerPositionXState, sf::Color::Black));
            gameWindow.draw(playerObject);
            gameWindow.display();
        } else {
            this->isMapGenerated = false;
        }
    }

    void Game::setObjectPositions() {

        int x_player, y_player, x_obj, y_obj = 0;

        while(true){

            x_player = rand() % (fieldDimentionY - 2);
            y_player = rand() % (fieldDimentionX - 1);

            x_obj = rand() % (fieldDimentionY - 2);
            y_obj = rand() % (fieldDimentionX - 1);

            if(!Game::arrayToMap[y_obj][x_obj] && !Game::arrayToMap[y_player][x_player]){
                break;
            }
        }

        this->playerPositionX = x_player;
        this->playerPositionY = y_player;
        this->objectivePositionX = x_obj;
        this->objectivePositionY = y_obj;
    }


    sf::RectangleShape Game::getWall(int x, int y, sf::Color wallColor){

        sf::RectangleShape nuevo;

        // Cuadrado de obstaculo o espacio negro
        sf::Vector2f mPlayerObs_size(sizeField, sizeField);
        nuevo.setSize(mPlayerObs_size);
        nuevo.setFillColor(wallColor);
        nuevo.setPosition(y, x);

        return nuevo;
    }

    void Game::createNewMap() {

       srand(time(0));
       for (int i = 0; i < (fieldDimentionX - 1); i++) {
            for (int j = 0; j < (fieldDimentionY - 2); j++) {
                Game::arrayToMap[i][j] = rand() % 2;
                Game::arrayToCheck[i][j] = false;
                this->isMapChecked = false;
            }
        }
    }

    bool Game::checkMovement(int x, int y) {

        int nextPositionY = (Game::playerObject.getPosition().y / sizeField) + y;
        int nextPositionX = (Game::playerObject.getPosition().x / sizeField) + x;

        bool isAValidPlay = !Game::arrayToMap[nextPositionY][nextPositionX];

        return isAValidPlay;
    }

    bool Game::checkMap(int y_actual, int x_actual) {

        // Check if this position is already previously checked.
        if (Game::arrayToCheck[y_actual][x_actual]) {
            return false;
        }

        // Check vertical limit in the map.
        if (y_actual > (fieldDimentionX - 1) || y_actual < 0) {
            return false;
        }

        // Check horizontal limit in the map.
        if (x_actual > (fieldDimentionY - 2) || x_actual < 0) {
            return false;
        }

        // This check if this point is playable.
        if (Game::arrayToMap[y_actual][x_actual]) {
            return false;
        }

        // Set this point as checked 
        Game::arrayToCheck[y_actual][x_actual] = true;
        
        // Check if this point is the point where is the objective.
        if (objectivePositionX == x_actual && objectivePositionY == y_actual) {
            
            return true;//!(this->distanceBetweenObjects < 5);
        }

        this->distanceBetweenObjects ++;

        if (Game::checkMap(y_actual - 1, x_actual)) {
            return true;
        } else {
            if (Game::checkMap(y_actual, x_actual - 1)) {
                return true;
            } else {
                if (Game::checkMap(y_actual + 1, x_actual)) {
                    return true;
                } else {
                    if (Game::checkMap(y_actual, x_actual + 1)) {
                        return true;
                    } else {
                        return false;
                    }
                }
            }
        }
    }

    void Game::renderMapAndObjects() {

        gameWindow.clear();

        for (int i = 0; i < (fieldDimentionX - 1); i++) {
            for (int j = 0; j < (fieldDimentionY - 2); j++) {
                if (Game::arrayToMap[i][j]) {
                    gameWindow.draw(getWall(i * sizeField, j * sizeField, sf::Color::White));
                }
            }
        }

        playerObject.setPosition(playerPositionX * sizeField, playerPositionY * sizeField);
        objetiveObject.setPosition(objectivePositionX * sizeField, objectivePositionY * sizeField);

        gameWindow.draw(playerObject);
        gameWindow.draw(objetiveObject);
        gameWindow.display();
    }

    void Game::createNewMapAndObjects() {

        bool isValidMap = false;
        while(!isValidMap) {

            distanceBetweenObjects = 0;
            Game::createNewMap();
            Game::setObjectPositions();
            
            isValidMap = Game::checkMap(playerPositionY, playerPositionX);

            if (isValidMap) {
                isMapChecked = true;
            }
        }
    }

    void Game::run() {

        while(gameWindow.isOpen()) {

            if (!isMapGenerated) {
                createNewMapAndObjects();
                renderMapAndObjects();
                isMapGenerated = true;
            }
            processEvents();
        }
    }

int main() {

    Game game;
    game.run();
}

