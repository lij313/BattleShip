#ifdef __APPLE__
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
#include <iostream>
#include <ctime>
#include <cmath>
#include <sstream>
#include "Ship.h"
#include "MathLib.h"

#define pi 3.1415926

Ship::Ship(std::string name, double MAX_HEALTH, double maxSpeed, TerrainGenerate islandMap){
	this->shipName = name;
	this->MAX_HEALTH = MAX_HEALTH;
	this->currentHealth = MAX_HEALTH;
	this->MAX_SPEED = maxSpeed;
	this->currentSpeed=0.0;
	this->telegraphPos=0.0;
	this->currentPos = Point3D(5,0,10);
	this->islandMap=islandMap;
	this->cameraRelativeAngle=0;
	int digit = 0;
	for(int i = 1; i <= 130; i++){
		stringstream streamText;
		streamText<<i;
		string text;
		streamText>>text;
		digit=text.length();

		string gunAPath = "Models/Z-39/GunAobj_000000";
		string gunXYPath = "Models/Z-39/GunXY_000000";

		gunAPath = gunAPath.substr(0, gunAPath.length()-digit);
		gunAPath+=text;
		gunXYPath = gunXYPath.substr(0, gunXYPath.length()-digit);
		gunXYPath+=text;

		this->GunFront.push_back(new ObjLoader(shipName, gunAPath, 0.1));
		this->GunRear.push_back(new ObjLoader(shipName, gunXYPath, 0.1));
	}
	this->shipHull = new ObjLoader(shipName, "", 0.1);
	this->isSunk = false;
	this->waterLevel=0;
	this->rollingAngle=0;
	this->gunAAngle=65;
	this->gunXYAngle=65;
	this->frontX=0;
	this->frontZ=0;
	this->beastMode=1;
}

std::string Ship::getName(){
	return shipName;
}

void Ship::drawShip(){

	if(isSunk){
		if(waterLevel >= -5)
			waterLevel-=0.005f;
		if(rollingAngle <= 180)
			rollingAngle+=0.3;
		glPushMatrix();
			glTranslatef(currentPos.getX(), waterLevel, currentPos.getZ());
			glRotatef(currentPos.getY(),0,1,0);
			glRotatef(rollingAngle, 0, 0, 1);
			if(shipName=="Z-39"){
				glRotatef(-90,0,1,0);
			}
			shipHull->Draw();
			GunFront[gunAAngle]->Draw();
			GunRear[gunXYAngle]->Draw();
		glPopMatrix();
		return;
	}

	glPushMatrix();
		glTranslatef(currentPos.getX(), 0, currentPos.getZ());
		glRotatef(currentPos.getY(),0,1,0);
		glRotatef(rollingAngle, 0, 0, 1);
		if(shipName=="Z-39"){
			glRotatef(-90,0,1,0);
		}
		
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		shipHull->Draw();
		GunFront[gunAAngle]->Draw();
		GunRear[gunXYAngle]->Draw();
	glPopMatrix();



	for(int i = 0; i < 4; i++){
		m_bullet[i].Draw();
	}
}

void Ship::setTelegraphPos(double position){
	if(isSunk)
		return;
	this->telegraphPos = position;
	std::cout << "Current telegraph: " << telegraphPos << std::endl;
}

double Ship::getTelegraphPos(){
	return telegraphPos;
}

Point3D Ship::getCurrentPos(){
	return currentPos;
}

void Ship::calcCurrentPos(){
	if(isSunk)
		return;

	double goalSpeed=telegraphPos*MAX_SPEED;

	if(currentSpeed < goalSpeed)
		currentSpeed+=0.1f * beastMode;
	else if(currentSpeed > goalSpeed)
		currentSpeed-=0.1f * beastMode;
	else
		currentSpeed=goalSpeed;

	double distance = currentSpeed*((17)/1000.0f);
	double radians = (currentPos.getY()*pi)/180;

	double dx = distance*sin(radians);
	double dz = distance*cos(radians);

	double magnitude = 0;
	double scale = 0;
	if(currentSpeed != 0){
		magnitude = sqrt(dx*dx+dz*dz);
		scale = 1/magnitude;
	}

	double unitDx = dx*scale;
	double unitDz = dz*scale;

	frontX = currentPos.getX()+unitDx*7;
	frontZ = currentPos.getZ()+unitDz*7;

	double height = islandMap.getHeight((int)frontX, (int)frontZ);

	if(frontX < 0 || frontX > 300)
		currentSpeed=0;
	if(frontZ < 0 || frontZ > 300)
		currentSpeed=0;
	if(rollingAngle > 12)
		currentSpeed=0;


	if(beastMode > 1.5){
		currentHealth-=1;
	}
	checkCollision(height);

	if(currentSpeed!=0)
		currentPos = Point3D(currentPos.getX()+dx, currentPos.getY(), currentPos.getZ()+dz);
	if(rollingAngle>=0)
		rollingAngle-=0.1;
	else
		rollingAngle+=0.1;
}

void Ship::checkCollision(double height){

	if(height > -0.3){
		double oldHealth = currentHealth;
		currentHealth-=currentSpeed*currentSpeed;
		std::cout << "Current health: " << currentHealth << " (" << oldHealth << "-" << currentSpeed*currentSpeed << ")" << ": you hit a rock." <<std::endl;
		
		checkHealth();
		currentSpeed=0;
	}
}

