#include "Game.h"
#include "PowerUp.h"
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <sstream>
#define M_PI acos(-1.0)

Game::Game() : window(sf::VideoMode(1900, 1100), "Archer Game", sf::Style::Close | sf::Style::Titlebar), isPaused(false) {
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::Vector2u windowSize(desktopMode.width, desktopMode.height);
    window.setSize(windowSize);
    window.setPosition(sf::Vector2i(0, 0));
    srand(time(nullptr));
    initializeEnemies();
    powerUp = PowerUp(rand() % 1900, rand() % 900, rand() % 3);
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                sf::Vector2f playerPosition = player.getSprite().getPosition();
                float deltaX = mousePosition.x - playerPosition.x;
                float deltaY = mousePosition.y - playerPosition.y;
                float angle = std::atan2(deltaY, deltaX) * 180 / M_PI;
                player.shoot(angle);
            }
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                isPaused = !isPaused;
                if (isPaused) {
                    pauseClock.restart();
                }
            }
            else if (event.key.code == sf::Keyboard::Q && isPaused) {
                window.close();
            }
        }
    }
}

void Game::update() {
    player.update(powerUp);
    if (!isPaused) {
        for (auto& enemy : enemies) {
            enemy->update();
        }

        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [this](std::unique_ptr<Enemy>& enemy) {
                    bool collidedWithPlayer = false;
                    player.checkArrowCollision(enemy, collidedWithPlayer);
                    if (enemy->getHealth() <= 0 || collidedWithPlayer) {
                        return true;
                    }
                    return false;
                }
        ), enemies.end());

        if (player.getLives() <= 0) {
            gameOver();
            return;
        }
    }
    if (enemies.empty()) {
        sf::Time elapsed = pauseClock.getElapsedTime();
        if (elapsed.asSeconds() >= 5) {
            initializeEnemies();
            generatePowerUp();
            pauseClock.restart();
        }
    }
    if (player.getSprite().getGlobalBounds().intersects(powerUp.getSprite().getGlobalBounds())) {
        int type = powerUp.getType();
        if (type == 0) {
            // Obs³uga ulepszenia dla ¿ycia
            player.increaseLives(1);
        }
        else if (type == 1) {
            // Obs³uga ulepszenia dla poziomu mocy
            player.increasePowerLevel(1);
        }
        else if (type == 2) {
            // Obs³uga ulepszenia dla prêdkoœci
            player.increaseSpeedMultiplier(0.5f);
        }

        // Usuniêcie ulepszenia z gry
        powerUp.reset();
    }
}

void Game::generatePowerUp() {
    // Losowa pozycja na mapie
    float posX = rand() % 1900;
    float posY = rand() % 900;

    // Losowy typ ulepszenia
    int type = rand() % 3; // Za³ó¿my, ¿e mamy trzy typy ulepszeñ

    // Tworzenie nowego ulepszenia
    powerUp = PowerUp(posX, posY, type);
}


void Game::render() {
    window.clear(sf::Color::Black);
    player.draw(window);
    for (auto& enemy : enemies) {
        enemy->draw(window);
    }

    powerUp.draw(window);

    if (isPaused) {
        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Error loading font\n";
            return;
        }

        sf::Text pauseText("Paused", font, 50);
        pauseText.setFillColor(sf::Color::White);
        pauseText.setPosition(window.getSize().x / 2 - pauseText.getGlobalBounds().width / 2, window.getSize().y / 2 - 100);
        window.draw(pauseText);

        sf::Text continueText("Press ESC to continue", font, 30);
        continueText.setFillColor(sf::Color::White);
        continueText.setPosition(window.getSize().x / 2 - continueText.getGlobalBounds().width / 2, window.getSize().y / 2);
        window.draw(continueText);

        sf::Text exitText("Press Q to quit", font, 30);
        exitText.setFillColor(sf::Color::White);
        exitText.setPosition(window.getSize().x / 2 - exitText.getGlobalBounds().width / 2, window.getSize().y / 2 + 50);
        window.draw(exitText);
    }

    window.display();
}

void Game::initializeEnemies() {
    for (int i = 0; i < 10; ++i) {
        enemies.emplace_back(std::make_unique<Enemy>(100, 1.0f, player));
    }
}

void Game::resetEnemies() {
    enemies.clear();
    initializeEnemies();
}

void Game::gameOver() {
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font\n";
        return;
    }

    loadScores();

    sf::RenderWindow gameOverWindow(sf::VideoMode(500, 300), "Game Over");

    sf::Text enterNameText("Enter your name:", font, 20);
    enterNameText.setFillColor(sf::Color::White);
    enterNameText.setPosition(50, 150);

    std::string playerName;

    while (gameOverWindow.isOpen()) {
        sf::Event event;
        while (gameOverWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameOverWindow.close();
                break;
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128 && event.text.unicode != '\b' && event.text.unicode != 13) {
                    playerName += static_cast<char>(event.text.unicode);
                }
                else if (event.text.unicode == '\b' && playerName.size() > 0) {
                    playerName.pop_back();
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                if (!playerName.empty()) {
                    PlayerData playerScore;
                    playerScore.name = playerName;
                    playerScore.score = player.getScore();
                    scores.push_back(playerScore);
                    std::sort(scores.begin(), scores.end(), [](const PlayerData& lhs, const PlayerData& rhs) {
                        return lhs.score > rhs.score;
                        });
                    if (scores.size() > 5) {
                        scores.resize(5);
                    }

                    saveScores();
                    gameOverWindow.close();
                    player.reset();
                    resetEnemies();
                    isPaused = false;
                    break;
                }
            }
        }

        sf::Text playerNameText(playerName, font, 20);
        playerNameText.setFillColor(sf::Color::White);
        playerNameText.setPosition(50, 180);

        sf::Text scoresText("High Scores:", font, 20);
        scoresText.setFillColor(sf::Color::White);
        scoresText.setPosition(50, 20);

        gameOverWindow.clear();
        gameOverWindow.draw(enterNameText);
        gameOverWindow.draw(playerNameText);
        gameOverWindow.draw(scoresText);

        for (size_t i = 0; i < std::min(scores.size(), static_cast<size_t>(5)); ++i) {
            std::stringstream ss;
            ss << i + 1 << ". " << scores[i].name << ": " << scores[i].score;
            sf::Text scoreEntry(ss.str(), font, 18);
            scoreEntry.setFillColor(sf::Color::White);
            scoreEntry.setPosition(50, 50 + i * 20);
            gameOverWindow.draw(scoreEntry);
        }

        gameOverWindow.display();
    }
}

void Game::loadScores() {
    scores.clear();
    std::ifstream file("scores.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening scores file\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        PlayerData playerScore;
        ss >> playerScore.name >> playerScore.score;
        scores.push_back(playerScore);
    }

    file.close();
}

void Game::saveScores() {
    std::ofstream file("scores.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening scores file\n";
        return;
    }

    for (const auto& score : scores) {
        file << score.name << " " << score.score << std::endl;
    }

    file.close();
}