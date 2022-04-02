#include <iostream>
#include <vector>
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include "World.h"
#include "Button.h"
#include "MathUtils.h"


class Boids : public olc::PixelGameEngine
{
    public:
        Boids()
        {
            sAppName = "Boids";
        }

    private:
        olc::vf2d vCursorCoords;

        /***********************************/
        /*              Menu               */
        /***********************************/
        Button cOpenButton;

        /*******************************************/
        /*                File Input               */
        /*******************************************/
        std::vector<Button> vFileButtons;

        /***********************************/
        /*              Edit               */
        /***********************************/
        int iCursorSize = 1;
        const int iMaxCursorSize = 8;

        /****************************************/
        /*                Save As               */
        /****************************************/
        std::string sCustomFilename;
        Button cSaveCustomButton;

        olc::TileTransformedView tv;
        World cWorld = World();
        int iGameTick;
        Button cSaveButton;
        Button cSaveAsButton;
        Button cResetButton;
        Button cIncreaseSizeButton;
        Button cDecreaseSizeButton;
        std::vector<Button> vTileTypes;
        bool bOnUI = false;

    public:
        bool OnUserCreate() override
        {
            /***********************************/
            /*              Menu               */
            /***********************************/
            cOpenButton = Button(olc::vf2d((ScreenWidth() / 2) - 20.0f, (ScreenHeight() / 2) - 8.0f),
                                 olc::vf2d(80.0f, 16.0f), "Open Map");

            /****************************************/
            /*                Save As               */
            /****************************************/
            sCustomFilename = "";
            cSaveCustomButton = Button(olc::vf2d((ScreenWidth() / 2) - 20.0f, (ScreenHeight() / 2) - 8.0f),
                                       olc::vf2d(80.0f, 16.0f), "Save");

            /*******************************************/
            /*                File Input               */
            /*******************************************/
            const std::filesystem::path maps{"maps"};
            int i = 0;
            float fPosOffset = 200.0f;
            for (auto const& dir_entry : std::filesystem::directory_iterator{maps})
            {
                std::stringstream ss;
                ss << dir_entry;
                std::string str = ss.str();
                str.erase(std::remove(str.begin(), str.end(), '"'), str.end());
                str.erase(0, 5);
                vFileButtons.push_back(Button(olc::vf2d((ScreenWidth() / 2) - 80.0f, (ScreenHeight() / 2) - fPosOffset),
                                              olc::vf2d(160.0f, 16.0f), str));
                fPosOffset -= 20.0f;
                i++;
            }
            std::cout << i << std::endl;

            /***********************************/
            /*              Edit               */
            /***********************************/
            iCurAppState = EDIT;
            iCurColorIndex = WHITE;
            vColors = {
                { olc::WHITE, "White" },
                { olc::RED,   "Red"   },
                { olc::BLUE,  "Blue"  }
            };
            tv = olc::TileTransformedView({ ScreenWidth(), ScreenHeight() }, { 32, 32 });
            cWorld.GenerateWorld();
            iGameTick = 0;
            cIncreaseSizeButton = Button( olc::vf2d(ScreenWidth() - 64.0f, 128.0f), olc::vf2d(16.0, 16.0f), "+" );
            cDecreaseSizeButton = Button( olc::vf2d(ScreenWidth() - 32.0f, 128.0f), olc::vf2d(16.0, 16.0f), "-" );
            olc::vf2d p = { (ScreenWidth() / 2) - 84, ScreenHeight() - 64 };

            cSaveButton = Button(p, olc::vf2d(80.0f, 16.0f), "Save Map");
            cSaveAsButton = Button(olc::vf2d(p.x - 108.0f, p.y), olc::vf2d(100.0f, 16.0f), "Save Map As");
            p.x += 88.0f;
            cResetButton = Button(p, olc::vf2d(80.0f, 16.0f), "Reset Map");
            p = olc::vf2d(10.0f, 15.0f);
            for (int i = 0; i < 3; i++)
            {
                vTileTypes.push_back(Button(p, olc::vf2d(80.0f, 16.0f), "Type 1"));
                p.y += 20.0f;
            }

            return true;
        }


