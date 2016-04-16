
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <stdint.h>
#include <iostream>

#include "TileSystem.hpp"
#include "Game.hpp"
#include "GlobalState.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1440, 768), "SFML works!");

    window.setFramerateLimit(120);

    GlobalState curIn;

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color(0,0,255,128));

    /*sf::RectangleShape shape2(sf::Vector2f(200.f,200.f));
    shape2.setFillColor(sf::Color::Red);
    shape2.setPosition(100.f,100.f);*/

    sf::View view;

    sf::Clock clock;
    clock.restart();

    sf::Clock FPS;
    float lastTime = 0;

    Game game;

    view.reset(sf::FloatRect(-200 + clock.getElapsedTime().asSeconds()*100.f, 100, window.getSize().x, window.getSize().y));
    view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

    if (game.map.loadTextures("mapdata/Textures.map")) {
        std::cout << "Could not load textures for map!\n";
        return 0;
    }

    if (game.map.loadMap("mapdata/Test512RGBBl.png")) {
        std::cout << "Could not load textures for map!\n";
        return 0;
    }

    // Test object.

    game.loadMOBTemplate("MRAP", ".png");

    MobileObject TestMOB;
    TestMOB.base = &(game.MOBTemplates[0]);
    TestMOB.position = sf::Vector2f(200,200);
    TestMOB.baseIndex = 0;

    Player TestPlayer("Player1",sf::Color::Red);
    TestPlayer.MOBs.push_back(TestMOB);
    TestPlayer.isLocal = true;

    TestPlayer.productionOptions.push_back("UnitProductionOrSomething");
    TestPlayer.productionOptions.push_back("Anotherone");
    TestPlayer.productionOptions.push_back("PlaceholderValue");
    TestPlayer.productionOptions.push_back("I'mOutOfIdeas!");
    TestPlayer.productionOptions.push_back("SoAManWalksIntoABar");
    TestPlayer.productionOptions.push_back("MRAP");

    TestPlayer.selectedUnits.push_back(0);

    game.players.push_back(TestPlayer);
    //game .push_back(TestMOB);


    curIn.viewport = sf::FloatRect(100.f,100.f,window.getSize().x, window.getSize().y);


    while (window.isOpen()) {
        sf::Event event;

        curIn.LMBPressed = false;
        curIn.RMBPressed = false;
        curIn.scroll = 0;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                curIn.LMBPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
                curIn.RMBPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
            }
        }

        window.clear();

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            curIn.viewport.left-=5;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            curIn.viewport.left+=5;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            curIn.viewport.top-=5;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            curIn.viewport.top+=5;
        }

        view.reset(curIn.viewport);

        window.setView(view);

        // RENDER game-world
        game.render(window);

        window.setView(window.getDefaultView());

        // RENDER UI.

        game.renderUI(window,curIn);

        //window.draw(shape2);
        window.draw(shape);

        window.display();

        game.update(clock);


        float deltaT = FPS.restart().asSeconds();
        //std::cout << 1.f/(deltaT) << '\n';
    }

    return 0;
};


