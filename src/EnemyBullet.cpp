// Düşman mermisi davranışlarını içeren dosya
#include "EnemyBullet.h"

// Yapıcı fonksiyon - düşman mermisi oluşturulurken çalışır
EnemyBullet::EnemyBullet(sf::Vector2f startPos)
{
    speed = 5.0f; // Merminin aşağı hareket hızı

    // Mermi şeklini ayarla (4x15 piksel dikdörtgen)
    shape.setSize({4, 15});

    // Mermi rengini turuncu yap (oyuncu mermisindn ayırt etmek için)
    shape.setFillColor(sf::Color(255, 165, 0));

    // Mermiyi düşmanın konumuna yerleştir
    shape.setPosition(startPos);
}

// Her karede çalışır - mermiyi aşağı hareket ettirir
void EnemyBullet::update()
{
    shape.move({0, speed}); // Aşağı doğru hareket (y ekseninde artı yön)
}

// Mermi ekrandan çıktı mı kontrol et
bool EnemyBullet::isOffScreen()
{
    return shape.getPosition().y > 600; // Y konumu 600'ün üstüne çıktıysa ekran dışı
}

// Mermiyi pencereye çiz
void EnemyBullet::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

// Merminin sınır kutusunu döndür
sf::FloatRect EnemyBullet::getBounds()
{
    return shape.getGlobalBounds();
}