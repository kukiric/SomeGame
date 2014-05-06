#include "Entity.h"

void Entity::removeChild(Entity* child)
{
    for (auto ent : children)
    {
        if (ent == child)
        {
            children.remove(ent);
        }
    }
}

Entity::Entity()
    : parent(nullptr)
{}

Entity::Entity::~Entity()
{
    if (parent)
    {
        parent->removeChild(this);
    }
}

const Entity* Entity::getParent() const
{
    return parent;
}

const Entity::EntList& Entity::getChildren() const
{
    return children;
}

void Entity::update(float delta)
{
    for (auto ent : children)
    {
        ent->update(delta);
    }
    this->think(delta);
}

void Entity::addChild(Entity* child)
{
    child->parent = this;
    children.push_front(child);
}

void Entity::setThinkRate(Tick ticks)
{
    thinkRate = ticks;
}

Tick Entity::getThinkRate() const
{
    return thinkRate;
}

Tick Entity::getNextThink() const
{
    return nextThink;
}