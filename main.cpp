#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include "ConvexPolygon.h"
#include "SFML\Graphics.hpp"

using namespace std;
using namespace sf;

Object* objects[4];

Text texts[4];
bool check_font = true;
Font font;

extern int mode;

void initializeText() {

	/** **/
	bool check_font = true;
	if (!font.loadFromFile("arial.ttf"))
		check_font = false;
	/** **/

	for (unsigned int i = 0; i < 4; i++) {

		texts[i].setFont(font);
		texts[i].setFillColor(Color::White);
	}

	texts[0].setString("Dynamic Convex Hull");
	texts[0].setPosition(Vector2f(WINDOW_WIDTH / 2.5f, 0));
	texts[0].setCharacterSize(static_cast<unsigned int>(PADDING_SIZE / 1.5f));

	texts[1].setString("1. Direct mode");
	texts[1].setFillColor(Color::Red);
	texts[1].setCharacterSize(static_cast<unsigned int>(PADDING_SIZE / 2.5f));
	texts[1].setPosition(Vector2f(WINDOW_WIDTH / 10.0f, WINDOW_HEIGHT - PADDING_SIZE / 1.5f));
	texts[2].setString("2. Demonstrative mode");
	texts[2].setCharacterSize(static_cast<unsigned int>(PADDING_SIZE / 2.5f));
	texts[2].setPosition(Vector2f(WINDOW_WIDTH / 3.0f + WINDOW_WIDTH / 15.0f, WINDOW_HEIGHT - PADDING_SIZE / 1.5f));
	texts[3].setString("Press 1/2 for mode changing");
	texts[3].setCharacterSize(static_cast<unsigned int>(PADDING_SIZE / 2.5f));
	texts[3].setPosition(Vector2f((WINDOW_WIDTH / 3.0f) * 2.0f + WINDOW_WIDTH / 15.0f, WINDOW_HEIGHT - PADDING_SIZE / 1.5f));
}

int main() {

	initializeText();
	RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Dynamic Convex Hull", Style::Titlebar | Style::Close);
	
	ConvexPolygon poly;
	objects[0] = &poly;

	poly.readFromFile("convex_hull.in");

	bool mouseLock = false;
	int stage = 0;

	while (window.isOpen()) {

		Event event;

		while (window.pollEvent(event)) {

			switch (event.type) {

			case Event::Closed:
				window.close();
				break;
			case Event::KeyPressed:
				if (event.key.code == Keyboard::Num1) {

					if (mode == modes::demonstrative && mouseLock) {

						stage = 0;
						mouseLock = false;

						for (Object *ptr : objects)
							delete ptr;
						objects[0] = &poly;
					}
					mode = modes::direct;
					texts[1].setFillColor(Color::Red);
					texts[2].setFillColor(Color::White);
				}
				if (event.key.code == Keyboard::Num2) {

					mode = modes::demonstrative;
					texts[1].setFillColor(Color::White);
					texts[2].setFillColor(Color::Red);
				}
				break;
			case Event::MouseButtonPressed:
				if (event.key.code == Mouse::Left) {

					Vector2f mouseCoord;
					mouseCoord.x = static_cast<float> (Mouse::getPosition(window).x);
					mouseCoord.y = static_cast<float> (Mouse::getPosition(window).y);

					if (mouseLock == false) {

						if ((mouseCoord.x >= PADDING_SIZE && mouseCoord.x <= WINDOW_WIDTH - PADDING_SIZE) &&
							(mouseCoord.y >= PADDING_SIZE && mouseCoord.y <= WINDOW_HEIGHT - PADDING_SIZE)) {
								
							if (mode == modes::demonstrative) {

								if (poly.getNumberPoints() >= 3 && !poly.insidePolygon(point(mouseCoord.x, mouseCoord.y))) {

									mouseLock = true;
									stage = 1;
								}
							}
							poly.addPoint(point(mouseCoord.x, mouseCoord.y));
						}
					} else {

						stage++;
						if (stage == 3) {

							stage = 0;
							mouseLock = false;

							for (Object *ptr : objects)
								delete ptr;
							objects[0] = &poly;
						}
					}
				}
				break;
			}
		}
		
		window.clear();
		
		/** **/
		objects[0]->draw(window);

		if (stage > 0) {

			if (stage > 1) {

				objects[2]->draw(window);
				objects[3]->draw(window);
			}
			objects[1]->draw(window);
		}

		if (check_font) {
			
			for (unsigned int i = 0; i < 4; i++)
				window.draw(texts[i]);
		}
		/** **/
		window.display();
	}
	return 0;
}