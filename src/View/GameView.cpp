#include "GameView.hpp"
#include <optional>

namespace Baba {

GameView::GameView(sf::RenderWindow &window, const Board &board)
    : m_window(window), m_board(board) {
    AssetManager::Get().LoadGameAssets();

    try {
        m_winSound.emplace(AssetManager::Get().GetSoundBuffer("win_sound"));
        m_winSound->setVolume(50.0f);
    } catch (...) {
    }

    try {
        m_loseSound.emplace(AssetManager::Get().GetSoundBuffer("lose_sound"));
        m_loseSound->setVolume(50.0f);
    } catch (...) {
    }

    float btnWidth = 100.0f;
    float btnHeight = 40.0f;
    const float padding = 10.0f;
    const float windowWidth = 800.0f;
    const float windowHeight = 600.0f;

    m_restartButton =
        sf::FloatRect({windowWidth - btnWidth - padding, windowHeight - btnHeight - padding},
                      {btnWidth, btnHeight});
    m_undoButton = sf::FloatRect(
        {windowWidth - 2 * btnWidth - 2 * padding, windowHeight - btnHeight - padding},
        {btnWidth, btnHeight});
}

struct SpriteConfig {
    std::string textureName;
    sf::Color color;

    explicit SpriteConfig(const std::string &name) : textureName(name), color(sf::Color::White) {}
};

SpriteConfig GetSpriteConfig(const EntityType type) {
    switch (type) {
    case EntityType::BABA:
        return SpriteConfig("baba_pic");
    case EntityType::FLAG:
        return SpriteConfig("flag_pic");
    case EntityType::ROCK:
        return SpriteConfig("rock_pic");
    case EntityType::WALL:
        return SpriteConfig("wall_obj");

    case EntityType::TEXT_BABA:
        return SpriteConfig("text_baba");
    case EntityType::TEXT_FLAG:
        return SpriteConfig("text_flag");
    case EntityType::TEXT_WALL:
        return SpriteConfig("text_wall");
    case EntityType::TEXT_ROCK:
        return SpriteConfig("text_rock");

    case EntityType::TEXT_IS:
        return SpriteConfig("text_is");
    case EntityType::TEXT_YOU:
        return SpriteConfig("text_you");
    case EntityType::TEXT_WIN:
        return SpriteConfig("text_win");
    case EntityType::TEXT_STOP:
        return SpriteConfig("text_stop");
    case EntityType::TEXT_PUSH:
        return SpriteConfig("text_push");
    case EntityType::TEXT_SINK:
        return SpriteConfig("text_sink");

    case EntityType::WATER:
        return SpriteConfig("water_obj");
    case EntityType::TEXT_WATER:
        return SpriteConfig("text_water");

    default:
        return SpriteConfig("baba_pic");
    }
}

void GameView::OnBoardChanged() {
    m_winPlayed = false;
    m_losePlayed = false;
}

void GameView::PlayLoseSound() {
    if (!m_losePlayed) {
        try {
            if (m_loseSound) m_loseSound->play();
        } catch (...) {
        }
        m_losePlayed = true;
        m_winPlayed = false;
    }
}
void GameView::PlayWinSound() {
    if (!m_winPlayed) {
        try {
            if (m_winSound) m_winSound->play();
        } catch (...) {
        }
        m_winPlayed = true;
        m_losePlayed = false;
    }
}
void GameView::Render() {
    for (int y = 0; y < m_board.GetHeight(); ++y) {
        for (int x = 0; x < m_board.GetWidth(); ++x) {
            const Position pos{x, y};
            const std::vector<Entity> &entities = m_board.GetEntitiesAt(pos);

            for (const Entity &entity : entities) {
                if (entity.GetType() == EntityType::EMPTY) continue;

                SpriteConfig config = GetSpriteConfig(entity.GetType());

                try {
                    const sf::Texture &texture = AssetManager::Get().GetTexture(config.textureName);
                    sf::Sprite sprite(texture);

                    sprite.setPosition({x * m_cellSize, y * m_cellSize});

                    sf::Vector2f size = sf::Vector2f(static_cast<float>(texture.getSize().x),
                                                     static_cast<float>(texture.getSize().y));

                    if (size.x > 0 && size.y > 0) {
                        sprite.setScale({m_cellSize / size.x, m_cellSize / size.y});
                    }

                    sprite.setColor(config.color);
                    m_window.draw(sprite);
                } catch (...) {
                }
            }
        }
    }

    if (m_board.IsWin()) {
        RenderLevelFinishBanner(true);
        PlayWinSound();
    } else if (m_board.IsLose()) {
        RenderLevelFinishBanner(false);
        PlayLoseSound();
    }
}

void GameView::DrawButton(sf::FloatRect bounds, const std::string &label, const sf::Color bgColor,
                          const sf::Color textColor) const {
    sf::RectangleShape bg({bounds.size.x, bounds.size.y});
    bg.setPosition({bounds.position.x, bounds.position.y});
    bg.setFillColor(bgColor);
    bg.setOutlineColor(sf::Color(255, 255, 255, 180));
    bg.setOutlineThickness(3.0f);
    m_window.draw(bg);

    sf::RectangleShape highlight({bounds.size.x - 6, 4});
    highlight.setPosition({bounds.position.x + 3, bounds.position.y + 3});
    highlight.setFillColor(sf::Color(255, 255, 255, 80));
    m_window.draw(highlight);

    float textX = bounds.position.x + bounds.size.x / 2.0f;
    float textY = bounds.position.y + bounds.size.y / 2.0f;

    sf::RectangleShape textBlock({static_cast<float>(label.length()) * 8.0f, 12.0f});
    textBlock.setPosition({textX - (label.length() * 4.0f), textY - 6.0f});
    textBlock.setFillColor(textColor);
    m_window.draw(textBlock);
}

void GameView::RenderUI() {
    const sf::Color undoBg(60, 80, 180);

    const sf::Color restartBg(200, 100, 50);

    sf::RectangleShape undoBgShape({m_undoButton.size.x, m_undoButton.size.y});
    undoBgShape.setPosition({m_undoButton.position.x, m_undoButton.position.y});
    undoBgShape.setFillColor(undoBg);
    undoBgShape.setOutlineColor(sf::Color(100, 140, 255));
    undoBgShape.setOutlineThickness(3.0f);
    m_window.draw(undoBgShape);

    sf::ConvexShape undoArrow;
    undoArrow.setPointCount(7);
    const float ux = m_undoButton.position.x + 50.0f;
    const float uy = m_undoButton.position.y + 20.0f;
    undoArrow.setPoint(0, sf::Vector2f(ux - 15, uy));
    undoArrow.setPoint(1, sf::Vector2f(ux - 5, uy - 10));
    undoArrow.setPoint(2, sf::Vector2f(ux - 5, uy - 5));
    undoArrow.setPoint(3, sf::Vector2f(ux + 15, uy - 5));
    undoArrow.setPoint(4, sf::Vector2f(ux + 15, uy + 5));
    undoArrow.setPoint(5, sf::Vector2f(ux - 5, uy + 5));
    undoArrow.setPoint(6, sf::Vector2f(ux - 5, uy + 10));
    undoArrow.setFillColor(sf::Color::White);
    m_window.draw(undoArrow);

    sf::RectangleShape restartBgShape({m_restartButton.size.x, m_restartButton.size.y});
    restartBgShape.setPosition({m_restartButton.position.x, m_restartButton.position.y});
    restartBgShape.setFillColor(restartBg);
    restartBgShape.setOutlineColor(sf::Color(255, 150, 100));
    restartBgShape.setOutlineThickness(3.0f);
    m_window.draw(restartBgShape);

    float rx = m_restartButton.position.x + 50.0f;
    float ry = m_restartButton.position.y + 20.0f;

    sf::CircleShape circle(12.0f);
    circle.setPosition({rx - 12, ry - 12});
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::White);
    circle.setOutlineThickness(3.0f);
    m_window.draw(circle);

