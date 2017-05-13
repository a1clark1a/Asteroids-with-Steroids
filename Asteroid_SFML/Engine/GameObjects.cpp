#include "stdafx.h"
#include "Game.h"
#include "GameObjects.h"
#include <iostream>
/*****************************************************************************
**********************GAMEOBJECT CLASS DEFINITION*******************************
*****************************************************************************/


GameObject::GameObject(std::string texturePath, const sf::Vector2f & pos)
	: m_pos(pos)
	, m_destroyed(false)
	, m_collisionRadius(0)
{		
	m_texture.loadFromFile(texturePath);
	m_sprite.setTexture(m_texture);
	m_sprite.setOrigin(m_sprite.getTextureRect().width * 0.5f, m_sprite.getTextureRect().height * 0.5f);
}

void GameObject::Destroy()
{
	m_destroyed = true;
}

bool GameObject::IsDestroyed()
{
	return m_destroyed;
}

bool GameObject::IsColliding(GameObject* other)
{
	bool result = false;
	sf::Vector2f vecToOther = other->m_pos - m_pos;
	float dist = sqrt(vecToOther.x * vecToOther.x + vecToOther.y  *vecToOther.y);
	if (this != other &&
		!IsDestroyed() &&
		!other->IsDestroyed() &&
		dist < (m_collisionRadius + other->m_collisionRadius))
	{
		result = true;
	}
	return result;
}

void GameObject::CollidedWith(GameObject * other)
{
}

void GameObject::Draw(sf::RenderWindow * window)
{
	sf::CircleShape collisionRadius;
	collisionRadius.setRadius(m_collisionRadius);
	collisionRadius.setOutlineThickness(2);
	collisionRadius.setFillColor(sf::Color::Transparent);
	collisionRadius.setPosition(m_pos);
	collisionRadius.setOrigin(m_collisionRadius, m_collisionRadius);
	collisionRadius.setOutlineColor(sf::Color::Red);
	//window->draw(collisionRadius);
	window->draw(m_sprite);
}

void GameObject::Update(sf::RenderWindow * window, float dt)
{
	float decelScalar = 0.5f;
	m_vel += m_accel * dt;
	
	LimitVelocity(dt);
	ApplyDrag(dt);

	m_pos += m_vel * dt;
	if (m_pos.x < 0)
	{
		m_pos.x = window->getSize().x;
	}
	else if (m_pos.x > window->getSize().x)
	{
		m_pos.x = 0;
	}

	if (m_pos.y < 0)
	{
		m_pos.y = window->getSize().y;
	}
	else if (m_pos.y > window->getSize().y)
	{
		m_pos.y = 0;
	}


	m_sprite.setRotation(m_angle);
	m_sprite.setPosition(m_pos);
}

void GameObject::LimitVelocity(float dt)
{
	float speed = sqrt(m_vel.x * m_vel.x + m_vel.y  *m_vel.y);	

	if (speed <= 0.1)
	{
		return;
	}

	sf::Vector2f normalizedVel = sf::Vector2f(m_vel.x / speed, m_vel.y / speed);

	if (speed > 500)
	{
		speed = 500;
	}

	m_vel.x = normalizedVel.x * speed;
	m_vel.y = normalizedVel.y * speed;
}

void GameObject::SetAngle(float angle)
{
	m_angle = angle;
}

float GameObject::GetAngle()
{
	return m_angle;
}

void GameObject::SetPos(const sf::Vector2f & pos)
{
	m_pos = pos;
}

void GameObject::SetAccel(float amount)
{	
	if (amount > 0)
	{
		float rotInRadians = DEG_TO_RAD * m_angle;
		m_accel = sf::Vector2f(amount * sin(rotInRadians), -amount * cos(rotInRadians));
	}
	else
	{
		m_accel = sf::Vector2f(0, 0);
	}
}

void GameObject::SetVelocity(float amount)
{
	if (amount > 0)
	{
		float rotInRadians = DEG_TO_RAD * m_angle;
		m_vel = sf::Vector2f(amount * sin(rotInRadians), -amount * cos(rotInRadians));
	}
	else
	{
		m_vel = sf::Vector2f(0, 0);
	}
}

void GameObject::SetOwner(Game * owner)
{
	m_owner = owner;
}

