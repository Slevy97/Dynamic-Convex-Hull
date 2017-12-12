#include "ConvexPolygon.h"
#include "VertexArray2.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

unsigned int ConvexPolygon::getNumberPoints() const {

	return this->points.size();
}

bool ConvexPolygon::insidePolygon(const point &p) const {

	unsigned int nrPoints = points.size();
	
	for (unsigned int i = 0; i < nrPoints; i++) {

		if (crossProduct(points[i], points[(i + 1) % nrPoints], p) < 0)
			return false;
	}
	return true;
}

void ConvexPolygon::sortPoints() {

	if (points.size() > 0) {

		int pos = 0;

		for (unsigned int i = 0; i < points.size(); i++) {

			if ((points[i].X < points[pos].X) || ((points[i].X == points[pos].X) && (points[i].Y < points[pos].Y)))
				pos = i;
		}

		swap(points[0], points[pos]);

		for (unsigned int i = 1; i < points.size() - 1; i++)
			for (unsigned int j = i + 1; j < points.size(); j++) {

				float res = crossProduct(points[0], points[i], points[j]);
				if ((res < 0) || ((res == 0) && dist(points[0], points[i]) > dist(points[0], points[j])))
					swap(points[i], points[j]);
			}
	}
}

int ConvexPolygon::binarySearch(const point& p) const {

	int i;
	unsigned int step;

	for (step = 1; step <= points.size(); step <<= 1);

	for (i = -1; step; step >>= 1) {
		if (i + step < points.size()) {

			if (crossProduct(points.front(), points[i + step], p) >= 0)
				i = i + step;
		}
	}
	return i + 1;
}

extern Object* objects[];
extern int mode;

void ConvexPolygon::addPoint(const point& p) {

	if (points.size() == 0) {
	
		points.push_back(p);
		
	}
	else if (points.size() == 1) {

		points.push_back(p);
		point aux = points[0];
		points[0] = min(points[0], points[1]);
		points[1] = max(aux, points[1]);
	}
	else if (points.size() == 2) {

		points.push_back(p);
		sortPoints();

		if (crossProduct(points[0], points[1], points[2]) == 0)
			points.erase(points.begin() + 1);
		
	}
	else {

		if (!insidePolygon(p)) {
			
			unsigned int lowLeft = 0;

			//Find low-left point
			for (unsigned int i = 0; i < points.size(); i++)
				if ((points[i].X < points[lowLeft].X) || ((points[i].X == points[lowLeft].X) && (points[i].Y < points[lowLeft].Y)))
					lowLeft = i;
			
			//Rotate vector so that the low-left point is the first one
			rotate(points.begin(), points.begin() + lowLeft, points.end());

			unsigned int nrPoints = points.size();
			unsigned int pos = binarySearch(p) % nrPoints;
		
			unsigned int upper = pos;
			unsigned int lower = pos;

			while (crossProduct(p, points[upper], points[(upper + 1) % nrPoints]) <= 0)
				upper = (upper + 1) % nrPoints;

			while (crossProduct(p, points[lower], points[(nrPoints + lower - 1) % nrPoints]) >= 0)
				lower = (nrPoints + lower - 1) % nrPoints;
			
			/** **/
			if (mode == modes::demonstrative) {

				objects[0] = new ConvexPolygon(*this);
				objects[1] = new VertexArray2(1, Vector2f(p.X, p.Y));
				objects[2] = new VertexArray2(2, Vector2f(p.X, p.Y), Vector2f(points[lower].X, points[lower].Y));
				objects[3] = new VertexArray2(2, Vector2f(p.X, p.Y), Vector2f(points[upper].X, points[upper].Y));
			}
			/** **/
			vector <point> newPoints;

			for (unsigned int i = upper; ; i = (i + 1) % nrPoints) {

				newPoints.push_back(points[i]);
				if (i == lower)
					break;
			}

			newPoints.push_back(p);

			points.clear();
			for (const point& aux : newPoints)
				points.push_back(aux);
		}
	}
}

void ConvexPolygon::printPoints() const {

	for (const point& p : points)
		cout << p.X << " " << p.Y << "\n";
}

void ConvexPolygon::readFromFile(const string& fileName) {

	ifstream in(fileName);

	if (in.is_open()) {

		points.clear();
		int n;
		point aux;
		bool corrupt_file = false;

		float minX = 0, minY = 0;
		float maxX = WINDOW_WIDTH - 2 * PADDING_SIZE, maxY = WINDOW_HEIGHT - 2 * PADDING_SIZE;

		in >> n;
		for (int i = 1; i <= n; i++) {

			if (!(in)) {

				corrupt_file = true;
				break;
			}

			in >> aux.X >> aux.Y;

			minX = min(minX, aux.X);
			minY = min(minY, aux.Y);
			maxX = max(maxX, aux.X);
			maxY = max(maxY, aux.Y);

			points.push_back(aux);
		}

		if (corrupt_file)
			points.clear();
		else {

			if (minX < 0 || minY < 0) {

				for (point& p : points) {

					p.X = p.X - minX;
					p.Y = p.Y - minY;
					maxX = max(maxX, aux.X);
					maxY = max(maxY, aux.Y);
				}
			}

			float scaleX = (WINDOW_WIDTH - 2 * PADDING_SIZE) / maxX;
			float scaleY = (WINDOW_HEIGHT - 2 * PADDING_SIZE) / maxY;
			
			for (point& p : points) {

				p.X = (p.X * scaleX) + PADDING_SIZE;
				p.Y = (p.Y * scaleY) + PADDING_SIZE;
			}
		}
		in.close();
	}
	else {

		cerr << "Could not open file " << fileName << "\n";
	}
}

using namespace sf;

void ConvexPolygon::draw(RenderWindow& window) const {
	
	Vertex dpoints[2];
	float radius = 3.5;
	CircleShape circle1, circle2;

	if (points.size() > 0) {
		
		switch (points.size()) {

		case 0:
			break;
		case 1:
			circle1.setRadius(radius);
			circle1.setPointCount(300);
			circle1.setPosition(points[0].X - radius, points[0].Y - radius);
			window.draw(circle1);
		
			break;
		case 2:
			dpoints[0] = Vertex(Vector2f(points[0].X, points[0].Y));
			dpoints[1] = Vertex(Vector2f(points[1].X, points[1].Y));
			//
			circle1.setRadius(radius);
			circle1.setPointCount(300);
			circle1.setPosition(points[0].X - radius, points[0].Y - radius);
			//
			circle2.setRadius(radius);
			circle2.setPointCount(300);
			circle2.setPosition(points[1].X - radius, points[1].Y - radius);
			//
			window.draw(dpoints, 2, Lines);
			window.draw(circle1);
			window.draw(circle2);
			break;
		default:

			ConvexShape shape;
			shape.setPointCount(points.size());
			shape.setFillColor(Color(160, 160, 160));

			for (unsigned int i = 0; i < points.size(); i++)
				shape.setPoint(i, Vector2f(points[i].X, points[i].Y));
			
			window.draw(shape);
			break;
		}
	}
}
