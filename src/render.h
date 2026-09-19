#include "organism.h"

class Render {
private:
	sf::RenderTarget& target;

public:
	Render(sf::RenderTarget& target_) :
		target{ target_ }
	{}

	void createRect(Organism& org, int length, int width, float framerate) {
		float dt = 1 / framerate;
		org.update();
		org.draw(target);
	}
};