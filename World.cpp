#include "World.h"

World::World()
{
    sGreeting = "Welcome to the world";
}

World::~World() {}


void World::GenerateWorld()
{
    StaticGenMap();
}


void World::StaticGenMap()
{
    m_pMapSprite = std::make_unique<olc::Renderable>();
    m_pMapSprite->Load("./res/sprites/tilemap.png");
    sMapString =
        "################################################################################################################################"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "################################################################################################################################";

    vSize = { 128, 24 };

    for (int i = 0; i < vSize.y; i++)
    {
        vMap.push_back(std::vector<char>());
        for (int j = 0; j < vSize.x; j++)
        {
            vMap[i].push_back(sMapString[i * vSize.x + j]);
        }
    }
}


void World::PrintWorld()
{
    // 2D vector
    for (int i = 0; i < vMap.size(); i++)
    {
        for (int j = 0; j < vMap[i].size(); j++)
        {
            std::cout << vMap[i][j];
        }
        std::cout << std::endl;
    }
}


// TODO - Make better (right now just loops through string and finds the first emtpy cell)
olc::vf2d World::FindSpawnableCell()
{
    // String
    for (int i = 0; i < vSize.y; i++)
    {
        for (int j = 0; j < vSize.x; j++)
        {
            if (sMapString[i * vSize.x + j] == '.')
            {
                return { j + 0.5f, i + 0.5f };
            }
        }
    }
}


olc::vi2d World::GetSize()
{
    return vSize;
}


void World::DrawMap(olc::TileTransformedView* tv, olc::vf2d vCursorCoords, int iCursorSize)
{
    olc::vi2d vCursorTile = { vCursorCoords.x - (iCursorSize / 2), vCursorCoords.y - (iCursorSize / 2) };
    int iOffset = iCursorSize / 2;
    olc::vi2d vTL = tv->GetTopLeftTile().max({ 0, 0 });
    olc::vi2d vBR = tv->GetBottomRightTile().min(vSize);
    olc::vi2d vTile;
    olc::Pixel cOutlineColor = olc::BLACK;
    int iTransparency = 255;
    for (vTile.y = vTL.y; vTile.y < vBR.y; vTile.y++)
    {
        for (vTile.x = vTL.x; vTile.x < vBR.x; vTile.x++)
        {
            if ((vTile.x >= vCursorTile.x && vTile.x <= (vCursorTile.x + iCursorSize - 1)) &&
                (vTile.y >= vCursorTile.y && vTile.y <= (vCursorTile.y + iCursorSize - 1)))
                iTransparency = 200;
            else
                iTransparency = 255;

            switch(vMap[vTile.y][vTile.x])
            {
                case('#'):
                    tv->FillRect(vTile, { 1.0f, 1.0f }, olc::Pixel(255, 255, 255, iTransparency));
                    tv->DrawRect(vTile, { 1.0f, 1.0f }, cOutlineColor);
                    break;

                case('+'):
                    tv->FillRect(vTile, { 1.0f, 1.0f }, olc::Pixel(255, 0, 0, iTransparency));
                    tv->DrawRect(vTile, { 1.0f, 1.0f }, cOutlineColor);
                    break;

                case('='):
                    tv->FillRect(vTile, { 1.0f, 1.0f }, olc::Pixel(0, 0, 255, iTransparency));
                    tv->DrawRect(vTile, { 1.0f, 1.0f }, cOutlineColor);
                    break;
                //tv->DrawPartialDecal(vTile, m_pMapSprite->Decal(), { 0.0f, 0.0f }, { 16.0f, 16.0f }, { 2.1f, 2.1f });
            }
        }
    }
    tv->DrawRect(vCursorTile, { iCursorSize, iCursorSize }, olc::DARK_GREY);
}


olc::vf2d World::FindRandomOpenSpot()
{
    auto ti = std::chrono::system_clock::now().time_since_epoch().count();
    srand(ti);
    olc::vf2d randCoord = { (int)rand() % vSize.x, (int)rand() % vSize.y };
    int index = randCoord.y * vSize.x + randCoord.x;
    while (index < sMapString.length() && sMapString[index] != '.')
    {
        randCoord = { rand() % vSize.x, rand() % vSize.y };
        index = randCoord.y * vSize.x + randCoord.x;
    }

    return randCoord;
}


