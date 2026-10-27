#pragma once
#include "../Model/Board.hpp"
#include <SFML/Window.hpp>

namespace Baba {

class GameController {
  public:
    explicit GameController(Board &board);

    void HandleInput(const sf::Event &event) const;

  private:
    Board &m_board;
};

}
