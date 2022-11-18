#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <iostream>

#define PIECE_WIDTH 60
#define PIECE_HEIGHT 60

#define LEVEL_HEIGHT 100
#define LEVEL_WIDTH 200

struct PLAYER
{
    float x = 0;
    float y = 0;
    int width = 8;
    int height = 8;
    
    float velocity_x = 0.0;
    float velocity_y = 0.0;
    bool jumping = true;
};

class EscapeRoom : public olc::PixelGameEngine
{
    
public:
 
    

public:
    
    float gravity = 70.0f;
    float friction = 0.9;
    
    int levelHeight = LEVEL_HEIGHT;
    int levelWidth  = LEVEL_WIDTH;
    
    PLAYER player;

    
    bool OnUserCreate() override
    {
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
            player.y = LEVEL_HEIGHT - player.height;
    }
    
    void updatePlayer(float elapsedTime)
    {
        // set velocity to zero if there is no movement
        player.velocity_x = 0;
        player.velocity_y = 0;
        
        if(IsFocused())
        {
            if(GetKey(olc::Key::UP).bHeld)
            {
                player.velocity_y = -50.0f;
            }
            if(GetKey(olc::Key::DOWN).bHeld)
            {
                player.velocity_y = 50.0f;
            }
            if(GetKey(olc::Key::LEFT).bHeld)
            {
                player.velocity_x = -50.0f;
            }
            if(GetKey(olc::Key::RIGHT).bHeld)
            {
                player.velocity_x = 50.0f;
            }
            if(GetKey(olc::Key::SPACE).bHeld)
            {
                // jump
                player.velocity_y = -300.0f;
            }
            
        }
        // Add gravity and friction
        player.velocity_y += gravity * elapsedTime;
        
        player.velocity_x *= friction;
        player.velocity_y *= friction;
        
        // update player position based on change in velocity
        player.x += player.velocity_x * elapsedTime;
        player.y += player.velocity_y * elapsedTime;
        
        boundaryCollisionDetection();
        
        FillRect(player.x, player.y, player.width, player.height, olc::GREEN);
    }
    
};


int main(int argc, char const *argv[]) {
    EscapeRoom demo;
    if (demo.Construct(LEVEL_WIDTH, LEVEL_HEIGHT, 8, 8))
        demo.Start();

    return 0;
}
