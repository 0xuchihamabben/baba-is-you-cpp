#include "Controller/GameController.hpp"
#include "Model/Board.hpp"
#include "View/AssetManager.hpp"
#include "View/GameView.hpp"
#include <SFML/Graphics.hpp>

using namespace Baba;

int g_currentLevel = 0;
const int g_totalLevels = 4;

void SetupLevel1(Board &board) {
    board.ClearBoard();

    for (int x = 0; x < 15; ++x) {
        board.AddEntity({x, 0}, EntityType::WALL);
        board.AddEntity({x, 9}, EntityType::WALL);
    }
    for (int y = 0; y < 10; ++y) {
        board.AddEntity({0, y}, EntityType::WALL);
        board.AddEntity({14, y}, EntityType::WALL);
    }

    board.AddEntity({1, 1}, EntityType::TEXT_BABA);
    board.AddEntity({2, 1}, EntityType::TEXT_IS);
    board.AddEntity({3, 1}, EntityType::TEXT_YOU);

    board.AddEntity({4, 3}, EntityType::ROCK);
    board.AddEntity({4, 4}, EntityType::ROCK);
    board.AddEntity({4, 5}, EntityType::ROCK);

    board.AddEntity({2, 4}, EntityType::BABA);

    board.AddEntity({12, 6}, EntityType::FLAG);

    board.AddEntity({1, 7}, EntityType::WALL);
    board.AddEntity({2, 7}, EntityType::WALL);
    board.AddEntity({3, 7}, EntityType::WALL);
    board.AddEntity({4, 7}, EntityType::WALL);
    board.AddEntity({1, 8}, EntityType::TEXT_ROCK);
    board.AddEntity({2, 8}, EntityType::TEXT_IS);
    board.AddEntity({3, 8}, EntityType::TEXT_PUSH);
    board.AddEntity({4, 8}, EntityType::WALL);

    board.AddEntity({6, 8}, EntityType::TEXT_FLAG);
    board.AddEntity({7, 8}, EntityType::TEXT_IS);
    board.AddEntity({8, 8}, EntityType::TEXT_WIN);

    board.AddEntity({10, 2}, EntityType::TEXT_WALL);
    board.AddEntity({10, 3}, EntityType::TEXT_IS);
    board.AddEntity({10, 4}, EntityType::TEXT_STOP);
}

void SetupLevel2(Board &board) {
    board.ClearBoard();

    for (int x = 0; x < 15; ++x) {
        board.AddEntity({x, 0}, EntityType::WALL);
        board.AddEntity({x, 9}, EntityType::WALL);
    }
    for (int y = 0; y < 10; ++y) {
        board.AddEntity({0, y}, EntityType::WALL);
        board.AddEntity({14, y}, EntityType::WALL);
    }

    board.AddEntity({1, 3}, EntityType::TEXT_BABA);
    board.AddEntity({1, 4}, EntityType::TEXT_IS);
    board.AddEntity({1, 5}, EntityType::TEXT_YOU);

    board.AddEntity({5, 7}, EntityType::TEXT_WALL);
    board.AddEntity({5, 8}, EntityType::TEXT_IS);
    board.AddEntity({6, 8}, EntityType::TEXT_STOP);

    board.AddEntity({10, 3}, EntityType::TEXT_FLAG);
    board.AddEntity({10, 4}, EntityType::TEXT_IS);

    board.AddEntity({8, 4}, EntityType::TEXT_WIN);

    board.AddEntity({4, 4}, EntityType::BABA);

    for (int x = 8; x < 13; ++x) {
        board.AddEntity({x, 1}, EntityType::WALL);
        board.AddEntity({x, 6}, EntityType::WALL);
    }
    for (int y = 1; y < 6; ++y) {
        board.AddEntity({8, y}, EntityType::WALL);
        board.AddEntity({12, y}, EntityType::WALL);
    }
    board.AddEntity({10, 2}, EntityType::FLAG);
}

