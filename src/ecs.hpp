#pragma once
#include "component.hpp"
#include "entity_manager.hpp"

#include <unordered_set>

struct ECS {
    EntityManager entity_manager;
    ComponentManager component_manager;

    Entity create_entity() { return entity_manager.create_entity(); };

    void destroy_entity(Entity entity) {
        component_manager.remove_all_components(entity);
        entity_manager.destroy_entity(entity);
    };
    template <typename T> void add_component(Entity entity, const T& component) {
        component_manager.add_component<T>(entity, component);
    };
    template <typename T> T* get_component(Entity entity) {
        return component_manager.get_component<T>(entity);
    };

    std::unordered_set<Entity>& entities() { return entity_manager.active_entities; }
    ComponentManager& components() { return component_manager; }
};
