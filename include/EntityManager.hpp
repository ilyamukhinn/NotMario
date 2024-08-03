#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Entity.hpp"
#include "Enums.hpp"

#include <vector>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;

class EntityManager
{
    EntityVec                           m_entities;
    EntityVec                           m_entitiesToAdd;
    std::map<EntityTag, EntityVec>      m_entityMap;
    size_t                              m_totalEntities{ 0 };

    void removeDeadEntities(EntityVec& vec);

public:

    EntityManager() = default;

    void update();

    std::shared_ptr<Entity> addEntity(const EntityTag& tag);

    EntityVec& getEntities();
    EntityVec& getEntities(const EntityTag& tag);
    std::map<EntityTag, EntityVec>& getEntityMap();
};

#endif // !ENTITY_MANAGER_H
