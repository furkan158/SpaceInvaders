// Düşman davranışlarını içeren dosya
#include "Enemy.h"

// Yapıcı fonksiyon - düşman oluşturulurken çalışır
Enemy::Enemy(sf::Vector2f startPos)
{
    alive = true; // Düşman başlangıçta hayatta

    // Düşman şeklini ayarla (30x30 piksel kare)
    shape.setSize({30, 30});

    // Düşman rengini satıra göre belirle (Cyberpunk teması)
    int row = (int)(startPos.y - 50) / 50;
    if (row == 0) shape.setFillColor(sf::Color(255, 0, 255));      // Pembe
    else if (row == 1) shape.setFillColor(sf::Color(204, 0, 255)); // Mor
    else if (row == 2) shape.setFillColor(sf::Color(153, 0, 255)); // Koyu mor
    else if (row == 3) shape.setFillColor(sf::Color(102, 0, 255)); // Mavi mor
    else shape.setFillColor(sf::Color(51, 0, 255));                // Mavi

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