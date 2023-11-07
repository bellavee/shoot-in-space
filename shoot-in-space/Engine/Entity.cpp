#include "Entity.h"

unsigned int Entity::idCounter = 0; // Initialize static member

Entity::Entity(std::string name)
    : _id(++idCounter), _name(name) // Use pre-increment to assign and then increase the id
{
}

Entity::~Entity() {}

template <typename T>
void Entity::AddComponent()
{
    auto component = std::make_shared<T>(); // Create a shared_ptr of the component
    component->AddEntity(this->_id, component);
    _components[typeid(T)] = component; // Store the component in the map
}

template <typename T>
T* Entity::GetComponent()
{
    auto it = _components.find(typeid(T));
    if (it != _components.end()) {
        return dynamic_cast<T*>(it->second.get());
    }
    return nullptr;
}