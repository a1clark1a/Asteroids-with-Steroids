#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameObjects.h"

class Game
{
public:
	Game();
	void Draw(sf::RenderWindow* window);
	void Update(sf::RenderWindow * window, float dt);
	void AddObject(GameObject* object);
	void RespawnPlayer();
	void SetScore(int scoreValue);
	void SetSpawnCount(int m_level);
	void SpawnAsteriods();
	void PlaySound(std::string soundPath);
	void SuperBar(float dt);
	void GameOver() { m_gameOver = true;  }
	void SetScoreOnFile(int scoreValue);
	void DecreaseAmmo();
	void AddAmmo(int ammo);
	int GetAmmoRemaining() { return m_ammoRemaining; }
	void SetBackgroundSize(sf::Vector2f(size));
	int ExtractHighScoreFromFile();

	virtual int GetSpawnCount();
	


	

private:
	std::vector<GameObject*> m_gameObjects;
	bool m_gameOver;
	int m_livesRemaining;
	int m_ammoRemaining;
	float m_timeUntilRespawn;
	sf::Font m_mainFont;
	int m_score;
	int m_HighScore;
	int m_spawnCount;
	int m_level = 0;
	bool m_SpawnAsteroids;
	bool m_superIsPressed;
	bool m_menuIsOpen;
	sf::Music m_Music;
	sf::SoundBuffer m_buffer;
	sf::Sound m_Sound;
	float m_superWidth;

};