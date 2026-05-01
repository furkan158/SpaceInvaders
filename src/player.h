// Oyuncu gemisinin özelliklerini tanımlayan başlık dosyası
#pragma once

#include <SFML/Graphics.hpp>

class Player
{
public:
    // Yapıcı fonksiyon - gemi oluşturulurken çalışır
    Player();

    // Gemiyi güncelle (hareket vs) - her karede çalışır
    void update();

    // Gemiyi ekrana çiz
    void draw(sf::RenderWindow& window);

    // Geminin ekrandaki konumunu döndür
    sf::Vector2f getPosition();

private:
    sf::ConvexShape shape; // Geminin şekli (üçgen)
    float speed;           // Geminin hareket hızı
};