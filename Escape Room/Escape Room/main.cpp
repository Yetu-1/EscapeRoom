#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class EscapeRoom : public olc::PixelGameEngine
{
    
public:
    EscapeRoom() {
        sAppName = "EscapeRoom";
    }

    olc::vf2d vLevelSize = {13, 9};
    olc::vf2d vBlockSize = {100, 100};
    olc::vf2d vBarrelSize = {58, 58};

    olc::Renderable wall;
    olc::Renderable barrel;

public:

    void DrawBackground()
    {
        for(int y = 0; y < vLevelSize.y; y++)
        {
            for(int x = 0; x < vLevelSize.x; x++)
            {
                DrawSprite(olc::vi2d({x, y}) * vBlockSize, wall.Sprite());
            }
        }
    }

    bool OnUserCreate() override
    {
        wall.Load("wall.png");
        barrel.Load("1.png");
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::BLACK);
        DrawBackground();
        //DrawSprite(olc::vi2d({10, 10}) * vBarrelSize, barrel.Sprite());
        return true;
    }
};


int main(int argc, char const *argv[]) {
	EscapeRoom demo;
	if (demo.Construct(1280, 848, 1, 1))
		demo.Start();

	return 0;
}
