#ifndef SHIP_H
#define SHIP_H

#include <string.h>
#include "MathLib.h"
#include "ObjLoader.h"
#include "TerrainGenerate.h"
#include <cstdlib>

struct Obstacles   //障碍物
{
	Point3D  pos;
	bool     IsShow;
	void Draw()
	{
		if (IsShow) {
			glColor3f(1.0, 0.0, 0);
			glPushMatrix();
			glTranslatef(pos.mX,0.5, pos.mZ);
			glutSolidCube(1.0);
			glPopMatrix();
		}
	};
};


//子弹
class bullet 
{
public:
	bullet();
	Point3D currentPos;
	Point3D beginPos;
	Point3D endPos;
	Point3D moveDir;//运动方向
	Point3D moveDir3D;//运动方向
	Point3D moveUp3D; //运动垂直方向--摄像机头朝上的方向
	bool    IsLive;//是否发射中
	void    Draw();//绘制
	void    Shoot(Point3D _beginPos, Point3D _endPos);//发射
	float   m_speed;//速度
	bool    Collision(Obstacles _ob);//是否碰撞
};
class Ship{
public:
	Ship(std::string name, double MAX_HEALTH, double maxSpeed, TerrainGenerate islandMap);
	std::string getName();
	void drawShip();
	double getMaxHealth();
	double getCurrentHealth(){return currentHealth;};
	double getMaxSpeed();
	double getCurrentSpeed(){return currentSpeed;};
	double setRudderAngle(double angle);
	double getTelegraphPos();
	Point3D getCurrentPos();
	bool isDestroyed(){return isSunk;};
	void setTelegraphPos(double position);
	void calcCurrentPos();
	void checkCollision(double height);
	void restoreHealth(){currentHealth=MAX_HEALTH;isSunk=false;waterLevel=0;};
	void setRelativeAngle(double angle);
	void fire(Point2D targetPos);
	void beastModeOn(){beastMode = 2;};
	void beastModeOff(){beastMode = 1;};
	bool CheckFoot(Obstacles _ob);
	bool checkObstacleCollision(Obstacles _obs);
	bullet getBullet(int i){return m_bullet[i];}
	bullet m_bullet[4];
private:
	std::string shipName;
	double MAX_HEALTH;
	double currentHealth;
	double MAX_SPEED;
	double currentSpeed;
	double telegraphPos;
	double waterLevel;
	double rollingAngle;
	double frontX;
	double frontZ;
	double beastMode;
	int cameraRelativeAngle;
	int gunAAngle;
	int gunXYAngle;
	Point3D currentPos;
	TerrainGenerate islandMap;
	ObjLoader *shipHull;
	std::vector<ObjLoader*> GunFront;
	std::vector<ObjLoader*> GunRear;
	bool isSunk;
	void checkHealth();
	double calcCurrentRudder();
	double updateCurrentPos(unsigned char key);
};
#endif