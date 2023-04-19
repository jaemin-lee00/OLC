#define OLC_PGE_APPLICATION
#include "C:\olcPixelGameEngine.h"

class Example : public olc::PixelGameEngine{
public:
	Example() {
		sAppName = "RayCast With DDA Algorithm";
	}

	olc::vf2d vPlayer = { 0,0 };
	olc::vi2d vMapSize = { 32,30 };
	olc::vi2d vCellSize  = { 16,16 };
	std::vector<int> vecMap;

public:
	bool OnUserCreate() override {
		// Construct Map;
		vecMap.resize(vMapSize.x * vMapSize.y);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		olc::vf2d vMouse = { float(GetMouseX()), float(GetMouseY()) };
		olc::vf2d vMouseCell = vMouse / vCellSize;
		olc::vi2d vCell = (olc::vi2d)vMouseCell; // implicit cast to integer, rounds down

		// Paint with right mouse button "solid" tiles
		if (GetMouse(1).bHeld) vecMap[vCell.y * vMapSize.x + vCell.x] = 1;

		// Move "Player" position
		if (GetKey(olc::Key::W).bHeld) vPlayer.y -= 25.0f * fElapsedTime;
		if (GetKey(olc::Key::S).bHeld) vPlayer.y += 25.0f * fElapsedTime;
		if (GetKey(olc::Key::A).bHeld) vPlayer.x -= 25.0f * fElapsedTime;
		if (GetKey(olc::Key::D).bHeld) vPlayer.x += 25.0f * fElapsedTime;

		
		// Form ray cast from player into scene
		olc::vf2d vRayStart = vPlayer;
		olc::vf2d vRayDir = (vMouseCell - vPlayer).norm();

		Clear(olc::BLACK);

		// Draw Map
		for (int y = 0; y < vMapSize.y; y++) {
			for (int x = 0; x < vMapSize.x; x++) {
				
				int cell = vecMap[y * vMapSize.x + x];
				if (cell == 1) {
					FillRect(olc::vf2d(x, y) * vCellSize, vCellSize, olc::BLUE);
				}
				// Draw Cell border
				DrawRect(olc::vf2d(x, y) * vCellSize, vCellSize, olc::DARK_GREY);
			}
		}

		//Draw ray between player and mouse if left mouse button held
		if (GetMouse(0).bHeld) {

			DrawLine(vPlayer * vCellSize, vMouse, olc::WHITE, 0xF0F0F0F0);
		}

		//Draw Player
		FillCircle(vPlayer * vCellSize, 4.0f, olc::RED);

		//Draw Mouse
		FillCircle(vMouse, 4.0f, olc::GREEN);
		return true;
	}

};

int main() {
	Example demo;
	if (demo.Construct(512, 480, 2, 2))
		demo.Start();
	return 0;
}