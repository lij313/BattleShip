#ifndef TERRAIN_GENERATE_H
#define TERRAIN_GENERATE_H

//should be similar to A3, TODO
class TerrainGenerate{
public:
	TerrainGenerate();
	TerrainGenerate(int repeats);
	void drawTerrain(float r, float g, float b);
	void drawWater(float r, float g, float b);
	void setHeight(int x, int z, double dY){terrainMapY[x][z]+=dY;};
	double getHeight(int x, int z){return terrainMapY[x][z];}

private:
	double terrainMapXZ[300][300][2];
	double terrainMapY[300][300];
	float normal[300][300][3];
	void initTerrainMapArray();
	void circleAlgorithm(int repeats);
	void eliminateUneligablePoints(double maxHeight);
	void calculateNormal();
};
#endif