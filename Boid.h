#include "olcPGEX_TransformedView.h"
#include "Entity.h"


class Boid : public Entity
{
    public:
        Boid(olc::vf2d vPos, olc::vf2d vVel);
        ~Boid();

        void SetPosX(float fPosX);
        void SetPosY(float fPosX);
        void ZeroVel();

        void DrawSelf(olc::PixelGameEngine* pge, olc::TileTransformedView* tv) override;
};
