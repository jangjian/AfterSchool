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
	int id_i;
	int id_j;
	int type;
	int is_clicked;		// 현재 뒤집혀진 카드인지
	int is_claered;		// 정답을 맞춘 카드인지
};

struct SBuffer {
	SoundBuffer BGM;

};
void swap_card(struct Card* c1, struct Card* c2)
{
	struct Card temp = *c1;
	*c1 = *c2;
	*c2 = temp;
}

int main(void)
{
	RenderWindow window(VideoMode(1200, 800), "AfterSchool2");
	window.setFramerateLimit(60);

	Vector2i mouse_pos;
	int flipped_num = 0;	// 현제 뒤집혀진 카드의 갯수
	int is_gameover = 0;
	int score = 0;
	int life = 10;

	long start_time;
	long spent_time;
	long delay_time;		// 바로 다시 ?로 뒤집혀지지 않도록 딜레이를 줌


	srand(time(0));

	struct SBuffer sb;
	sb.BGM.loadFromFile("./resources/sounds/bgm.ogg");

	//BGM
	Sound BGM_sound;
	BGM_sound.setBuffer(sb.BGM);
	BGM_sound.setVolume(90);
	BGM_sound.setLoop(1);       //BGM 무한반복
	BGM_sound.play();

	Texture t[8 + 1+ 4];
	t[0].loadFromFile("./resources/images/card8.png");
	t[1].loadFromFile("./resources/images/card1.png");
	t[2].loadFromFile("./resources/images/card2.png");
	t[3].loadFromFile("./resources/images/card3.png");
	t[4].loadFromFile("./resources/images/card4.png");
	t[5].loadFromFile("./resources/images/card5.png");
	t[6].loadFromFile("./resources/images/card6.png");
	t[7].loadFromFile("./resources/images/card7.png");
	t[8].loadFromFile("./resources/images/card0.png");
	t[9].loadFromFile("./resources/images/gameclear.png");
	t[10].loadFromFile("./resources/images/gameover.png");
	t[11].loadFromFile("./resources/images/background.png");
	t[12].loadFromFile("./resources/images/restart.png");

	Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(Color::Black);
	text.setPosition(0, 0);
	char info[40];

	// gameclear
	Sprite gameclear_sprite;
	gameclear_sprite.setTexture(t[9]);
	gameclear_sprite.setPosition((1200 - 640) / 2, (800 - 600) / 2);

	// gameover
	Sprite gameover_sprite;
	gameover_sprite.setTexture(t[10]);
	gameover_sprite.setPosition((1200-640)/2, (800-600)/2);

	// background
	Sprite bg_sprite;
	bg_sprite.setTexture(t[11]);
	bg_sprite.setPosition(0,0);

	// restart
	Sprite rs_sprite;
	rs_sprite.setTexture(t[12]);
	rs_sprite.setPosition((1200 - 640) / 2, (800 - 400) / 2);

	struct Card compare_card;
	struct Card cards[S][S];
	int n = 0;
	for (int i = 0; i < S; i++)
	{
		for (int j = 0; j < S; j++)
		{
			cards[i][j].sprite.setSize(Vector2f(CARD_W, CARD_H));
			cards[i][j].sprite.setTexture(&t[0]);
			cards[i][j].type = 1 + n / 2;
			cards[i][j].is_clicked = 0;
			cards[i][j].is_claered = 0;
			n++;
		}
	}

	for (int i = 0; i < 100; i++)
	{
		swap_card(&cards[rand() % S][rand() % S], &cards[rand() % S][rand() % S]);
	}

	for (int i = 0; i < S; i++)
	{
		for (int j = 0; j < S; j++)
		{
			cards[i][j].id_i = i;
			cards[i][j].id_j = j;
			cards[i][j].sprite.setPosition(j * CARD_W + 300, i * CARD_H);
		}
	}

	start_time = clock();
	delay_time = start_time;


	while (window.isOpen())
	{
		mouse_pos = Mouse::getPosition(window);
		spent_time = clock() - start_time;

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
					for (int i = 0; i < S; i++)
					{
						for (int j = 0; j < S; j++)
						{
							if (flipped_num < 2)
							{
								// 마우스 위치가 cards[i][j]의 위치에 해당한다면?
								if (cards[i][j].sprite.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
								{
									// 뒤집혀지지 않은 카드만 뒤집겠다.
									if (cards[i][j].is_clicked == 0)
									{
										cards[i][j].is_clicked = 1;
										flipped_num++;
										// 뒤집혀진 카드가 한 개라면
										if (flipped_num == 1)
										{
											compare_card = cards[i][j];
										}
										// 두 개를 뒤집었다면
										else if (flipped_num == 2)
										{
											if (compare_card.type == cards[i][j].type)
											{
												cards[i][j].is_claered = 1;
												cards[compare_card.id_i][compare_card.id_j].is_claered = 1;
												score += 1;
											}
											// 두 카드가 다른 종류이면
											else
											{
												delay_time = spent_time;
												life -= 1;
											}
										}
									}
								}
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
				// 클릭이 된 상태거나, 정답을 맞춘 카드이면
				if (cards[i][j].is_clicked == 1 || cards[i][j].is_claered ==1)
					// 그림이 있는 sprite로 변경(카드를 뒤집겠다는 의미)
					cards[i][j].sprite.setTexture(&t[cards[i][j].type]);
				else 
					// 카드는 ???상태
					cards[i][j].sprite.setTexture(&t[0]);
			}
		}

		// 뒤집힌 카드가 2개라면 
		if (flipped_num == 2)
		{
			// 두 카드가 뒤집힌지 2초 이내라면
			if (spent_time - delay_time < 1000)
			{

			}
			else
			{
				// 다 ?상태로 만들어버리겠다
				for (int i = 0; i < S; i++)
					for (int j = 0; j < S; j++)
						cards[i][j].is_clicked = 0;
				flipped_num = 0;
			}
		}

		if (is_gameover == 1)
		{
			spent_time = clock();
			for (int i = 0; i < S; i++)
			{
				for (int j = 0; j < S; j++)
				{
					cards[i][j].sprite.setSize(Vector2f(CARD_W, CARD_H));
					cards[i][j].sprite.setTexture(&t[0]);
					cards[i][j].type = 1 + n / 2;
					cards[i][j].is_clicked = 0;
					cards[i][j].is_claered = 0;
					n++;

				}
				for (int i = 0; i < S; i++)
				{
					for (int j = 0; j < S; j++)
					{
						window.draw(cards[i][j].sprite);
					}
				}
				is_gameover = 0;

			}
			score = 0;
			life = 10;
		}

		sprintf(info, "TIME : %d | LIFE : %d\n SCORE : %d",spent_time/1000, life, score);
		text.setString(info);
		window.draw(bg_sprite);

		window.clear(Color::White);
		for (int i = 0; i < S; i++)
		{
			for (int j = 0; j < S; j++)
			{
				window.draw(cards[i][j].sprite);
			}
		}

		if (score == 8 && spent_time<=40000)
		{
			window.draw(gameclear_sprite);
			window.draw(rs_sprite);
			is_gameover = 1;
		}

		if (spent_time == 40000 || life == 0)
		{
			window.draw(gameover_sprite);
			window.draw(rs_sprite);
			is_gameover = 1;
			
		}

		window.draw(text);

		window.display();
	}

	return 0;
}