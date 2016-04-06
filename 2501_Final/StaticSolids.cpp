#include "StaticSolids.h"

Wall::Wall(vec::Vector2 _topleft, vec::Vector2 _bottomright, sf::Texture* _tex)
{
	wallSprite.setTexture(*_tex);

	sf::IntRect texRect;
	texRect.left = 0;
	texRect.top = 0;
	texRect.width = _bottomright.getX() - _topleft.getX();
	texRect.height = _bottomright.getY() - _topleft.getY();
	wallSprite.setTextureRect(texRect);
	wallSprite.setOrigin(((_bottomright - _topleft) / 2.0f).getSfVec());

	Rect* r = new Rect(_topleft, _bottomright);
	col.addShape(r);

	setPosition(_topleft + ((_bottomright - _topleft)/2.0f));
	
	setSolid(true);
	setStatic(true);
}

Wall::~Wall() {}

void Wall::setPosition(vec::Vector2 newPos)
{
	GameObject::setPosition(newPos);
	wallSprite.setPosition(newPos.getSfVec());
}

void Wall::setRotation(double _rotation)
{
	GameObject::setRotation(_rotation);
	wallSprite.setRotation(_rotation);
}

void Wall::onCollide(Collidable& other)
{

}


void Wall::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(wallSprite, states);
	if (Global::DEBUG)
		target.draw(col, states);
}



//    PLAYER SHIELD (Basically just a wall)    //

PlayerShield::PlayerShield(vec::Vector2 _topleft, vec::Vector2 _bottomright, sf::Texture* _tex) 
	: Wall(_topleft, _bottomright, _tex)
{
	setStatic(false);
	setSolid(false);
}

PlayerShield::~PlayerShield() {}


void PlayerShield::onCollide(Collidable& other)
{
	// If a player is trying to pass through, block them
	if (other.getTag() == sf::String("Player"))
	{
		// Get the normal
		vec::Vector2 normal;

		Collidable::collide(other, *this, normal);
		Collidable::collideBody(other, normal);
	}
}






//    TILE (Just a drawable thing)    //

Tile::Tile(sf::Texture* tex_, std::vector<vec::Vector2>& points_, vec::Vector2& position_)
	: tileShape(sf::TrianglesStrip, points_.size()),
	tileTex(tex_)
{
	tileTex->setRepeated(true);

	for (auto it = points_.begin(); it != points_.end(); ++it)
	{
		sf::Vector2f vPos = (*it + position_).getSfVec();

		sf::Vertex v(vPos, (*it).getSfVec());
	}
}

Tile::~Tile() {}



void Tile::setPosition(vec::Vector2 newPos)
{
	sf::Vector2f dif = (newPos - getPosition()).getSfVec();
	GameObject::setPosition(newPos);

	for (int i = 0; i < tileShape.getVertexCount(); ++i)
	{
		tileShape[i].position += dif;
	}
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = tileTex;
	target.draw(tileShape, states);
}