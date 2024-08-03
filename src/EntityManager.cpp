#include "EntityManager.hpp"

void EntityManager::update() {
    for (auto e : this->m_entitiesToAdd) {
        this->m_entities.push_back(e);
        this->m_entityMap[e->tag()].push_back(e);
    }

    this->m_entitiesToAdd.clear();

    this->removeDeadEntities(this->m_entities);
    for (auto& kv : this->m_entityMap) {
        this->removeDeadEntities(kv.second);
    }
}

void EntityManager::removeDeadEntities(EntityVec& vec) {
    std::erase_if(vec, [](const std::shared_ptr<Entity>& entity) {
        return !entity->isActive();
        }
    );
}

std::shared_ptr<Entity> EntityManager::addEntity(const EntityTag& tag) {
    auto entity = std::shared_ptr<Entity>(new Entity(this->m_totalEntities++, tag));
    this->m_entitiesToAdd.push_back(entity);
    return entity;
}

EntityVec& EntityManager::getEntities() {
    return m_entities;
}

EntityVec& EntityManager::getEntities(const EntityTag& tag) {
    return this->m_entityMap[tag];
}