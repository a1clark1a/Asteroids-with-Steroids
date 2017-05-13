/*****************************************************************************
*********************** MY FIRST ASTEROID GAME *******************************
*****************************************************************************/

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Game.h"
#include "GameObjects.h"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>

bool fullscreen = false;
int main()
{
	srand(static_cast<unsigned int>(time(0)));
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Welcome to SDVA 103!");		
	Game game;
	game.RespawnPlayer();
	game.AddAmmo(30); // might be too exposed??? Need change maybe??
	
	

	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Time dt = clock.restart();
		sf::Event event;
		while (window.pollEvent(event))
		{
			
			if (event.type == sf::Event::Closed)
				window.close();
		}
		


		window.clear();	
		
		game.Draw(&window);
		game.Update(&window, dt.asSeconds());
		window.display();
	}

	return 0;
}
