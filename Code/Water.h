//#ifndef TERRAIN_GENERATE_H
//#define TERRAIN_GENERATE_H
#include "MathLib.h"
//should be similar to A3, TODO
class Water{
public:
	Water();

private:
	double wave(int i, double x, double y);
	double waveHeight(double x, double y);
	double dWavedx(int i, double x, double y);
	double dWavedy(int i, double x, double y);
	Point3D waveNormal(double x, double y);
	
};
//#endif