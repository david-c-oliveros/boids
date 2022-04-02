#include <iostream>
#include <memory>
#include <fstream>
#include <algorithm>
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"


enum TileType
{
    WHITE,
    RED,
    BLUE
};


struct color_t
{
    olc::Pixel pColor;
    std::string sName;
};


class World
{
    public:
        World();
        ~World();

    public:
        std::unique_ptr<olc::Renderable> m_pMapSprite;
        std::string sGreeting;
        std::string sMapString;
        std::vector<std::vector<char>> vMap;
        olc::vi2d vSize;
        void GenerateWorld();
        void StaticGenMap();
        void ProcGenMap();
        void PrintWorld();
        olc::vf2d FindSpawnableCell();
        olc::vi2d GetSize();
        void DrawMap(olc::TileTransformedView* tv, olc::vf2d vCursorCoords, int iCursorSize);
        olc::vf2d FindRandomOpenSpot();
        void AddSolidTile(olc::vi2d index, TileType tTileType, int iCursorSize);
        void RemoveSolidTile(olc::vi2d index, int iCursorSize);
        void SaveMapToFile();
        void SaveMapAs(std::string sFilename);
        void LoadMapFromFile(std::string filname);
        void ResetMap();
};
