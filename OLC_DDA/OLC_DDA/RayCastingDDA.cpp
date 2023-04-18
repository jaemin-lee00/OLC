#define OLC_PGE_APPLICATION
#include "C:\olcPixelGameEngine.h"

class Example : public olc::PixelGameEngine {

public :
	Example() {
	
		sAppName = "RayCast With DDA Algorithm";
	}
};


//class Example : public olc::PixelGameEngine{
//public:
//	Example() {
//		sAppName = "RayCast With DDA Algorithm";
//	}
//
//	olc::vf2d vPlayer = { 0,0 };
//	olc::vi2d vMapSize = { 32,30 };
//	olc::vi2d vCellSize  = { 16,16 };
//	std::vector<int> vecMap;
//
//public:
//	bool OnUserCreate() override {
//		 Construct Map
//		vecMap.resize(vMapSize.x * vMapSize.y);
//		return true;
//	}
//
//	bool OnUserUpdate(float fElapsedTime) override {
//		olc::vf2d vMouse = { float(GetMouseX()), float(GetMouseY()) };
//		olc::vf2d vMouseCell = vMouse / vCellSize;
//		olc::vi2d vCell = (olc::vi2d)vMouseCell; // implicit cast to integer, rounds down
//
//	}
//
//};
//
//int main() {
//
//	return 0;
//}