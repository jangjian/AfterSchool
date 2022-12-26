#include <stdio.h>
#include <SFML/Graphics.hpp>

using namespace sf;

int main(void) {
	//윈도우창 생성
	RenderWindow window(VideoMode(640, 480), "AfterSchool");

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if(event.type == Event::Closed)
				window.close();
		}
			
	}

	return 0;
}