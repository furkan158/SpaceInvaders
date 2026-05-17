// Bariyer davranışlarını içeren dosya
#include "Barrier.h"

// Yapıcı fonksiyon - bariyer oluşturulurken çalışır
Barrier::Barrier(sf::Vector2f startPos)
{
    health = 3;   // 3 can ile başla
    alive = true; // Başlangıçta ayakta

    // Bariyer şeklini ayarla (60x12 piksel)
    shape.setSize({60, 12});

    // Sadece çerçeve - cyberpunk cyan
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color(0, 255, 255));
    shape.setOutlineThickness(2);

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
        shape.setOutlineColor(sf::Color(255, 165, 0)); // Turuncu çerçeve
    else if (health == 1)
        shape.setOutlineColor(sf::Color::Red);          // Kırmızı çerçeve
    else
        alive = false; // Can bitti, bariyer yıkıldı
}

// Bariyer ayakta mı?
bool Barrier::isAlive()
{
    return alive;
}