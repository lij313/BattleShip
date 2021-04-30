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
#include <cstdlib>
#include "TerrainGenerate.h"

void TerrainGenerate::initTerrainMapArray(){
	for(int i = 0; i < sizeof(terrainMapXZ)/sizeof(terrainMapXZ[0]); i++){
		for(int j = 0; j < sizeof(terrainMapXZ[0])/sizeof(terrainMapXZ[0][0]); j++){
			terrainMapXZ[i][j][0]=i;
			terrainMapXZ[i][j][1]=j;
			terrainMapY[i][j]=0;
		}
	}
}

void TerrainGenerate::eliminateUneligablePoints(double shiftDown){

    double currentMax=-1;
    double currentMin=999;

    //find current min and max height
    for(int x = 0; x < sizeof(terrainMapY)/sizeof(terrainMapY[0]); x++){
		for(int z = 0; z < sizeof(terrainMapY[0])/sizeof(terrainMapY[0][0]); z++){
			if(terrainMapY[x][z] > currentMax)
				currentMax = terrainMapY[x][z];
			else if(terrainMapY[x][z] < currentMin)
				currentMin = terrainMapY[x][z];
		}
	}

	for(int i = 0; i < sizeof(terrainMapY)/sizeof(terrainMapY[0]); i++){
		for(int j = 0; j < sizeof(terrainMapY[0])/sizeof(terrainMapY[0][0]); j++){
			terrainMapY[i][j] -= currentMin;
			terrainMapY[i][j] -= shiftDown;
		}
	}
}

void TerrainGenerate::circleAlgorithm(int repeats){

	srand((unsigned int)time(NULL));

	//repeat the algorithm for a number of times
	for(int i = 0; i < repeats; i++){
		int randomX=std::rand()%500; //choose random x value
		int randomY=std::rand()%500; //choose random y value
		int randomR=(std::rand()%(int)(500*0.2))+1; //choose random radius
		double height=(double(rand()) / double((RAND_MAX)))+0.5; //choose random z value

		//check eligable points
		for(int x = 0; x < sizeof(terrainMapY)/sizeof(terrainMapY[0]); x++){
			for(int y = 0; y < sizeof(terrainMapY[0])/sizeof(terrainMapY[0][0]); y++){
				int dx = x - randomX;
				int dy = y - randomY;
				double distance = sqrtf((dx*dx)+(dy*dy));
				double pd = (distance*2.0)/(randomR*2.0);
				if(fabs(pd) <= 1.0)
					terrainMapY[x][y] += (height/2.0) + (cos(pd*3.14))*(height/2.0);
			}
		}
	}
	//createNormalVectors();
	eliminateUneligablePoints(5);
}

void TerrainGenerate::calculateNormal(){
	float vec1[3];
	float vec2[3];
	for(int x = 0; x < sizeof(terrainMapXZ)/sizeof(terrainMapXZ[0])-1; x++){
		for(int z = 0; z < sizeof(terrainMapXZ[0])/sizeof(terrainMapXZ[0][0])-1; z++){
			vec1[0] = terrainMapXZ[x][z+1][0]-terrainMapXZ[x][z][0];
			vec1[1] = terrainMapY[x][z+1]-terrainMapY[x][z];
			vec1[2] = terrainMapXZ[x][z+1][1]-terrainMapXZ[x][z][1];

			vec2[0] = terrainMapXZ[x+1][z][0]-terrainMapXZ[x][z][0];
			vec2[1] = terrainMapY[x+1][z]-terrainMapY[x][z];
			vec2[2] = terrainMapXZ[x+1][z][1]-terrainMapXZ[x][z][1];

			float normalx = vec1[1]*vec2[2]-vec1[2]*vec2[1];
 			float normaly = vec1[2]*vec2[0]-vec1[0]*vec2[2];
			float normalz = vec1[0]*vec2[1]-vec1[1]*vec2[0];
			normal[x][z][0] = normalx;
			normal[x][z][1] = normaly;
			normal[z][z][2] = normalz;
		}
	}
}

TerrainGenerate::TerrainGenerate(){
	initTerrainMapArray();
	calculateNormal();
}

TerrainGenerate::TerrainGenerate(int repeats){
	initTerrainMapArray();
	circleAlgorithm(repeats);
	calculateNormal();
}

void TerrainGenerate::drawTerrain(float r, float g, float b){
	float defuse[4] = {r, g, b, 1 };
	float ambient[4] = {r/1.2, g/1.2, b/1.2, 1};
	glBegin(GL_QUADS);
	for(int x = 0; x < sizeof(terrainMapXZ)/sizeof(terrainMapXZ[0])-1; x++){
		for(int z = 0; z < sizeof(terrainMapXZ[0])/sizeof(terrainMapXZ[0][0])-1; z++){
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glColor3f(r,g,b);
			
			glMaterialfv(GL_FRONT, GL_DIFFUSE, defuse);
			glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
			
			for(int i = 0; i < 4; i++){
				switch (i){
					case 0:
						glNormal3fv(normal[x][z]);
						glVertex3f(terrainMapXZ[x][z][0], terrainMapY[x][z], terrainMapXZ[x][z][1]);
						break;
					case 1:
						glNormal3fv(normal[x][z+1]);
						glVertex3f(terrainMapXZ[x][z+1][0], terrainMapY[x][z+1], terrainMapXZ[x][z+1][1]);
						
						break;
					case 2:
						glNormal3fv(normal[x+1][z+1]);
						glVertex3f(terrainMapXZ[x+1][z+1][0], terrainMapY[x+1][z+1], terrainMapXZ[x+1][z+1][1]);
						break;
					case 3:
						glNormal3fv(normal[x+1][z]);
						glVertex3f(terrainMapXZ[x+1][z][0], terrainMapY[x+1][z], terrainMapXZ[x+1][z][1]);
						break;
				}
			}
			
		}
	}
	glEnd();
}

void TerrainGenerate::drawWater(float r, float g, float b){
	float defuse[4] = {r, g, b, 0.5};
	float ambient[4] = {r/1.2, g/1.2, b/1.2, 0.5};
	glBegin(GL_QUADS);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(r,g,b);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, defuse);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);

		glNormal3f(0, 5, 0);
		glVertex3f(0,0,0);
		glVertex3f(0, 0, 300);
		glVertex3f(300, 0, 300);
		glVertex3f(300, 0, 0);
	glEnd();
}