#include "organism.h"

class Render {
private:
	sf::RenderTarget& target;
	sf::RectangleShape rect;

public:
	Render(sf::RenderTarget& target_) :
		target{ target_ },
		rect({ 30.0f, 30.0f })
	{}

	void create(Organism& org, int length, int width, float framerate) {
		float dt = 1 / framerate;
		org.update();

		rect.setPosition(org.position);
		rect.setFillColor(org.color);
		target.draw(rect);
	}
};