void GameObject::ApplyDrag(float dt)
{
	if (m_accel.x == 0 && m_accel.y == 0)
	{
		float dragAmount = dt * 0.9f;
		m_vel.x -= dragAmount* m_vel.x;
		m_vel.y -= dragAmount * m_vel.y;
	}
}

/*****************************************************************************
**********************BULLET CLASS DEFINITION*******************************
*****************************************************************************/

Bullet::Bullet(const sf::Vector2f & pos)
	: GameObject("Sprites/PNG/Lasers/laserBlue08.png", pos)
	, m_timeAlive(0.0f)
{
	m_sprite.setScale(0.3, 0.3);
	SetCollisionRadius(10);
}

void Bullet::Update(sf::RenderWindow * window, float dt)
{
	GameObject::Update(window, dt);
	m_angle += dt * 360;
	m_timeAlive += dt;

	if (m_timeAlive > 1.0f)
	{
		Destroy();
	}
}

void Bullet::CollidedWith(GameObject * other)
{
	Asteroid* asteroid = dynamic_cast<Asteroid*>(other);
	if (asteroid)
	{
		Destroy();
		other->Destroy();
	}	
}

/*****************************************************************************
**********************PLAYER CLASS DEFINITION*******************************
*****************************************************************************/


Player::Player(std::string texturePath, const sf::Vector2f & pos)
	: GameObject(texturePath, pos)
	, m_firing(false)
	, m_fireCooldown(0.0f)
	, m_invulnerableTime(3.0f)
	
	
	
{
	SetCollisionRadius(30);
	m_sprite.setScale(0.7, 0.7);
	
}

void Player::Draw(sf::RenderWindow* window)
{
	GameObject::Draw(window);
	
	if (m_invulnerableTime > 0.0f)
	{
		sf::CircleShape shape;
		shape.setRadius(40);
		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineThickness(10 * m_invulnerableTime/3.0f + 1);
		sf::Color color = sf::Color::Blue;
		color.a = 255 * m_invulnerableTime / 3.0f;
		shape.setOutlineColor(color);
		shape.setOrigin(40, 40);
		shape.setPosition(m_pos.x, m_pos.y);
		window->draw(shape);
		
	}

	if (m_invulnerableTime > 0)
	{
		float sinValue = sin(m_invulnerableTime * 25); // Between -1 and 1
		sinValue += 1; // Between 0-2
		sinValue *= 0.5; // Between 0-1	
		float alpha = sinValue * 255; // 0 -255
		sf::Color shipColor = sf::Color::White;
		shipColor.a = alpha;
		m_sprite.setColor(shipColor);
		//std::cout << "Sin is " << sinValue << std::endl;
	}
	else
	{
		m_sprite.setColor(sf::Color::White);
	}
}

