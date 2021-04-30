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
#include <cstdlib>
#include <cmath>
#include <vector>
#include <sstream>
#include "MathLib.h"
#include "Ship.h"
#include "TerrainGenerate.h"
#include "Water.h"
#include "Camera.h"
#include<string>
#include<sstream>	

GLdouble eye[] = { -10, 5, -10 };
GLdouble lookAt[] = { 250, 0, 250 };
GLdouble up[] = { 0, 1, 0 };

GLdouble snipeEye[] = { 0, 200, 0 };
GLdouble snipeLookAt[] = { 0, 0, 0 };

double radius = 20.0;
double mouse_x;
double mouse_y;
double theta;
double phi;

float light_pos[4] = {50,10,250,1};
float amb[4] = {0.5,0.5,0.5,1};
float diff[4] = {0.5,0.5,0.5,1};
float spec[4] = {0.5,0.5,0.5,1};

int lastX;
int lastY;

bool snipeMode = false;
bool keys[256];
bool specialKeys[256];
bool rightButtonPressed = false;
bool waterDepthMode = false;
bool gameWon = false;
float m_control[3] = {5,0,10};//------------
Obstacles m_Obstacles[3];//障碍物
float  mTime = 10*60; //Time allows
TerrainGenerate stableWater = TerrainGenerate();
TerrainGenerate islandMap = TerrainGenerate(450);

//string shipName = "YamatoA";
string shipName = "Z-39";

