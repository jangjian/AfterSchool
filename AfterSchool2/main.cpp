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
	int is_clicked;		// ���� �������� ī������
	int is_claered;		// ������ ���� ī������
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
	int flipped_num = 0;	// ���� �������� ī���� ����
	int is_gameover = 0;
	int score = 0;
	int life = 10;

	long start_time;
	long spent_time;
	long delay_time;		// �ٷ� �ٽ� ?�� ���������� �ʵ��� �����̸� ��


	srand(time(0));

	struct SBuffer sb;
	sb.BGM.loadFromFile("./resources/sounds/bgm.ogg");

	//BGM
	Sound BGM_sound;
	BGM_sound.setBuffer(sb.BGM);
	BGM_sound.setVolume(90);
	BGM_sound.setLoop(1);       //BGM ���ѹݺ�
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
				// �� �� ������ �� ���� ����
			case Event::MouseButtonPressed:
				if (event.mouseButton.button == Mouse::Left)
				{
					for (int i = 0; i < S; i++)
					{
						for (int j = 0; j < S; j++)
						{
							if (flipped_num < 2)
							{
								// ���콺 ��ġ�� cards[i][j]�� ��ġ�� �ش��Ѵٸ�?
								if (cards[i][j].sprite.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
								{
									// ���������� ���� ī�常 �����ڴ�.
									if (cards[i][j].is_clicked == 0)
									{
										cards[i][j].is_clicked = 1;
										flipped_num++;
										// �������� ī�尡 �� �����
										if (flipped_num == 1)
										{
											compare_card = cards[i][j];
										}
										// �� ���� �������ٸ�
										else if (flipped_num == 2)
										{
											if (compare_card.type == cards[i][j].type)
											{
												cards[i][j].is_claered = 1;
												cards[compare_card.id_i][compare_card.id_j].is_claered = 1;
												score += 1;
											}
											// �� ī�尡 �ٸ� �����̸�
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
				// Ŭ���� �� ���°ų�, ������ ���� ī���̸�
				if (cards[i][j].is_clicked == 1 || cards[i][j].is_claered ==1)
					// �׸��� �ִ� sprite�� ����(ī�带 �����ڴٴ� �ǹ�)
					cards[i][j].sprite.setTexture(&t[cards[i][j].type]);
				else 
					// ī��� ???����
					cards[i][j].sprite.setTexture(&t[0]);
			}
		}

		// ������ ī�尡 2����� 
		if (flipped_num == 2)
		{
			// �� ī�尡 �������� 2�� �̳����
			if (spent_time - delay_time < 1000)
			{

			}
			else
			{
				// �� ?���·� ���������ڴ�
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