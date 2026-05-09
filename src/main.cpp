// Uzay gemisi oyunu - Space Invaders
// YZM104 Programlama II - Oyun Projesi


#include <string>  // to_string için
#include <SFML/Graphics.hpp>  // SFML grafik kütüphanesi
#include "Player.h"           // Oyuncu gemisi sınıfı
#include "Bullet.h"           // Oyuncu mermisi sınıfı
#include "Enemy.h"            // Düşman sınıfı
#include "EnemyBullet.h"      // Düşman mermisi sınıfı
#include <vector>             // Dinamik dizi
#include <algorithm>          // remove_if için
#include <cstdlib>            // rand() için
#include <ctime>              // time() için

int main()
{
    
    srand(time(0));  // Rastgele sayı üreteci için tohum ayarladık

    // 800x600 piksel boyutunda pencere oluşturduk
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Space Invaders");
    window.setFramerateLimit(60);

    // Oyuncu gemisini oluşturduk
    Player player;

    
    int score = 0;    // Skor değişkeni
    int lives = 3;    // Can değişkeni (3 can ile başla)
    
    // Oyun durumu
    bool gameOver = false;   
    bool playerWon = false;
    
    
    // Font ve yazı nesneleri
    sf::Font font;
    font.openFromFile("assets/impact.ttf"); // Fontu yükle
    
    sf::Text scoreText(font);               // Skor yazısı
    scoreText.setCharacterSize(24);         // Yazı boyutu
    scoreText.setFillColor(sf::Color::White); // Yazı rengi
    scoreText.setPosition({10, 10});        // Sol üst köşe

    sf::Text livesText(font);              // Can yazısı
    livesText.setCharacterSize(24);        // Yazı boyutu
    livesText.setFillColor(sf::Color::White); // Yazı rengi
    livesText.setPosition({650, 10});      // Sağ üst köşe

    // Oyuncu mermi listesi
    std::vector<Bullet> bullets;

    // Düşman listesi
    std::vector<Enemy> enemies;

    // Düşman mermi listesi
    std::vector<EnemyBullet> enemyBullets;

    // Düşman formasyonu oluştur (5 satır x 10 sütun)
    for (int row = 0; row < 5; row++)
    {
        for (int col = 0; col < 10; col++)
        {
            float x = 50 + col * 70;
            float y = 50 + row * 50;
            enemies.push_back(Enemy(sf::Vector2f(x, y)));
        }
    }

    // Düşman hareket yönü (1 = sağ, -1 = sol)
    int direction = 1;

    // Düşman hareket sayacı
    int moveTimer = 0;

    // Düşman ateş sayacı
    int shootTimer = 0;

    // Space tuşu kontrolü
    bool spacePressed = false;

    // Ana oyun döngüsü
    while (window.isOpen())
    {
        // Oyun bitti veya kazandıysa döngüyü durdur
        if (gameOver || playerWon)
            break;
        // Olayları kontrol et
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Space tuşuyla ateş et
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        {
            if (!spacePressed)
            {
                bullets.push_back(Bullet(player.getPosition()));
                spacePressed = true;
            }
        }
        else
        {
            spacePressed = false;
        }

        // Oyuncuyu güncelle
        player.update();

        // Oyuncu mermilerini güncelle
        for (auto& bullet : bullets)
            bullet.update();

        // Ekran dışına çıkan oyuncu mermilerini sil
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                [](Bullet& b) { return b.isOffScreen(); }),
            bullets.end()
        );
        

        // Tüm düşmanlar öldü mü kontrol et
        bool allDead = true;
        for (auto& enemy : enemies)
        {
            if (enemy.isAlive())
            {
                allDead = false;
                break;
            }
        }
        if (allDead)
            playerWon = true; // Oyuncu kazandı


        // Düşman hareketi
        moveTimer++;
        if (moveTimer >= 10)
        {
            moveTimer = 0;

            bool hitRight = false;
            bool hitLeft = false;

            for (auto& enemy : enemies)
            {
                if (!enemy.isAlive()) continue;
                if (enemy.getBounds().position.x + enemy.getBounds().size.x >= 780)
                    hitRight = true;
                if (enemy.getBounds().position.x <= 20)
                    hitLeft = true;
            }

            if (hitRight && direction == 1)
            {
                direction = -1;
                for (auto& enemy : enemies)
                    enemy.moveDown();
            }
            else if (hitLeft && direction == -1)
            {
                direction = 1;
                for (auto& enemy : enemies)
                    enemy.moveDown();
            }
            else
            {
                for (auto& enemy : enemies)
                {
                    if (direction == 1)
                        enemy.moveRight();
                    else
                        enemy.moveLeft();
                }
            }
        }

        // Düşman ateş etme (her 60 karede bir rastgele düşman ateş eder)
        shootTimer++;
        if (shootTimer >= 60)
        {
            shootTimer = 0;

            // Hayatta olan düşmanları bul
            std::vector<int> aliveIndices;
            for (int i = 0; i < enemies.size(); i++)
            {
                if (enemies[i].isAlive())
                    aliveIndices.push_back(i);
            }

            // Rastgele bir düşman seç ve ateş ettir
            if (!aliveIndices.empty())
            {
                int randomIndex = aliveIndices[rand() % aliveIndices.size()];
                enemyBullets.push_back(EnemyBullet(enemies[randomIndex].getBounds().position));
            }
        }

        // Düşman mermilerini güncelle
        for (auto& eb : enemyBullets)
            eb.update();

        // Oyuncu mermisi - düşman çarpışma kontrolü
        for (auto& bullet : bullets)
        {
            for (auto& enemy : enemies)
            {
                if (!enemy.isAlive()) continue;
                if (bullet.getBounds().findIntersection(enemy.getBounds()))
                {
                    enemy.kill();
                    bullet.setOffScreen();
                    score += 10; // Düşman öldürünce 10 puan ekle
                }
            }
        }

        // Düşman mermisi - oyuncu çarpışma kontrolü
        for (auto& eb : enemyBullets)
        {
            if (eb.getBounds().findIntersection(player.getBounds()))
            {
                eb.setOffScreen(); // Mermiyi sil
                lives--; // Bir can azalt
                if (lives <= 0)
                    gameOver = true; // Can bitti, oyun bitti
            }
        }
        
        // Ekran dışına çıkan düşman mermilerini sil
        enemyBullets.erase(
            std::remove_if(enemyBullets.begin(), enemyBullets.end(),
                [](EnemyBullet& eb) { return eb.isOffScreen(); }),
            enemyBullets.end()
        );

        
        scoreText.setString("SKOR: " + std::to_string(score)); // Skor yazısını güncelle
        livesText.setString("CAN: " + std::to_string(lives)); // Can yazısını güncelle

        // Ekranı temizle
        window.clear(sf::Color::Black);

        // Gemiyi çiz
        player.draw(window);

        // Oyuncu mermilerini çiz
        for (auto& bullet : bullets)
            bullet.draw(window);

        // Düşmanları çiz
        for (auto& enemy : enemies)
            enemy.draw(window);

        // Düşman mermilerini çiz
        for (auto& eb : enemyBullets)
            eb.draw(window);

        
        window.draw(scoreText);   // Skoru ekrana çiz
        window.draw(livesText);   // Canı ekrana çiz
        
            // Ekrana yansıt
        window.display();
    }
    
    // Oyun bitti ekranı
    while (window.isOpen())
    {
        // Olayları kontrol et
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::KeyPressed>())
            {
                if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)
                    window.close();
            }
        }

        // Ekranı temizle
        window.clear(sf::Color::Black);

        // Arka kutu - ortalı
        sf::RectangleShape box;
        box.setSize({500, 280});
        box.setFillColor(sf::Color(17, 17, 17));
        box.setOutlineColor(sf::Color::Red);
        box.setOutlineThickness(2);
        box.setPosition({150, 160});

        // Oyun sonu yazısı - ortalı
        sf::Text endText(font);
        endText.setCharacterSize(70);
        if (gameOver)
        {
            endText.setFillColor(sf::Color::Red);
            endText.setString("GAME OVER");
        }
        else
        {
            endText.setFillColor(sf::Color::Green);
            endText.setString("KAZANDIN!");
        }
        sf::FloatRect endBounds = endText.getLocalBounds();
        endText.setOrigin({endBounds.size.x / 2.f, 0});
        endText.setPosition({400, 170});

        // Ayırıcı çizgi
        sf::RectangleShape line;
        line.setSize({460, 1});
        line.setFillColor(sf::Color(51, 51, 51));
        line.setPosition({170, 255});

        // Skor yazısı - ortalı
        sf::Text scoreEndText(font);
        scoreEndText.setCharacterSize(36);
        scoreEndText.setFillColor(sf::Color::White);
        scoreEndText.setString("SKOR: " + std::to_string(score));
        sf::FloatRect scoreBounds = scoreEndText.getLocalBounds();
        scoreEndText.setOrigin({scoreBounds.size.x / 2.f, 0});
        scoreEndText.setPosition({400, 265});

        // Can yazısı - ortalı
        sf::Text livesEndText(font);
        livesEndText.setCharacterSize(32);
        livesEndText.setFillColor(sf::Color(255, 165, 0));
        livesEndText.setString("CAN: " + std::to_string(lives));
        sf::FloatRect livesBounds = livesEndText.getLocalBounds();
        livesEndText.setOrigin({livesBounds.size.x / 2.f, 0});
        livesEndText.setPosition({400, 315});

        // Çıkış yazısı - ortalı
        sf::Text exitText(font);
        exitText.setCharacterSize(18);
        exitText.setFillColor(sf::Color(85, 85, 85));
        exitText.setString("Cikmak icin ESC tusuna bas");
        sf::FloatRect exitBounds = exitText.getLocalBounds();
        exitText.setOrigin({exitBounds.size.x / 2.f, 0});
        exitText.setPosition({400, 400});

        // Çiz
        window.draw(box);
        window.draw(line);
        window.draw(endText);
        window.draw(scoreEndText);
        window.draw(livesEndText);
        window.draw(exitText);

        // Ekrana yansıt
        window.display();
    }

    return 0;
}