//Ship shipZ39 = Ship(shipName, 100, 27.5);
Ship *shipZ39 = new Ship(shipName, 1000, 35.4, islandMap);
Camera shipCamera = Camera(shipZ39);
int   mBulletEye = 1;
float mBulletEyePos[9];
//string shipName2 = "mba1";
//Ship sampleOBJ = Ship(shipName2, 100, 0, 0, 0, 0, 0, 0);
void DoRasterString(float x, float y, float z,const char* s)
{
	glRasterPos3f((GLfloat)x, (GLfloat)y, (GLfloat)z);

	char c;			// one character to print
	for (; (c = *s) != '\0'; s++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
}
/*
void reshape(int w, int h){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (double)h/(double)w, 0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glTranslated(0,0,-200);
	//glRotated(45, 1,1,1);
}
*/

//not working
void mouse(int button, int state, int x, int y) {
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	y = height - y;
	switch (button) {
	case GLUT_LEFT_BUTTON:

		break;
	case GLUT_RIGHT_BUTTON:
		shipCamera.move(0, x, y);
		rightButtonPressed = !rightButtonPressed;
		lastX = x;
		lastY = y;
		break;
	case GLUT_MIDDLE_BUTTON:

		break;
	case 3:  //mouse wheel scrolls
		shipCamera.move(1, x, y);
		break;
	case 4:
		shipCamera.move(-1, x, y);
		break;
		glutPostRedisplay();
	}
}

void motion(int x, int y) {
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	y = height - y;
	if (rightButtonPressed)
		shipCamera.move(0, x, y);
	lastX = x;
	lastY = y;
	glutPostRedisplay();
}

void keypressCheck() {

	double currentTelegraphPos = shipZ39->getTelegraphPos();

	if (keys[(int)'q'] == 1 || keys[27] == 1)
		exit(0);

	if (keys[(int)'a'] == 1 || keys[(int)'A'] == 1)
		shipZ39->setRudderAngle(0.5f);

	if (keys[(int)'d'] == 1 || keys[(int)'D'] == 1)
		shipZ39->setRudderAngle(-0.5f);

	if (keys[(int)'r'] == 1) {
		shipZ39->restoreHealth();
	}
	
	
if (keys[(int)'i'] == 1) {
		m_control[1] += 0.1;
	}

if (keys[(int)'k'] == 1) {
		m_control[1] -= 0.1;
	}
	if (keys[(int)'j'] == 1) {
		m_control[0] -= 0.1;
	}
	if (keys[(int)'l'] == 1) {
		m_control[0] += 0.1;
	}
	if (keys[(int)'u'] == 1) {
		m_control[2] += 0.1;
	}if (keys[(int)'o'] == 1) {
		m_control[2] -= 0.1;
	}
	
}

void specialCheck() {
	if (specialKeys[GLUT_KEY_DOWN] == 1) {
		if (snipeMode) {
			if (snipeEye[0] > 0) {
				snipeEye[0] -= 1.0f;
				snipeLookAt[0] -= 1.0f;
			}
		}
		else
			eye[0] -= 0.1f;
	}
	if (specialKeys[GLUT_KEY_UP] == 1) {
		if (snipeMode) {
			if (snipeEye[0] < 300) {
				snipeEye[0] += 1.0f;
				snipeLookAt[0] += 1.0f;
			}
		}
		else
			eye[0] += 0.1f;
	}
	if (specialKeys[GLUT_KEY_LEFT] == 1) {
		if (snipeMode) {
			if (snipeEye[2] > 0) {
				snipeEye[2] -= 1.0f;
				snipeLookAt[2] -= 1.0f;
			}
		}
		else
			eye[2] -= 0.1f;
	}
	if (specialKeys[GLUT_KEY_RIGHT] == 1) {
		if (snipeMode) {
			if (snipeEye[2] < 300) {
				snipeEye[2] += 1.0f;
				snipeLookAt[2] += 1.0f;
			}
		}
		else
			eye[2] += 0.1f;
	}
	if (specialKeys[GLUT_KEY_PAGE_UP] == 1) {
		if (snipeMode) {
			if (snipeEye[1] < 300)
				snipeEye[1] += 1.0f;
		}
		else
			eye[1] += 0.1f;
	}
	if (specialKeys[GLUT_KEY_PAGE_DOWN] == 1) {
		if (snipeMode) {
			if (snipeEye[1] > 5)
				snipeEye[1] -= 1.0f;
		}
		else
			eye[1] -= 0.1f;
	}
}

/** 2D renderings (like bitmap images) in here!
 */
void displayOrthographic() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1080, 0, 1080);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glClear(GL_DEPTH_BUFFER_BIT);

	if (snipeMode) 
	{
		glBegin(GL_LINES);
		glColor3f(0.8, 0, 0);
		glVertex2f(500, 540);
		glVertex2f(580, 540);
		glEnd();

		glBegin(GL_LINES);
		glColor3f(0.8, 0, 0);
		glVertex2f(540, 500);
		glVertex2f(540, 580);
		glEnd();
	}

	//绘制障碍物
	for (int i = 0; i < 3; i++)
	{
		if(m_Obstacles[i].IsShow){
		glColor3f(0.8, 0.8, 0);
		glBegin(GL_QUADS);
		glVertex2f(m_Obstacles[i].pos.getZ() - 2, m_Obstacles[i].pos.getX() - 2);
		glVertex2f(m_Obstacles[i].pos.getZ() + 2, m_Obstacles[i].pos.getX() - 2);
		glVertex2f(m_Obstacles[i].pos.getZ() + 2, m_Obstacles[i].pos.getX() + 2);
		glVertex2f(m_Obstacles[i].pos.getZ() - 2, m_Obstacles[i].pos.getX() + 2);
		glEnd();
		}
	}


    //glPushMatrix();
	//glColor3f(1.0, 0.0, 0.0);
	//glTranslatef(m_control[2],m_control[0],0);
    //glBegin(GL_QUADS);
	//glVertex2f( -2,     -2);
	//glVertex2f(  2,     -2);
	//glVertex2f(  2,      2);
	//glVertex2f( -2,      2);
    //glEnd();
	//glPopMatrix();
	
	
	
	glBegin(GL_QUADS);
	if (shipZ39->isDestroyed())
		glColor3f(0.1, 0.1, 0.1);
	else if (waterDepthMode)
		glColor3f(0, 1, 0);
	else
		glColor3f(1, 0, 0);
	glVertex2f(shipZ39->getCurrentPos().getZ() - 2, shipZ39->getCurrentPos().getX() - 2);
	glVertex2f(shipZ39->getCurrentPos().getZ() + 2, shipZ39->getCurrentPos().getX() - 2);
	glVertex2f(shipZ39->getCurrentPos().getZ() + 2, shipZ39->getCurrentPos().getX() + 2);
	glVertex2f(shipZ39->getCurrentPos().getZ() - 2, shipZ39->getCurrentPos().getX() + 2);
	glEnd();

	for (int i = 0; i < 300; i++) {
		for (int j = 0; j < 300; j++) {
			double height = islandMap.getHeight(i, j);
			if (height <= 0 && !waterDepthMode) {
				glBegin(GL_POINTS);
				glColor3f(0.1, 0.3, 0.8);
				glVertex2f(j, i);
				glEnd();
			}if (height > 0 && !waterDepthMode) {
				glBegin(GL_POINTS);
				glColor3f(0, 1, 0);
				glVertex2f(j, i);
				glEnd();
			}if (waterDepthMode) {
				glBegin(GL_POINTS);
				if (height > -0.3)
					glColor3f(1, 0, 0);
				else
					glColor3f(0, 0, 1);
				glVertex2f(j, i);
				glEnd();
			}
		}
	}

	int textInt = shipZ39->getCurrentSpeed();
	stringstream streamText;
	streamText << textInt;
	string text;
	streamText >> text;

	glPushMatrix();
	glTranslatef(700, 500, 0);
	glScalef(0.6, 0.6, 1);
	for (int i = 0; i < text.length(); i++) {
		glColor3f(0.6, 0.3, 0);
		glLineWidth(5);
		glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
	}
	glPopMatrix();

	textInt = shipZ39->getCurrentHealth();
	stringstream healthStream;
	healthStream << textInt;
	string health;
	healthStream >> health;
	glPushMatrix();
	glTranslatef(10, 400, 0);
	glScalef(0.3, 0.3, 1);
	for (int i = 0; i < health.length(); i++) {
		glColor3f(1, 0.3, 0);
		glLineWidth(5);
		glutStrokeCharacter(GLUT_STROKE_ROMAN, health[i]);
	}
	glPopMatrix();


	double textDouble = shipZ39->getTelegraphPos();
	stringstream telegraphStream;
	telegraphStream << textDouble;
	string telegraph;
	telegraphStream >> telegraph;
	glPushMatrix();
	glTranslatef(10, 350, 0);
	glScalef(0.3, 0.3, 1);
	for (int i = 0; i < telegraph.length(); i++) {
		glColor3f(0.5, 0.3, 0.2);
		glLineWidth(5);
		glutStrokeCharacter(GLUT_STROKE_ROMAN, telegraph[i]);
	}
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
}


