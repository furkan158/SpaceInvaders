// Mermi davranışlarını içeren dosya
#include "Bullet.h"

// Yapıcı fonksiyon - mermi oluşturulurken çalışır
Bullet::Bullet(sf::Vector2f startPos)
{
    speed = 8.0f; // Merminin yukarı hareket hızı

    // Mermi şeklini ayarla (4x15 piksel dikdörtgen)
    shape.setSize({4, 15});

    // Mermi rengini sarı yap
    shape.setFillColor(sf::Color::Yellow);

    // Mermiyi geminin konumuna yerleştir
    shape.setPosition(startPos);
}

// Her karede çalışır - mermiyi yukarı hareket ettirir
void Bullet::update()
{
    shape.move({0, -speed}); // Yukarı doğru hareket (y ekseninde eksi yön)
}

// Mermi ekrandan çıktı mı kontrol et
bool Bullet::isOffScreen()
{
    return shape.getPosition().y < 0; // Y konumu 0'ın altına düştüyse ekran dışı
}

// Mermiyi pencereye çiz
void Bullet::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

// Merminin sınır kutusunu döndür (çarpışma tespiti için)
sf::FloatRect Bullet::getBounds()
{
    return shape.getGlobalBounds();
}

// Mermiyi zorla ekran dışı yap
void Bullet::setOffScreen()
{
    shape.setPosition({0, -100}); // Ekran dışına taşı
}