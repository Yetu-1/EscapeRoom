#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <iostream>

#define PIECE_WIDTH 60
#define PIECE_HEIGHT 60

#define LEVEL_HEIGHT 848
#define LEVEL_WIDTH 1280

struct PLAYER
{
    float x = 0.0f;
    float y = 0.0f;
    float width = 2;
    float height = 2;
    
    float velocity_x = 0.0f;
    float velocity_y = 0.0f;
    bool jumping = true;
};

class EscapeRoom : public olc::PixelGameEngine
{
    
public:
 
    

public:
    
    float gravity = 1.0f;
    float friction = 0.9;
    
    int levelHeight = LEVEL_HEIGHT;
    int levelWidth  = LEVEL_WIDTH;
    
    PLAYER player;
    
    std::string personPng = "01A.png";
    std::string personIdlePng = "01.png";
    
    olc::Sprite* person = nullptr;
    olc::Decal* person_dec = nullptr;
    
    uint8_t frame = 0;
    uint8_t frame_count = 0;
    uint8_t frame_count_idle;
    
    olc::vf2d vBlockSize = {1, 1};
    olc::vf2d vPlayerSize = {2, 2};
    
    bool OnUserCreate() override
    {
        person = new olc::Sprite(personPng);
        person_dec = new olc::Decal(person);
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::VERY_DARK_GREY);
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
        if(player.y < 0)
            player.y = 0;
        else if ( player.y + player.height > LEVEL_HEIGHT)
        {
            player.velocity_y = 0;
            player.y = LEVEL_HEIGHT - player.height;
        }
    }
    
    void updatePlayer(float elapsedTime)
    {
        
        // Input event handler
        if(IsFocused())
        {
            if(GetKey(olc::Key::LEFT).bHeld)
            {
                player.velocity_x = -22.0f;
            }
            else if(GetKey(olc::Key::RIGHT).bHeld)
            {
                player.velocity_x = 22.0f;
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
        DrawDecal(olc::vf2d{player.x, player.y}, person_dec, olc::vf2d{player.width, player.height});
        //FillRect(player.x, player.y, player.width, player.height, olc::GREEN);
    }
    
};


int main(int argc, char const *argv[]) {
    EscapeRoom demo;
    if (demo.Construct(LEVEL_WIDTH, LEVEL_HEIGHT, 1, 1))
        demo.Start();

    return 0;
}
