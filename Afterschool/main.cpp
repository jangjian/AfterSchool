#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

int main(void)
{
    //윈도우창 생성
    RenderWindow window(VideoMode(640, 480), "AfterSchool");
    window.setFramerateLimit(60);

    srand(time(0));

    RectangleShape player;
    player.setSize(Vector2f(40, 40));
    player.setPosition(100, 100);
    player.setFillColor(Color::Red);
    int player_speed = 5;

    RectangleShape enemy[5];
    int enemy_life[5];
    //enemy 초기화
    for (int i = 0; i < 5; i++) {
        enemy[i].setSize(Vector2f(70, 70));
        enemy[i].setFillColor(Color::Yellow);
        enemy_life[i] = 1;
        enemy[i].setPosition(rand()%300+300, rand()%380);
    }

    //윈도우가 열려있을 때까지 반복
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event)) {
            //종료(X) 버튼을 누르면 Event::Closed
            switch (event.type)
            {
            case Event::Closed:
                window.close();//윈도우 닫는다.
                break;
            //키보드를 눌렀을 때(누른 순간만을 감지)
            case Event::KeyPressed:
            {
                //스페이스 키 누르면 모든 적 출현
                if (event.key.code == Keyboard::Space) 
                {
                    for (int i = 0; i < 5; i++) {
                        enemy[i].setSize(Vector2f(70, 70));
                        enemy[i].setFillColor(Color::Yellow);
                        enemy_life[i] = 1;
                        enemy[i].setPosition(rand() % 300 + 300, rand() % 380);
                    }
                }
                break;
            }

            }
        }

        //방향키 start
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            player.move(-player_speed, 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            player.move(player_speed, 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            player.move(0, -player_speed);
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            player.move(0, player_speed);
        }//방향키 end


        //enemy와의 충돌
        for (int i = 0; i < 5; i++)
        {
            if (enemy_life[i] > 0)
            {
                if (player.getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
                {
                    printf("enemy[%d]과 충돌\n", i);
                    enemy_life[i] -= 1;
                }

            }
        }
        window.clear(Color::Black);

        //draw는 나중에 호출할수록 우선 순위가 높아짐
        for (int i = 0; i < 5; i++) {
            if (enemy_life[i] > 0)
                window.draw(enemy[i]);

        }
        window.draw(player);
        window.display();
    }

    return 0;
}