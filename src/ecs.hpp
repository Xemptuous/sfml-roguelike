#pragma once
#include "component.hpp"
#include "entity_manager.hpp"

#include <unordered_set>

struct ECS {
    std::unique_ptr<EntityManager> entity_manager;
    std::unique_ptr<ComponentManager> component_manager;

    ECS() {
        this->entity_manager    = std::make_unique<EntityManager>(EntityManager{});
        this->component_manager = std::make_unique<ComponentManager>(ComponentManager{});
    }

    // Entity create_entity() { return entity_manager.create_entity(); };

    void destroy_entity(Entity entity) {
        component_manager->remove_all_components(entity);
        entity_manager->destroy_entity(entity);
    };

    template <typename T> void add_component(Entity entity, const T& component) {
        component_manager->add_component<T>(entity, component);
    };

    template <typename T> T* get_component(Entity entity) {
        return component_manager->get_component<T>(entity);
    };

    template <typename T> bool has_component(Entity entity) {
        return component_manager->has_component<T>(entity);
    }

    std::unordered_set<Entity>& entities() { return entity_manager->active_entities; }
    std::unique_ptr<ComponentManager>& components() { return component_manager; }

    struct EntityBuilder {
        ECS& ecs;
        Entity entity;

        explicit EntityBuilder(ECS& ecs) : ecs(ecs), entity(ecs.entity_manager->create_entity()) {}

        template <typename T> EntityBuilder& with(const T& component) {
            ecs.add_component(entity, component);
            return *this;
        }

        Entity build() { return entity; }
    };

    EntityBuilder create_entity() { return EntityBuilder(*this); };
};
