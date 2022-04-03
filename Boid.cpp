#include "Boid.h"


Boid::Boid(olc::vf2d vPos, olc::vf2d vVel)
    : Entity(vPos, vVel), bPerching(false), bPerchCoolDown(false),
      cPerchTimer(Counter(40)), cPerchCoolDownTimer(Counter(20))
{
}


Boid::~Boid() {}


void Boid::SetPosX(float fPosX)
{
    m_vPos.x = fPosX;
}


void Boid::SetPosY(float fPosY)
{
    m_vPos.y = fPosY;
}


void Boid::ZeroVel()
{
    m_vPos.x = 0;
    m_vPos.y = 0;
}


void Boid::DrawSelf(olc::PixelGameEngine* pge, olc::TileTransformedView* tv)
{
    tv->FillCircle(m_vPos, 0.1f);
}
