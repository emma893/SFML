#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <stdlib.h>
#include <iostream>

// std::cout << "EJEMPLO_STRING: "  + std::to_string(EJEMPLO_INT) << std::endl;

sf::RenderWindow window;

sf::Texture groundFieldTexture;
sf::Texture mountainTexture;
sf::Texture wallTexture;

sf::Sprite groundFieldSprite;
sf::Sprite mountainSprite;
sf::Sprite wallSprite;

int xRand = 0;
int yRand = 0;

int campoVertical = 500;
int campoHorizontal = 900;
int sizePiso = 25;

int limiteCampoVertical = campoVertical / sizePiso;
int limiteCampoHorizontal = campoHorizontal / sizePiso;

void renderMountain (bool vectorRender[], sf::Sprite spriteDesiertoBase, sf::Sprite spriteDesierto, sf::RenderWindow &window) {
    int positionAnimation = 0;
    for(size_t i = 1; i < 11; i++) {

        positionAnimation = i * 5;
        for(size_t j = 2; j < limiteCampoHorizontal; j++) {

            if (vectorRender[j]) {

                yRand = j * sizePiso;
                spriteDesiertoBase.setPosition(xRand, yRand - 6);
                window.draw(spriteDesiertoBase);
                spriteDesierto.setTextureRect(sf::IntRect(0, 0, sizePiso, positionAnimation - 6));
                spriteDesierto.setPosition(xRand, yRand - positionAnimation);
                window.draw(spriteDesierto);
            }
        }
        positionAnimation = 0;
        window.display();
        sf::sleep(sf::milliseconds(5));
    }
}

void renderWall (int limiteCampoVertical, int position_x, int position_y, sf::Sprite wallSprite, sf::RenderWindow &window) {

    if (position_x == limiteCampoVertical - 1) {
        wallSprite.setColor(sf::Color(100, 100, 100));
    }

    wallSprite.setTextureRect(sf::IntRect(
        rand() % (100 + 1),
        rand() % (100 + 1), 
        sizePiso, 
        sizePiso
    ));
    wallSprite.setPosition(position_y * sizePiso, position_x * sizePiso);
    window.draw(wallSprite);
}

void setTextures(
    sf::Sprite &groundFieldSprite,
    sf::Sprite &mountainSprite,
    sf::Sprite &wallSprite,
    sf::Texture &groundFieldTexture,
    sf::Texture &mountainTexture,
    sf::Texture &wallTexture) {

    if (!groundFieldTexture.loadFromFile("textures/desert_cracks.png"))
    {
        // error...
    }

    if (!mountainTexture.loadFromFile("textures/desert_mountain.png", sf::IntRect(0, 0, sizePiso, 7)))
    {
        // error...
    }

    if (!wallTexture.loadFromFile("textures/montain_forest.png"))
    {
        // error...
    }

    groundFieldSprite.setTexture(groundFieldTexture);
    mountainSprite.setTexture(mountainTexture);
    wallSprite.setTexture(wallTexture);
}

int main()
{
    // Display windows on black.
    sf::RenderWindow window(sf::VideoMode(campoHorizontal, campoVertical), "Graficos");
    window.clear(sf::Color::Black);
    
    // Setting textures and sprint to render the field.
    setTextures(
        groundFieldSprite,
        mountainSprite,
        wallSprite,
        groundFieldTexture,
        mountainTexture,
        wallTexture
    );

    // line to set random bools.
    srand(time(0));

    // Iterate vertical and horizontal limit to render the field.
    for (int position_x = 0; position_x < limiteCampoVertical; position_x++) {
        for (int position_y = 0; position_y < limiteCampoHorizontal; position_y++) {

            groundFieldSprite.setTextureRect(sf::IntRect(
                rand() % (100 + 1),
                rand() % (100 + 1),
                sizePiso,
                sizePiso
            ));
            
            // Detectar si la posicion es muro.
            bool isXField = (position_x > 0 && position_x < (limiteCampoVertical - 1));
            bool isYField = (position_y > 0 && position_y < (limiteCampoHorizontal - 1));

            if (isXField && isYField) {

                // Rendering ground in the field.
                groundFieldSprite.setPosition((position_y * sizePiso), (position_x * sizePiso));
                window.draw(groundFieldSprite);
            } else {

                // Rendering wall in the field.
                renderWall(limiteCampoVertical, position_x, position_y, wallSprite, window);
            }
        }
    }
    bool vectorRender[limiteCampoHorizontal - 1] = {false};

    window.display();
    sf::sleep(sf::milliseconds(rand() % 50));
    // wallSprite.setColor(sf::Color(255, 255, 255));
    for (int x = 1; x < limiteCampoHorizontal - 1; x++) {
        xRand =  x * sizePiso;
        for (int y = 3; y < limiteCampoVertical; y++) {
            if ((rand() % 10 + 1) % 2 == 0) {
                vectorRender[y] = true;
            } else {
                vectorRender[y] = false;
            }
        }
        renderMountain(vectorRender, mountainSprite, wallSprite, window);
    }
    window.display();

    while(window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
            }
        }
    }
    return 0;
}