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
    std::string sLevel =
        "............."
        "............."
        "............."
        "............."
        "............."
        "............."
        "..........P.."
        ".............";
    
    std::string personPng = "01A.png";
    std::string personIdlePng = "01.png";
    
    olc::vf2d vLevelSize = {13, 9};
    olc::vf2d vBlockSize = {100, 100};
    olc::vf2d vPlayerSize = {2, 2};
    
    olc::vf2d vPlayerPos = {0, 0};
    olc::vf2d vPlayerVel = {0, 0};
    
    olc::Renderable wall;
    olc::Renderable wall2;

    olc::Sprite* person = nullptr;
    olc::Decal* person_dec = nullptr;
    
    uint8_t frame = 0;
    uint8_t frame_count = 0;
    uint8_t frame_count_idle;
    

public:

    void DrawBackground()
    {
        for(int y = 0; y < vLevelSize.y; y++)
        {
            for(int x = 0; x < vLevelSize.x; x++)
            {
                if( y >= 7)
                    DrawSprite(olc::vi2d({x, y}) * vBlockSize, wall2.Sprite());
                else
                    DrawSprite(olc::vi2d({x, y}) * vBlockSize, wall.Sprite());
            }
        }
    }

    bool OnUserCreate() override
    {
        wall.Load("wall.png");
        wall2.Load("wall2.png");
            
            
        person = new olc::Sprite("01.png");
        person_dec = new olc::Decal(person);
        
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        auto GetTile = [&](int x, int y)
        {
            if(x >= 0 && x < vLevelSize.x && y >= 0 && y < vLevelSize.y)
                return sLevel[y * vLevelSize.x + x];
            else
                return ' ';
        };
        
        auto SetTile = [&](int x, int y, char c)
        {
            if(x >= 0 && x < vLevelSize.x && y >= 0 && y < vLevelSize.y)
                sLevel[y * vLevelSize.x + x] = c;
        };
        
        vPlayerVel = {0, 0};
        
        // Handle input
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
                if ( frame > 0 && frame < 10)
                {
                    personPng[0] = char(int(frame/10) + 48);
                    personPng[1] = char(frame + 48);
                }
                else{
                    personPng[0] = char(int(frame/10) + 48);
                    personPng[1] = char(int(frame-10) + 48);
                }
                person = new olc::Sprite(personPng);
                person_dec = new olc::Decal(person);
                vPlayerVel.x = -2.0f;
                if(!(frame_count % 4))
                    frame = (++frame) % 14;
                if(frame == 0)
                    frame = 1;
                frame_count++;
            }
            if(GetKey(olc::Key::RIGHT).bHeld)
            {
                if ( frame > 0 && frame < 10)
                {
                    personPng[0] = char(int(frame/10) + 48);
                    personPng[1] = char(frame + 48);
                }
                else{
                    personPng[0] = char(int(frame/10) + 48);
                    personPng[1] = char(int(frame-10) + 48);
                }
                person = new olc::Sprite(personPng);
                person_dec = new olc::Decal(person);
                vPlayerVel.x = 2.0f;
                if(!(frame_count % 4))
                    frame = (++frame) % 14;
                if(frame == 0)
                    frame = 1;
                frame_count++;
            }
            
        }
        
        if(!(frame_count_idle % 5) && !(GetKey(olc::Key::RIGHT).bHeld) && !(GetKey(olc::Key::LEFT).bHeld) )
        {
            if ( frame > 0 && frame < 10)
            {
                personIdlePng[0] = char(int(frame/10) + 48);
                personIdlePng[1] = char(frame + 48);
            }
            else if(frame >= 10 && frame < 20){
                personIdlePng[0] = char(int(frame/10) + 48);
                personIdlePng[1] = char(int(frame-10) + 48);
            }
            else{
                personIdlePng[0] = char(int(frame/10) + 48);
                personIdlePng[1] = char(int(frame-20) + 48);
            }
            
            person = new olc::Sprite(personIdlePng);
            person_dec = new olc::Decal(person);
            
            frame = (++frame) % 26;
            if(frame == 0)
                frame = 1;
            //std::cout<<personPng<<std::endl;
        }
        
        frame_count_idle++;
        vPlayerPos.x = vPlayerPos.x + vPlayerVel.x * fElapsedTime;
        vPlayerPos.y = vPlayerPos.y + vPlayerVel.y * fElapsedTime;
        
        Clear(olc::BLACK);
        DrawBackground();

        
        //Draw Tiles
        for(int x = 0; x < vLevelSize.x; x++)
        {
            for(int  y = 0; y < vLevelSize.y; y++)
            {
                char sTileType = GetTile(x, y);
                switch(sTileType)
                {
                    case 'P':
                        DrawDecal(olc::vf2d{vPlayerPos.x, vPlayerPos.y} * vBlockSize, person_dec,vPlayerSize);
                        break;
                    default:
                        break;
                }
            }
        }
        
        return true;
    }
};


int main(int argc, char const *argv[]) {
    EscapeRoom demo;
    if (demo.Construct(1280, 848, 1, 1))
        demo.Start();

    return 0;
}
