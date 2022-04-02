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
        float fScalar = 0.7;
        float fMinDis = 1.0;
        bool bBounds = true;

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
                vBoids.push_back(Boid({ 0.0f, 0.0f }, { rX, rY }));
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
                /***********************************/
                /*        Check if perching        */
                /***********************************/
                if (vBoids[i].bPerching)
                {
                    if (!vBoids[i].cPerchTimer.Check())
                    {
                        vBoids[i].cPerchTimer.Update();
                        continue;
                    }
                    else
                    {
                        vBoids[i].bPerching = false;
                        vBoids[i].cPerchTimer.Reset();
                    }
                }

                if (!bBounds)
                    WrapMap(i);

                v1 = Rule1(i);
                v2 = Rule2(i);
                v3 = Rule3(i);
                v4 = BoundPos(i);
                v5 = StrongWind();
                v6 = TendToPlace(i, vCenterScreen);

                vBoids[i].SetVel(vBoids[i].GetVel() + v1 + v2 + v3 + v4);
                LimitVel(i);
                vBoids[i].SetPos(vBoids[i].GetPos() + 0.1f * vBoids[i].GetVel());
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

            return (vCenterMass - vBoids[iCurBoidIndex].GetPos()) / 40;
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

            return (vPerceivedVel - vBoids[iCurBoidIndex].GetVel()) / 16;
        }


        olc::vf2d StrongWind()
        {
            return { -0.1f, 0.0f };
        }


        olc::vf2d TendToPlace(int iCurBoidIndex, olc::vf2d vPlace = { 1.0f, 1.0f })
        {
            return (vPlace - vBoids[iCurBoidIndex].GetPos()) / 80;
        }


        olc::vf2d BoundPos(int iCurBoidIndex)
        {
            float fGroundLevel = vBottomRight.y;
            float fXmin = 0.0f;
            float fXmax = vBottomRight.x;
            float fYmin = 0.0f;
            float fYmax = vBottomRight.y;
            olc::vf2d vCurPos = vBoids[iCurBoidIndex].GetPos();
            olc::vf2d vVec = { 0.0f, 0.0f };
            float fScalar = 0.4f;

            if (vCurPos.y > fGroundLevel)
            {
                vBoids[iCurBoidIndex].SetPosY(fGroundLevel - 0.3f);
                vBoids[iCurBoidIndex].bPerching = true;
                vBoids[iCurBoidIndex].cPerchTimer.Start();
            }

            if (vCurPos.x < fXmin)
                vVec.x = fScalar;
            else if (vCurPos.x > fXmax)
                vVec.x = -fScalar;

            if (vCurPos.y < fYmin)
                vVec.y = fScalar;
            else if (vCurPos.y > fYmax)
                vVec.y = -fScalar;

            return vVec;
        }


        void LimitVel(int iCurBoidIndex)
        {
            float fVLim = 10.0f;
            olc::vf2d vVec;
            olc::vf2d vCurVel = vBoids[iCurBoidIndex].GetVel();

            if (abs(vCurVel.mag2()) > fVLim)
                vBoids[iCurBoidIndex].SetVel((vCurVel / abs(vCurVel.mag2())) * fVLim);
        }


        void WrapMap(int iCurBoidIndex)
        {
            olc::vf2d vCurPos = vBoids[iCurBoidIndex].GetPos();

            if (vCurPos.x < -0.2f)
                vBoids[iCurBoidIndex].SetPosX(vBottomRight.x + 0.2f);
            else if (vCurPos.x > vBottomRight.x + 0.2f)
                vBoids[iCurBoidIndex].SetPosX(-0.2f);

            if (vCurPos.y < -0.2f)
                vBoids[iCurBoidIndex].SetPosY(vBottomRight.x + 0.2f);
            else if (vCurPos.y > vBottomRight.y + 0.2f)
                vBoids[iCurBoidIndex].SetPosY(-0.2f);
        }


        void RenderBoids()
        {
            Clear(olc::VERY_DARK_BLUE);
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
