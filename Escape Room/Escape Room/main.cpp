#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <iostream>

#define PIECE_WIDTH 60
#define PIECE_HEIGHT 60

#define LEVEL_HEIGHT 928
#define LEVEL_WIDTH 1508

struct PLAYER
{
    float x = 0.0f;
    float y = 0.0f;
    float width = 116;
    float height = 116;
    
    float velocity_x = 0.0f;
    float velocity_y = 0.0f;
    bool jumping = true;
};

class EscapeRoom : public olc::PixelGameEngine
{
    
public:
    EscapeRoom() {
        sAppName = "EscapeRoom";
    }
    
    std::string gameLevel =
        "................"
        "................"
        "................"
        "................"
        "................"
        "................"
        "...B........B..."
        "################";
    
    olc::vi2d vLevelSize = { 16, 8 };
    

public:
    
    float gravity = 30.0f;
    float friction = 0.9;
    
    int levelHeight = LEVEL_HEIGHT;
    int levelWidth  = LEVEL_WIDTH;
    
    PLAYER player;
    
    std::string personPng = "01A.png";
    std::string personIdlePng = "01.png";
    
    olc::Sprite* person = nullptr;
    olc::Decal* person_dec = nullptr;
    
    olc::Sprite* bucket = nullptr;
    olc::Decal* bucket_dec = nullptr;
    
    olc::Renderable sky;
    olc::Renderable ground;
    olc::Renderable background;
    
    uint8_t frame = 0;
    uint8_t frame_count = 0;
    uint8_t frame_count_idle = 0;
    
    olc::vf2d vTileSize = {116, 116};
    olc::vf2d vPlayerSize = {2, 2};
    
    bool OnUserCreate() override
    {
        sky.Load("background.png");
        ground.Load("Ground.png");
        background.Load("Background1.png");
        
        person = new olc::Sprite(personIdlePng);
        person_dec = new olc::Decal(person);
        
        bucket = new olc::Sprite("Bucket.png");
        bucket_dec = new olc::Decal(bucket);
        
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        auto GetTile = [&](int x, int y)
        {
            if(x >= 0 && x < vLevelSize.x && y >= 0 && y < vLevelSize.y)
                return gameLevel[y * vLevelSize.x + x];
            else
                return ' ';
        };
        
        auto SetTile = [&](int x, int y, char c)
        {
            if(x >= 0 && x < vLevelSize.x && y >= 0 && y < vLevelSize.y)
                gameLevel[y * vLevelSize.x + x] = c;
        };
        
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
                    case 'B':
                        DrawDecal(olc::vf2d{(float)x, (float)y} * vTileSize, bucket_dec, olc::vf2d{1, 1});
                        break;
                    case '#':
                        DrawSprite(olc::vi2d({x, y}) * vTileSize, ground.Sprite());
                        break;
                    default:
                        //DrawSprite(olc::vi2d({x, y}) * vTileSize, ground.Sprite());
                        break;
                }
            }
        }
        
        updatePlayer(fElapsedTime);
        
        return true;
    }
    
    void boundaryCollisionDetection()
    {
        // x axis boundary collision
        if(player.x < 0)
            player.x = 0;
        else if ( player.x + player.width > LEVEL_WIDTH)
            player.x = LEVEL_WIDTH - player.width;
        
        // y axis boundary collision
        if(player.y < 0){
            player.velocity_y = 0;
            player.y = 0;
        }
        else if ( player.y + player.height > LEVEL_HEIGHT - player.height)
        {
            player.velocity_y = 0;
            player.y = LEVEL_HEIGHT - 2*player.height;
        }
    }
    
    void updatePlayer(float elapsedTime)
    {
        
        // Input event handler
        if(IsFocused())
        {
            if(GetKey(olc::Key::LEFT).bHeld)
            {
                // Update character velocity
                player.velocity_x = -100.0f;
                
                // Update character frame
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
                if(!(frame_count % 2))
                    frame = (++frame) % 14;
                if(frame == 0)
                    frame = 1;
                frame_count++;
            }
            else if(GetKey(olc::Key::RIGHT).bHeld)
            {
                // Update player velocity
                player.velocity_x = 100.0f;
                
                // Update character frame
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

                if(!(frame_count % 2))
                    frame = (++frame) % 14;
                if(frame == 0)
                    frame = 1;
                frame_count++;
            }
            else{
                // set velocity to zero if there is no movement
                player.velocity_x = 0;
            }
            
            if(GetKey(olc::Key::SPACE).bPressed)
            {
                // jump
                player.velocity_y = -500.0f;
            }

        }
        
        // Add gravity and friction
        player.velocity_y += gravity;
        
        // update player position based on change in velocity
        player.x += player.velocity_x * elapsedTime;
        player.y += player.velocity_y * elapsedTime;
        
        boundaryCollisionDetection();
        
        if(!(frame_count_idle % 3) && !(GetKey(olc::Key::RIGHT).bHeld) && !(GetKey(olc::Key::LEFT).bHeld) )
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
        DrawDecal(olc::vf2d{player.x, player.y}, person_dec, olc::vf2d{2, 2});
    }
    
    void DrawBackground(){
        DrawSprite(0, 0, background.Sprite());
    }
    
};


int main(int argc, char const *argv[]) {
    EscapeRoom demo;
    if (demo.Construct(LEVEL_WIDTH, LEVEL_HEIGHT, 1, 1))
        demo.Start();

    return 0;
}