    sf::ConvexShape arrowHead;
    arrowHead.setPointCount(3);
    arrowHead.setPoint(0, sf::Vector2f(rx + 10, ry - 8));
    arrowHead.setPoint(1, sf::Vector2f(rx + 18, ry - 4));
    arrowHead.setPoint(2, sf::Vector2f(rx + 10, ry));
    arrowHead.setFillColor(sf::Color::White);
    m_window.draw(arrowHead);
}

int GameView::HandleClick(const sf::Vector2i mousePos) const {
    const sf::Vector2f pos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    if (m_undoButton.contains(pos)) {
        return 1;
    }
    if (m_restartButton.contains(pos)) {
        return 2;
    }
    return 0;
}

sf::FloatRect GameView::GetUndoButtonBounds() const { return m_undoButton; }

sf::FloatRect GameView::GetRestartButtonBounds() const { return m_restartButton; }

void GameView::RenderLevelFinishBanner(const bool isWin) const {
    const sf::Vector2f viewSize = m_window.getView().getSize();
    sf::RectangleShape dimmer(viewSize);
    dimmer.setFillColor(sf::Color(0, 0, 0, 170));
    m_window.draw(dimmer);

    const sf::Vector2f panelSize(viewSize.x * 0.62f, viewSize.y * 0.22f);
    sf::RectangleShape panel(panelSize);
    panel.setOrigin(panelSize * 0.5f);
    panel.setPosition({viewSize.x * 0.5f, viewSize.y * 0.5f});
    panel.setFillColor(sf::Color(32, 38, 76, 230));
    panel.setOutlineThickness(4.0f);
    panel.setOutlineColor(sf::Color(255, 214, 120, 235));
    m_window.draw(panel);

    const sf::Font &font = AssetManager::Get().GetFont("main_font");
    const std::string message =
        isWin ? "You won!\nPress N for next level" : "Wasted.\nPress R for restart";
    sf::Text title(font, message, static_cast<unsigned int>(viewSize.y * 0.05f));
    title.setFillColor(sf::Color(255, 240, 200));
    title.setStyle(sf::Text::Bold);

    sf::FloatRect bounds = title.getLocalBounds();
    title.setOrigin(
        {bounds.position.x + bounds.size.x * 0.5f, bounds.position.y + bounds.size.y * 0.5f});
    title.setPosition({viewSize.x * 0.5f, viewSize.y * 0.5f});

    sf::Text shadow = title;
    shadow.setFillColor(sf::Color(20, 20, 40, 200));
    shadow.move({2.f, 2.f});

    m_window.draw(shadow);
    m_window.draw(title);
}

}
