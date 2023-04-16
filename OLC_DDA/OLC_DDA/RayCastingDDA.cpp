#define OLC_PGE_APPLICATION
#include "C:\olcPixelGameEngine.h"

class Example : public olc::PixelGameEngine{
public:
	Example() {
		
	}

	olc::vf2d vPlayer = { 0,0 };
	olc::vi2d vMapSize = { 32,30 };
	olc::vi2d VCellSize  = { 16,16 };
	olc::vf2d vPlayer = { 0,0 };

public:
	bool OnUserCreate() override {
		// Construct Map
	}
};

int main() {

	return 0;
}