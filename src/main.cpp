// Uzay gemisi oyunu - Space Invaders
// YZM104 Programlama II - Oyun Projesi

#include <SFML/Graphics.hpp>  // SFML grafik kütüphanesi
#include "Player.h"           // Oyuncu gemisi sınıfı

int main()
{
    // 800x600 piksel boyutunda "Space Invaders" başlıklı pencere oluştur
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Space Invaders");

    // Kare hızını 60 FPS ile sınırla (saniyede 60 kare)
    window.setFramerateLimit(60);

    // Oyuncu gemisini oluştur
    Player player;

    // Ana oyun döngüsü - pencere açık olduğu sürece çalışır
    while (window.isOpen())
    {
        // Olayları kontrol et
        while (const std::optional event = window.pollEvent())
        {
            // Kullanıcı pencereyi kapattıysa oyunu bitir
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Oyuncu gemisini güncelle (hareketi işle)
        player.update();

        // Ekranı siyah renkle temizle
        window.clear(sf::Color::Black);

        // Oyuncu gemisini çiz
        player.draw(window);

        // Çizilen her şeyi ekrana yansıt
        window.display();
    }

    return 0; // Program başarıyla bitti
}