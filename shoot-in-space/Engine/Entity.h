#pragma once

#include <memory>
#include <string>
#include <vector>
#include <typeindex>
#include <unordered_map>
#include "IComponent.h"

static unsigned int id = 0;

class Entity
{
public:

	Entity(std::string name);

	~Entity();

	template <typename T>
	void AddComponent()
	{
		auto component = std::make_shared<T>(); // Create a shared_ptr of the component
		component->AddEntity(this->_id, component);
		_components[typeid(T)] = component; // Store the component in the map
	}

	template <typename T>
	T* GetComponent()
	{
		auto it = _components.find(typeid(T));
		if (it != _components.end()) {
			return dynamic_cast<T*>(it->second.get());
		}
		return nullptr;
	}

private:
	static unsigned int idCounter; // To give each entity a unique ID
	unsigned int _id;
	std::string _name;
	std::unordered_map<std::type_index, std::shared_ptr<IComponent>> _components;


};