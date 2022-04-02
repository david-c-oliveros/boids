#include "Entity.h"


Entity::Entity(olc::vf2d vPos, olc::vf2d vVel) :
    Object(vPos), m_vVel(vVel) {}


Entity::~Entity() {}


olc::vf2d Entity::GetVel()
{
    return m_vVel;
}


void Entity::SetVel(olc::vf2d vVel)
{
    m_vVel = vVel;
}
