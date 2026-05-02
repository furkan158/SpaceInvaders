// Mermi sınıfının özelliklerini tanımlayan başlık dosyası
#pragma once

#include <SFML/Graphics.hpp>

class Bullet
{
public:
    // Yapıcı fonksiyon - mermi oluşturulurken başlangıç konumunu alır
    Bullet(sf::Vector2f startPos);

    // Mermiyi güncelle (yukarı hareket ettir)
    void update();

    // Mermiyi ekrana çiz
    void draw(sf::RenderWindow& window);

    // Mermi ekrandan çıktı mı?
    bool isOffScreen();
    // Mermiyi zorla ekran dışı yap (çarpışma sonrası silmek için)
    void setOffScreen();

    // Merminin konumunu döndür (çarpışma kontrolü için)
    sf::FloatRect getBounds();

private:
    sf::RectangleShape shape; // Merminin şekli (dikdörtgen)
    float speed;              // Merminin hızı
};