bool Ship::checkObstacleCollision(Obstacles _obs){
	Point2D shipVec2D = Point2D(frontX,frontZ);
	Point2D obsPos2D = Point2D(_obs.pos.mX, _obs.pos.mZ);
	if(shipVec2D.distanceTo(obsPos2D) <= 1.5 && _obs.IsShow){
		double oldHealth = currentHealth;
		currentSpeed-= 0.8*(currentSpeed);
		currentHealth+=200;
		std::cout << "Current health: " << currentHealth << " (" << oldHealth << "+" << 200 << ")" << ": you hit an obstacle." <<std::endl;
		return true;
	}
	return false;
}

void Ship::checkHealth(){
	if(currentHealth <= 0){
		isSunk=true;
		std::cout << "You are destroyed." <<std::endl;
	}
}

double Ship::setRudderAngle(double angle){
	if(currentSpeed!=0){
		currentPos=Point3D(currentPos.getX(), currentPos.getY()+angle*currentSpeed*0.1, currentPos.getZ());
		if(abs(rollingAngle) <= 10)
			rollingAngle+=angle*0.1*abs(currentSpeed);
	}
	return 0;
}

void Ship::setRelativeAngle(double angle){

	if(isSunk)
		return;

	cameraRelativeAngle = fmod(angle+360,360);

	int relativeGunXYAngle = cameraRelativeAngle+180;
	int relativeGunAAngle = cameraRelativeAngle;

	relativeGunAAngle = (relativeGunAAngle)%360;
	relativeGunXYAngle = (relativeGunXYAngle)%360;

	if(relativeGunAAngle>309)
		relativeGunAAngle=309;
	if(relativeGunAAngle<50)
		relativeGunAAngle=50;
	if(relativeGunXYAngle>309)
		relativeGunXYAngle=309;
	if(relativeGunXYAngle<50)
		relativeGunXYAngle=50;

	gunXYAngle = (relativeGunXYAngle-50)/2;
	gunAAngle = (relativeGunAAngle-50)/2;
}

void Ship::fire(Point2D targetPos){


	Point3D endpos;//目标值

	double distance = targetPos.distanceTo(Point2D(currentPos.getX(), currentPos.getZ()));
	double dispersion = sqrt(distance);

	for(int i = 0; i < 4; i++){
		double x = fmod(std::rand(),dispersion);
		double z = fmod(std::rand(),dispersion);

		x-=dispersion/2;
		x+= + targetPos.getX();
		z-=dispersion/2;
		z+= + targetPos.getY();

		endpos.setPos(x, 1, z);
		m_bullet[i].Shoot(currentPos, endpos);//发射
	}

}
bool Ship::CheckFoot(Obstacles _ob)
{
	for(int i = 0; i < 4; i++){
		if(m_bullet[i].Collision(_ob)){
			islandMap.setHeight(m_bullet[i].endPos.mX, m_bullet[i].endPos.mZ, -1);
			return true;
		}
	}
    return false;
}


bullet::bullet()
{
	IsLive = false;

	m_speed = 1.5;
}

void bullet::Shoot(Point3D _beginPos, Point3D _endPos)
{
	if (IsLive)
		return;

	beginPos   = _beginPos;
	currentPos = Point3D(beginPos.mX, 0, beginPos.mZ);
	endPos     = _endPos;

	moveDir.setPos(endPos.mX - beginPos.mX, 0, endPos.mZ - beginPos.mZ);
	moveDir = moveDir.normalize();//水平方向
	moveDir3D.setPos(0,1,0);//三维方向
    //计算摄像机头部朝向
	{
	moveUp3D = moveDir.across(moveDir3D);
	moveUp3D = moveDir.across(moveUp3D);
	}
	IsLive = true;
}

void bullet::Draw()
{

	if (!IsLive)
		return;

	//刷新位置
	currentPos.mX += moveDir.mX * m_speed;
	currentPos.mZ += moveDir.mZ * m_speed;
	if (currentPos.distanceTo(endPos)*2.0 > beginPos.distanceTo(endPos))//上升阶段
	{
		   currentPos.mY += m_speed;

			//计算三维方向--》用来设置相机的朝向
        	moveDir3D.setPos(moveDir.mX,m_speed,moveDir.mZ);
        	moveDir3D = moveDir3D.normalize();//三维方向
	}
	else
	{

		currentPos.mY -= m_speed;

		//计算三维方向--》用来设置相机的朝向
         moveDir3D.setPos(moveDir.mX,-m_speed,moveDir.mZ);
         moveDir3D = moveDir3D.normalize();//三维方向
		if (currentPos.mY < 0.2)//掉入海面
			IsLive = false;
	}
     //计算摄像机头部朝向
      {
      moveUp3D = moveDir.across(moveDir3D);
      moveUp3D = moveDir.across(moveUp3D);
      }

	//绘制子弹
	{
		glPushMatrix();
		float amb[3]={0.5,0,0};
		float diff[3]={1,0,0};
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
		glTranslatef(currentPos.getX(), currentPos.getY(), currentPos.getZ());
		glutSolidSphere(0.3, 50, 50);
		glPopMatrix();
	}
	

}


bool bullet::Collision(Obstacles _ob)//是否碰撞
{
    //子弹是否运行 
	if (!IsLive)
		return false;
	//障碍物是否存在
	if (!_ob.IsShow)
		return false;

	//计算子弹和障碍物的距离
	float dis = sqrt( (_ob.pos.mX - currentPos.mX)*(_ob.pos.mX - currentPos.mX) + (_ob.pos.mY - currentPos.mY)*(_ob.pos.mY - currentPos.mY) + (_ob.pos.mZ - currentPos.mZ)*(_ob.pos.mZ - currentPos.mZ) );
	if (dis < 2.0)
	{
		IsLive = false;
		return true;
	}
	
	return false;
}