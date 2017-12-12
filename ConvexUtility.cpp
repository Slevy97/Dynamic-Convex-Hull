#include "ConvexUtility.h"

int mode = modes::direct;

float crossProduct(const point &p, const point &q, const point &r) {

	return (q.X - p.X) * (r.Y - p.Y) - (r.X - p.X) * (q.Y - p.Y);
}

float dist(const point &p1, const point& p2) {

	return (p2.X - p1.X) * (p2.X - p1.X) + (p2.Y - p1.Y) * (p2.Y - p1.Y);
}