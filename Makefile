# Space Invaders - YZM104 Programlama II
# Derleme komutu: mingw32-make

# Kullanilacak derleyici
CXX = g++

# SFML kutuphanesinin header dosyalarinin yolu
CXXFLAGS = -I C:/SFML-3.0.0/include

# SFML kutuphanesinin baglanti bayraklari
LDFLAGS = -L C:/SFML-3.0.0/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -mwindows

# Derlenecek kaynak dosyalar
SRCS = src/main.cpp src/Player.cpp src/Bullet.cpp src/Enemy.cpp src/EnemyBullet.cpp src/Barrier.cpp

# Olusturulacak calistirilebilir dosyanin adi
TARGET = SpaceInvaders

# Varsayilan hedef - projeyi derle
all:
	g++ $(SRCS) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

# Derleme ciktisini temizle
clean:
	del $(TARGET).exe
