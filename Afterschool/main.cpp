/*

TODO list
1) 아이템 : 속도증가, 데미지 증가
2) 총알 시스템 개편
3) sound effect 개선


*/


#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <SFML/Audio.hpp>


using namespace sf;

struct Player {
    RectangleShape sprite;
    int speed;
    int score;
    int life;
    float x, y;     //플레이어 좌표
};
struct Enemy {
    RectangleShape sprite;
    int speed;
    int score;
    int life;
    SoundBuffer explosion_buffer;
    Sound explosion_sound;
    int respawn_time;
};
struct Bullet
{
    RectangleShape sprite;
    int is_fired; //발사 여부
};
struct Textures {
    Texture bg;         // 배경 이미지
    Texture enemy;     // 적 이미지
    Texture gameover;   // 게임오버 이미지
    Texture player;     // 플레이어 이미지
};

//obj1과 obj2의 충돌 여부. 충돌하면 1을 반환, 안하면 0을 반환
int is_collide(RectangleShape obj1, RectangleShape obj2)
{
    return obj1.getGlobalBounds().intersects(obj2.getGlobalBounds());
}

//전역변수 
const int ENEMY_NUM = 10;                   //enemy의 최대 개수           
const int BULLET_NUM = 50;                  //bullet의 최대 개수
const int W_WIDTH = 1200, W_HEIGHT = 600;   //창의 크기
const int GO_WIDTH = 640, GO_HEIGHT = 450;  //게임오버 그림의 크기



