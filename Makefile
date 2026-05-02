CXX = g++
CXXFLAGS = -I C:/SFML-3.0.0/include
LDFLAGS = -L C:/SFML-3.0.0/lib -lsfml-graphics -lsfml-window -lsfml-system

all:
	g++ src/main.cpp src/Player.cpp src/Bullet.cpp src/Enemy.cpp src/EnemyBullet.cpp -o SpaceInvaders $(CXXFLAGS) $(LDFLAGS)
