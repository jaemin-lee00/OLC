#define OLC_PGE_APPLICATION
#include "C:\olcPixelGameEngine.h"

class Example : public olc::PixelGameEngine{
public:
	Example() {
		sAppName = "RayCast With DDA Algorithm";
	}

	olc::vf2d vPlayer = { 0,0 };
	olc::vi2d vMapSize = { 32,30 };
	olc::vi2d VCellSize  = { 16,16 };
	std::vector<int> vecMap;

public:
	bool OnUserCreate() override {
		// Construct Map
		vecMap.resize(vMapSize.x * vMapSize.y);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
	
	}

};

int main() {

	return 0;
}