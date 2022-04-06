#include <vector>
#include "ProcGen.h"
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include "Boid.h"
#include "Slider.h"
#include "Counter.h"


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
        bool bFollowPerchedBoids = true;
        bool bFollowCursor = false;
        bool bScatter = false;

        Counter cScatterTimer = Counter(50);
        Slider cBoidCountSlider;

    public:
        bool OnUserCreate() override
        {
            iGameTick = 0;
            tv = olc::TileTransformedView({ ScreenWidth(), ScreenHeight() }, { 32, 32});
            vBottomRight = tv.GetBottomRightTile();
            vCenterScreen = { vBottomRight.x / 2, vBottomRight.y / 2 };

            cBoidCountSlider = Slider(vCenterScreen, { 20.0f, 100.0f });
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
            HandleInput();
            UpdateBoids();
            Render();

            iGameTick++;

            return true;
        }


        void HandleInput()
        {
            if (GetKey(olc::Key::S).bReleased)
            {
                std::cout << "Scatter" << std::endl;
                bScatter = true;
                cScatterTimer.Start();
            }
            if (GetKey(olc::Key::M).bReleased) bFollowCursor = !bFollowCursor;
            if (GetMouse(2).bPressed) tv.StartPan(GetMousePos());
            if (GetMouse(2).bHeld) tv.UpdatePan(GetMousePos());
            if (GetMouse(2).bReleased) tv.EndPan(GetMousePos());
            if (GetMouseWheel() > 0) tv.ZoomAtScreenPos(2.0f, GetMousePos());
            if (GetMouseWheel() < 0) tv.ZoomAtScreenPos(0.5f, GetMousePos());
        }


        void InitializePositions()
        {
        }


        void UpdateBoids()
        {
            olc::vf2d v1, v2, v3, v4, v5, v6;
            float fScalarV1 = 1.0f;
            float fScalarV2 = 1.0f;
            float fScalarV3 = 1.0f;
            float fScalarV6 = 1.0f;

            if (bScatter)
            {
                if (cScatterTimer.Check())
                {
                    bScatter = false;
                    cScatterTimer.Reset();
                }
                cScatterTimer.Update();
                bFollowCursor = false;
            }

            fScalarV1 = bScatter ? 0.0f : 1.0f;
            fScalarV6 = bFollowCursor ? 1.0f : 0.0f;
            fScalarV6 = bScatter ? -3.0f : 0.0f;

            for (int i = 0; i < vBoids.size(); i++)
            {
                /***************************************/
                /*        Perch Cool Down Timer        */
                /***************************************/
                if (!vBoids[i].cPerchCoolDownTimer.Check())
                    vBoids[i].cPerchCoolDownTimer.Update();
                else
                {
                    vBoids[i].bPerchCoolDown = false;
                    vBoids[i].cPerchCoolDownTimer.Reset();
                }

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
                        vBoids[i].bPerchCoolDown = true;
                        vBoids[i].cPerchCoolDownTimer.Start();
                        vBoids[i].cPerchTimer.Reset();
                    }
                }

                if (!bBounds)
                    WrapMap(i);

                olc::vf2d vCursorCoords = tv.ScreenToWorld(GetMousePos());
                v1 = fScalarV1 * Rule1(i);
                v2 = fScalarV2 * Rule2(i);
                v3 = fScalarV3 * Rule3(i);
                v4 = BoundPos(i);
                v5 = StrongWind();
                v6 = fScalarV6 * TendToPlace(i, CenterOfMass(false));

                vBoids[i].SetVel(vBoids[i].GetVel() + v1 + v2 + v3 + v4 + v6);
                LimitVel(i);
                vBoids[i].SetPos(vBoids[i].GetPos() + 0.1f * vBoids[i].GetVel());
            }
        }


        /********************************************/
        /*        Fly towards center of mass        */
        /********************************************/
        olc::vf2d Rule1(int iCurBoidIndex)
        {
            olc::vf2d vCenterMass = CenterOfMass(true, iCurBoidIndex);
            return (vCenterMass - vBoids[iCurBoidIndex].GetPos()) / 40;
        }


        olc::vf2d Rule2(int iCurBoidIndex)
        {
            olc::vf2d vDeltaPos = { 0.0f, 0.0f };

            for (int i = 0; i < vBoids.size(); i++)
            {
                if (i != iCurBoidIndex && (!vBoids[i].bPerching) || bFollowPerchedBoids)
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
                if (i != iCurBoidIndex && (!vBoids[i].bPerching) || bFollowPerchedBoids)
                    vPerceivedVel += vBoids[i].GetVel();
            }

            vPerceivedVel /= (vBoids.size() - 1);

            return (vPerceivedVel - vBoids[iCurBoidIndex].GetVel()) / 16;
        }


        olc::vf2d CenterOfMass(bool bExclude, int iExcludeIndex = NULL)
        {
            olc::vf2d vCenterMass = { 0.0f, 0.0f };

            for (int i = 0; i < vBoids.size(); i++)
            {
                if (bExclude)
                {
                    if (i != iExcludeIndex && (!vBoids[i].bPerching) || bFollowPerchedBoids)
                        vCenterMass += vBoids[i].GetPos();
                }
                else
                {
                    vCenterMass += vBoids[i].GetPos();
                }
            }

            return vCenterMass /= (vBoids.size() - 1);
        }


        olc::vf2d StrongWind()
        {
            return { -0.1f, 0.0f };
        }


        olc::vf2d TendToPlace(int iCurBoidIndex, olc::vf2d vPlace = { 1.0f, 1.0f })
        {
            return (vPlace - vBoids[iCurBoidIndex].GetPos()) / 40;
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

            if (!vBoids[iCurBoidIndex].bPerchCoolDown)
            {
                if (vCurPos.y > fGroundLevel)
                {
                    vBoids[iCurBoidIndex].SetPosY(fGroundLevel - 0.3f);
                    vBoids[iCurBoidIndex].bPerching = true;
                    vBoids[iCurBoidIndex].cPerchTimer.Start();
                }
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


        void Render()
        {
            RenderUI();
            RenderBoids();
        }


        void RenderUI()
        {
            if (bScatter)
                DrawStringDecal({ (ScreenWidth() / 2) - 64.0f, 40.0f }, "Scatter");

            if (bFollowCursor)
                DrawStringDecal({ (ScreenWidth() / 2) - 64.0f, 20.0f }, "Following cursor");
            cBoidCountSlider.DrawSelf(this);
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
