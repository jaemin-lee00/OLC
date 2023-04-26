#define OLC_PGE_APPLICATION
#include "olcConsoleGameEngine.h"

class OneLoneCoder_PathFinding : public olcConsoleGameEngine{
	public:
		OneLoneCoder_PathFinding() {
		
		}


	protected:
		virtual bool OnUserCreate() {
			return true;
		}

		virtual bool OnUserUpdate(float fElapsedTime) {
			return true;
		}
};

int main() {

	OneLoneCoder_PathFinding game;
	game.ConstructConsole(160, 160, 6, 6);
	game.Start();
	return 0;
}