void Player::Update(sf::RenderWindow * window, float dt)
{
	GameObject::Update(window, dt);
	m_fireCooldown -= dt;
	m_invulnerableTime -= dt;
	m_firing = false;
	

	SetAccel(0.0f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		SetAngle(GetAngle() + 180 * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		SetAngle(GetAngle() - 180 * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		SetAccel(400.0f);
	}

	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		m_firing = true;	
		
	}

	
	

	if (m_firing && m_fireCooldown <= 0.0f && m_invulnerableTime < 0.0f)
	{
		bool tripleShot =sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
		m_fireCooldown = tripleShot ? 0.5f : 0.2f;
		
		if (tripleShot && m_owner->GetAmmoRemaining() > 0)
		{
			for (int i = 0; i < 3; i++)
			{
				Bullet* bullet = new Bullet(m_pos);
				bullet->SetAngle(m_angle - 15 + i*15);
				bullet->SetVelocity(800);
				m_owner->AddObject(bullet);		
				m_owner->PlaySound("Sound/Laser_Shoot1.wav");
				m_owner->DecreaseAmmo();
			}

		}
		else
		{
			Bullet* bullet = new Bullet(m_pos);
			bullet->SetAngle(m_angle);
			bullet->SetVelocity(500);
			m_owner->AddObject(bullet);
			m_owner->PlaySound("Sound/Laser_Shoot1.wav");
			
		}
	}
}



void Player::CollidedWith(GameObject * other)
{
	Asteroid* asteroid = dynamic_cast<Asteroid*>(other);
	if (asteroid && m_invulnerableTime < 0.0f)
	{
		Destroy();
		m_owner->PlaySound("Sound/Hit_Hurt.wav");
		m_owner->RespawnPlayer();	
	}
}


void Player::MakeInvulnerable()
{
	m_invulnerableTime = 3.0f;
}





/*****************************************************************************
**********************ASTEROID CLASS DEFINITION*******************************
*****************************************************************************/

Asteroid::Asteroid(std::string texturePath, const sf::Vector2f& pos)
	: GameObject(texturePath, pos)
{
	m_rotationRate = rand()% 45 + 45; // between 45 - 90
	m_rotationRate *= rand() % 2 == 0 ? 1 : -1;		
	
}

void Asteroid::Update(sf::RenderWindow * window, float dt)
{
	m_angle += dt * m_rotationRate;
	GameObject::Update(window, dt);
	
}




SmallAsteroid::SmallAsteroid(const sf::Vector2f & pos)
	: Asteroid("Sprites/PNG/Meteors/MeteorBrown_small1.png", pos)
{
	m_scoreValue = 20;
	SetCollisionRadius(10);
}

void SmallAsteroid::Destroy()
{
	GameObject::Destroy();
	m_owner->PlaySound("Sound/Explosion.wav");
	m_owner->SetScore(m_scoreValue);
}

MediumAsteroid::MediumAsteroid(const sf::Vector2f & pos)
	: Asteroid("Sprites/PNG/Meteors/MeteorBrown_med1.png", pos)
{
	m_scoreValue = 50;
	SetCollisionRadius(30);
}

void MediumAsteroid::Destroy()
{
	GameObject::Destroy();
	m_owner->PlaySound("Sound/Explosion.wav");
	m_owner->SetScore(m_scoreValue);
	for (int i = 0; i < 3; i++)
	{
		SmallAsteroid* smallAsteroid = new SmallAsteroid(m_pos);
		smallAsteroid->SetAngle(rand() % 360);
		smallAsteroid->SetVelocity(100);
		m_owner->AddObject(smallAsteroid);
		
	}
}


LargeAsteroid::LargeAsteroid(const sf::Vector2f & pos)
	: Asteroid("Sprites/PNG/Meteors/MeteorBrown_big1.png", pos)
{
	m_scoreValue = 100;
	SetCollisionRadius(50);
}

void LargeAsteroid::Update(sf::RenderWindow* window, float dt)
{
	GameObject::Update(window, dt);
	
}

void LargeAsteroid::Destroy()
{
	GameObject::Destroy();
	m_owner->PlaySound("Sound/Explosion.wav");
	m_owner->SetScore(m_scoreValue);
	AmmoPack* ammoPack = new AmmoPack("Sprites/PNG/Power-ups/powerupBlue_star.png", GetPosition());
	m_owner->AddObject(ammoPack);
	
	
	

	for (int i = 0; i < 3; i++)
	{
		MediumAsteroid* medAsteroid = new MediumAsteroid(m_pos);
		medAsteroid->SetAngle(rand() % 360);
		medAsteroid->SetVelocity(100);
		m_owner->AddObject(medAsteroid);
		
		
	}
}
/*****************************************************************************
**********************AMMOPACK CLASS DEFINITION*******************************
*****************************************************************************/

AmmoPack::AmmoPack(std::string texturePath, sf::Vector2f& pos)
	: GameObject(texturePath, pos)
{
	SetCollisionRadius(30);
	m_ammoCount = rand() % 5 + 10;
	SetVelocity(10);
	m_ammoPackLifeTime = 2.0f;
}

void AmmoPack::Update(sf::RenderWindow* window, float dt)
{
	GameObject::Update(window, dt);
	m_ammoPackLifeTime -= dt;
	std::cout << "ammo : " << m_ammoPackLifeTime << std::endl;
	if (m_ammoPackLifeTime  <= 0)
	{
		Destroy();
		m_ammoPackLifeTime = 2.0f;
	}
	
}

void AmmoPack::Destroy()
{
	GameObject::Destroy();
	
}

void AmmoPack::CollidedWith(GameObject * other)
{
	Player * player = dynamic_cast<Player*>(other);
	if (player)
	{
		Destroy();
		m_owner->AddAmmo(m_ammoCount);
		m_owner->PlaySound("Sound/PowerUp.wav");
	}

}

