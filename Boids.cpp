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
        olc::TileTransformedView tv;
        std::vector<Boid> vBoids;
        olc::vf2d vCenterScreen;

    public:
        bool OnUserCreate() override
        {
            tv = olc::TileTransformedView({ ScreenWidth(), ScreenHeight() }, { 32, 32});
            InitializePositions();
            vCenterScreen = { 16.0f, 16.0f };
            for (int i = 0; i < 10; i++)
            {
                float rX = procgen::normRand();
                float rY = procgen::normRand();
                std::cout << rX << std::endl;
                std::cout << rY << std::endl;
                vBoids.push_back(Boid({ rY, rX }, { rX, rY }));
            }

            return true;
        }


        bool OnUserUpdate(float fElapsedTime) override
        {
            UpdateBoids();
            RenderBoids();

            return true;
        }


        void InitializePositions()
        {
        }


        void UpdateBoids()
        {
            olc::vf2d v1, v2, v3;

            for (int i = 0; i < vBoids.size(); i++)
            {
                olc::vf2d vCurPos = vBoids[i].GetPos();

                //if (vCurPos.x > 32.0f)
                //    vBoids[i].SetPos({ 0.0f, vCurPos.y });
                //if (vCurPos.y > 32.0f)
                //    vBoids[i].SetPos({ vCurPos.x, 0.0f });

                v1 = Rule1(i);
                v2 = Rule2(i);
                v3 = Rule3(i);

                //std::cout << "v1: " << v1 << std::endl;
                //std::cout << "v2: " << v2 << std::endl;
                //std::cout << "v3: " << v3 << std::endl;

                olc::vf2d vVecToCenter = 0.3f * (vCenterScreen - vBoids[i].GetPos()).norm();

                vBoids[i].SetVel(std::clamp(vBoids[i].GetVel() + v1 + v2 + v3, { -0.1f, -0.1f }, { 0.1f, 0.1f }));
                vBoids[i].SetPos(vBoids[i].GetPos() + vBoids[i].GetVel());
                //std::cout << vBoids[i].GetPos() << std::endl;
            }
        }


        olc::vf2d Rule1(int iCurBoidIndex)
        {
            olc::vf2d vSumPos = { 0.0f, 0.0f };

            for (int i = 0; i < vBoids.size(); i++)
            {
                if (i != iCurBoidIndex)
                    vSumPos += vBoids[i].GetPos();
            }

            return vSumPos / vBoids.size();
        }


        olc::vf2d Rule2(int iCurBoidIndex)
        {
            olc::vf2d vDeltaPos = { 0.0f, 0.0f };

            for (int i = 0; i < vBoids.size(); i++)
            {
                if (i != iCurBoidIndex)
                    if (abs(vBoids[i].GetPos().mag2() - vBoids[iCurBoidIndex].GetPos().mag2()) < 100)
                        vDeltaPos -= vBoids[i].GetPos() - vBoids[iCurBoidIndex].GetPos();
            }

            return vDeltaPos;
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

            return (vPerceivedVel - vBoids[iCurBoidIndex].GetVel()) / vBoids.size();
        }


        void RenderBoids()
        {
            Clear(olc::VERY_DARK_BLUE);
            tv.FillCircle({ 10.0f, 10.0f }, 0.2f, olc::RED);
            for (int i = 0; i < vBoids.size(); i++)
            {
                vBoids[i].DrawSelf(&tv);
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
