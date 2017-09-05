/*****************************************************************************
*********************** MY FIRST ASTEROID GAME *******************************
*****************************************************************************/

#include "stdafx.h"
#include "Game.h"
#include "GameObjects.h"
#include <iostream>


bool fullscreen = false;
int main()
{
	srand(static_cast<unsigned int>(time(0)));
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Welcome to SDVA 103!");		
	Game game;
	

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
