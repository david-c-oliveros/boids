#include "olcPGEX_TransformedView.h"
#include "Entity.h"
#include "Counter.h"


class Boid : public Entity
{
    public:
        Boid(olc::vf2d vPos, olc::vf2d vVel);
        ~Boid();

        bool bPerching;
        bool bPerchCoolDown;
        Counter cPerchTimer;
        Counter cPerchCoolDownTimer;

        void SetPosX(float fPosX);
        void SetPosY(float fPosX);
        void ZeroVel();

        void DrawSelf(olc::PixelGameEngine* pge, olc::TileTransformedView* tv) override;
};
