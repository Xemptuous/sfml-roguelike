#pragma once
#include <cstdint>

using Entity = std::uint64_t;

#include <cassert>
#include <memory>
#include <typeindex>
#include <unordered_map>

// Base interface for component storage
struct IComponentStorage {
    virtual ~IComponentStorage()       = default;
    virtual void remove(Entity entity) = 0;
};

// Templated storage for each component type
template <typename T> struct ComponentStorage : IComponentStorage {
    std::unordered_map<Entity, T> components;

    void add(Entity entity, const T& component) { components[entity] = component; };
    T* get(Entity entity) {
        auto it = components.find(entity);
        return (it != components.end()) ? &it->second : nullptr;
    };
    void remove(Entity entity) override { components.erase(entity); };
};

// ComponentManager to handle all component types
struct ComponentManager {
    std::unordered_map<std::type_index, std::unique_ptr<IComponentStorage>> storages;

    // Get or create storage for component type T
    template <typename T> ComponentStorage<T>& get_storage() {
        std::type_index index(typeid(T));
        if (storages.find(index) == storages.end()) {
            storages[index] = std::make_unique<ComponentStorage<T>>();
        }
        return *static_cast<ComponentStorage<T>*>(storages[index].get());
    };
    template <typename T> void add_component(Entity entity, const T& component) {

        get_storage<T>().add(entity, component);
    };
    template <typename T> T* get_component(Entity entity) { return get_storage<T>().get(entity); };
    template <typename T> void remove_component(Entity entity) { get_storage<T>().remove(entity); };

    void remove_all_components(Entity entity) {
        for (const auto& pair : storages)
            pair.second->remove(entity);
    };
};
