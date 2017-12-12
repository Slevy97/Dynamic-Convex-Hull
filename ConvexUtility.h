#ifndef CONVEX_UTILITY_H_INCLUDED
#define CONVEX_UTILITY_H_INCLUDED

#include <utility>
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960
#define PADDING_SIZE 50
#define X first
#define Y second

enum modes{direct, demonstrative};

typedef std::pair<float, float> point;
float crossProduct(const point&, const point&, const point&);
float dist(const point&, const point&);

#endif // CONVEX_UTILITY_H_INCLUDED
