/*****************************************************************************
**********************ENGINE CLASS DEFINITION*******************************
*****************************************************************************/


#include "Game.h"
#include "GameObjects.h"
#include <iostream>
#include <fstream>


Game::Game()
	: m_gameOver(false)
	, m_timeUntilRespawn(2.0f)
	, m_livesRemaining(4)
	, m_superWidth(0)
	, m_menuIsOpen(true)
	
	
{
	m_mainFont.loadFromFile("Fonts/kenpixel_high_square.ttf");
	m_Music.openFromFile("Sound/testSong.ogg");
	m_Music.play();
}



void Game::Draw(sf::RenderWindow * window)
{	

	if (m_menuIsOpen != true)
	{
		//Drawing the Background
		sf::RectangleShape background(sf::Vector2f(1280, 720));
		sf::Texture backgroundTexture;
		backgroundTexture.loadFromFile("Sprites/Backgrounds/star.png");
		background.setTexture(&backgroundTexture);
		window->draw(background);

		// Drawing the SuperBar
		sf::RectangleShape superBar(sf::Vector2f(m_superWidth, 20));
		superBar.setPosition(600, 680);
		window->draw(superBar);

		for (int i = 0; i < m_gameObjects.size(); i++)
		{
			GameObject* current = m_gameObjects[i];
			current->Draw(window);

		}

		// Draw Lives Remaining
		for (int i = 0; i < m_livesRemaining; i++)
		{
			sf::Texture texture;
			texture.loadFromFile("Sprites/PNG/playerShip2_red.png");
			sf::Sprite sprite(texture);
			sprite.setScale(sf::Vector2f(0.4f, 0.4f));
			sprite.setPosition(sf::Vector2f(i * 40, 50));
			window->draw(sprite);

		}

		// Print Score
		sf::Text scoreText;
		scoreText.setFont(m_mainFont);
		scoreText.setString("SCORE: " + std::to_string(m_score));
		scoreText.setCharacterSize(30);
		scoreText.setPosition(10, 10);
		window->draw(scoreText);

		//Print Level
		sf::Text levelText;
		levelText.setFont(m_mainFont);
		levelText.setString("Level: " + std::to_string(m_level));
		levelText.setCharacterSize(40);
		levelText.setPosition(600, 5);
		window->draw(levelText);

		//Print Ammo
		sf::Text ammoText;
		ammoText.setFont(m_mainFont);
		ammoText.setString("Ammo: " + std::to_string(m_ammoRemaining));
		ammoText.setCharacterSize(20);
		ammoText.setPosition(10, 100);
		window->draw(ammoText);

		//Print HighScore
		sf::Text highScoreText;
		highScoreText.setFont(m_mainFont);
		highScoreText.setCharacterSize(20);
		highScoreText.setPosition(1200, 10);
		highScoreText.setString("HighScore \n" + std::to_string(m_HighScore));
		window->draw(highScoreText);

		m_HighScore = ExtractHighScoreFromFile();

		if (m_score > m_HighScore)
		{
			SetScoreOnFile(m_score);
			std::cout << m_HighScore << std::endl;

		}

		if (m_gameOver)
		{
			sf::Text gameOverText;
			gameOverText.setFont(m_mainFont);
			gameOverText.setString("GAME OVER!");
			gameOverText.setCharacterSize(110);
			gameOverText.setPosition(450, 150);
			window->draw(gameOverText);


		}
	}

	else
	{
		sf::Text menuText;
		menuText.setFont(m_mainFont);
		menuText.setString("ASTEROIDS with STEROIDS!");
		menuText.setCharacterSize(80);
		menuText.setColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
		menuText.setPosition(380, 150);
		window->draw(menuText);

		sf::Text startText;
		startText.setFont(m_mainFont);
		startText.setString("Press Enter to Start ");
		startText.setColor(sf::Color(sf::Color(255, 0, 0, rand() % 255)));
		startText.setCharacterSize(50);
		startText.setPosition(500, 350);
		window->draw(startText);

		

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			m_menuIsOpen = false;
		}
	}
}

