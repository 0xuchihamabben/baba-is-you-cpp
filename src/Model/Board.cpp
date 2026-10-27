#include "Board.hpp"
#include <algorithm>
#include <iostream>

namespace Baba {

Board::Board(const int width, const int height) : m_width(width), m_height(height) {
    m_grid.resize(height, std::vector<std::vector<Entity>>(width));
}

Board::~Board() = default;

Board::Board(const Board &other)
    : m_width(other.m_width), m_height(other.m_height), m_grid(other.m_grid),
      m_activeRules(other.m_activeRules) {}

Board &Board::operator=(const Board &other) {
    if (this != &other) {
        m_width = other.m_width;
        m_height = other.m_height;
        m_grid = other.m_grid;
        m_activeRules = other.m_activeRules;
    }
    return *this;
}

void Board::AddObserver(IObserver *observer) { m_observers.push_back(observer); }

void Board::NotifyObservers() {
    for (IObserver *&m_observer : m_observers) {
        m_observer->OnBoardChanged();
    }
}

void Board::AddEntity(Position pos, EntityType type) {
    if (pos.x >= 0 && pos.x < m_width && pos.y >= 0 && pos.y < m_height) {
        m_grid[pos.y][pos.x].emplace_back(type);
    }
}

void Board::RemoveEntity(Position pos, EntityType type) {
    if (pos.x >= 0 && pos.x < m_width && pos.y >= 0 && pos.y < m_height) {
        std::vector<Entity> &entities = m_grid[pos.y][pos.x];
        std::erase_if(entities, [type](const Entity &e) { return e.GetType() == type; });
    }
}

const std::vector<Entity> &Board::GetEntitiesAt(Position pos) const {
    if (pos.x >= 0 && pos.x < m_width && pos.y >= 0 && pos.y < m_height) {
        return m_grid[pos.y][pos.x];
    }
    static std::vector<Entity> empty;
    return empty;
}

std::vector<Entity> &Board::operator[](const Position pos) {
    if (pos.x >= 0 && pos.x < m_width && pos.y >= 0 && pos.y < m_height) {
        return m_grid[pos.y][pos.x];
    }
    static std::vector<Entity> empty;
    return empty;
}

void Board::MovePlayer(Direction dir) {
    SaveState();
    ParseRules();

    std::vector<Position> youPositions;
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            Position pos{x, y};
            if (HasPropertyAt(pos, Property::YOU)) {
                youPositions.push_back(pos);
            }
        }
    }

    if (dir == Direction::Down) {
        std::sort(youPositions.begin(), youPositions.end(),
                  [](Position a, Position b) { return a.y > b.y; });
    } else if (dir == Direction::Up) {
        std::sort(youPositions.begin(), youPositions.end(),
                  [](Position a, Position b) { return a.y < b.y; });
    } else if (dir == Direction::Right) {
        std::sort(youPositions.begin(), youPositions.end(),
                  [](Position a, Position b) { return a.x > b.x; });
    } else if (dir == Direction::Left) {
        std::sort(youPositions.begin(), youPositions.end(),
                  [](Position a, Position b) { return a.x < b.x; });
    }

    bool anyMoved = false;
    std::vector<Position> processed;

    for (const Position &youPosition : youPositions) {
        if (MoveEntity(youPosition, dir)) {
            anyMoved = true;
        }
    }

    if (anyMoved) {
        ParseRules();
        ProcessSink();
        NotifyObservers();
    }
}

