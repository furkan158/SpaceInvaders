// Düşman mermisi sınıfının özelliklerini tanımlayan başlık dosyası
#pragma once

#include <SFML/Graphics.hpp>

class EnemyBullet
{
public:
    // Yapıcı fonksiyon - merminin başlangıç konumunu alır
    EnemyBullet(sf::Vector2f startPos);

    // Mermiyi güncelle (aşağı hareket ettir)
    void update();

    // Mermiyi ekrana çiz
    void draw(sf::RenderWindow& window);

    // Mermi ekrandan çıktı mı?
    bool isOffScreen();

    // Mermiyi zorla ekran dışı yap (çarpışma sonrası silmek için)
    void setOffScreen();

    // Merminin sınır kutusunu döndür (çarpışma tespiti için)
    sf::FloatRect getBounds();

private:
    sf::RectangleShape shape; // Merminin şekli
    float speed;              // Merminin hızı
};