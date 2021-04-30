#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <math.h>
#include "MathLib.h"

Point2D::Point2D(){
	this->mX = 0;
	this->mY = 0;
}

Point2D::Point2D(double inX, double inY){
	this->mX = inX;
	this->mY = inY;
	
}

void Point2D::setPos(double inX, double inY){
	this->mX = inX;
	this->mY = inY;
}

double Point2D::dot(Point2D a, Point2D b){
	double product =  a.getX()*b.getX() + a.getY()*b.getY();
    return product; 
}

double Point2D::getX(){
	return mX;
}

double Point2D::getY(){
	return mY;
}

double Point2D::distanceTo(Point2D other)
{
	return sqrt((other.mX - this->mX) * (other.mX - this->mX) + (other.mY - this->mY) * (other.mY - this->mY));
}


Point3D::Point3D(){
	this->mX = 0;
	this->mY = 0;
	this->mZ = 0;
}
double Point3D::distanceTo(Point3D other)
{
	return sqrt((other.mX - mX) * (other.mX - mX) + (other.mZ - mZ) * (other.mZ - mZ));
}
double Point3D::fastDistanceTo(Point3D other)
{
    return sqrt((other.mX - mX) * (other.mX - mX)+(other.mY - mY) * (other.mY - mY) + (other.mZ - mZ) * (other.mZ - mZ));
}
Point3D::Point3D(double inX, double inY, double inZ){
	this->mX = inX;
	this->mY = inY;
	this->mZ = inZ;
}

void Point3D::setPos(double inX, double inY, double inZ){
	this->mX = inX;
	this->mY = inY;
	this->mZ = inZ;
}

double Point3D::getX(){
	return mX;
}

double Point3D::getY(){
	return mY;
}

double Point3D::getZ(){
	return mZ;
} 

Point3D Point3D::normalize(){
	double x = mX;
	double y = mY;
	double z = mZ;
	double l = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
	return Point3D(x/l,y/l,z/l);
}

 Point3D Point3D::across(Point3D other)
 {
     double x =  mY* other.mZ - mZ* other.mY;
     double y =  mZ* other.mX - mX* other.mZ;
     double z =  mX* other.mY - mY* other.mX;
     return Point3D(x,y,z);
 }