void display(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 500);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    glColor3f(1, 1, 1);
    char string[25];
	sprintf(string,"%.1f",mTime);
	//itoa(mTime, string, 10);
	DoRasterString(0,    0.37, -1, "Time Remaining:");
	DoRasterString(0.15, 0.37, -1, string);



	keypressCheck();
	specialCheck();

	//时间倒计时结束
	if(mTime <= 0)
	{	
		glDisable(GL_LIGHTING);
		glColor3f(1, 0, 0);
		DoRasterString(0,    0.17, -1, "Game Over, Try Again");
		glutSwapBuffers();
		return ;
	}

	//检测是否所有的障碍物都没有了
	bool IsObstacles = false;
	for(int i=0;i<3;i++)
	{
	  if(m_Obstacles[i].IsShow == true)
	        IsObstacles = true;
	}
	if(IsObstacles == false)
	{		
		gameWon = true;
		mTime = 1.0;	
		glDisable(GL_LIGHTING);
		glColor3f(0, 1, 0);
	    DoRasterString(0,    0.17, -1, "Congrats!!! You  win!");
    	glutSwapBuffers();
    	return ;
	}


	shipCamera.move(0, lastX, lastY);
	shipZ39->setRelativeAngle((int)(shipCamera.getAngleAround()));

	lookAt[0] = shipZ39->getCurrentPos().getX();
	lookAt[2] = shipZ39->getCurrentPos().getZ();


    //子弹的视角
   if(mBulletEye == -1 && shipZ39->m_bullet[0].IsLive)
   {
       snipeMode = -snipeMode;
       mBulletEyePos[0] = shipZ39->m_bullet[0].currentPos.mX;
       mBulletEyePos[1] = shipZ39->m_bullet[0].currentPos.mY;
       mBulletEyePos[2] = shipZ39->m_bullet[0].currentPos.mZ;
       mBulletEyePos[3] = shipZ39->m_bullet[0].endPos.mX;
       mBulletEyePos[4] = 0;
       mBulletEyePos[5] = shipZ39->m_bullet[0].endPos.mZ;
       mBulletEyePos[6] = shipZ39->m_bullet[0].moveUp3D.mX;
       mBulletEyePos[7] = shipZ39->m_bullet[0].moveUp3D.mY;
       mBulletEyePos[8] = shipZ39->m_bullet[0].moveUp3D.mZ;
       gluLookAt(mBulletEyePos[0],mBulletEyePos[1],mBulletEyePos[2],mBulletEyePos[3],mBulletEyePos[4],mBulletEyePos[5],mBulletEyePos[6],mBulletEyePos[7],mBulletEyePos[8]);
   }
   else if (snipeMode == 1)
    {
   		gluLookAt(snipeEye[0], snipeEye[1], snipeEye[2], snipeLookAt[0], snipeLookAt[1], snipeLookAt[2], 1, 0, 0);
   	}
	else
	{
		gluLookAt(shipCamera.getPosition().getX(), shipCamera.getPosition().getY(), shipCamera.getPosition().getZ(), lookAt[0], 0, lookAt[2], up[0], up[1], up[2]);
	}


	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	shipZ39->drawShip();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

    //glPushMatrix();
    //glColor3f(1, 0, 0);
	//glTranslatef(m_control[0],m_control[1],m_control[2]);
	//glutSolidSphere(1.1, 50, 50);
	//cout << "x:" <<m_control[0] << "  y:" <<m_control[1]<< "  z:" <<m_control[2]<< endl;
	//glPopMatrix();

	for(int i = 0; i < 4; i++){
		if(shipZ39->getBullet(i).currentPos.mY<=0.3){
			islandMap.setHeight((int)shipZ39->getBullet(0).endPos.mX, (int)shipZ39->getBullet(0).endPos.mZ, -1);
			islandMap.setHeight((int)shipZ39->getBullet(1).endPos.mX, (int)shipZ39->getBullet(1).endPos.mZ, -1);
			islandMap.setHeight((int)shipZ39->getBullet(2).endPos.mX, (int)shipZ39->getBullet(2).endPos.mZ, -1);
			islandMap.setHeight((int)shipZ39->getBullet(3).endPos.mX, (int)shipZ39->getBullet(3).endPos.mZ, -1);
		}
	}

	//绘制障碍物
	for (int i = 0; i < 3; i++)
	{
		m_Obstacles[i].Draw();
		if (shipZ39->CheckFoot(m_Obstacles[i]))
			m_Obstacles[i].IsShow = false;
		
		//检测是否碰撞
		if (shipZ39->checkObstacleCollision(m_Obstacles[i]))
			m_Obstacles[i].IsShow = false;
	}

		

	islandMap.drawTerrain(0.1, 1, 0.1);
	stableWater.drawWater(0.1, 0.2, 1);

	displayOrthographic();

	glutSwapBuffers();
}

