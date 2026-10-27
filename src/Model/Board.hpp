#pragma once
#include "Direction.hpp"
#include "Entity.hpp"
#include "IObserver.hpp"
#include "Position.hpp"
#include <map>
#include <vector>

namespace Baba {

class Board {
  public:
    Board(int width, int height);
    ~Board();
    Board(const Board &other);
    Board &operator=(const Board &other);

    void AddObserver(IObserver *observer);
    void NotifyObservers();

    void AddEntity(Position pos, EntityType type);
    void RemoveEntity(Position pos, EntityType type);
    const std::vector<Entity> &GetEntitiesAt(Position pos) const;

    void MovePlayer(Direction dir);
    void PullPlayer(Direction dir);
    void ParseRules();

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

    std::vector<Entity> &operator[](Position pos);

    bool IsWin() const;
    bool IsLose() const;

    void ClearBoard();

    void SaveState();
    void Undo();
    void Restart();
    void SetInitialState();

  private:
    bool MoveEntity(Position pos, Direction dir);
    bool CanMoveInto(Position targetPos, Direction dir);
    bool HasPropertyAt(Position pos, Property prop) const;

    void ResetRules();
    void ProcessSink();

    int m_width;
    int m_height;
    std::vector<std::vector<std::vector<Entity>>> m_grid;
    std::vector<IObserver *> m_observers;

    std::map<EntityType, std::set<Property>> m_activeRules;

    std::vector<std::vector<std::vector<std::vector<Entity>>>> m_history;
    std::vector<std::vector<std::vector<Entity>>> m_initialGrid;
};

}
