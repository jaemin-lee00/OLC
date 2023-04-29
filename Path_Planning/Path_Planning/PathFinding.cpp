#define OLC_PGE_APPLICATION
#include "olcConsoleGameEngine.h"

using namespace std;

class OneLoneCoder_PathFinding : public olcConsoleGameEngine{

public:
		OneLoneCoder_PathFinding() {
			m_sAppName = L"Path Finding";
		}

private:
	struct sNode {

		bool bObstacle = false;				// Is the node an obstruction?
		bool bVisited = false;				// Have we searched this node before?
		float fGlobalGoal;					// Distance to goal so far
		float fLocalGoal;					// Distance to goal if we took the alternative route
		int x;								// Nodes position in 2D space
		int y;								//
		vector<sNode*> vecNeighbours;		// Connections to neighbours
		sNode* parent;						// Node connecting to this node that offers shortest parent

	};

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
