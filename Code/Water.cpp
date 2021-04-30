#ifdef __APPLE__
#  define GL_SILENCE_DEPRECATION
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <time.h>
#include "Water.h"
#include "MathLib.h"
#include "TerrainGenerate.h"


const double pi = 3.14;
double waterHeight;

/*time_t timer;
struct tm y2k = {0};
time(&timer); 
double myTime= difftime(timer,mktime(&y2k));
**/
double myTime = 5;

int numWaves;
double amplitude[8]={1,1,1,1,1,1,1,2};
double wavelength[8]={10,5,10,5,10,5,10,5};
double speed[8]={5,5,5,5,5,5,5,5};
Point2D directionP = Point2D(10,10);
Point2D direction[8] ={directionP,directionP,directionP,directionP,
directionP,directionP,directionP,directionP};


double Water::wave(int i, double x, double y) {
    double frequency = 2*pi/wavelength[i];
    double phase = speed[i] * frequency;
    double theta = Point2D::dot(direction[i], Point2D(x, y));
    return amplitude[i] * sin(theta * frequency + myTime * phase);
}

double Water::waveHeight(double x, double y) {
    float height = 0.0;
    for (int i = 0; i < numWaves; ++i)
        height += wave(i, x, y);
    return height;
}
double Water::dWavedx(int i, double x, double y) {
    double frequency = 2*pi/wavelength[i];
    double phase = speed[i] * frequency;
    double theta = Point2D::dot(direction[i], Point2D(x, y));
    double A = amplitude[i] * direction[i].getX() * frequency;
    return A * cos(theta * frequency + myTime * phase);
}

double Water::dWavedy(int i, double x, double y) {
    double frequency = 2*pi/wavelength[i];
    double phase = speed[i] * frequency;
    double theta = Point2D::dot(direction[i], Point2D(x, y));
    double A = amplitude[i] * direction[i].getY() * frequency;
    return A * cos(theta * frequency + myTime * phase);
}

Point3D Water::waveNormal(double x, double y) {
    double dx = 0.0;
    double dy = 0.0;
    for (int i = 0; i < numWaves; ++i) {
        dx += dWavedx(i, x, y);
        dy += dWavedy(i, x, y);
    }
    Point3D n = Point3D(-dx, -dy, 1.0);
    return n.normalize();
}

Water::Water(){
    TerrainGenerate stableWater = TerrainGenerate();
    //waveHeight();
    //waveNormal();
}