void World::AddSolidTile(olc::vi2d index, TileType tTileType, int iCursorSize)
{
    char cTile;
    switch(tTileType)
    {
        case(0):
            cTile = '#';
            break;
        case(1):
            cTile = '+';
            break;
        case(2):
            cTile = '=';
            break;
    }

    if (index.x >= vSize.x)
    {
        int iDeltaSize = index.x - vSize.x;
        vSize.x += iDeltaSize;
        // 2D vector
        for (int j = 0; j < iDeltaSize; j++)
        {
            vMap[0].push_back('#');
        }
        for (int i = 1; i < vMap.size() - 1; i++)
        {
            vMap[i][vSize.x - iDeltaSize - 1] = '.';
            for (int j = 0; j < iDeltaSize; j++)
            {
                vMap[i].push_back('.');
            }
            vMap[i][vSize.x - 1] = '#';
        }
        for (int j = 0; j < iDeltaSize; j++)
        {
            vMap[vMap.size() - 1].push_back('#');
        }
    }
    else if (iCursorSize == 1)
    {
        if (index.y >= 0 && index.y < vSize.y && vMap[index.y][index.x] == '.')
            vMap[index.y][index.x] = cTile;
    }
    else
    {
        /*****************************/
        /*        Cursor Size        */
        /*****************************/
        int iOffset = iCursorSize / 2;
        int i = index.y * vSize.x + index.x;
        for (int y = std::clamp(index.y - iOffset, 0, vSize.y); y < std::clamp(index.y + iOffset, 0, vSize.y); y++)
        {
            for (int x = std::clamp(index.x - iOffset, 0, vSize.x); x < std::clamp(index.x + iOffset, 0, vSize.x); x++)
            {
                if (y >= 0 && y < vSize.y && x < vSize.x && vMap[y][x] == '.')
                    vMap[y][x] = cTile;
            }
        }
        /*****************************/
    }
}


void World::RemoveSolidTile(olc::vi2d index, int iCursorSize)
{
    if (iCursorSize == 1)
    {
        if (index.y >= 0 && index.y < vSize.y && index.x < vSize.x && vMap[index.y][index.x] != '.')
            vMap[index.y][index.x] = '.';
    }
    int iOffset = iCursorSize / 2;
    int i = index.y * vSize.x + index.x;
    for (int y = std::clamp(index.y - iOffset, 0, vSize.y); y < std::clamp(index.y + iOffset, 0, vSize.y); y++)
    {
        for (int x = std::clamp(index.x - iOffset, 0, vSize.x); x < std::clamp(index.x + iOffset, 0, vSize.x); x++)
        {
            if (y >= 0 && y < vSize.y && x < vSize.x && vMap[y][x] != '.')
                vMap[y][x] = '.';
        }
    }
}


void World::SaveMapToFile()
{
    std::ofstream fout;
    int i = 0;
    std::string filename = "";
    while(true)
    {
        filename = "maps/world_map_" + std::to_string(i) + ".txt";
        if (!std::filesystem::exists(filename))
        {
            fout.open(filename);
            break;
        }
        i++;
    }

    for (int i = 0; i < vSize.y; i++)
    {
        for (int j = 0; j < vSize.x; j++)
        {
            fout << vMap[i][j];
        }
        fout << std::endl;
    }
    fout.close();
}


void World::SaveMapAs(std::string sFilename)
{
    std::cout << "Save map as" << std::endl;
}


void World::LoadMapFromFile(std::string filename)
{
    std::ifstream fin;
    if (std::filesystem::exists(filename))
    {
        fin.open(filename);
    }
    std::string line;
    vMap.clear();
    int y = 0;
    int x = 0;
    while (fin >> line)
    {
        std::stringstream ss;
        ss << line;
        vMap.push_back(std::vector<char>());
        char c;
        x = 0;
        while (ss.get(c))
        {
            vMap[y].push_back(c);
            x++;
        }
        y++;
    }
    vSize = {x, y};
    std::cout << vSize << std::endl;
}


void World::ResetMap()
{
    vSize = { 128, 24 };
    for (int i = 0; i < vSize.y; i++)
    {
        std::cout << vMap[i].size() << std::endl;
        vMap[i].resize(vSize.x);
        for (int j = 0; j < vSize.x; j++)
        {
            vMap[i][j] = sMapString[i * vSize.x + j];
        }
    }
}
