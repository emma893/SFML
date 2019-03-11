#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <stdlib.h>
#include <iostream>

sf::Texture texture1;
sf::Texture texture2;
sf::Texture texture4;

sf::RenderWindow window2;

sf::Sprite spriteDesierto1;
sf::Sprite spriteDesierto2;
sf::Sprite spriteDesierto4;

int xRand = 0;
int yRand = 0;

int campoVertical = 500;
int campoHorizontal = 900;
int sizePiso = 25;

int limiteCampoVertical = campoVertical / sizePiso;
int limiteCampoHorizontal = campoHorizontal / sizePiso;

void functionRender (bool vectorRender[], sf::Sprite spriteDesiertoBase, sf::Sprite spriteDesierto, sf::RenderWindow &window) {
    int positionAnimation = 0;
    for(size_t i = 1; i < 11; i++)
    {
        positionAnimation = i * 5;
        for(size_t j = 2; j < limiteCampoHorizontal; j++)
        {
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

int main()
{
    sf::RenderWindow window2(sf::VideoMode(campoHorizontal, campoVertical), "My window");
    window2.clear(sf::Color::Black);

    if (!texture1.loadFromFile("../textures/desert_cracks.png"))
    {
        // error...
    }

    if (!texture4.loadFromFile("../textures/montain_forest.png"))
    {
        // error...
    }

    if (!texture2.loadFromFile("../textures/desert_mountain.png", sf::IntRect(0, 0, sizePiso, 7)))
    {
        // error...
    }

    spriteDesierto1.setTexture(texture1);
    spriteDesierto2.setTexture(texture2);
    spriteDesierto4.setTexture(texture4);

    std::cout << "acho: "  + std::to_string(campoHorizontal) + ". alto: "  + std::to_string(campoVertical) << std::endl;

    for (int x = 0; x < limiteCampoVertical; x++) {
        for (int y = 0; y < limiteCampoHorizontal; y++) {
            spriteDesierto1.setTextureRect(sf::IntRect(rand() % 100 + 1, rand() % 100 + 1, sizePiso, sizePiso));
            std::cout << "Y: " + std::to_string(y) + ". X: " + std::to_string(x) << std::endl;
            if ((x > 0 && x < limiteCampoVertical - 1) && (y > 0 && y < limiteCampoHorizontal - 1)) {
                spriteDesierto1.setPosition(y * sizePiso, x * sizePiso);
                window2.draw(spriteDesierto1);
            } else {
                if (x == limiteCampoVertical - 1) {
                    spriteDesierto4.setColor(sf::Color(100, 100, 100));
                }else {
                    spriteDesierto4.setColor(sf::Color(255, 255, 255));
                }
                spriteDesierto4.setTextureRect(sf::IntRect(rand() % 100 + 1, rand() % 100 + 1, sizePiso, sizePiso));
                spriteDesierto4.setPosition(y * sizePiso, x * sizePiso);
                window2.draw(spriteDesierto4);
            }
        }
    }
    bool vectorRender[limiteCampoHorizontal - 1] = {false};
    
    window2.display();
    sf::sleep(sf::milliseconds(rand() % 50));
    spriteDesierto4.setColor(sf::Color(255, 255, 255));
    for (int x = 1; x < limiteCampoHorizontal - 1; x++) {
        xRand =  x * sizePiso;
        for (int y = 3; y < limiteCampoVertical; y++) {
            if ((rand() % 10 + 1) % 2 == 0) {
                vectorRender[y] = true;
            } else {
                vectorRender[y] = false;
            }
        }
        functionRender(vectorRender, spriteDesierto2, spriteDesierto4, window2);
    }
    window2.display();

    while(window2.isOpen()) {
        sf::Event event;
        while (window2.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window2.close();
        }
    }
    return 0;
}