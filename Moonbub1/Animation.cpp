#include <SFML/Graphics.hpp>

using namespace sf;

struct Player {
	RectangleShape sprite;
	int fps;			// 
	int idx;			// 애니메이션 index
};

int main(void)
{
	RenderWindow window(VideoMode(1200, 800), "Animation");
	window.setFramerateLimit(60);

	Texture run[10];
	run[0].loadFromFile("./animation/run__000.png");
	run[1].loadFromFile("./animation/run__001.png");
	run[2].loadFromFile("./animation/run__002.png");
	run[3].loadFromFile("./animation/run__003.png");
	run[4].loadFromFile("./animation/run__004.png");
	run[5].loadFromFile("./animation/run__005.png");
	run[6].loadFromFile("./animation/run__006.png");
	run[7].loadFromFile("./animation/run__007.png");
	run[8].loadFromFile("./animation/run__008.png");
	run[9].loadFromFile("./animation/run__009.png");

	struct Player player;
	player.fps = 10;
	player.sprite.setTexture(&run[0]);
	player.sprite.setSize(Vector2f(90, 120));
	player.sprite.setPosition(200, 600);
	
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;
			default:
				break;
			}
		}
		window.clear(Color::Magenta);

		window.draw(player.sprite);
		window.display();
	}
	return 0;
}