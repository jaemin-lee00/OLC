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

	std::vector<std::tuple<float, float, float>> vecVisibilityPolygonPoints;

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
			}
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

	void CalculateVisibilityPolygon(float ox, float oy, float radius) {
		//	Get rid of existing polygon
		vecVisibilityPolygonPoints.clear();

		// For each edge in Polygon
		for (auto& e1 : vecEdges) {
			// Take the start point, then the end point (we could use a
			// non-duplicated points here, it would be more optiaml)
			for (int i = 0; i < 2; i++) {
				
				float rdx, rdy;
				rdx = (i == 0 ? e1.sx : e1.ex) - ox;
				rdy = (i == 0 ? e1.sy : e1.ey) - oy;
			
				float base_ang = atan2f(rdy, rdx);

				float ang = 0;
				// For each point, cast 3 rays, 1 directly at point
				// and 1 a little bit either side
				for (int j = 0; j < 3; j++) {
				
					if (j == 0) ang = base_ang - 0.0001f;
					if (j == 1) ang = base_ang;
					if (j == 2) ang = base_ang + 0.0001f;

					//Create ray along angle for required distance
					rdx = radius * cosf(ang);
					rdy = radius * sinf(ang);

					float min_t1 = INFINITY;
					float min_px = 0, min_py = 0, min_ang = 0;
					bool bValid = false;

					// Check for ray intersection with all edges
					for (auto& e2 : vecEdges) {
						//Create line segment vector
						float sdx = e2.ex - e2.sx;
						float sdy = e2.ey - e2.sy;

						if (fabs(sdx - rdx) > 0.0f && fabs(sdy - rdy) > 0.0f) {
							// t2 is normalize distance from line segment start to line segment end of intersect point
							float t2 = (rdx * (e2.sy - oy) + (rdy * (ox - e2.sx))) / (sdx * rdy - sdy * rdx);
							// t1 is normalize distance from source along ray to ray length of intersect point
							float t1 = (e2.sx + sdx * t2 - ox) / rdx;
							
							// If intersect point exist along ray, and along line
							// segment then interscet point is valid
							if (t1 > 0 && t2 >= 0 && t2 <= 1.0f) {
								
								// Check if this intersect point is closset to source.
								// If it is, then store this point and reject others
								if (t1 < min_t1) {
									
									min_t1 = t1;
									min_px = ox + rdx * t1;
									min_py = oy + rdy * t1;
									min_ang = atan2f(min_py - oy, min_px - ox);
									bValid = true;
								}
							}
						}
					}

					// Add intersection point to visibility polygon perimeter
					if(bValid){
						vecVisibilityPolygonPoints.push_back({ min_ang, min_px, min_py});
					}
					
				}
			}
		}

		// Sort perimeter points by angle from source. This will allow
		// us to draw a triangle fan
		std::sort(
			vecVisibilityPolygonPoints.begin(),
			vecVisibilityPolygonPoints.end(),
			[&](const std::tuple<float, float, float>& t1, const std::tuple<float, float, float>& t2) {

				return std::get<0>(t1) < std::get<0>(t2);
			});
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

		// Take a region of "TileMap" and convert it to "PolyMap" - This is done
		// every frame here, but could be a pre-preocessing stage depending on
		// how your final application interacts with tilemaps

		ConvertTileMapToPolyMap(0, 0, 40, 30, fBlockWidth, nWorldWidth);

		if (GetMouse(1).bHeld) {
			CalculateVisibilityPolygon(fSourceX, fSourceY, 1000.0f);
		}

		//Drawing

		Clear(olc::BLACK);

		int nRaysCast = vecVisibilityPolygonPoints.size();
		DrawString(4, 4, "Rays Cast: " + to_string(nRayCast));
		
		if (GetMouse(1).bHeld && vecVisibilityPolygonPoints.size() > 1) {
			
			//Draw each triangle in fan
			for (int i = 0; i < vecVisibilityPolygonPoints.size() - 1; i++) {
				
				FillTriangle(
					fSourceX,
					fSourceY,

					std::get<1>(vecVisibilityPolygonPoints[i]),
					std::get<2>(vecVisibilityPolygonPoints[i]),

					std::get<1>(vecVisibilityPolygonPoints[i + 1]),
					std::get<2>(vecVisibilityPolygonPoints[i + 1]));
			}


			FillTriangle(
				fSourceX,
				fSourceY,

				std::get<1>(vecVisibilityPolygonPoints[vecVisibilityPolygonPoints.size() - 1]),
				std::get<2>(vecVisibilityPolygonPoints[vecVisibilityPolygonPoints.size() - 1]),

				std::get<1>(vecVisibilityPolygonPoints[0]),
				std::get<2>(vecVisibilityPolygonPoints[0]));
		}

		//Draw Blocks from TileMap
		for (int x = 0; x < nWorldWidth; x++) {
			for (int y = 0; y < nWorldHeight; y++) {

				// i = y * witdh + x;
				if (world[y * nWorldWidth + x].exist)
					FillRect(x * fBlockWidth, y * fBlockWidth, fBlockWidth, fBlockWidth, olc::BLUE);
			}
		}

		//Draw Edges from PolyMap
		for (auto& e : vecEdges) {
			
			DrawLine(e.sx, e.sy, e.ex, e.ey);
			FillCircle(e.sx, e.sy, 3, olc::RED);
			FillCircle(e.ex, e.ey, 3, olc::RED);
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
