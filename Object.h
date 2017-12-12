#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED
#include "SFML\Graphics.hpp"

class Object {

	public:
		virtual void draw(sf::RenderWindow&) const = 0;
		virtual ~Object() {}
};

#endif // !OBJECT_H_INCLUDED
