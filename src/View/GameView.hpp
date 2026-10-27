#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <optional>
#include "../Model/Board.hpp"
#include "../Model/IObserver.hpp"
#include "AssetManager.hpp"

namespace Baba {

class GameView : public IObserver {
public:
    GameView(sf::RenderWindow& window, const Board& board);

    void OnBoardChanged() override;
    void PlayLoseSound();
    void PlayWinSound();
    void Render();
    void RenderUI();

    int HandleClick(sf::Vector2i mousePos) const;

    sf::FloatRect GetUndoButtonBounds() const;
    sf::FloatRect GetRestartButtonBounds() const;

private:
    sf::RenderWindow& m_window;
    const Board& m_board;
    float m_cellSize = 48.0f;

    sf::FloatRect m_undoButton;
    sf::FloatRect m_restartButton;
    std::optional<sf::Sound> m_winSound;
    std::optional<sf::Sound> m_loseSound;
    bool m_winPlayed = false;
    bool m_losePlayed = false;

    void DrawButton(sf::FloatRect bounds, const std::string& label, sf::Color bgColor, sf::Color textColor) const;
    void RenderLevelFinishBanner(bool isWin) const;
};

}
