#include <vector>
#include "ProcGen.h"
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include "Boid.h"


class Boids : public olc::PixelGameEngine
{
    public:
        Boids()
        {
            sAppName = "Boids";
        }

    private:
        int iGameTick;
        olc::TileTransformedView tv;
        std::vector<Boid> vBoids;
        olc::vf2d vCenterScreen;
        olc::vi2d vBottomRight;
        float fScalar = 0.3;
        float fMinDis = 1.0;

    public:
        bool OnUserCreate() override
        {
            iGameTick = 0;
            tv = olc::TileTransformedView({ ScreenWidth(), ScreenHeight() }, { 32, 32});
            vBottomRight = tv.GetBottomRightTile();
            vCenterScreen = { vBottomRight.x / 2, vBottomRight.y / 2 };
            for (int i = 0; i < 100; i++)
            {
                float rX = procgen::normRand();
                float rY = procgen::normRand();
                vBoids.push_back(Boid(vCenterScreen, { rX, rY }));
            }

            return true;
        }


        bool OnUserUpdate(float fElapsedTime) override
        {
            UpdateBoids();
            RenderBoids();

            iGameTick++;

            return true;
        }


        void InitializePositions()
        {
        }


        void UpdateBoids()
        {
            olc::vf2d v1, v2, v3, v4, v5, v6;

            std::cout << vBoids[0].GetPos() << std::endl;
            for (int i = 0; i < vBoids.size(); i++)
            {
                olc::vf2d vCurPos = vBoids[i].GetPos();

                /**********************/
                /*        Tile        */
                /**********************/
//                if (abs(vCurPos.x) > vBottomRight.x - 1)
//                    vBoids[i].SetPosX(vBottomRight.x - 1.1);
//                if (abs(vCurPos.y) > vBottomRight.y - 1)
//                    vBoids[i].SetPosY(vBottomRight.y - 1.1);

                if (vCurPos.x < -0.2f)
                    vBoids[i].SetPosX(vBottomRight.x + 0.2f);
                else if (vCurPos.x > vBottomRight.x + 0.2f)
                    vBoids[i].SetPosX(-0.2f);

                if (vCurPos.y < -0.2f)
                    vBoids[i].SetPosY(vBottomRight.x + 0.2f);
                else if (vCurPos.y > vBottomRight.y + 0.2f)
                    vBoids[i].SetPosY(-0.2f);

                v1 = Rule1(i);
                v2 = Rule2(i);
                v3 = Rule3(i);
                v4 = StayInBounds(i);
                v5 = StrongWind();
                v6 = TendToPlace(i, vCenterScreen);

//                if (v1.mag2() > 0.0f)
//                    std::cout << "v1: " << v1.mag2() << std::endl;
//                if (v2.mag2() > 9.0f)
//                    std::cout << "v2: " << v2.mag2() << std::endl;
//                if (v3.mag2() > 9.0f)
//                    std::cout << "v3: " << v3.mag2() << std::endl;

                vBoids[i].SetVel(fScalar * (vBoids[i].GetVel() + v1 + v2 + v3 + v4 + v5));
                vBoids[i].SetPos(vBoids[i].GetPos() + vBoids[i].GetVel());
            }
        }


        /********************************************/
        /*        Fly towards center of mass        */
        /********************************************/
        olc::vf2d Rule1(int iCurBoidIndex)
        {
            olc::vf2d vCenterMass = { 0.0f, 0.0f };

            for (int i = 0; i < vBoids.size(); i++)
            {
                if (i != iCurBoidIndex)
                    vCenterMass += vBoids[i].GetPos();
            }

            vCenterMass /= (vBoids.size() - 1);

            return (vCenterMass - vBoids[iCurBoidIndex].GetPos()) / 20;
        }


        olc::vf2d Rule2(int iCurBoidIndex)
        {
            olc::vf2d vDeltaPos = { 0.0f, 0.0f };

            for (int i = 0; i < vBoids.size(); i++)
            {
                if (i != iCurBoidIndex)
                    if (abs((vBoids[i].GetPos() - vBoids[iCurBoidIndex].GetPos()).mag2()) < fMinDis)
                        vDeltaPos = vDeltaPos - (vBoids[i].GetPos() - vBoids[iCurBoidIndex].GetPos());
            }

            return 1.0f * vDeltaPos;
        }


        olc::vf2d Rule3(int iCurBoidIndex)
        {
            olc::vf2d vPerceivedVel = { 0.0f, 0.0f };

            for (int i = 0; i < vBoids.size(); i++)
            {
                if (i != iCurBoidIndex)
                    vPerceivedVel += vBoids[i].GetVel();
            }

            vPerceivedVel /= (vBoids.size() - 1);

            return (vPerceivedVel - vBoids[iCurBoidIndex].GetVel()) / 8;
        }


        olc::vf2d StayInBounds(int iCurBoidIndex)
        {
            olc::vf2d vBoundsVec = { 0.0f, 0.0f };
            olc::vf2d vCurPos = vBoids[iCurBoidIndex].GetVel();

            if (vBottomRight.x - vCurPos.x < 2.0f)
                vBoundsVec.x = 0.5f * (vBottomRight.x - vCurPos.x);
            if (vBottomRight.y - vCurPos.y < 2.0f)
                vBoundsVec.y = vBottomRight.y - vCurPos.y;

            return vCurPos;
        }


        olc::vf2d StrongWind()
        {
            return { -0.1f, 0.0f };
        }


        olc::vf2d TendToPlace(int iCurBoidIndex, olc::vf2d vPlace = { 1.0f, 1.0f })
        {
            return (vPlace - vBoids[iCurBoidIndex].GetPos()) / 100;
        }


        void RenderBoids()
        {
            Clear(olc::VERY_DARK_BLUE);
            tv.DrawRect({ 0.0f, 0.0f }, { 1.0f, 1.0f }, olc::RED);
            tv.DrawRect({ vBottomRight.x - 1.0f, vBottomRight.y - 1.0f }, { 1.0f, 1.0f }, olc::RED);
            for (int i = 0; i < vBoids.size(); i++)
            {
                vBoids[i].DrawSelf(this, &tv);
            }
        }
};



int main()
{
    Boids b;
    if (b.Construct(960, 480, 2, 2, false, true))
        b.Start();

    return 0;
}
