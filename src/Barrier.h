// Bariyer sınıfının özelliklerini tanımlayan başlık dosyası
#pragma once

#include <SFML/Graphics.hpp>

class Barrier
{
public:
    // Yapıcı fonksiyon - bariyerin konumunu alır
    Barrier(sf::Vector2f startPos);

    // Bariyeri ekrana çiz
    void draw(sf::RenderWindow& window);

    // Bariyerin sınır kutusunu döndür (çarpışma tespiti için)
    sf::FloatRect getBounds();

    // Bariyere hasar ver
    void takeDamage();

    // Bariyer hala ayakta mı?
    bool isAlive();

private:
    sf::RectangleShape shape; // Bariyerin şekli
    int health;               // Bariyerin canı (3 ile başlar)
    bool alive;               // Ayakta mı?
};