#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <SFML/Audio.hpp>

using namespace sf;

int main(void)
{

    //윈도우창 생성
    RenderWindow window(VideoMode(640, 480), "AfterSchool");
    window.setFramerateLimit(60);

    srand(time(0));

    long start_time = clock(); //게임 시작 시간
    long spent_time; //게임 진행 시간

    //text
    Font font;
    font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
    
    Text text;
    text.setFont(font);
    text.setCharacterSize(30);  //폰트 크기
    text.setFillColor(Color(255, 255, 255));
    text.setPosition(0, 0);
    char info[40];
    
    // 플레이어
    RectangleShape player;
    player.setSize(Vector2f(40, 40));
    player.setPosition(100, 100);
    player.setFillColor(Color::Red);
    int player_speed = 5;
    int player_score = 0;
    
    //적
    RectangleShape enemy[5];
    int enemy_life[5];
    int enemy_score = 100;  //적을 잡을 때 얻는 점수
    SoundBuffer enemy_explosion_buffer;
    enemy_explosion_buffer.loadFromFile("./resources/sounds/rumble.flac");
    Sound enemy_explosion_sound;
    enemy_explosion_sound.setBuffer(enemy_explosion_buffer);
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

        spent_time = clock() - start_time;

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
                    player_score += enemy_score;

                    //TODO : 코드 refactoring 필요
                    if (enemy_life[i] == 0)
                    {
                        enemy_explosion_sound.play();
                    }
                }

            }
   
        }

        sprintf(info, "score : %d time : %d"
            , player_score, spent_time/1000);
        text.setString(info);

        window.clear(Color::Black);

        //draw는 나중에 호출할수록 우선 순위가 높아짐
        for (int i = 0; i < 5; i++) {
            if (enemy_life[i] > 0)
                window.draw(enemy[i]);
        }
        window.draw(player);
        window.draw(text);

        window.display();
    }

    return 0;
}