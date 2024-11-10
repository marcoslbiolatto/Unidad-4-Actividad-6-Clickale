#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace sf;

// Clase para el enemigo
class Enemy {
public:
    Sprite sprite;
    bool isAlive;
    float speedX;

    Enemy(Texture& texture, Vector2u windowSize) {
        sprite.setTexture(texture);
        sprite.setScale(0.3f, 0.3f);
        respawn(windowSize);
        speedX = 2.0f;
        isAlive = true;
    }

    void respawn(Vector2u windowSize) {
        float x = static_cast<float>(rand() % (windowSize.x - static_cast<int>(sprite.getGlobalBounds().width)));
        float y = static_cast<float>(rand() % (windowSize.y - static_cast<int>(sprite.getGlobalBounds().height)));
        sprite.setPosition(x, y);
        isAlive = true;
    }

    void move(Vector2u windowSize) {
        sprite.move(speedX, 0);
        if (sprite.getPosition().x <= 0 || sprite.getPosition().x + sprite.getGlobalBounds().width >= windowSize.x) {
            speedX = -speedX;
        }
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    RenderWindow window(VideoMode(1000, 600), "Juego Simple con Puntaje");

    Texture textureCrosshair, textureEnemy;
    if (!textureCrosshair.loadFromFile("mira/crosshair.png") || !textureEnemy.loadFromFile("enemigo/et.png")) {
        std::cerr << "Error al cargar las texturas" << std::endl;
        return -1;
    }

    Sprite crosshair(textureCrosshair);
    crosshair.setOrigin(crosshair.getGlobalBounds().width / 2, crosshair.getGlobalBounds().height / 2);
    crosshair.setScale(0.1f, 0.1f);

    Enemy enemy(textureEnemy, window.getSize());

    int score = 0;
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error al cargar la fuente" << std::endl;
        return -1;
    }
    Text scoreText("Puntaje: 10", font, 24);
    scoreText.setFillColor(Color::Black);
    scoreText.setPosition(10, 10);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseMoved) {
                crosshair.setPosition(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
            }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                if (enemy.isAlive && enemy.sprite.getGlobalBounds().contains(mousePos)) {
                    enemy.isAlive = false;
                    score++;
                    scoreText.setString("Puntaje: " + std::to_string(score));
                    enemy.respawn(window.getSize());
                }
            }
        }

        if (enemy.isAlive) {
            enemy.move(window.getSize());
        }

        window.clear(Color::White);
        if (enemy.isAlive) {
            window.draw(enemy.sprite);
        }
        window.draw(crosshair);
        window.draw(scoreText);
        window.display();
    }

    return 0;