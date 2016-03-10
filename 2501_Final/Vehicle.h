#pragma once

#include "GameObject.h"
//#include "Player.h"

class Player;

/*					Vehicle
	Entities visible on screen, can be interacted 
	with to get in (Trigger?). Basically a storage
	class - holds different accelerations and data
	on movement. Also stores the textures of the 
	ship.
*/	

class Vehicle : public Entity {
public:
	virtual ~Vehicle() {}

	// update is written by each ship
	virtual void update(const sf::Time&) = 0;	// from Updatable

	virtual void onCollide(const Collidable& other);	// from Collide

	float getAcceleration();
	float getTopSpeed();
	float getRotationSpeed();

	void enter(Player* pass);

protected:
	float accel, topSpeed, rotateSpeed;
	sf::Texture shipTexture;
	Player* passenger;
	sf::Sprite ship;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;	// from Drawable
};

class BasicShip : public Vehicle {
public:
	BasicShip();
	~BasicShip();

	void update(const sf::Time&);
};