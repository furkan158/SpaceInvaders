// Düşman sınıfının özelliklerini tanımlayan başlık dosyası
#pragma once

#include <SFML/Graphics.hpp>

class Enemy
{
public:
    // Yapıcı fonksiyon - düşmanın başlangıç konumunu alır
    Enemy(sf::Vector2f startPos);

    // Düşmanı güncelle (hareket vs)
    void update();

    // Düşmanı ekrana çiz
    void draw(sf::RenderWindow& window);

    // Düşmanı sağa hareket ettir
    void moveRight();

    // Düşmanı sola hareket ettir
    void moveLeft();

    // Düşmanı aşağı hareket ettir
    void moveDown();

    // Düşmanın sınır kutusunu döndür (çarpışma tespiti için)
    sf::FloatRect getBounds();

    // Düşman hala hayatta mı?
    bool isAlive();

    // Düşmanı öldür
    void kill();

private:
    sf::RectangleShape shape; // Düşmanın şekli
    bool alive;               // Hayatta mı?
};