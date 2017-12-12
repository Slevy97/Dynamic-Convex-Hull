#ifndef CONVEX_POLYGON_H_INCLUDED
#define CONVEX_POLYGON_H_INCLUDED

#include "Object.h"
#include <vector>
#include <string>
#include "SFML\Graphics.hpp"
#include "ConvexUtility.h"

class ConvexPolygon : public Object {

private:
	std::vector<point> points;
	void sortPoints();
	int binarySearch(const point& p) const;

public:

	void readFromFile(const std::string&);
	bool insidePolygon(const point&) const;
	void addPoint(const point&);

	unsigned int getNumberPoints() const;
	void printPoints() const;
	void draw(sf::RenderWindow&) const;
};

#endif // CONVEX_POLYGON_H_INCLUDED