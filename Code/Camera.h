#ifndef CAMERAHPP
#define CAMERAHPP

#include <cmath>
#include <map>
#include "MathLib.h"
#include "Ship.h"

class Camera {

    public :
        Camera(Ship *ship);
        Camera() {}
        void move(int dir, int x, int y);
        float getAngleAround(){return angleAround;};
        Point3D getPosition(){return position;};

    private :
        Ship *ship;
        Point3D position;
        int lastX;
        int lastY;
        float distance;
        float angleAround;
        float pitch;

        void calculateZoom(int dir, int x, int y);
        void calculatePitch(int x, int y);
        void calcAngleAround(int x, int y);
        void calcCameraPos(float horizontalDistance, float verticalDistance);
        float calcHorizontalDistance();
        float calcVerticalDistance();

};
                                                                                 
#endif
