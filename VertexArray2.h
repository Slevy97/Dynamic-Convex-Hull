#ifndef VERTEX_ARRAY2_H_INCLUDED
#define VERTEX_ARRAY2_H_INCLUDED

#include "Object.h"
#include "SFML\Graphics.hpp"

using namespace sf;

class VertexArray2 : public Object {

private:
	int nrPoints;
	Vertex v_array[2];

public:

	VertexArray2(int nrPoints = 0, Vector2f fp = Vector2f(0, 0), Vector2f sp = Vector2f(0, 0));
	void draw(sf::RenderWindow&) const;
};
#endif //VERTEX_ARRAY2_H_INCLUDED