void SetupLevel3(Board &board) {
    board.ClearBoard();

    for (int x = 0; x < 15; ++x) {
        board.AddEntity({x, 0}, EntityType::WALL);
        board.AddEntity({x, 9}, EntityType::WALL);
    }
    for (int y = 0; y < 10; ++y) {
        board.AddEntity({0, y}, EntityType::WALL);
        board.AddEntity({14, y}, EntityType::WALL);
    }

    board.AddEntity({1, 1}, EntityType::TEXT_BABA);
    board.AddEntity({1, 2}, EntityType::TEXT_IS);
    board.AddEntity({1, 3}, EntityType::TEXT_YOU);

    board.AddEntity({1, 5}, EntityType::TEXT_WALL);
    board.AddEntity({1, 6}, EntityType::TEXT_IS);
    board.AddEntity({1, 7}, EntityType::TEXT_STOP);

    board.AddEntity({3, 1}, EntityType::TEXT_WATER);
    board.AddEntity({3, 2}, EntityType::TEXT_IS);
    board.AddEntity({3, 3}, EntityType::TEXT_SINK);

    board.AddEntity({10, 7}, EntityType::TEXT_ROCK);
    board.AddEntity({11, 7}, EntityType::TEXT_IS);
    board.AddEntity({12, 7}, EntityType::TEXT_PUSH);

    board.AddEntity({10, 8}, EntityType::TEXT_FLAG);
    board.AddEntity({11, 8}, EntityType::TEXT_IS);
    board.AddEntity({12, 8}, EntityType::TEXT_WIN);

    board.AddEntity({5, 3}, EntityType::BABA);

    board.AddEntity({7, 5}, EntityType::ROCK);
    board.AddEntity({8, 5}, EntityType::ROCK);

    for (int x = 6; x < 10; ++x) {
        board.AddEntity({x, 4}, EntityType::WATER);
    }

    board.AddEntity({12, 3}, EntityType::FLAG);

    board.AddEntity({5, 4}, EntityType::WALL);
    board.AddEntity({10, 4}, EntityType::WALL);
}

void SetupLevel4(Board &board) {
    board.ClearBoard();

    for (int x = 0; x < 15; ++x) {
        board.AddEntity({x, 0}, EntityType::WALL);
        board.AddEntity({x, 9}, EntityType::WALL);
    }
    for (int y = 0; y < 10; ++y) {
        board.AddEntity({0, y}, EntityType::WALL);
        board.AddEntity({14, y}, EntityType::WALL);
    }

    board.AddEntity({1, 1}, EntityType::TEXT_WALL);
    board.AddEntity({2, 1}, EntityType::TEXT_IS);
    board.AddEntity({3, 1}, EntityType::TEXT_YOU);

    board.AddEntity({8, 7}, EntityType::TEXT_FLAG);
    board.AddEntity({9, 7}, EntityType::TEXT_IS);
    board.AddEntity({10, 7}, EntityType::TEXT_STOP);

    board.AddEntity({12, 4}, EntityType::TEXT_WIN);

    board.AddEntity({4, 4}, EntityType::TEXT_BABA);

    board.AddEntity({7, 3}, EntityType::BABA);

    board.AddEntity({10, 3}, EntityType::FLAG);

    board.AddEntity({5, 5}, EntityType::WALL);
    board.AddEntity({6, 5}, EntityType::WALL);
}

void LoadLevel(Board &board, int levelNum) {
    switch (levelNum) {
    case 0:
        SetupLevel1(board);
        break;
    case 1:
        SetupLevel2(board);
        break;
    case 2:
        SetupLevel3(board);
        break;
    case 3:
        SetupLevel4(board);
        break;
    default:
        SetupLevel1(board);
        break;
    }
    board.SetInitialState();
}

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Baba Is You Clone - Level 1");
    window.setFramerateLimit(60);

    Board board(15, 10);
    LoadLevel(board, g_currentLevel);

    GameView view(window, board);
    board.AddObserver(&view);

    GameController controller(board);

    bool requestClose = false;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                requestClose = true;
                continue;
            }

            const sf::Event::KeyPressed *keyPressed = event->getIf<sf::Event::KeyPressed>();
            if (keyPressed != nullptr) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    requestClose = true;
                    continue;
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::N) {
                    g_currentLevel = (g_currentLevel + 1) % g_totalLevels;
                    LoadLevel(board, g_currentLevel);
                    window.setTitle("Baba Is You Clone - Level " +
                                    std::to_string(g_currentLevel + 1));
                }
            }

            const sf::Event::MouseButtonPressed *mousePressed =
                event->getIf<sf::Event::MouseButtonPressed>();
            if (mousePressed != nullptr && mousePressed->button == sf::Mouse::Button::Left) {
                int clicked = view.HandleClick(mousePressed->position);
                if (clicked == 1) {
                    board.Undo();
                } else if (clicked == 2) {
                    board.Restart();
                }
            }

            controller.HandleInput(*event);
        }

        if (requestClose) {
            window.close();
            break;
        }

        window.clear(sf::Color::Black);
        view.Render();
        view.RenderUI();
        window.display();
    }

    AssetManager::Get().Clear();

    return 0;
}
