// Oyuncu gemisinin davranışlarını içeren dosya
#include "Player.h"

// Yapıcı fonksiyon - gemi ilk oluşturulduğunda çalışır
Player::Player()
{
    speed = 5.0f; // Geminin hareket hızı (piksel/kare)

    // Üçgen şekli oluştur (3 noktalı)
    shape.setPointCount(3);

    // Üçgenin noktalarını belirle (yukarı bakan üçgen)
    shape.setPoint(0, sf::Vector2f(0, -20));  // Üst nokta
    shape.setPoint(1, sf::Vector2f(-15, 20)); // Sol alt nokta
    shape.setPoint(2, sf::Vector2f(15, 20));  // Sağ alt nokta

    // Gemi rengini yeşil yap
    shape.setFillColor(sf::Color::Green);

    // Gemiyi ekranın alt ortasına yerleştir
    shape.setPosition({400, 550});
}

// Her karede çalışır - klavye kontrolü
void Player::update()
{
    // Sol ok tuşuna basılıysa sola git
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        // Sınır kontrolü - ekrandan çıkmasın
        if (shape.getPosition().x > 20)
            shape.move({-speed, 0});
    }

    // Sağ ok tuşuna basılıysa sağa git
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        // Sınır kontrolü - ekrandan çıkmasın
        if (shape.getPosition().x < 780)
            shape.move({speed, 0});
    }
}

// Gemiyi pencereye çiz
void Player::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

// Geminin konumunu döndür (mermi için lazım olacak)
sf::Vector2f Player::getPosition()
{
    return shape.getPosition();
}

// Oyuncunun sınır kutusunu döndür
sf::FloatRect Player::getBounds()
{
    return shape.getGlobalBounds();
}

