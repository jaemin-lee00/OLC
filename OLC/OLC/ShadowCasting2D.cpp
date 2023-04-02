// ShadowCasting2D.cpp : This file contains the 'main' function. Programe execution begins and ends there.
//

#include <iostream>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

struct sEdge
{
	float sx, sy;
	float ex, ey;
};

struct sCell
{
	int edge_id[4];
	bool edge_exist[4];
	bool exist = false;
};

#define NORTH	0
#define SOUTH	1
#define EAST	2
#define WEST	3



class ShadowCasting2D : public olc::PixelGameEngine{
	
public :
	ShadowCasting2D() {
		sAppName = "ShadowCasting2D";
	}

private :
	//Manages most variables associated with the map
	//The composition of the tile is determined by the point struct
	//the int data type gives the datat volatility.
	sCell* world;
	int nWorldWidth = 40;
	int nWorldHeight = 30;

	std::vector<sEdge> vecEdges;

	void ConvertTileMapToPolyMap(int sx, int sy, int w, int h, float fBlockWidth, int pitch) {
		//Clear "PloyMap"
		vecEdges.clear();

		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				for (int j = 0; j < 4; j++) {
					
					//Clear each node`s edge of the map
					world[(y + sy) * pitch + (x + sx)].edge_exist[j] = false;
					world[(y + sy) * pitch + (x + sx)].edge_id[j] = 0; 
				}
			// Iterate through region from top left to bottom right
			//  Connet the edges of each block
			for (int x = 1; x < w - 1; x++) {
				for (int y = 1; y < h - 1; y++) {
						
					//Create some convenient indices
					int i = (y + sy) * pitch + (x + sx);		//This
					int n = (y + sy - 1) * pitch + (x + sx);	//Northern Neighbour
					int s = (y + sy + 1) * pitch + (x + sx);	//Southern Neighbour
					int w = (y + sy) * pitch + (x + sx - 1);	//Western Neighbour
					int e = (y + sy) * pitch + (x + sx + 1);	//Eastern Neighbour
					
					//If this cell exists, check if it needs edges
					if (world[i].exist) {
					
						//If this cell has no western neighbour, It needs a wersten edge
						if (!world[w].exist) {
							
							//It can either extend it from its northern neighbour if they have
							//one , or It can start a new one.
							if (world[n].edge_exist[WEST]) {
								
								//Northern neighbour has a western edge, so grow it downwards
								vecEdges[world[n].edge_id[WEST]].ey += fBlockWidth;
								world[i].edge_id[WEST] = world[n].edge_id[WEST];
								world[i].edge_exist[WEST] = true;
							}
							else{

								//Northern neighbour does not have one, so create one
								sEdge edge;
								edge.sx = (sx + x) * fBlockWidth;
								edge.sy = (sy + y) * fBlockWidth;

								edge.ex = edge.sx;
								edge.ey = edge.sy + fBlockWidth;

								// Add edge to Polygon Pool
								int edge_id = vecEdges.size();
								vecEdges.push_back(edge);

								// Update tile information with edge information
								world[i].edge_id[WEST] = edge_id;
								world[i].edge_exist[WEST] = true;
							}
						}

						//If this cell has no eastern neighbour, It needs a eastern edge
						if (!world[e].exist) {

							//It can either extend it from its eastern neighbour if they have
							//one , or It can start a new one.
							if (world[n].edge_exist[EAST]) {

								//Northern neighbour has a western edge, so grow it downwards
								vecEdges[world[n].edge_id[EAST]].ey += fBlockWidth;
								world[i].edge_id[EAST] = world[n].edge_id[EAST];
								world[i].edge_exist[EAST] = true;
							}
							else {

								//Eastern neighbour does not have one, so create one
								sEdge edge;
								edge.sx = (sx + x + 1) * fBlockWidth;
								edge.sy = (sy + y) * fBlockWidth;

								edge.ex = edge.sx;
								edge.ey = edge.sy + fBlockWidth;

								// Add edge to Polygon Pool
								int edge_id = vecEdges.size();
								vecEdges.push_back(edge);

								// Update tile information with edge information
								world[i].edge_id[EAST] = edge_id;
								world[i].edge_exist[EAST] = true;
							}
						}

						//If this cell has no nothern neighbour, It needs a northern edge
						if (!world[n].exist) {

							//It can either extend it from its western neighbour if they have
							//one , or It can start a new one.
							if (world[w].edge_exist[NORTH]) {

								//Western neighbour has a western edge, so grow it eastwards
								vecEdges[world[w].edge_id[NORTH]].ex += fBlockWidth;
								world[i].edge_id[NORTH] = world[w].edge_id[NORTH];
								world[i].edge_exist[NORTH] = true;
							}
							else {

								//Western neighbour does not have one, so create one
								sEdge edge;
								edge.sx = (sx + x) * fBlockWidth;
								edge.sy = (sy + y) * fBlockWidth;

								edge.ex = edge.sx + fBlockWidth;
								edge.ey = edge.sy;

								// Add edge to Polygon Pool
								int edge_id = vecEdges.size();
								vecEdges.push_back(edge);

								// Update tile information with edge information
								world[i].edge_id[NORTH] = edge_id;
								world[i].edge_exist[NORTH] = true;
							}
						}

						//If this cell has no southern neighbour, It needs a southern edge
						if (!world[s].exist) {

							//It can either extend it from its western neighbour if they have
							//one , or It can start a new one.
							if (world[w].edge_exist[SOUTH]) {

								//Western neighbour has a western edge, so grow it eastwards
								vecEdges[world[w].edge_id[SOUTH]].ex += fBlockWidth;
								world[i].edge_id[SOUTH] = world[w].edge_id[SOUTH];
								world[i].edge_exist[SOUTH] = true;
							}
							else {

								//Western neighbour does not have one, so create one
								sEdge edge;
								edge.sx = (sx + x) * fBlockWidth;
								edge.sy = (sy + y + 1) * fBlockWidth;

								edge.ex = edge.sx + fBlockWidth;
								edge.ey = edge.sy;

								// Add edge to Polygon Pool
								int edge_id = vecEdges.size();
								vecEdges.push_back(edge);

								// Update tile information with edge information
								world[i].edge_id[SOUTH] = edge_id;
								world[i].edge_exist[SOUTH] = true;
							}
						}

					}
				}
			}

			}
		}
	}
public :

	//Involved in the intial creation of a map or object.
	bool OnUserCreate() override {
		world = new sCell[nWorldWidth * nWorldHeight];
		return true;
	}

	//After the program is created, it is involved in overall operation
	bool OnUserUpdate(float fElapsedTime) override {
		float fBlockWidth = 16.0f;
		float fSourceX = GetMouseX();
		float fSourceY = GetMouseY();

		//Set tile mpa block to on or off
		if (GetMouse(0).bReleased) {

			// i = y * witdh + x;
			int i = ((int)fSourceY / (int)fBlockWidth) * nWorldWidth + ((int)fSourceX / (int)fBlockWidth);
			world[i].exist = !world[i].exist;
		}

		//Drawing

		Clear(olc::BLACK);

		//Draw Blocks from TileMap
		for (int x = 0; x < nWorldWidth; x++) {
			for (int y = 0; y < nWorldHeight; y++) {

				// i = y * witdh + x;
				if (world[y * nWorldWidth + x].exist)
					FillRect(x * fBlockWidth, y * fBlockWidth, fBlockWidth, fBlockWidth, olc::BLUE);
			}
		}

		return true;
	}
};

int main()
{
	ShadowCasting2D demo;

	if (demo.Construct(640, 480, 2, 2))
		demo.Start();

}
