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
		olc::vf2d vRayUnitStepSize = { sqrt(1 + (vRayDir.y / vRayDir.x) * (vRayDir.y / vRayDir.x)), sqrt(1 + (vRayDir.x / vRayDir.y) * (vRayDir.x / vRayDir.y)) };

		olc::vi2d vMapCheck = vRayStart;
		olc::vf2d vRayLength1D;

		olc::vi2d vStep;
		
		//Increase the length of the Ray
		//To the X
		if (vRayDir.x < 0) {
		
			vStep.x = -1;
			vRayLength1D.x = (vRayStart.x - float(vMapCheck.x)) * vRayUnitStepSize.x;
		}
		else {
			
			vStep.x = 1;
			vRayLength1D.x = (float(vMapCheck.x + 1) - vRayStart.x) * vRayUnitStepSize.x;
		}
		// To the y
		if (vRayDir.y < 0) {

			vStep.y = -1;
			vRayLength1D.y = (vRayStart.y - float(vMapCheck.y)) * vRayUnitStepSize.y;
		}
		else {

			vStep.y = 1;
			vRayLength1D.y = (float(vMapCheck.y + 1) - vRayStart.y) * vRayUnitStepSize.y;
		}

		bool bTileFound = false;
		float fMaxDistance = 100.0f;
		float fDistance = 0.0f;
		

		while (!bTileFound && fDistance < fMaxDistance) {
		
			// Walk
			if (vRayLength1D.x < vRayLength1D.y) {
				
				vMapCheck.x += vStep.x;
				fDistance = vRayLength1D.x;
				vRayLength1D.x += vRayUnitStepSize.x;
			}
			else {
			
				vMapCheck.y += vStep.y;
				fDistance = vRayLength1D.y;
				vRayLength1D.y += vRayUnitStepSize.y;
			}

			//Check out of map
			if (vMapCheck.x >= 0 && vMapCheck.x < vMapSize.x &&
				vMapCheck.y >= 0 && vMapCheck.y < vMapSize.y) {
				//Check Tile is found at the mouse point
				if (vecMap[vMapCheck.y * vMapSize.x + vMapCheck.x] == 1) {

					bTileFound = true;
				}
			}
		}

		olc::vf2d vIntersection;
		if (bTileFound) {
			
			vIntersection = vRayStart + vRayDir * fDistance;
		}

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

			if (bTileFound) {
				
				DrawCircle(vIntersection * vCellSize, 4.0f, olc::YELLOW);
			}
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