#include "GameController.hpp"
#include <functional>

namespace Baba {

GameController::GameController(Board &board) : m_board(board) {}
void GameController::HandleInput(const sf::Event &event) const {
    const sf::Event::KeyPressed *keyPressed = event.getIf<sf::Event::KeyPressed>();

    if (keyPressed != nullptr) {
        const bool ctrl = keyPressed->control;

        const std::function<void(const Direction)> handleMove = [&](const Direction direction) {
            if (ctrl) this->m_board.PullPlayer(direction);
            else this->m_board.MovePlayer(direction);
        };

        switch (keyPressed->scancode) {
        case sf::Keyboard::Scancode::Up:
        case sf::Keyboard::Scancode::W:
            handleMove(Direction::Up);
            break;
        case sf::Keyboard::Scancode::Down:
        case sf::Keyboard::Scancode::S:
            handleMove(Direction::Down);
            break;
        case sf::Keyboard::Scancode::Left:
        case sf::Keyboard::Scancode::A:
            handleMove(Direction::Left);
            break;
        case sf::Keyboard::Scancode::Right:
        case sf::Keyboard::Scancode::D:
            handleMove(Direction::Right);
            break;
        case sf::Keyboard::Scancode::R:
            m_board.Restart();
            break;
        case sf::Keyboard::Scancode::Z:
            m_board.Undo();
            break;
        default:
            break;
        }
    }
}

}