        bool OnUserUpdate(float fElapsedTime) override
        {
            if (iCurAppState == EDIT)
            {
                if (GetKey(olc::Key::TAB).bReleased) iCurAppState = MENU;
            }
            else if (iCurAppState == MENU)
            {
                if (GetKey(olc::Key::TAB).bReleased) iCurAppState = EDIT;
            }

            switch(iCurAppState)
            {
                case(EDIT):
                    HandleEditInput();
                    RenderEdit();
                    cSaveAsButton.Update();
                    cSaveButton.Update();
                    cResetButton.Update();
                    cIncreaseSizeButton.Update();
                    cDecreaseSizeButton.Update();
                    for (int i = 0; i < vTileTypes.size(); i++)
                    {
                        vTileTypes[i].Update();
                    }
                    break;

                case(MENU):
                    HandleMenuInput();
                    RenderMenu();
                    cOpenButton.Update();
                    break;

                case(FILE_SELECT):
                    HandleFileSelectInput();
                    RenderFileSelect();
                    for (int i = 0; i < vFileButtons.size(); i++)
                    {
                        vFileButtons[i].Update();
                    }
                    break;

                case(SAVE_AS):
                    HandleSaveAsInput();
                    RenderSaveAsInput();
                    break;

                default:
                    break;
            }
            iGameTick++;

            return true;
        }


        void RenderMenu()
        {
            Clear(olc::VERY_DARK_BLUE);
            cOpenButton.DrawSelf(this);
        }


        void RenderSaveAsInput()
        {
            Clear(olc::VERY_DARK_BLUE);
            olc::vf2d p = cSaveCustomButton.GetPos();
            p.y -= 40.0f;
            DrawStringDecal(p, sCustomFilename);
            cSaveCustomButton.DrawSelf(this);
        }


        void RenderFileSelect()
        {
            Clear(olc::VERY_DARK_BLUE);
            for (int i = 0; i < vFileButtons.size(); i++)
            {
                vFileButtons[i].DrawSelf(this);
            }
        }


        void RenderEdit()
        {
            Clear(olc::VERY_DARK_BLUE);
            std::string sSelectedTileType = vColors[iCurColorIndex].sName;
            olc::vf2d vStrPos = { 10.0f, 85.0f };
            DrawStringDecal(vStrPos, vColors[iCurColorIndex].sName, vColors[iCurColorIndex].pColor);
            cIncreaseSizeButton.DrawSelf(this);
            cDecreaseSizeButton.DrawSelf(this);
            cWorld.DrawMap(&tv, vCursorCoords, iCursorSize);
            cSaveAsButton.DrawSelf(this);
            cSaveButton.DrawSelf(this);
            cResetButton.DrawSelf(this);
            for (int i = 0; i < vTileTypes.size(); i++)
            {
                vTileTypes[i].DrawSelf(this);
            }
        }


        void HandleMenuInput()
        {
            olc::vf2d vCursorScreenCoords = GetMousePos();
            if (cOpenButton.ButtonHover(vCursorScreenCoords))
            {
                bOnUI = true;
                if (GetMouse(0).bPressed)
                {
                    cOpenButton.Pressed();
                    iCurAppState = FILE_SELECT;
                }
            }
        }


        void HandleSaveAsInput()
        {
            for (int i = 0; i < olc::ENUM_END; i++)
            {
                if (GetKey((olc::Key)i).bPressed)
                    sCustomFilename += i + 64;
            }

            olc::vf2d vCursorScreenCoords = GetMousePos();
            if (cSaveCustomButton.ButtonHover(vCursorScreenCoords))
            {
                bOnUI = true;
                if (GetMouse(0).bPressed)
                {
                    cSaveCustomButton.Pressed();
                    iCurAppState = EDIT;
                    cWorld.SaveMapAs(sCustomFilename);
                }
            }
        }


