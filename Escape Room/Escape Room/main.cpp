#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class EscapeRoom : public olc::PixelGameEngine {
    public:
        EscapeRoom() {
            sAppName = "EscapeRoom";
        }
        
        std::string sLevel =
            "################"
            "#..............#"
            "#.+.........+..#"
            "#..............#"
            "#....P.........#"
            "#..............#"
            "#..............#"
            "#..............#"
            "#..............#"
            "#...++....#....#"
            "#.........#....#"
            "#..............#"
            "#..............#"
            "#..............#"
            "################";
    
    olc::vf2d vLevelSize = {16, 15};
    olc::vf2d vBlockSize = {16, 15};
    
    struct block
    {
        block()
        {
            
        }
        
        virtual void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, const olc::vi2d& size)
        {
            
        }
        
        virtual bool Push(const int from)
        {
            return true;
        }
        
        virtual void Move(const int to)
        {

        }
        
    };
    
    struct block_solid : public block
    {
        void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, const olc::vi2d& size) override
        {
            pge->FillRect(pos * size, size, olc::BLUE);
        }
    };
    
    struct block_player : public block
    {
        void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, const olc::vi2d& size) override
        {
            pge->FillRect(pos * size, size, olc::WHITE);
        }
    };
    
    struct block_simple : public block
    {
        void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, const olc::vi2d& size) override
        {
            pge->FillRect(pos * size, size, olc::RED);
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
                            vLevel.emplace_back(std::make_unique<block_solid>());
                            break;
                        case 'P':
                            vLevel.emplace_back(std::make_unique<block_player>());
                            break;
                        case '+':
                            vLevel.emplace_back(std::make_unique<block_simple>());
                            break;
                        default:
                            vLevel.emplace_back(nullptr);
                    }
                }
            }
        }
    
        bool OnUserCreate() override {
            LoadLevel(0);
            return true;
        }

        bool OnUserUpdate(float fElapsedTime) override {
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
                        b->DrawSelf(this, vTilePos, vBlockSize);
                    }
                }
            }
            return true;
        }
};


int main(int argc, char const *argv[]) {
	EscapeRoom demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}
