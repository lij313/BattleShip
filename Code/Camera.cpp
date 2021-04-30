#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
#include "Camera.h"
#include <ctime>
#include <cmath>
#include <iostream>
#define pi 3.1415926

Camera::Camera(Ship *ship){
    this->ship=ship;
    this->distance = 20;
    this->angleAround = 0;
    this->pitch = 20;
}

float Camera::calcHorizontalDistance(){
    float radians = (pitch*pi)/180;
    return distance*cos(radians);
}

float Camera::calcVerticalDistance(){
    float radians = (pitch*pi)/180;
    return distance*sin(radians);
}

void Camera::move(int dir, int x, int y){
    calculateZoom(dir, x, y);
    calculatePitch(x, y);
    calcAngleAround(x, y);
    float horizontalDistance = calcHorizontalDistance();
    float verticalDistance = calcVerticalDistance();
    calcCameraPos(horizontalDistance, verticalDistance);
    lastX=x;
    lastY=y;
}

void Camera::calculateZoom(int dir, int x, int y){
    float zoomLevel = dir;
    distance-=zoomLevel*0.3f;
    if(distance < 10)
        distance=10;
    if(distance > 200)
        distance = 200;
}

void Camera::calculatePitch(int x, int y){
    int dy=y-lastY;
    float deltaPitch = dy*0.3;
    pitch += deltaPitch;
}

void Camera::calcAngleAround(int x, int y){
    int dx=x-lastX;
    float angleChange = dx*0.3;
    angleAround-=angleChange;
    angleAround=std::fmod(angleAround,360);
}

void Camera::calcCameraPos(float horizontalDistance, float verticalDistance){
    float theta = ship->getCurrentPos().getY() + angleAround;
    float radians = (theta*pi)/180;
    float offsetX = horizontalDistance*sin(radians);
    float offsetZ = horizontalDistance*cos(radians);

    float x = ship->getCurrentPos().getX() - offsetX;
    float z = ship->getCurrentPos().getZ() - offsetZ;
    if(verticalDistance < 0)
        verticalDistance = 1;
    position = Point3D(x, verticalDistance, z);
}