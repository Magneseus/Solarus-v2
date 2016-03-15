#include "Player.h"

Player::Player()
{
	vel = vec::Vector2(0,0);

	vehicle = new TransportShip();

	addObjectStatic(vehicle);

	vehicle = new BasicShip();
	vehicle->setPosition(vec::Vector2(200, 250));
	vehicle->vel = vec::Vector2(10, 10);

	addObjectStatic(vehicle);

	Turret* turret = new Turret(vec::Vector2(500, 100), new Weapon(0, 50, 250), 180, 360);
	addObjectStatic(turret);

	vehicle = NULL;

	spawn();

	vehicleEnterTime = sf::seconds(1);

	playerTexture.loadFromFile("img/player.png");
	player.setTexture(playerTexture);
	
	player.setOrigin(25, 25);

	position = vec::Vector2(300, 300);
	rotation = 0;

	accelRate = onFootAccel;
	topSpeed = onFootTopSpeed;
	rotateSpeed = onFootRotateSpeed;

	// Add a collison box for player
	Rect* r = new Rect(vec::Vector2(-20, -20), vec::Vector2(20, 20));
	col.addShape(r);
	setSolid(true);

	// Set collision box tag
	setTag(sf::String("Player"));
}

Player::~Player()
{

}

void Player::update(const sf::Time& delta) {

	motion = STILL;
	turning = STILL;
	strafe = STILL;

	if (vehicle == NULL) {
		vel.setX(0);
		vel.setY(0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		if (vehicle == NULL) {
			vel.setY(-100);
		}
		else {
			motion = FORWARD;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		if (vehicle == NULL) {
			vel.setY(100);
		}
		else {
			motion = REVERSE;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		if (vehicle == NULL) {
			vel.setX(100);
		}
		else {
			strafe = RIGHT;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		if (vehicle == NULL) {
			vel.setX(-100);
		}
		else {
			strafe = LEFT;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
		brakesOn = true;
	}
	else {
		brakesOn = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) && 
			vehicleEnterCooldown.getElapsedTime() > vehicleEnterTime)
	{
		exitVehicle();
	}

	Entity::update(delta);

	player.setPosition(position.getX(), position.getY());

	if (vehicle != NULL) {
		vehicle->setPosition(position);
		vehicle->setRotation(rotation);
		vehicle->update(delta);
	}

	player.setRotation(rotation);

	// Set the textures
	if (motion == FORWARD) {
		//shipTexture = shipThrusting;
	}
	else {
		//shipTexture = shipStill;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		if (vehicle == NULL) {
			// on foot shoot
			currentWeapon->shoot(toRadians(rotation), position);
		}
		else {
			// in ship shoot
			currentLoadout->primary->shoot(toRadians(rotation), position);
		}
	} else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
		if (vehicle == NULL) {
			switchWeapons();
		}
		else {
			currentLoadout->secondary->shoot(toRadians(rotation), position);
		}
	}
}

void Player::enterVehicle(Vehicle* v) {
	if (v != NULL) {
		remObjectStatic(v);
	}
	else { return; }	// if it's not a vehicle, don't try to use it

	vehicle = v;

	rotation = v->ship.getRotation();
	position = v->getPosition();

	accelRate = v->getAcceleration();
	topSpeed = v->getTopSpeed();
	rotateSpeed = v->getRotationSpeed();
	dragValue = v->getDragValue();

	switchLoadouts(v->weapons);

	vehicleEnterCooldown.restart();
}

void Player::exitVehicle() {
	if (vel.getMag() < 50) {

		addObjectStatic(vehicle);

		vehicle = NULL;
		vel.setX(0);
		vel.setY(0);

		accelRate = onFootAccel;
		topSpeed = onFootTopSpeed;
		rotateSpeed = onFootRotateSpeed;
		dragValue = 0;

		switchLoadouts(onFootLoadout);

		vehicleEnterCooldown.restart();
	}
}

void Player::switchLoadouts(Loadout* newest) {
	currentLoadout = newest;
	currentWeapon = currentLoadout->primary;
}

void Player::switchWeapons() {
	if (currentWeapon == currentLoadout->primary) {
		currentWeapon = currentLoadout->secondary;
	} else {
		currentWeapon = currentLoadout->primary;
	}
}

void Player::spawn() {
	// deal with reseting player here
	// called at the beginning of game/round?

	onFootLoadout = new Loadout();
	onFootLoadout->primary = new Weapon(1, 10, 350);
	onFootLoadout->secondary = new Weapon(2, 10, 500);

	switchLoadouts(onFootLoadout);
}

void Player::onCollide(Collidable& other)
{
	std::cout << "COLLIDE!: " << other.getTag().toAnsiString() << std::endl;
	if (other.getTag() == "Vehicle" && 
			vehicle == NULL && 
			vehicleEnterCooldown.getElapsedTime() > vehicleEnterTime)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		{
			enterVehicle(dynamic_cast<Vehicle*>(&other));
		}
	}
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//states.transform.rotate(bearing, pos.getX(), pos.getY());

	if (vehicle != NULL) {
		target.draw(*vehicle, states);
	} else {
		target.draw(player, states);
	}
	// Draw collision box
	target.draw(col, states);
}