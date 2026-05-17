// Uzay gemisi oyunu - Space Invaders
// YZM104 Programlama II - Oyun Projesi


#include <string>  // to_string için
#include <SFML/Graphics.hpp>  // SFML grafik kütüphanesi
#include <SFML/Audio.hpp>     // Ses kütüphanesi
#include "Player.h"           // Oyuncu gemisi sınıfı
#include "Bullet.h"           // Oyuncu mermisi sınıfı
#include "Enemy.h"            // Düşman sınıfı
#include "EnemyBullet.h"      // Düşman mermisi sınıfı
#include "Barrier.h"          // Bariyer sınıfı
#include <vector>             // Dinamik dizi
#include <algorithm>          // remove_if için
#include <cstdlib>            // rand() için
#include <ctime>              // time() için

int main()
{
    
    srand(time(0));  // Rastgele sayı üreteci için tohum ayarladık

    // 800x600 piksel boyutunda pencere oluşturduk
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Space Invaders");
    window.setFramerateLimit(60); // Kare hızını 60 FPS ile sınırladık

    // Oyuncu gemisini oluşturduk
    Player player;

    // Yıldızlı arka plan için rastgele yıldız konumları
    std::vector<sf::CircleShape> stars;
    for (int i = 0; i < 50; i++)
    {
        sf::CircleShape star;
        star.setRadius(1.0f);
        star.setFillColor(sf::Color(255, 0, 255)); // Pembe yıldızlar
        star.setPosition({(float)(rand() % 800), (float)(rand() % 600)});
        stars.push_back(star);
    }

    
    int score = 0;    // Skor değişkeni
    int lives = 3;    // Can değişkeni (3 can ile başla)
    int level = 1;     // Seviye değişkeni
    
    // Oyun durumu
    bool gameOver = false;   
    bool playerWon = false;
    
    
    // Font ve yazı nesneleri
    sf::Font font;
    font.openFromFile("assets/impact.ttf"); // Fontu yükle

    // Ses dosyalarını yükle
    sf::SoundBuffer shootBuffer;
    shootBuffer.loadFromFile("assets/mixkit-short-laser-gun-shot-1670.wav");
    sf::Sound shootSound(shootBuffer);  // Ateş sesi

    sf::SoundBuffer explosionBuffer;
    explosionBuffer.loadFromFile("assets/mixkit-arcade-game-explosion-2759.wav");
    sf::Sound explosionSound(explosionBuffer);  // Patlama sesi
    
    sf::Text scoreText(font);               // Skor yazısı
    scoreText.setCharacterSize(24);         // Yazı boyutu
    scoreText.setFillColor(sf::Color::White); // Yazı rengi
    scoreText.setPosition({20, 15});        // Sol üst köşe

    sf::Text livesText(font);              // Can yazısı
    livesText.setCharacterSize(24);        // Yazı boyutu
    livesText.setFillColor(sf::Color::White); // Yazı rengi
    livesText.setPosition({713, 15});      // Sağ üst köşe

    sf::Text levelText(font);              // Seviye yazısı
    levelText.setCharacterSize(24);        // Yazı boyutu
    levelText.setFillColor(sf::Color::Yellow); // Yazı rengi
    levelText.setPosition({375, 15});      // Üst orta

    // Oyuncu mermi listesi
    std::vector<Bullet> bullets;

    // Düşman listesi
    std::vector<Enemy> enemies;

    // Düşman mermi listesi
    std::vector<EnemyBullet> enemyBullets;

    // Bariyer listesi
    std::vector<Barrier> barriers;

    // 4 bariyer oluştur (alt kısımda eşit aralıklı)
    barriers.push_back(Barrier(sf::Vector2f(100, 480)));
    barriers.push_back(Barrier(sf::Vector2f(270, 480)));
    barriers.push_back(Barrier(sf::Vector2f(440, 480)));
    barriers.push_back(Barrier(sf::Vector2f(610, 480)));

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


    // Ana menü döngüsü
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::KeyPressed>())
            {
                if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Enter)
                    goto startGame; // Enter'a basılınca oyunu başlat
            }
        }

        window.clear(sf::Color::Black);

        // Yıldızları çiz
        for (auto& star : stars)
            window.draw(star);

        // Dış çerçeve - pembe
        sf::RectangleShape outerBox;
        outerBox.setSize({790, 590});
        outerBox.setFillColor(sf::Color::Transparent);
        outerBox.setOutlineColor(sf::Color(255, 0, 255));
        outerBox.setOutlineThickness(2);
        outerBox.setPosition({5, 5});

        // İç çerçeve - cyan ince
        sf::RectangleShape innerBox;
        innerBox.setSize({774, 574});
        innerBox.setFillColor(sf::Color::Transparent);
        innerBox.setOutlineColor(sf::Color(0, 255, 255, 60));
        innerBox.setOutlineThickness(1);
        innerBox.setPosition({13, 13});

        // Köşe süsleri - sol üst
        sf::RectangleShape c1h, c1v;
        c1h.setSize({30, 3}); c1h.setFillColor(sf::Color(0, 255, 255)); c1h.setPosition({5, 5});
        c1v.setSize({3, 30}); c1v.setFillColor(sf::Color(0, 255, 255)); c1v.setPosition({5, 5});

        // Köşe süsleri - sağ üst
        sf::RectangleShape c2h, c2v;
        c2h.setSize({30, 3}); c2h.setFillColor(sf::Color(0, 255, 255)); c2h.setPosition({765, 5});
        c2v.setSize({3, 30}); c2v.setFillColor(sf::Color(0, 255, 255)); c2v.setPosition({792, 5});

        // Köşe süsleri - sol alt
        sf::RectangleShape c3h, c3v;
        c3h.setSize({30, 3}); c3h.setFillColor(sf::Color(0, 255, 255)); c3h.setPosition({5, 592});
        c3v.setSize({3, 30}); c3v.setFillColor(sf::Color(0, 255, 255)); c3v.setPosition({5, 562});

        // Köşe süsleri - sağ alt
        sf::RectangleShape c4h, c4v;
        c4h.setSize({30, 3}); c4h.setFillColor(sf::Color(0, 255, 255)); c4h.setPosition({765, 592});
        c4v.setSize({3, 30}); c4v.setFillColor(sf::Color(0, 255, 255)); c4v.setPosition({792, 562});

        // Ayırıcı çizgi
        sf::RectangleShape menuLine;
        menuLine.setSize({740, 1});
        menuLine.setFillColor(sf::Color(255, 0, 255, 120));
        menuLine.setPosition({30, 310});

        // SPACE INVADERS yazısı
        sf::Text titleText(font);
        titleText.setCharacterSize(80);
        titleText.setFillColor(sf::Color(0, 255, 255));
        titleText.setString("SPACE INVADERS");
        sf::FloatRect titleBounds = titleText.getLocalBounds();
        titleText.setOrigin({titleBounds.size.x / 2.f, 0});
        titleText.setPosition({400, 180});

        // ENTER yazısı
        sf::Text startText(font);
        startText.setCharacterSize(32);
        startText.setFillColor(sf::Color(255, 0, 255));
        startText.setString("< ENTER: Baslat >");
        sf::FloatRect startBounds = startText.getLocalBounds();
        startText.setOrigin({startBounds.size.x / 2.f, 0});
        startText.setPosition({400, 340});

        // Kontroller yazısı
        sf::Text controlsText(font);
        controlsText.setCharacterSize(20);
        controlsText.setFillColor(sf::Color(102, 0, 255));
        controlsText.setString("Sol/Sag: Hareket      Space: Ates      ESC: Cik");
        sf::FloatRect controlsBounds = controlsText.getLocalBounds();
        controlsText.setOrigin({controlsBounds.size.x / 2.f, 0});
        controlsText.setPosition({400, 410});

        // Versiyon yazısı
        sf::Text versionText(font);
        versionText.setCharacterSize(16);
        versionText.setFillColor(sf::Color(34, 34, 34));
        versionText.setString("YZM104 - Programlama II");
        sf::FloatRect versionBounds = versionText.getLocalBounds();
        versionText.setOrigin({versionBounds.size.x / 2.f, 0});
        versionText.setPosition({400, 510});

        window.draw(outerBox);
        window.draw(innerBox);
        window.draw(c1h); window.draw(c1v);
        window.draw(c2h); window.draw(c2v);
        window.draw(c3h); window.draw(c3v);
        window.draw(c4h); window.draw(c4v);
        window.draw(menuLine);
        window.draw(titleText);
        window.draw(startText);
        window.draw(controlsText);
        window.draw(versionText);
        window.display();
    }

    startGame:

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
                shootSound.play(); // Ateş sesi çal
                spacePressed = true;    // Tuş bırakıldı
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
        
        // Düşman alta ulaştı mı kontrol et
        for (auto& enemy : enemies)
        {
            if (enemy.isAlive() && enemy.getBounds().position.y + enemy.getBounds().size.y >= 560)
            {
                gameOver = true; // Düşman alta ulaştı, oyun bitti
                break;
            }
        }
        
        if (allDead)
        {
            level++;          // Seviyeyi artır
            
            if (level > 99)    // 99. seviyeden sonra oyun bitti
            {
                playerWon = true;
            }
            else
            {
                // Yeni seviye için düşmanları yeniden oluştur
                enemies.clear();
                for (int row = 0; row < 5; row++)
                {
                    for (int col = 0; col < 10; col++)
                    {
                        float x = 50 + col * 70;
                        float y = 50 + row * 50;
                        enemies.push_back(Enemy(sf::Vector2f(x, y)));
                    }
                }
                // Mermileri temizle
                bullets.clear();
                enemyBullets.clear();
            }
        }


        // Düşman hareketi
        moveTimer++;
        if (moveTimer >= (10 - level * 2))
        {
            moveTimer = 0;

            bool hitRight = false;  // Sağ veya sol kenara çarpan düşman var mı kontrol et
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
        if (shootTimer >= (60 - level * 5))
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
                    explosionSound.play(); // Patlama sesi çal
                }
            }
        }

        // Oyuncu mermisi - bariyer çarpışma kontrolü
        for (auto& bullet : bullets)
        {
            for (auto& barrier : barriers)
            {
                if (!barrier.isAlive()) continue;
                if (bullet.getBounds().findIntersection(barrier.getBounds()))
                {
                    barrier.takeDamage(); // Bariyere hasar ver
                    bullet.setOffScreen(); // Mermiyi sil
                }
            }
        }

        // Düşman mermisi - bariyer çarpışma kontrolü
        for (auto& eb : enemyBullets)
        {
            for (auto& barrier : barriers)
            {
                if (!barrier.isAlive()) continue;
                if (eb.getBounds().findIntersection(barrier.getBounds()))
                {
                    barrier.takeDamage(); // Bariyere hasar ver
                    eb.setOffScreen();    // Mermiyi sil
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
        levelText.setString("LEVEL: " + std::to_string(level)); // Seviye yazısını güncelle

        // Ekranı temizle
        window.clear(sf::Color::Black);
        // Yıldızları çiz
        for (auto& star : stars)
            window.draw(star);

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

        // Bariyerleri çiz
        for (auto& barrier : barriers)
            barrier.draw(window);
        window.draw(scoreText);   // Skoru ekrana çiz
        window.draw(livesText);   // Canı ekrana çiz
        window.draw(levelText);   // Seviyeyi ekrana çiz
        
        // Dış çerçeve - pembe
        sf::RectangleShape gameOuterBox;
        gameOuterBox.setSize({790, 590});
        gameOuterBox.setFillColor(sf::Color::Transparent);
        gameOuterBox.setOutlineColor(sf::Color(255, 0, 255));
        gameOuterBox.setOutlineThickness(2);
        gameOuterBox.setPosition({5, 5});

        // İç çerçeve - cyan ince
        sf::RectangleShape gameInnerBox;
        gameInnerBox.setSize({774, 574});
        gameInnerBox.setFillColor(sf::Color::Transparent);
        gameInnerBox.setOutlineColor(sf::Color(0, 255, 255, 60));
        gameInnerBox.setOutlineThickness(1);
        gameInnerBox.setPosition({13, 13});

        // Köşe süsleri - sol üst
        sf::RectangleShape gc1h, gc1v;
        gc1h.setSize({30, 3}); gc1h.setFillColor(sf::Color(0, 255, 255)); gc1h.setPosition({5, 5});
        gc1v.setSize({3, 30}); gc1v.setFillColor(sf::Color(0, 255, 255)); gc1v.setPosition({5, 5});

        // Köşe süsleri - sağ üst
        sf::RectangleShape gc2h, gc2v;
        gc2h.setSize({30, 3}); gc2h.setFillColor(sf::Color(0, 255, 255)); gc2h.setPosition({765, 5});
        gc2v.setSize({3, 30}); gc2v.setFillColor(sf::Color(0, 255, 255)); gc2v.setPosition({792, 5});

        // Köşe süsleri - sol alt
        sf::RectangleShape gc3h, gc3v;
        gc3h.setSize({30, 3}); gc3h.setFillColor(sf::Color(0, 255, 255)); gc3h.setPosition({5, 592});
        gc3v.setSize({3, 30}); gc3v.setFillColor(sf::Color(0, 255, 255)); gc3v.setPosition({5, 562});

        // Köşe süsleri - sağ alt
        sf::RectangleShape gc4h, gc4v;
        gc4h.setSize({30, 3}); gc4h.setFillColor(sf::Color(0, 255, 255)); gc4h.setPosition({765, 592});
        gc4v.setSize({3, 30}); gc4v.setFillColor(sf::Color(0, 255, 255)); gc4v.setPosition({792, 562});

        window.draw(gameOuterBox);
        window.draw(gameInnerBox);
        window.draw(gc1h); window.draw(gc1v);
        window.draw(gc2h); window.draw(gc2v);
        window.draw(gc3h); window.draw(gc3v);
        window.draw(gc4h); window.draw(gc4v);
            // Ekrana yansıt
        window.display();
    }
    
    // Oyun bitti ekranı
    while (window.isOpen())
    {
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

        window.clear(sf::Color::Black);

        // Yıldızları çiz
        for (auto& star : stars)
            window.draw(star);

        // Dış çerçeve - pembe
        sf::RectangleShape outerBox;
        outerBox.setSize({790, 590});
        outerBox.setFillColor(sf::Color::Transparent);
        outerBox.setOutlineColor(sf::Color(255, 0, 255));
        outerBox.setOutlineThickness(2);
        outerBox.setPosition({5, 5});

        // İç çerçeve - cyan ince
        sf::RectangleShape innerBox;
        innerBox.setSize({774, 574});
        innerBox.setFillColor(sf::Color::Transparent);
        innerBox.setOutlineColor(sf::Color(0, 255, 255, 60));
        innerBox.setOutlineThickness(1);
        innerBox.setPosition({13, 13});

        // Köşe süsleri - sol üst
        sf::RectangleShape c1h, c1v;
        c1h.setSize({30, 3}); c1h.setFillColor(sf::Color(0, 255, 255)); c1h.setPosition({5, 5});
        c1v.setSize({3, 30}); c1v.setFillColor(sf::Color(0, 255, 255)); c1v.setPosition({5, 5});

        // Köşe süsleri - sağ üst
        sf::RectangleShape c2h, c2v;
        c2h.setSize({30, 3}); c2h.setFillColor(sf::Color(0, 255, 255)); c2h.setPosition({765, 5});
        c2v.setSize({3, 30}); c2v.setFillColor(sf::Color(0, 255, 255)); c2v.setPosition({792, 5});

        // Köşe süsleri - sol alt
        sf::RectangleShape c3h, c3v;
        c3h.setSize({30, 3}); c3h.setFillColor(sf::Color(0, 255, 255)); c3h.setPosition({5, 592});
        c3v.setSize({3, 30}); c3v.setFillColor(sf::Color(0, 255, 255)); c3v.setPosition({5, 562});

        // Köşe süsleri - sağ alt
        sf::RectangleShape c4h, c4v;
        c4h.setSize({30, 3}); c4h.setFillColor(sf::Color(0, 255, 255)); c4h.setPosition({765, 592});
        c4v.setSize({3, 30}); c4v.setFillColor(sf::Color(0, 255, 255)); c4v.setPosition({792, 562});

        // Ayırıcı çizgi
        sf::RectangleShape endLine;
        endLine.setSize({740, 1});
        endLine.setFillColor(sf::Color(255, 0, 255, 120));
        endLine.setPosition({30, 310});

        // Oyun sonu yazısı
        sf::Text endText(font);
        endText.setCharacterSize(90);
        if (gameOver)
        {
            endText.setFillColor(sf::Color(255, 0, 255));
            endText.setString("GAME OVER");
        }
        else
        {
            endText.setFillColor(sf::Color(0, 255, 255));
            endText.setString("KAZANDIN!");
        }
        sf::FloatRect endBounds = endText.getLocalBounds();
        endText.setOrigin({endBounds.size.x / 2.f, 0});
        endText.setPosition({400, 160});

        // Skor yazısı
        sf::Text scoreEndText(font);
        scoreEndText.setCharacterSize(36);
        scoreEndText.setFillColor(sf::Color::White);
        scoreEndText.setString("SKOR: " + std::to_string(score));
        sf::FloatRect scoreBounds = scoreEndText.getLocalBounds();
        scoreEndText.setOrigin({scoreBounds.size.x / 2.f, 0});
        scoreEndText.setPosition({400, 340});

        // Can yazısı
        sf::Text livesEndText(font);
        livesEndText.setCharacterSize(30);
        livesEndText.setFillColor(sf::Color(255, 165, 0));
        livesEndText.setString("CAN: " + std::to_string(lives));
        sf::FloatRect livesBounds = livesEndText.getLocalBounds();
        livesEndText.setOrigin({livesBounds.size.x / 2.f, 0});
        livesEndText.setPosition({400, 395});

        // Seviye yazısı
        sf::Text levelEndText(font);
        levelEndText.setCharacterSize(30);
        levelEndText.setFillColor(sf::Color::Yellow);
        levelEndText.setString("LEVEL: " + std::to_string(level));
        sf::FloatRect levelBounds = levelEndText.getLocalBounds();
        levelEndText.setOrigin({levelBounds.size.x / 2.f, 0});
        levelEndText.setPosition({400, 445});

        // Çıkış yazısı
        sf::Text exitText(font);
        exitText.setCharacterSize(20);
        exitText.setFillColor(sf::Color(102, 0, 255));
        exitText.setString("Cikmak icin ESC tusuna bas");
        sf::FloatRect exitBounds = exitText.getLocalBounds();
        exitText.setOrigin({exitBounds.size.x / 2.f, 0});
        exitText.setPosition({400, 520});

        window.draw(outerBox);
        window.draw(innerBox);
        window.draw(c1h); window.draw(c1v);
        window.draw(c2h); window.draw(c2v);
        window.draw(c3h); window.draw(c3v);
        window.draw(c4h); window.draw(c4v);
        window.draw(endLine);
        window.draw(endText);
        window.draw(scoreEndText);
        window.draw(livesEndText);
        window.draw(levelEndText);
        window.draw(exitText);
        window.display();
    }

    return 0;
}