// Bariyer davranışlarını içeren dosya
#include "Barrier.h"

// Yapıcı fonksiyon - bariyer oluşturulurken çalışır
Barrier::Barrier(sf::Vector2f startPos)
{
    health = 3;   // 3 can ile başla
    alive = true; // Başlangıçta ayakta

    // Bariyer şeklini ayarla (60x20 piksel)
    shape.setSize({60, 20});

    // Bariyer rengini yeşil yap
    shape.setFillColor(sf::Color::Green);

    // Bariyeri konuma yerleştir
    shape.setPosition(startPos);
}

// Bariyeri ekrana çiz (ayaktaysa)
void Barrier::draw(sf::RenderWindow& window)
{
    if (alive)
        window.draw(shape);
}

// Bariyerin sınır kutusunu döndür
sf::FloatRect Barrier::getBounds()
{
    return shape.getGlobalBounds();
}

// Bariyere hasar ver
void Barrier::takeDamage()
{
    health--; // Bir can azalt

    // Cana göre renk değiştir
    if (health == 2)
        shape.setFillColor(sf::Color(255, 165, 0)); // Turuncu
    else if (health == 1)
        shape.setFillColor(sf::Color::Red);          // Kırmızı
    else
        alive = false; // Can bitti, bariyer yıkıldı
}

// Bariyer ayakta mı?
bool Barrier::isAlive()
{
    return alive;
}