void Game::Update(sf::RenderWindow * window, float dt)
{

	//Super Ability
	//Need to move to Player class
	m_superIsPressed = false;

	if (!m_gameOver)
	{
		if (m_superWidth < 100)
		{
			SuperBar(dt);
		}
		else
		{
			sf::Text superBarText;
			
			
			
			superBarText.setFont(m_mainFont);
			superBarText.setCharacterSize(20);
			superBarText.setPosition(600, 650);
			superBarText.setString("SUPER BOMB READY!");
			superBarText.setColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
			window->draw(superBarText);
			m_superWidth = 100;



			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
			{
				m_superIsPressed = true;
				m_superWidth = 0;
				PlaySound("Sound/Explosion3.wav");
			}

		}
	}
	

	//Drawing player by spawn time
	if (m_timeUntilRespawn > 0)
	{
		m_timeUntilRespawn -= dt;
		if (m_timeUntilRespawn <= 0)
		{
			Player* player = new Player("Sprites/PNG/playerShip2_red.png", sf::Vector2f(600, 300));
			AddObject(player);
			PlaySound("Sound/Respawn.wav");
		}
	}

	//For every start of level/game spawn asteroids and loop, if player is found/spawned make player invulnerable
	if (m_SpawnAsteroids == true)
	{
		SpawnAsteriods();
		for (int i = m_gameObjects.size() - 1; i >= 0; i--)
		{
			GameObject* current = m_gameObjects[i];
			Player* player = dynamic_cast<Player*>(current);
			if (player != NULL)
			{
				player->MakeInvulnerable();
			}
		}
	}
	
	m_SpawnAsteroids = true;

	//Check and loop gameObjects array and see if any are asteroids, then if super is pressed destroy asteroids
	if (m_SpawnAsteroids == true)
	{
		int count = m_gameObjects.size();
		for (int i = 0; i < count; i++)
		{
			GameObject* current = m_gameObjects[i];
			current->Update(window, dt);
			if (dynamic_cast<Asteroid*>(current) != NULL)
			{
				m_SpawnAsteroids = false;
				
				if (m_superIsPressed == true)
				{
							
					(current)->Destroy();
					//std::cout << "I am triggered" << std::endl;
					m_superWidth = 0;
				}
			}

		}
		
		m_superIsPressed = false;
	}
	
	
		// See if any collisions occurred
		for (int i = 0; i < m_gameObjects.size(); i++)
		{
			GameObject* current = m_gameObjects[i];
			for (int j = 0; j < m_gameObjects.size(); j++)
			{
				GameObject* other = m_gameObjects[j];
				if (current->IsColliding(other))
				{
					current->CollidedWith(other);
				}
			}
		}

		//Check for all objects and if any is returns destroyed to true, delete from array
		for (int i = m_gameObjects.size() - 1; i >= 0; i--)
		{
			
			GameObject* current = m_gameObjects[i];
			if (current->IsDestroyed())
			{
				delete current;
				m_gameObjects.erase(m_gameObjects.begin() + i);
				
			}
		}

	

	
}


//Function for adding objects in game
void Game::AddObject(GameObject * object)
{
	object->SetOwner(this);
	m_gameObjects.push_back(object);
}

//Function to spawnplayer
void Game::RespawnPlayer()
{
	if (m_livesRemaining > 0)
	{
		m_livesRemaining--;
		m_timeUntilRespawn = 2.0f;
	}
	else 
	{
		GameOver();
	}
}



void Game::SetScore(int scoreValue)
{
	m_score += scoreValue;
}

void Game::SetSpawnCount(int m_level)
{
	m_spawnCount = 4 + m_level;
}

int Game::GetSpawnCount()
{
	return m_spawnCount;
}


//Spawn asteroids every level, increasing no of spawns by level
void Game::SpawnAsteriods()
{
	m_level += 1;
	PlaySound("Sound/Bounce.wav");
	SetSpawnCount(m_level);
	for (int i = 0; i < GetSpawnCount(); i++)
	{
		Asteroid* newAsteroid;
		if (rand() % 2 == 0)
		{
			newAsteroid = new SmallAsteroid(sf::Vector2f(rand() % 1000 + 100, rand() % 600 + 50));
		}
		else
		{
			newAsteroid = new LargeAsteroid(sf::Vector2f(rand() % 1000 + 100, rand() % 600 + 50));
		}
		
		
		newAsteroid->SetAngle(rand() % 360);
		newAsteroid->SetVelocity(150);
		AddObject(newAsteroid);
	}
	//std::cout << "number of spawn: " << GetSpawnCount() << std::endl;
	m_SpawnAsteroids = false;
}


//Function to play sound
void Game::PlaySound(std::string soundPath)
{
	m_buffer.loadFromFile(soundPath);
	m_Sound.setBuffer(m_buffer);
	m_Sound.play();
}

void Game::SuperBar(float dt)
{
	m_superWidth += 10 * dt;
	//std::cout << m_superWidth << std::endl;
	
}


//Function that outputs score into a textfile
void Game::SetScoreOnFile(int scoreValue)
{
	std::ofstream myfile("HighScore.txt");
	if (myfile.is_open())
	{
		myfile << scoreValue << std::endl;
	}
	else std::cout << "Unable to open file" << std::endl;
}

//Function that extracts score from a textfile
int Game::ExtractHighScoreFromFile()
{
	std::string line;
	int score;
	std::ifstream myfile("HighScore.txt");
	if (myfile.is_open())
	{
		std::getline(myfile, line);
		score = std::stoi(line);
		
		myfile.close();
	}

	return score;
}


//Ammo function handler
void Game::DecreaseAmmo()
{
	m_ammoRemaining--;
}

void Game::AddAmmo(int ammo)
{
	m_ammoRemaining += ammo;
	
}


void Game::SetBackgroundSize(sf::Vector2f(size))
{
	
}