int main(void)
{

    struct Textures t;
    t.bg.loadFromFile("./resources/images/background.png");
    t.enemy.loadFromFile("./resources/images/enemy.png");
    t.gameover.loadFromFile("./resources/images/gameover.png");
    t.player.loadFromFile("./resources/images/player.png");

    //윈도우창 생성
    RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool");
    window.setFramerateLimit(60);

    srand(time(0));

    long start_time = clock();  // 게임 시작 시간
    long spent_time;            // 게임 진행 시간
    long fired_time = 0;            // 최근에 발사한 시간
    int is_gameover = 0;

    //BGM
    SoundBuffer BGM_buffer;
    BGM_buffer.loadFromFile("./resources/sounds/bgm.ogg");
    Sound BGM_sound;
    BGM_sound.setBuffer(BGM_buffer);
    BGM_sound.setLoop(1);       //BGM 무한반복
    BGM_sound.play();

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
    Sprite bg_sprite;
    bg_sprite.setTexture(t.bg);
    bg_sprite.setPosition(0, 0);

    //게임오버
    
    Sprite gameover_sprite;
    gameover_sprite.setTexture(t.gameover);
    gameover_sprite.setPosition((W_WIDTH-GO_WIDTH)/2, (W_HEIGHT-GO_HEIGHT)/2);
    
    // 플레이어
    struct Player player;
    player.sprite.setTexture(&t.player);
    player.sprite.setPosition(100, 100);
    player.sprite.setSize(Vector2f(170,181));
    player.x = player.sprite.getPosition().x;
    player.y = player.sprite.getPosition().y;
    player.speed = 5;
    player.score = 0;
    player.life = 3;

    // 총알
    int bullet_speed = 20;
    int bullet_idx = 0;
    int bullet_delay = 500;     // 딜레이 0.5초

    struct Bullet bullet[BULLET_NUM] ;

    for (int i = 0; i < BULLET_NUM; i++) {
        bullet[i].sprite.setSize(Vector2f(10, 10));
        bullet[i].sprite.setPosition(player.x + 50, player.y + 15);        //임시 테스트
        bullet[i].is_fired = 0;
    }
    
    
    // 적
    struct Enemy enemy[ENEMY_NUM];

    //enemy 초기화
    for (int i = 0; i < ENEMY_NUM; i++)
    {
        // TODO : 굉장히 비효율적인 코드이므로 나중에 refactoring
        enemy[i].explosion_buffer.loadFromFile("./resources/sounds/pop1.flac");
        enemy[i].explosion_sound.setBuffer(enemy[i].explosion_buffer);
        enemy[i].score = 100;
        enemy[i].respawn_time = 8;
        enemy[i].sprite.setTexture(&t.enemy);
        enemy[i].sprite.setSize(Vector2f(90, 90));
        enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 380);
        enemy[i].life = 1;
        enemy[i].speed = -(rand() % 10 + 1);
    }

    //윈도우가 열려있을 때까지 반복
    while (window.isOpen())
    {
        spent_time = clock() - start_time;
        player.x = player.sprite.getPosition().x;
        player.y = player.sprite.getPosition().y;

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
                ////스페이스 키 누르면 총 발사
                //if (event.key.code == Keyboard::Space) 
                //{
                //    for (int i = 0; i < 1; i++)
                //    {
                //        bullet.sprite.move(bullet.speed, 0);
                //    }
                //}
                break;
                }
            }
        }


        /*game 상태 update*/
        if (player.life <= 0) {
            is_gameover = 1;
        }

        /*Player update*/
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

        //Player 이동 범위 제한
        if (player.x < 0) 
            player.sprite.setPosition(0, player.y);
        else if(player.x > W_WIDTH - 170)       //그림의 폭
            player.sprite.setPosition(W_WIDTH - 170, player.y);

        if (player.y < 0) 
            player.sprite.setPosition(player.x, 0);
        else if (player.y > W_HEIGHT - 181)     //그림의 높이
            player.sprite.setPosition(player.x, W_HEIGHT - 181);
       

        /*Bullet update*/
       //총알 발사
        //TODO : 50번 이후부터는 안나가는 버그 수정할 것
        printf("bullet_idx %d\n", bullet_idx);
        if (Keyboard::isKeyPressed(Keyboard::Space))
        {
            if (spent_time - fired_time > bullet_delay) {
                //총알이 발사되어 있지 않다면
                if (!bullet[bullet_idx].is_fired)
                {
                    bullet[bullet_idx].sprite.setPosition(player.x + 50, player.y);
                    bullet[bullet_idx].is_fired = 1;
                    bullet_idx++;   //다음 총알이 발사할 수 있도록
                    fired_time = spent_time;        //총알 장전
                }
            }
        }
        for (int i = 0; i < BULLET_NUM; i++) {
            if (bullet[i].is_fired)
            {
                bullet[i].sprite.move(bullet_speed, 0);
                if (bullet[i].sprite.getPosition().x > W_WIDTH)
                    bullet[i].is_fired = 0;
            }
        }

        /*Enemy update*/
        for (int i = 0; i < ENEMY_NUM; i++)
        {
           // 10초마다 enemy가 젠
            if (spent_time % (1000 * enemy[i].respawn_time) < 1000 / 60 + 1)
            {
                enemy[i].sprite.setTexture(&t.enemy);
                enemy[i].sprite.setSize(Vector2f(70, 70));
                enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 380);
                enemy[i].life = 1;
                // 10초마다 enemy의 속도+1
                enemy[i].speed = -(rand() % 10 + 1 + (spent_time / 1000 / enemy[i].respawn_time));
            }

            if (enemy[i].life > 0)
            {
                // player, enemy와의 충돌
                if (is_collide(player.sprite, enemy[i].sprite))
                {
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

                // bullet, enemy와의 충돌
                for (int j = 0; j < BULLET_NUM; j++)
                {
                    if (is_collide(bullet[j].sprite, enemy[i].sprite))
                    {
                        if (bullet[j].is_fired)
                        {
                            enemy[i].life -= 1;
                            player.score += enemy[i].score;

                            //TODO : 코드 refactoring 필요
                            if (enemy[i].life == 0)
                            {
                                enemy[i].explosion_sound.play();
                            }
                            bullet[j].is_fired = 0;
                        }
                    }
                }
                enemy[i].sprite.move(enemy[i].speed, 0);
            }
   
        }

       
        sprintf(info, "Life : %d | Score : %d | Time : %d"
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
        for (int i = 0; i < BULLET_NUM; i++)
            if (bullet[i].is_fired)
                window.draw(bullet[i].sprite);

        if (is_gameover) {
            window.draw(gameover_sprite);
            //TODO : 게임이 멈추는 것을 구현할 것
        }

        window.display();
    }

    return 0;
}