#pragma once

#include <memory>
#include <string>
#include <vector>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include "IComponent.h"


static unsigned int id = 0;

class Entity
{
public:

	Entity(std::string name);

	~Entity();

	template <class T>
	void AddComponent();

	template <class T>
	T* GetComponent();

private:
	static unsigned int idCounter; // To give each entity a unique ID
	unsigned int _id;
	std::string _name;
	std::unordered_map<std::type_index, std::shared_ptr<IComponent>> _components;


};