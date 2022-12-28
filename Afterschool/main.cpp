#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <SFML/Audio.hpp>

using namespace sf;

struct Player {
    RectangleShape sprite;
    int speed;
    int score;
    int life;
};
struct Enemy {
    RectangleShape sprite;
    int speed;
    int score;
    int life;
    SoundBuffer explosion_buffer;
    Sound explosion_sound;
};

int main(void)
{

    //윈도우창 생성
    RenderWindow window(VideoMode(640, 480), "AfterSchool");
    window.setFramerateLimit(60);

    srand(time(0));

    long start_time = clock(); //게임 시작 시간
    long spent_time; //게임 진행 시간
    int is_gameover = 0;

    //text
    Font font;
    font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
    
    Text text;
    text.setFont(font);
    text.setCharacterSize(30);  //폰트 크기
    text.setFillColor(Color(255, 255, 255));
    text.setPosition(0, 0);
    char info[40];

    //배경
    Texture bg_texture;
    bg_texture.loadFromFile("./resources/images/background.png");
    Sprite bg_sprite;
    bg_sprite.setTexture(bg_texture);
    bg_sprite.setPosition(0, 0);

    //게임오버
    Texture gameover_texture;
    gameover_texture.loadFromFile("./resources/images/gameover.png");
    Sprite gameover_sprite;
    gameover_sprite.setTexture(gameover_texture);
    gameover_sprite.setPosition((640-320)/2, (480-240)/2);
    
    // 플레이어
    struct Player player;
    player.sprite.setSize(Vector2f(40, 40));
    player.sprite.setPosition(100, 100);
    player.sprite.setFillColor(Color::Red);
    player.speed = 5;
    player.score = 0;
    player.life = 3;
    
    //적
    const int ENEMY_NUM = 10;
    struct Enemy enemy[ENEMY_NUM];

    //enemy 초기화
    for (int i = 0; i < ENEMY_NUM; i++) {

        //TODO : 굉장히 비효율적인 코드이므로 나중에 repactoring
        enemy[i].explosion_buffer.loadFromFile("./resources/sounds/rumble.flac");
        enemy[i].explosion_sound.setBuffer(enemy[i].explosion_buffer);
        enemy[i].score = 100;

        enemy[i].sprite.setSize(Vector2f(70, 70));
        enemy[i].sprite.setFillColor(Color::Yellow);
        enemy[i].sprite.setPosition(rand()%300+300, rand()%380);
        enemy[i].life = 1;
        enemy[i].speed = -(rand() % 10 + 1);
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
                    for (int i = 0; i < ENEMY_NUM; i++) {
                        enemy[i].sprite.setSize(Vector2f(70, 70));
                        enemy[i].sprite.setFillColor(Color::Yellow);
                        enemy[i].sprite.setPosition(rand() % 300 + 300, rand() % 380);
                        enemy[i].life = 1;
                        enemy[i].speed = -(rand() % 10 + 1);
                    }
                }
                break;
            }

            }
        }

        spent_time = clock() - start_time;

        //방향키 start
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            player.sprite.move(-player.speed, 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            player.sprite.move(player.speed, 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            player.sprite.move(0, -player.speed);
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            player.sprite.move(0, player.speed);
        }//방향키 end


        //enemy와의 충돌
        for (int i = 0; i < ENEMY_NUM; i++)
        {
            if (enemy[i].life > 0)
            {
                if (player.sprite.getGlobalBounds().intersects(enemy[i].sprite.getGlobalBounds()))
                {
                    printf("enemy[%d]과 충돌\n", i);
                    enemy[i].life -= 1;
                    player.score += enemy[i].score;

                    //TODO : 코드 refactoring 필요
                    if (enemy[i].life == 0)
                    {
                        enemy[i].explosion_sound.play();
                    }
                }
                //적이 왼쪽 끝에 진입하려는 순간
                else if (enemy[i].sprite.getPosition().x < 0)
                {
                    player.life -= 1; 
                    enemy[i].life = 0;
                }

                enemy[i].sprite.move(enemy[i].speed, 0);
            }
   
        }

        if (player.life <= 0) {
            is_gameover = 1;
        }

       
        sprintf(info, "life : %d score : %d time : %d"
            ,player.life, player.score, spent_time/1000);
        text.setString(info);

        window.clear(Color::Black);
        window.draw(bg_sprite);

        //draw는 나중에 호출할수록 우선 순위가 높아짐
        for (int i = 0; i < ENEMY_NUM; i++) {
            if (enemy[i].life > 0)
                window.draw(enemy[i].sprite);
        }
        window.draw(player.sprite);
        window.draw(text);
       
        if (is_gameover) {
            window.draw(gameover_sprite);
        }

        window.display();
    }

    return 0;
}