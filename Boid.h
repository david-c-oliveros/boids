#include "olcPGEX_TransformedView.h"
#include "Entity.h"


class Boid : public Entity
{
    public:
        Boid(olc::vf2d vPos, olc::vf2d vVel);
        ~Boid();

        void DrawSelf(olc::TileTransformedView* tv) override;
};