void Board::PullPlayer(Direction dir) {
    SaveState();
    ParseRules();

    std::vector<Position> youPositions;
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            Position pos{x, y};
            if (HasPropertyAt(pos, Property::YOU)) {
                youPositions.push_back(pos);
            }
        }
    }

    bool anyMoved = false;
    const Position delta = GetDelta(dir);
    const Position oppositeDelta = {-delta.x, -delta.y};

    for (size_t i = 0; i < youPositions.size(); ++i) {
        Position currentPos = youPositions[i];
        Position nextPos = currentPos + delta;
        Position behindPos = currentPos + oppositeDelta;

        if (!CanMoveInto(nextPos, dir)) {
            continue;
        }

        std::vector<Entity> &currentEntities = m_grid[currentPos.y][currentPos.x];
        std::vector<Entity> toMove;
        std::vector<Entity> toStay;

        for (Entity &e : currentEntities) {
            bool isYou = m_activeRules[e.GetType()].contains(Property::YOU);
            if (isYou) {
                toMove.push_back(e);
            } else {
                toStay.push_back(e);
            }
        }

        if (toMove.empty()) continue;

        m_grid[currentPos.y][currentPos.x] = toStay;
        for (const Entity &e : toMove) {
            m_grid[nextPos.y][nextPos.x].push_back(e);
        }
        anyMoved = true;

        if (behindPos.x >= 0 && behindPos.x < m_width && behindPos.y >= 0 &&
            behindPos.y < m_height) {
            std::vector<Entity> &behindEntities = m_grid[behindPos.y][behindPos.x];
            std::vector<Entity> toPull;
            std::vector<Entity> toRemain;

            for (const Entity &e : behindEntities) {
                bool isPush = m_activeRules[e.GetType()].contains(Property::PUSH);
                if (isPush) {
                    toPull.push_back(e);
                } else {
                    toRemain.push_back(e);
                }
            }

            if (!toPull.empty()) {
                m_grid[behindPos.y][behindPos.x] = toRemain;
                for (const Entity &e : toPull) {
                    m_grid[currentPos.y][currentPos.x].push_back(e);
                }
            }
        }
    }

    if (anyMoved) {
        ParseRules();
        NotifyObservers();
    }
}

bool Board::MoveEntity(const Position pos, const Direction dir) {
    const std::vector<Entity> &currentEntities = (*this)[pos];
    if (currentEntities.empty()) return true;

    const Position nextPos = pos + GetDelta(dir);

    if (!CanMoveInto(nextPos, dir)) {
        return false;
    }

    std::vector<Entity> toMove;
    std::vector<Entity> toStay;

    for (const Entity &e : currentEntities) {
        const bool isYou = m_activeRules[e.GetType()].contains(Property::YOU);
        const bool isPush = m_activeRules[e.GetType()].contains(Property::PUSH);

        if (isYou || isPush) {
            toMove.push_back(e);
        } else {
            toStay.push_back(e);
        }
    }

    if (toMove.empty()) return true;

    m_grid[pos.y][pos.x] = toStay;
    for (const Entity &entity : toMove) {
        m_grid[nextPos.y][nextPos.x].push_back(entity);
    }

    return true;
}

bool Board::CanMoveInto(const Position targetPos, const Direction dir) {
    if (targetPos.x < 0 || targetPos.x >= m_width || targetPos.y < 0 || targetPos.y >= m_height) {
        return false;
    }

    const std::vector<Entity> &entities = m_grid[targetPos.y][targetPos.x];

    for (const Entity &entity : entities) {
        if (m_activeRules[entity.GetType()].contains(Property::STOP)) {
            return false;
        }
    }

    bool hasPushable = false;
    for (const Entity &entity : entities) {
        if (m_activeRules[entity.GetType()].contains(Property::PUSH)) {
            hasPushable = true;
            break;
        }
    }

    if (hasPushable) {
        std::vector<Position> dummy;
        return MoveEntity(targetPos, dir);
    }

    return true;
}

bool Board::HasPropertyAt(Position pos, const Property prop) const {
    if (pos.x < 0 || pos.x >= m_width || pos.y < 0 || pos.y >= m_height) return false;

    const std::vector<Entity> &entities = m_grid[pos.y][pos.x];
    for (const Entity &entity : entities) {
        std::map<EntityType, std::set<Property>>::const_iterator it =
            m_activeRules.find(entity.GetType());
        if (it != m_activeRules.end() && it->second.contains(prop)) {
            return true;
        }
    }
    return false;
}

bool Board::IsWin() const {
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            const Position pos{x, y};
            bool hasYou = HasPropertyAt(pos, Property::YOU);
            bool hasWin = HasPropertyAt(pos, Property::WIN);
            if (hasYou && hasWin) return true;
        }
    }
    return false;
}

void Board::ResetRules() {
    m_activeRules.clear();
    for (int i = static_cast<int>(EntityType::TEXT_BABA);
         i <= static_cast<int>(EntityType::TEXT_SINK); ++i) {
        m_activeRules[static_cast<EntityType>(i)].insert(Property::PUSH);
    }
}

static bool IsNoun(const EntityType t) {
    return t >= EntityType::TEXT_BABA && t <= EntityType::TEXT_WATER;
}

static bool IsIS(const EntityType t) { return t == EntityType::TEXT_IS; }

