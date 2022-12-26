#include <stdio.h>
#include <SFML/Graphics.hpp>

using namespace sf;

int main(void) {
	//윈도우창 생성
	RenderWindow window(VideoMode(640, 480), "AfterSchool");

	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100,100);
	

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {

				//종료(x)버튼을 누르면 Event::Closed(0)
			case Event::Closed:
				window.close();
			}

		}
		window.draw(player);
		window.display();
	}
	return 0;
}