#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace sf;

const int S = 4;
const int CARD_W = 150;
const int CARD_H = 200;

struct Card
{
	RectangleShape sprite;
	int id;
	int type;
	int is_clicked;
};

int main(void)
{
	RenderWindow window(VideoMode(1200, 800), "AfterSchool2");
	window.setFramerateLimit(60);

	Vector2i mouse_pos;
	int click_cnt = 0;		//마우스 누른 횟수

	Texture t[8 + 1];
	t[0].loadFromFile("./resources/images/card8.png");
	t[1].loadFromFile("./resources/images/card1.png");
	t[2].loadFromFile("./resources/images/card2.png");
	t[3].loadFromFile("./resources/images/card3.png");
	t[4].loadFromFile("./resources/images/card4.png");
	t[5].loadFromFile("./resources/images/card5.png");
	t[6].loadFromFile("./resources/images/card6.png");
	t[7].loadFromFile("./resources/images/card7.png");
	t[8].loadFromFile("./resources/images/card0.png");

	Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(Color::White);
	text.setPosition(0, 0);
	char info[40];

	struct Card cards[S][S];
	int n = 0;
	for (int i = 0; i < S; i++)
	{
		for (int j = 0; j < S; j++)
		{
			cards[i][j].sprite.setSize(Vector2f(CARD_W, CARD_H));
			cards[i][j].sprite.setPosition(j * CARD_W, i * CARD_H);
			cards[i][j].sprite.setTexture(&t[0]);
			cards[i][j].type = 1 + n / 2;
			cards[i][j].id = n + 1;
			cards[i][j].is_clicked = 0;
			n++;
		}
	}

	while (window.isOpen())
	{
		mouse_pos = Mouse::getPosition(window);

		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed : 
				window.close();
				break;
				// 한 번 누르면 한 번만 적용
			case Event::MouseButtonPressed:
				if (event.mouseButton.button == Mouse::Left)
				{
					click_cnt++;		//필요없는 코드
					for (int i = 0; i < S; i++)
					{
						for (int j = 0; j < S; j++)
						{
							if (cards[i][j].sprite.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
							{
								cards[i][j].is_clicked = 1;
							}
						}
					}
				}
			}
		}

		for (int i = 0; i < S; i++)
		{
			for (int j = 0; j < S; j++)
			{
				// 클릭이 된 상태면 
				if (cards[i][j].is_clicked)
				{
					// 그림이 있는 sprite로 변경
					cards[i][j].sprite.setTexture(&t[cards[i][j].type]);
				}
			}
		}

		sprintf(info, "(%d, %d) clicks %d\n"
			, mouse_pos.x, mouse_pos.y, click_cnt);
		text.setString(info);

		window.clear(Color::Black);
		for (int i = 0; i < S; i++)
		{
			for (int j = 0; j < S; j++)
			{
				window.draw(cards[i][j].sprite);
			}
		}
		window.draw(text);
		window.display();
	}	

	return 0;
}