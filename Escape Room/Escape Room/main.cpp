#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class EscapeRoom : public olc::PixelGameEngine {
    public:
        EscapeRoom() {
            sAppName = "EscapeRoom";
        }
        
        std::string sLevel =
            "################"
            "################"
            "################"
            "################"
            "################"
            "################"
            "################"
            "################"
            "################"
            "################"
            "################"
            "################"
            "################"
            "################"
            "################";
    
    olc::vf2d vLevelSize = {16, 15};
    olc::vf2d vBlockSize = {100, 100};
    
    olc::Renderable gfxTiles;
    
    struct block
    {
        block()
        {
            
        }
        
        virtual void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, const olc::vi2d& size, const olc::Renderable& skin)
        {
            
        }
        
    };
    
    struct wall_tiles : public block
    {
        void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, const olc::vi2d& size, const olc::Renderable& skin) override
        {
            pge->DrawPartialSprite(pos * size, skin.Sprite(), olc::vi2d(0, 0) * size, size);
        }
    };
    
    struct player : public block
    {
        void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, const olc::vi2d& size, const olc::Renderable& skin) override
        {
            pge->DrawPartialSprite(pos * size, skin.Sprite(), olc::vi2d(0, 0) * size, size);
        }
    };
    
    struct barrel : public block
    {
        void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, const olc::vi2d& size, const olc::Renderable& skin) override
        {
            pge->DrawPartialSprite(pos * size, skin.Sprite(), olc::vi2d(0, 0) * size, size);
        }

    };


    
    std::vector<std::unique_ptr<block>> vLevel;
        

    public:
    
        void LoadLevel(int n)
        {
            for(int y = 0; y < vLevelSize.y; y++)
            {
                for(int x = 0; x < vLevelSize.x; x++)
                {
                    switch(sLevel[(y * vLevelSize.x) + x ])
                    {
                        case '#':
                            vLevel.emplace_back(std::make_unique<wall_tiles>());
                            break;
                        case 'P':
                            vLevel.emplace_back(std::make_unique<player>());
                            break;
                        case '+':
                            vLevel.emplace_back(std::make_unique<barrel>());
                            break;
                        default:
                            vLevel.emplace_back(nullptr);
                    }
                }
            }
        }
    
        bool OnUserCreate() override
        {
            gfxTiles.Load("wall.png");
            LoadLevel(0);
            return true;
        }

        bool OnUserUpdate(float fElapsedTime) override
        {
            Clear(olc::BLACK);
            
            auto id = [&](olc::vi2d& pos) { return (pos.y * vLevelSize.x) + pos.x; };
            
            olc::vi2d vTilePos = {0,0};
            for (vTilePos.y = 0; vTilePos.y < vLevelSize.y; vTilePos.y++)
            {
                for(vTilePos.x = 0; vTilePos.x < vLevelSize.x; vTilePos.x++)
                {
                    auto& b = vLevel[id(vTilePos)];
                    
                    if(b)
                    {
                        b->DrawSelf(this, vTilePos, vBlockSize, gfxTiles);
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
