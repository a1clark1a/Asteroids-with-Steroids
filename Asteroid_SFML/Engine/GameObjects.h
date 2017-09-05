#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#define DEG_TO_RAD (0.0174532925f)

class Game;

class GameObject
{
public:
	GameObject(std::string texturePath, const sf::Vector2f& pos);
	virtual void Draw(sf::RenderWindow* window);
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void LimitVelocity(float dt);
	virtual void ApplyDrag(float dt);

	void SetAngle(float angle);
	float GetAngle();
	
	void SetPos(const sf::Vector2f& pos);
	void SetAccel(float amount);
	void SetVelocity(float amount);
	void SetOwner(Game* owner);
	virtual void Destroy();
	bool IsDestroyed();

	void SetCollisionRadius(float val) { m_collisionRadius = val;  }
	float GetCollisionRadius() { return m_collisionRadius; }

	bool IsColliding(GameObject* other);
	virtual void CollidedWith(GameObject* other);
	sf::Sprite m_sprite;

protected:
	sf::Vector2f m_pos;
	sf::Vector2f m_vel;
	sf::Vector2f m_accel;

	sf::Texture m_texture;

	Game* m_owner;
	float m_angle;
	float m_collisionRadius;
	bool m_destroyed;
};

class Bullet : public GameObject
{
public:
	Bullet(const sf::Vector2f& pos);	
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void ApplyDrag(float dt) {};
	virtual void CollidedWith(GameObject* other);
	float m_timeAlive;
};


class Player : public GameObject
{
public:
	Player(std::string texturePath, const sf::Vector2f& pos);
	
	virtual void Draw(sf::RenderWindow* window);
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void CollidedWith(GameObject* other);
	virtual void MakeInvulnerable();
	
	
private:
	bool m_firing;
	float m_fireCooldown;
	float m_invulnerableTime;

	
};

class Asteroid : public GameObject
{
public:
	Asteroid(std::string texturePath, const sf::Vector2f& pos);
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void ApplyDrag(float dt) {};
	sf::Vector2f GetPosition() { return m_pos; }
	int m_scoreValue;
	
private:
	float m_rotationRate;
	
};

class SmallAsteroid : public Asteroid
{
public:
	SmallAsteroid(const sf::Vector2f& pos);
	virtual void Destroy();
	
};

class MediumAsteroid : public Asteroid
{
public:
	MediumAsteroid(const sf::Vector2f& pos);
	virtual void Destroy();

};

class LargeAsteroid : public Asteroid
{
public:
	LargeAsteroid(const sf::Vector2f& pos);
	virtual void Update(sf::RenderWindow* window, float dt);
	virtual void Destroy();
	
private:
	int m_ammoPackLifeTime;
};

class AmmoPack : public GameObject
{
public:
	AmmoPack(std::string texturePath, const sf::Vector2f& pos);
	virtual void Update(sf::RenderWindow* window, float dt);
	virtual void ApplyDrag(float dt) {};
	virtual void CollidedWith(GameObject * other);
	virtual void Destroy();
	


private:
	int m_ammoCount;
	float m_ammoPackLifeTime;
};


class Coin : public GameObject
{
public:
	Coin(std::string texturePath, const sf::Vector2f& pos);
	virtual void Update(sf::RenderWindow* window, float dt);
	virtual void ApplyDrag(float dt) {};
	virtual void Destroy();
	
	int m_scoreValue;

private:
	float m_coinLifeTime;

};

class GoldCoin : public Coin
{
public:
	GoldCoin(const sf::Vector2f& pos);
	virtual void CollidedWith(GameObject * other);
	
};

class SilverCoin : public Coin
{
public:
	SilverCoin(const sf::Vector2f& pos);
	virtual void CollidedWith(GameObject * other);
	

};