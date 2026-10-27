#pragma once
#include <set>

namespace Baba {

enum class EntityType {
    // Objects
    BABA,
    FLAG,
    WALL,
    ROCK,
    WATER,
    EMPTY,

    // Text
    TEXT_BABA,
    TEXT_FLAG,
    TEXT_WALL,
    TEXT_ROCK,
    TEXT_WATER,
    TEXT_IS,
    TEXT_YOU,
    TEXT_WIN,
    TEXT_STOP,
    TEXT_PUSH,
    TEXT_SINK
};

enum class Property { YOU, WIN, STOP, PUSH, SINK };

class Entity {
  public:
    explicit Entity(const EntityType type) : m_type(type) {}

    EntityType GetType() const { return m_type; }

    void AddProperty(const Property prop) { m_properties.insert(prop); }
    void ClearProperties() { m_properties.clear(); }
    bool HasProperty(const Property prop) const { return m_properties.contains(prop); }

    bool IsText() const { return m_type >= EntityType::TEXT_BABA; }

  private:
    EntityType m_type;
    std::set<Property> m_properties;
};

}
