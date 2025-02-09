#pragma once
#include <cstdint>

using Entity = std::uint64_t;

#include <cassert>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

struct Position {
    int x, y;
};

struct pair_hash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

// Base interface for component storage
struct IComponentStorage {
    virtual ~IComponentStorage()       = default;
    virtual void remove(Entity entity) = 0;
    virtual bool has(Entity)           = 0;
};

// Templated storage for each component type
template <typename T> struct ComponentStorage : IComponentStorage {
    std::unordered_map<Entity, T> components;

    void add(Entity entity, const T& component) { components[entity] = component; };
    T* get(Entity entity) {
        auto it = components.find(entity);
        return (it != components.end()) ? &it->second : nullptr;
    };
    bool has(Entity entity) override { return components.find(entity) != components.end(); }
    void remove(Entity entity) override { components.erase(entity); };
};

// ComponentManager to handle all component types
struct ComponentManager {
    std::unordered_map<std::type_index, std::unique_ptr<IComponentStorage>> storages;
    std::unordered_map<std::pair<int, int>, Entity, pair_hash> positionMap;
    std::vector<std::string> eventLogs;

    // Get or create storage for component type T
    template <typename T> ComponentStorage<T>& get_storage() {
        std::type_index index(typeid(T));
        // create if not found
        if (storages.find(index) == storages.end()) {
            storages[index] = std::make_unique<ComponentStorage<T>>();
        }
        return *static_cast<ComponentStorage<T>*>(storages[index].get());
    };

    template <typename T> void add_component(Entity entity, const T& component) {
        get_storage<T>().add(entity, component);
        if constexpr (std::is_same<T, Position>::value) {
            positionMap[{component.x, component.y}] = entity;
        }
    };

    template <typename T> T* get_component(Entity entity) { return get_storage<T>().get(entity); };
    template <typename T> void remove_component(Entity entity) {
        if constexpr (std::is_same<T, Position>::value) {
            Position* pos = get_component<Position>(entity);
            if (pos) {
                positionMap.erase({pos->x, pos->y});
            }
        }
        get_storage<T>().remove(entity);
    };

    template <typename T> bool has_component(Entity entity) {
        auto& storage = get_storage<T>();
        return storage.has(entity);
    }

    void remove_all_components(Entity entity) {
        for (const auto& pair : storages)
            pair.second->remove(entity);

        // Clean up positionMap
        for (auto it = positionMap.begin(); it != positionMap.end();) {
            if (it->second == entity) {
                it = positionMap.erase(it);
            } else {
                ++it;
            }
        }
    };

    void update_position(Entity entity, int newX, int newY) {
        Position* pos = get_component<Position>(entity);
        if (pos) {
            // Remove old position
            positionMap.erase({pos->x, pos->y});

            // Update position & add new mapping
            pos->x = newX;
            pos->y = newY;

            positionMap[{newX, newY}] = entity;
        }
    }
};