        void HandleFileSelectInput()
        {
            olc::vf2d vCursorScreenCoords = GetMousePos();
            for (int i = 0; i < vFileButtons.size(); i++)
            {
                if (vFileButtons[i].ButtonHover(vCursorScreenCoords))
                {
                    bOnUI = true;
                    if (GetMouse(0).bPressed)
                    {
                        vFileButtons[i].Pressed();
                        cWorld.LoadMapFromFile("maps/" + vFileButtons[i].sText);
                        std::cout << vFileButtons[i].sText << std::endl;
                        iCurAppState = EDIT;
                    }
                }
            }
        }


        void HandleEditInput()
        {
            bOnUI = false;
            if (GetMouse(2).bPressed) tv.StartPan(GetMousePos());
            if (GetMouse(2).bHeld) tv.UpdatePan(GetMousePos());
            if (GetMouse(2).bReleased) tv.EndPan(GetMousePos());
            if (GetMouseWheel() > 0) tv.ZoomAtScreenPos(2.0f, GetMousePos());
            if (GetMouseWheel() < 0) tv.ZoomAtScreenPos(0.5f, GetMousePos());

            olc::vf2d vCursorScreenCoords = GetMousePos();
            vCursorCoords = tv.ScreenToWorld(vCursorScreenCoords);
            for (int i = 0; i < vTileTypes.size(); i++)
            {
                if (vTileTypes[i].ButtonHover(vCursorScreenCoords))
                {
                    bOnUI = true;
                    if (GetMouse(0).bPressed)
                    {
                        vTileTypes[i].Pressed();
                        iCurColorIndex = TileType(i);
                    }
                }
            }

            if (cSaveAsButton.ButtonHover(vCursorScreenCoords))
            {
                bOnUI = true;
                if (GetMouse(0).bPressed)
                {
                    cSaveAsButton.Pressed();
                    iCurAppState = SAVE_AS;
                }
            }

            if (cSaveButton.ButtonHover(vCursorScreenCoords))
            {
                bOnUI = true;
                if (GetMouse(0).bPressed)
                {
                    std::cout << "Saving map to file" << std::endl;
                    cSaveButton.Pressed();
                    cWorld.SaveMapToFile();
                }
            }

            if (cResetButton.ButtonHover(vCursorScreenCoords))
            {
                bOnUI = true;
                if (GetMouse(0).bPressed)
                {
                    std::cout << "Reseting map" << std::endl;
                    cResetButton.Pressed();
                    cWorld.ResetMap();
                }
            }

            if (cIncreaseSizeButton.ButtonHover(vCursorScreenCoords))
            {
                bOnUI = true;
                if (GetMouse(0).bPressed)
                {
                    if (iCursorSize == 1)
                        iCursorSize = 2;
                    else
                        iCursorSize += 2;

                    if (iCursorSize > iMaxCursorSize)
                        iCursorSize = iMaxCursorSize;
                    cIncreaseSizeButton.Pressed();
                }
            }

            if (cDecreaseSizeButton.ButtonHover(vCursorScreenCoords))
            {
                bOnUI = true;
                if (GetMouse(0).bPressed)
                {
                    iCursorSize -= 2;
                    if (iCursorSize < 2)
                        iCursorSize = 1;
                    cDecreaseSizeButton.Pressed();
                }
            }

            if (!bOnUI)
            {
                if (GetMouse(0).bHeld)
                {
                    cWorld.AddSolidTile(vCursorCoords, iCurColorIndex, iCursorSize);
                }

                if (GetMouse(1).bHeld)
                {
                    cWorld.RemoveSolidTile(tv.ScreenToWorld(vCursorScreenCoords), iCursorSize);
                }
            }
        }
};



int main()
{
    TileMapEditor tme;
    if (tme.Construct(720, 480, 2, 2, false, true))
        tme.Start();

    return 0;
}