void keyboardDown(unsigned char key, int x, int y) {
	double currentTelegraphPos = shipZ39->getTelegraphPos();

	if (key == (int)'w' || key == (int)'W') {
		if (currentTelegraphPos < 1) {
			shipZ39->setTelegraphPos(currentTelegraphPos + 0.25);
		}
		return;
	}

	if (key == (int)'s' || key == (int)'S') {
		if (currentTelegraphPos > -0.25) {
			shipZ39->setTelegraphPos(currentTelegraphPos - 0.25);
		}
		return;
	}

	if (key == (int)'m' || key == (int)'M') {
		waterDepthMode = !waterDepthMode;
		return;
	}

	if (key == 32) {
		if(snipeMode)
			shipZ39->fire(Point2D(snipeLookAt[0], snipeLookAt[2]));
		return;
	}

	if (key == (int)'b' || key == (int)'B') {
		shipZ39->beastModeOn();
		return;
	}

    if(key ==(int)'p' || key  == (int)'P')
    {
      mBulletEye = -mBulletEye;
    }
	keys[(int)key] = true;
}

void keyboardUp(unsigned char key, int x, int y) {
	if (key == (int)'b' || key == (int)'B') {
		shipZ39->beastModeOff();
		return;
	}
	keys[key] = false;
}

void specialDown(int key, int x, int y) {
	if (key == GLUT_KEY_INSERT) {
		snipeMode = !snipeMode;
		snipeLookAt[0] = shipZ39->getCurrentPos().getX();
		snipeLookAt[1] = 0;
		snipeLookAt[2] = shipZ39->getCurrentPos().getZ();
		snipeEye[0] = shipZ39->getCurrentPos().getX();
		snipeEye[1] = 200;
		snipeEye[2] = shipZ39->getCurrentPos().getZ();
		return;
	}

	specialKeys[key] = true;
}

void specialUp(int key, int x, int y) {
	specialKeys[(int)key] = false;
}

void calcShipPos(int val) {

	float delta = (17) / 1000.0f;

	shipZ39->calcCurrentPos();

	glutTimerFunc(17, calcShipPos, 0);

    if(!gameWon)
        mTime = mTime - 0.17;
	if(mTime < 0) mTime = 0;
	glutPostRedisplay();
}

void init(void) {
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);

	glutSpecialFunc(specialDown);
	glutSpecialUpFunc(specialUp);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);

	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutSetCursor(GLUT_CURSOR_NONE);

	lookAt[0] = shipZ39->getCurrentPos().getX();
	lookAt[2] = shipZ39->getCurrentPos().getZ();

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

	glMatrixMode(GL_TEXTURE);
	glScalef(-1, 1, 1);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);

	//enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < 3; i++)
	{
		m_Obstacles[i].IsShow = true;
		m_Obstacles[i].pos.mX = (float)rand() / (float)RAND_MAX * 300;
		m_Obstacles[i].pos.mZ = (float)rand() / (float)RAND_MAX * 300;
		m_Obstacles[i].pos.mY = 0;
	}

	glutTimerFunc(17, calcShipPos, 0);
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(1080, 1080);

	glutCreateWindow("Naval Combat");	//creates the window

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glutDisplayFunc(display);	//registers "display" as the display callback function

	init();

	glutMainLoop();				//starts the event glutMainLoop
	delete shipZ39;
	return(0);
}
