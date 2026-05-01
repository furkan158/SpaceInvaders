// Uzay gemisi oyunu - Space Invaders
// YZM104 Programlama II - Oyun Projesi

#include <SFML/Graphics.hpp>  // SFML grafik kütüphanesi
#include "Player.h"           // Oyuncu gemisi sınıfı
#include "Bullet.h"           // Mermi sınıfı
#include <vector>             // Birden fazla mermi tutmak için
#include <algorithm>          // remove_if için gerekli

int main()
{
    // 800x600 piksel boyutunda pencere oluştur
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Space Invaders");

    // Kare hızını 60 FPS ile sınırla
    window.setFramerateLimit(60);

    // Oyuncu gemisini oluştur
    Player player;

    // Mermileri tutan liste (vector = dinamik dizi)
    std::vector<Bullet> bullets;

    // Space tuşu kontrolü için - her basışta bir mermi çıksın
    bool spacePressed = false;

    // Ana oyun döngüsü
    while (window.isOpen())
    {
        // Olayları kontrol et
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Space tuşuna basılıysa mermi oluştur
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        {
            if (!spacePressed) // Tuş ilk kez basıldıysa
            {
                // Geminin konumundan mermi oluştur
                bullets.push_back(Bullet(player.getPosition()));
                spacePressed = true;
            }
        }
        else
        {
            spacePressed = false; // Tuş bırakıldı
        }

        // Oyuncuyu güncelle
        player.update();

        // Tüm mermileri güncelle
        for (auto& bullet : bullets)
            bullet.update();

        // Ekran dışına çıkan mermileri sil
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                [](Bullet& b) { return b.isOffScreen(); }),
            bullets.end()
        );

        // Ekranı temizle
        window.clear(sf::Color::Black);

        // Gemiyi çiz
        player.draw(window);

        // Tüm mermileri çiz
        for (auto& bullet : bullets)
            bullet.draw(window);

        // Ekrana yansıt
        window.display();
    }

    return 0;
}