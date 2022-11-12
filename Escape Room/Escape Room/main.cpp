#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <iostream>

#define PIECE_WIDTH 60
#define PIECE_HEIGHT 60

class EscapeRoom : public olc::PixelGameEngine
{
    
public:
    EscapeRoom() {
        sAppName = "EscapeRoom";
    }
private:
    std::string sLevel;
    olc::vi2d vLevelSize;
    olc::vf2d vCameraPos;
    olc::vf2d vPlayerPos;
    olc::vf2d vPlayerVel;
public:


    bool OnUserCreate() override
    {
        vLevelSize = {64, 16};
        vCameraPos = {0, 0};
        vPlayerPos = vCameraPos;
        
        sLevel =    "..#######......................................................."
                    "................................................................"
                    "................................................................"
                    "...................................###############.............."
                    "...................................##..........................."
                    "................................................................"
                    "...............................################................."
                    ".............................####..............................."
                    "........................#####################..................."
                    "###########################################...#################."
                    "..........................................#..#.................."
                    "............................###############..#.................."
                    "............................#................#.................."
                    "............................#.################.................."
                    "............................###................................."
                    "................................................................";
        
        
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        //Utility Lamdas
        auto GetTile= [&](int x, int y)
        {
            if(x >= 0 && x < vLevelSize.x && y >= 0 && y < vLevelSize.y)
                return sLevel[y * vLevelSize.x + x];
            else
                return ' ';
        };
        
        auto SetTile= [&](int x, int y, char c)
        {
            if(x >= 0 && x < vLevelSize.x && y >= 0 && y < vLevelSize.y)
                sLevel[y * vLevelSize.x + x] = c;
        };
        
        vPlayerVel = {0,0};
        
        // Handle Input
        if(IsFocused())
        {
            if(GetKey(olc::Key::UP).bHeld)
            {
                  vPlayerVel.y = -6.0f;
            }
            if(GetKey(olc::Key::DOWN).bHeld)
            {
                  vPlayerVel.y = 6.0f;
            }
            if(GetKey(olc::Key::LEFT).bHeld)
            {
                  vPlayerVel.x = -6.0f;
            }
            if(GetKey(olc::Key::RIGHT).bHeld)
            {
                  vPlayerVel.x = 6.0f;
            }
        }
        
        vPlayerPos.x = vPlayerPos.x + vPlayerVel.x * fElapsedTime;
        vPlayerPos.y = vPlayerPos.y + vPlayerVel.y * fElapsedTime;
        
        vCameraPos.x = vPlayerPos.x;
        vCameraPos.y = vPlayerPos.y;
        
        // Draw Level
        olc::vi2d vTileSize = {16, 16};
        olc::vi2d vVisibleTiles = { ScreenWidth()/vTileSize.x, ScreenHeight()/vTileSize.y };
        
        // Calculate Top Leftmost visible tile ( camera represents the middle of the screen)
        olc::vf2d vOffset = { vCameraPos.x - ( (float)vVisibleTiles.x/2.0f), vCameraPos.y - ( (float)vVisibleTiles.y/2.0f) };
        
        // Clamp camera to game boundaries
        if (vOffset.x < 0) vOffset.x = 0;
        if (vOffset.y < 0) vOffset.y = 0;
        if (vOffset.x > vLevelSize.x - vVisibleTiles.x) vOffset.x = vLevelSize.x - vVisibleTiles.x;
        if (vOffset.y > vLevelSize.y - vVisibleTiles.y) vOffset.y = vLevelSize.y - vVisibleTiles.y;
        
        // Draw visible tile map
        for(int x = 0; x < vVisibleTiles.x; x++)
        {
            for(int y = 0; y < vVisibleTiles.y; y++)
            {
                char sTileID = GetTile(x + vOffset.x, y + vOffset.y);
                switch(sTileID)
                {
                    case '.':
                        FillRect(x * vTileSize.x, y * vTileSize.y, (x + 1) * vTileSize.x, (y + 1) * vTileSize.y, olc::CYAN);
                        break;
                    case '#':
                        FillRect(x * vTileSize.x, y * vTileSize.y, (x + 1) * vTileSize.x, (y + 1) * vTileSize.y, olc::RED);
                        break;
                    default:
                        break;
                }
            }
        }
        
        //Draw Player
        FillRect((vPlayerPos.x - vOffset.x) * vTileSize.x, (vPlayerPos.y - vOffset.y) * vTileSize.y, vTileSize.x, vTileSize.y, olc::BLUE);
        
        return true;
    }
};


int main(int argc, char const *argv[]) {
    EscapeRoom demo;
    if (demo.Construct(160, 120, 8, 8))
        demo.Start();

    return 0;
}
