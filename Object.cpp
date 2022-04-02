#include "Object.h"


Object::Object(olc::vf2d vPos) :
    m_vPos(vPos) {}


Object::~Object() {}


olc::vf2d Object::GetPos()
{
    return m_vPos;
}


void Object::SetPos(olc::vf2d vPos)
{
    m_vPos = vPos;
}
