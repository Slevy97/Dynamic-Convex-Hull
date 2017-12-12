#include "VertexArray2.h"

VertexArray2::VertexArray2(int nrPoints, Vector2f fp, Vector2f sp) : nrPoints(nrPoints) {

	this->v_array[0] = Vertex(fp);
	this->v_array[1] = Vertex(sp);
	this->v_array[0].color = Color::Red;
	this->v_array[1].color = Color::Red;
}

void VertexArray2::draw(RenderWindow& window) const {

	if (nrPoints == 1) {
		
		float radius = 3.5;
		CircleShape circle;
		circle.setRadius(radius);
		circle.setPointCount(300);
		circle.setPosition(v_array[0].position.x - radius, v_array[0].position.y - radius);
		window.draw(circle);
	}
	else
		window.draw(v_array, nrPoints, sf::Lines);
}
