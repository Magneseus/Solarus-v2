#include "Model.h"

Model::Model()
{

}

Model::~Model()
{
	
}

void Model::update(const sf::Time& delta)
{
	// Check collidables for invalid objects
	for (auto it = collidables.begin(); it != collidables.end(); )
	{
		if ((*it)->removeFunc())
		{
			(*it)->andType(0b110);
			it = collidables.erase(it);
		}
		else
			++it;
	}

	// Collide all objects
	for (auto colA = collidables.begin(); colA != collidables.end(); ++colA)
	{
		// Otherwise, collide with all other objects
		for (auto colB = colA; colB != collidables.end(); ++colB)
		{
			// If they are determined to be colliding, call the callback function for both
			if (Collidable::collide(**colA, **colB))
			{
				(*colA)->onCollide(**colB);
				(*colB)->onCollide(**colA);
			}
		}
	}

	// Update all the updateables
	for (auto it = updatables.begin(); it != updatables.end(); )
	{
		// Check if it needs to be removed
		if ((*it)->removeFunc())
		{
			(*it)->andType(0b011);
			it = updatables.erase(it);
		}
		else
		{
			if (*it != NULL)
				(*it)->update(delta);
			++it;
		}

	}
}




//      GETTERS & SETTERS      //


/*
This function simply adds a Updatable object to the list to be rendered.
*/
void Model::addUpdatable(Updatable* u)
{
	// If we have a null pointer, don't add it to the list
	if (!u) return;

	updatables.push_back(u);
}

/*
This function removes a Updatable object from our render list.

Return:
- true: The object was stored in our list, and was flagged to be removed
- false: The object wasn't stored in our list, not flagged for removal
*/
bool Model::delUpdatable(Updatable* u)
{
	bool contains = false;
	for (auto it = updatables.begin(); it != updatables.end() && !contains; it++)
	{
		// Object was found
		if (*it == u)
		{
			contains = true;
			break;
		}
	}

	// If we have a pointer to the object stored, "delete" it
	if (contains)
	{
		u->deleteObject();
		u->andType(0b011);
	}

	// Tell them if we had an object stored and flagged it
	return contains;
}

/*
This function simply adds a Collidable object to the list to be rendered.
*/
void Model::addCollidable(Collidable* u)
{
	// If we have a null pointer, don't add it to the list
	if (!u) return;

	collidables.push_back(u);
}

/*
This function removes a Collidable object from our render list.

Return:
- true: The object was stored in our list, and was flagged to be removed
- false: The object wasn't stored in our list, not flagged for removal
*/
bool Model::delCollidable(Collidable* u)
{
	bool contains = false;
	for (auto it = collidables.begin(); it != collidables.end() && !contains; it++)
	{
		// Object was found
		if (*it == u)
		{
			contains = true;
			break;
		}
	}

	// If we have a pointer to the object stored, "delete" it
	if (contains)
	{
		u->deleteObject();
		u->andType(0b110);
	}

	// Tell them if we had an object stored and flagged it
	return contains;
}