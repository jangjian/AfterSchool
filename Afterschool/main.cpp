/*

TODO list
1) ������ : �ӵ�����, ������ ����
2) �Ѿ� �ý��� ����
3) sound effect ����


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
    float x, y;     //�÷��̾� ��ǥ
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
    int is_fired; //�߻� ����
};
struct Textures {
    Texture bg;         // ��� �̹���
    Texture enemy;     // �� �̹���
    Texture gameover;   // ���ӿ��� �̹���
    Texture player;     // �÷��̾� �̹���
};

//obj1�� obj2�� �浹 ����. �浹�ϸ� 1�� ��ȯ, ���ϸ� 0�� ��ȯ
int is_collide(RectangleShape obj1, RectangleShape obj2)
{
    return obj1.getGlobalBounds().intersects(obj2.getGlobalBounds());
}

//�������� 
const int ENEMY_NUM = 10;                   //enemy�� �ִ� ����           
const int BULLET_NUM = 50;                  //bullet�� �ִ� ����
const int W_WIDTH = 1200, W_HEIGHT = 600;   //â�� ũ��
const int GO_WIDTH = 640, GO_HEIGHT = 450;  //���ӿ��� �׸��� ũ��



int main(void)
{

    struct Textures t;
    t.bg.loadFromFile("./resources/images/background.png");
    t.enemy.loadFromFile("./resources/images/enemy.png");
    t.gameover.loadFromFile("./resources/images/gameover.png");
    t.player.loadFromFile("./resources/images/player.png");

    //������â ����
    RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool");
    window.setFramerateLimit(60);

    srand(time(0));

    long start_time = clock(); //���� ���� �ð�
    long spent_time; //���� ���� �ð�
    int is_gameover = 0;

    //BGM
    SoundBuffer BGM_buffer;
    BGM_buffer.loadFromFile("./resources/sounds/bgm.ogg");
    Sound BGM_sound;
    BGM_sound.setBuffer(BGM_buffer);
    BGM_sound.setLoop(1);       //BGM ���ѹݺ�
    BGM_sound.play();

    //text
    Font font;
    font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
    
    Text text;
    text.setFont(font);
    text.setCharacterSize(30);  //��Ʈ ũ��
    text.setFillColor(Color(255, 255, 255));
    text.setPosition(0, 0);
    char info[40];

    //���
    Sprite bg_sprite;
    bg_sprite.setTexture(t.bg);
    bg_sprite.setPosition(0, 0);

    //���ӿ���
    
    Sprite gameover_sprite;
    gameover_sprite.setTexture(t.gameover);
    gameover_sprite.setPosition((W_WIDTH-GO_WIDTH)/2, (W_HEIGHT-GO_HEIGHT)/2);
    
    // �÷��̾�
    struct Player player;
    player.sprite.setTexture(&t.player);
    player.sprite.setPosition(100, 100);
    player.sprite.setSize(Vector2f(170,181));
    player.x = player.sprite.getPosition().x;
    player.y = player.sprite.getPosition().y;
    player.speed = 5;
    player.score = 0;
    player.life = 3;

    // �Ѿ�
    int bullet_speed = 20;
    int bullet_idx = 0;

    struct Bullet bullet[BULLET_NUM] ;

    for (int i = 0; i < BULLET_NUM; i++) {
        bullet[i].sprite.setSize(Vector2f(10, 10));
        bullet[i].sprite.setPosition(player.x + 50, player.y + 15);        //�ӽ� �׽�Ʈ
        bullet[i].is_fired = 0;
    }
    
    
    // ��
    struct Enemy enemy[ENEMY_NUM];

    //enemy �ʱ�ȭ
    for (int i = 0; i < ENEMY_NUM; i++)
    {
        // TODO : ������ ��ȿ������ �ڵ��̹Ƿ� ���߿� refactoring
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

    //�����찡 �������� ������ �ݺ�
    while (window.isOpen())
    {
        spent_time = clock() - start_time;
        player.x = player.sprite.getPosition().x;
        player.y = player.sprite.getPosition().y;

        Event event;
        while (window.pollEvent(event)) {
            //����(X) ��ư�� ������ Event::Closed
            switch (event.type)
            {
            case Event::Closed:
                window.close();//������ �ݴ´�.
                break;
            //Ű���带 ������ ��(���� �������� ����)
            case Event::KeyPressed:
                {
                ////�����̽� Ű ������ �� �߻�
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


        /*game ���� update*/
        if (player.life <= 0) {
            is_gameover = 1;
        }

        /*Player update*/
        //����Ű start
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
        }//����Ű end

        //Player �̵� ���� ����
        if (player.x < 0) 
            player.sprite.setPosition(0, player.y);
        else if(player.x > W_WIDTH - 170)       //�׸��� ��
            player.sprite.setPosition(W_WIDTH - 170, player.y);

        if (player.y < 0) 
            player.sprite.setPosition(player.x, 0);
        else if (player.y > W_HEIGHT - 181)     //�׸��� ����
            player.sprite.setPosition(player.x, W_HEIGHT - 181);
       

        /*Bullet update*/
       //�Ѿ� �߻�
        //TODO : 50�� ���ĺ��ʹ� �ȳ����� ���� ������ ��
        printf("bullet_idx %d\n", bullet_idx);
        if (Keyboard::isKeyPressed(Keyboard::Space))
        {
            if (!bullet[bullet_idx].is_fired)
            {
                bullet[bullet_idx].sprite.setPosition(player.x + 50, player.y);
                bullet[bullet_idx].is_fired = 1;
                bullet_idx++;   //���� �Ѿ��� �߻��� �� �ֵ���
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
           // 10�ʸ��� enemy�� ��
            if (spent_time % (1000 * enemy[i].respawn_time) < 1000 / 60 + 1)
            {
                enemy[i].sprite.setTexture(&t.enemy);
                enemy[i].sprite.setSize(Vector2f(70, 70));
                enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 380);
                enemy[i].life = 1;
                // 10�ʸ��� enemy�� �ӵ�+1
                enemy[i].speed = -(rand() % 10 + 1 + (spent_time / 1000 / enemy[i].respawn_time));
            }

            if (enemy[i].life > 0)
            {
                // player, enemy���� �浹
                if (is_collide(player.sprite, enemy[i].sprite))
                {
                    enemy[i].life -= 1;
                    player.score += enemy[i].score;

                    //TODO : �ڵ� refactoring �ʿ�
                    if (enemy[i].life == 0)
                    {
                        enemy[i].explosion_sound.play();
                    }
                }
                //���� ���� ���� �����Ϸ��� ����
                else if (enemy[i].sprite.getPosition().x < 0)
                {
                    player.life -= 1; 
                    enemy[i].life = 0;
                }

                // bullet, enemy���� �浹
                for (int j = 0; j < BULLET_NUM; j++)
                {
                    if (is_collide(bullet[j].sprite, enemy[i].sprite))
                    {
                        if (bullet[j].is_fired)
                        {
                            enemy[i].life -= 1;
                            player.score += enemy[i].score;

                            //TODO : �ڵ� refactoring �ʿ�
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

        //draw�� ���߿� ȣ���Ҽ��� �켱 ������ ������
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
            //TODO : ������ ���ߴ� ���� ������ ��
        }

        window.display();
    }

    return 0;
}