static bool IsPropertyText(const EntityType t) {
    return t >= EntityType::TEXT_YOU && t <= EntityType::TEXT_SINK;
}

static EntityType GetSubject(EntityType t) {
    switch (t) {
    case EntityType::TEXT_BABA:
        return EntityType::BABA;
    case EntityType::TEXT_FLAG:
        return EntityType::FLAG;
    case EntityType::TEXT_WALL:
        return EntityType::WALL;
    case EntityType::TEXT_ROCK:
        return EntityType::ROCK;
    case EntityType::TEXT_WATER:
        return EntityType::WATER;
    default:
        return EntityType::EMPTY;
    }
}

static Property GetPropertyFromText(EntityType t) {
    switch (t) {
    case EntityType::TEXT_YOU:
        return Property::YOU;
    case EntityType::TEXT_WIN:
        return Property::WIN;
    case EntityType::TEXT_STOP:
        return Property::STOP;
    case EntityType::TEXT_PUSH:
        return Property::PUSH;
    case EntityType::TEXT_SINK:
        return Property::SINK;
    default:
        return Property::STOP;
    }
}

void Board::ParseRules() {
    ResetRules();

    // Horizontal Scan
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width - 2; ++x) {
            std::vector<Entity> &cell1 = m_grid[y][x];
            std::vector<Entity> &cell2 = m_grid[y][x + 1];
            std::vector<Entity> &cell3 = m_grid[y][x + 2];

            for (const Entity &e1 : cell1) {
                if (!IsNoun(e1.GetType())) continue;

                for (const Entity &e2 : cell2) {
                    if (!IsIS(e2.GetType())) continue;

                    for (const Entity &e3 : cell3) {
                        if (IsPropertyText(e3.GetType())) {
                            m_activeRules[GetSubject(e1.GetType())].insert(
                                GetPropertyFromText(e3.GetType()));
                        }
                    }
                }
            }
        }
    }

    // Vertical Scan
    for (int x = 0; x < m_width; ++x) {
        for (int y = 0; y < m_height - 2; ++y) {
            std::vector<Entity> &cell1 = m_grid[y][x];
            std::vector<Entity> &cell2 = m_grid[y + 1][x];
            std::vector<Entity> &cell3 = m_grid[y + 2][x];

            for (const Entity &e1 : cell1) {
                if (!IsNoun(e1.GetType())) continue;

                for (const Entity &e2 : cell2) {
                    if (!IsIS(e2.GetType())) continue;

                    for (const Entity &e3 : cell3) {
                        if (IsPropertyText(e3.GetType())) {
                            m_activeRules[GetSubject(e1.GetType())].insert(
                                GetPropertyFromText(e3.GetType()));
                        }
                    }
                }
            }
        }
    }
}

void Board::SaveState() { m_history.push_back(m_grid); }

void Board::Undo() {
    if (!m_history.empty()) {
        m_grid = m_history.back();
        m_history.pop_back();
        ParseRules();
        NotifyObservers();
    }
}

void Board::Restart() {
    if (!m_initialGrid.empty()) {
        m_grid = m_initialGrid;
        m_history.clear();
        ParseRules();
        NotifyObservers();
    }
}

void Board::SetInitialState() { m_initialGrid = m_grid; }

void Board::ClearBoard() {
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            m_grid[y][x].clear();
        }
    }
    m_history.clear();
    m_activeRules.clear();
}

void Board::ProcessSink() {
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            std::vector<Entity> &entities = m_grid[y][x];

            bool hasSink = false;
            bool hasOther = false;

            for (const Entity &entity : entities) {
                if (m_activeRules[entity.GetType()].contains(Property::SINK)) {
                    hasSink = true;
                } else if (!entity.IsText()) {
                    hasOther = true;
                }
            }

            if (hasSink && hasOther) {
                std::vector<Entity> remaining;
                for (const Entity &entity : entities) {
                    const bool isSink = m_activeRules[entity.GetType()].contains(Property::SINK);
                    const bool isText = entity.IsText();
                    if (!isSink && isText) {
                        remaining.push_back(entity);
                    }
                }
                m_grid[y][x] = remaining;
            }
        }
    }
}

bool Board::IsLose() const {
    // Si le debut de jeu
    if (m_history.empty()) {
        return false;
    }
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            if (HasPropertyAt({x, y}, Property::YOU)) {
                return false;
            }
        }
    }
    return true;
}
}
