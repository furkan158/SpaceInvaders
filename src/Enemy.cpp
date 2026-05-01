// Düşman davranışlarını içeren dosya
#include "Enemy.h"

// Yapıcı fonksiyon - düşman oluşturulurken çalışır
Enemy::Enemy(sf::Vector2f startPos)
{
    alive = true; // Düşman başlangıçta hayatta

    // Düşman şeklini ayarla (30x30 piksel kare)
    shape.setSize({30, 30});

    // Düşman rengini kırmızı yap
    shape.setFillColor(sf::Color::Red);

    // Düşmanı başlangıç konumuna yerleştir
    shape.setPosition(startPos);
}

// Düşmanı sağa hareket ettir
void Enemy::moveRight()
{
    shape.move({2, 0});
}

// Düşmanı sola hareket ettir
void Enemy::moveLeft()
{
    shape.move({-2, 0});
}

// Düşmanı aşağı hareket ettir
void Enemy::moveDown()
{
    shape.move({0, 20});
}

// Düşmanı ekrana çiz (hayattaysa)
void Enemy::draw(sf::RenderWindow& window)
{
    if (alive)
        window.draw(shape);
}

// Düşmanın sınır kutusunu döndür
sf::FloatRect Enemy::getBounds()
{
    return shape.getGlobalBounds();
}

// Düşman hayatta mı?
bool Enemy::isAlive()
{
    return alive;
}

// Düşmanı öldür
void Enemy::kill()
{
    alive = false;
}

// update fonksiyonu (şimdilik boş, hareket main'den kontrol edilecek)
void Enemy::update()
{
}