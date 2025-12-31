#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include <cstdint>

using Entity = uint32_t;

class Registry 
{
private:
	Entity next_id = 1;
	std::unordered_map<std::type_index, std::unordered_map<Entity, std::shared_ptr<void>>> components;

public:
	Entity createEntity() { return next_id++; };

	// adds a component to an entity
	template<typename T>
	void addComponent(Entity e, T component) 
	{
		components[std::type_index(typeid(T))][e] = std::make_shared<T>(component);
	}

	// returns a component of an entity
	template<typename T>
	T& get(Entity e)
	{
		return *std::static_pointer_cast<T>(components[std::type_index(typeid(T))].at(e));
	}

	// returns all entities with given components
	template<typename... Components>
	std::vector<Entity> view() 
	{
		std::vector<Entity> result;
		if constexpr(sizeof...(Components) == 0) return result;

		auto& first_map = components[std::type_index(typeid(std::tuple_element_t<0, std::tuple<Components...>>))];

		for (auto& [entity, comp] : first_map)
		{
			bool has_all = ((components[std::type_index(typeid(Components))].count(entity) > 0) && ...);
			if (has_all) result.push_back(entity);
		}

		return result;
	}

	// checks if an entity has a specific component
	template<typename T>
	bool has(Entity e)
	{
    auto it = components.find(std::type_index(typeid(T)));
    if (it == components.end()) return false;
    return it->second.count(e) > 0;
	}

};