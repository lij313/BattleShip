#include "ObjLoader.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <sstream>
using namespace std;

ObjLoader::ObjLoader(){

}

ObjLoader::ObjLoader(string shipName, string relativeFilePath, double scale){

	string filepath;
	if(relativeFilePath.length()==0){
		filepath = "Models/";
		filepath += shipName+"/"+shipName;
	}else{
		filepath=relativeFilePath;
	}

	string line;
	fstream f;

	string filename=filepath+".obj";

	cout << filename << std::endl;

	f.open(filename.c_str(), ios::in);

	if (!f.is_open()){
		cout << "Something Went Wrong When Opening Objfiles" << endl;
		cout << filename << endl;
		return;
	}

	int fIndex = 0;
	while (!f.eof()){
		getline(f, line);

		vector<string> parameters;

		string tailMark = " ";

		string ans = "";

		line = line.append(tailMark);

		for(int i = 0; i < line.length(); i++) {

			char ch = line[i];
			if (ch != ' ') {
				ans+=ch;
			}
			else {
				parameters.push_back(ans);
				ans = "";
			}
		}

		if (parameters[0] == "v") {

			vector<GLfloat>Point;

			for (int i = 1; i < parameters.size(); i++) {

				GLfloat xyz = atof(parameters[i].c_str());

				xyz*=scale;

				Point.push_back(xyz);
			}

			vSets.push_back(Point);
		}else if (parameters[0] == "f") {
			vector<GLint>vIndexSets;
			vector<GLint>vnIndexSets;
			vector<GLint>vtIndexSets;

			GLint index;

			for (int i = 1; i < parameters.size(); i++){
				string x = parameters[i];
				string ans = "";

				int countSlash = 0;

				for (int j = 0; j < x.length(); j++) {
					char ch = x[j];
					if (ch != '/') {
						ans += ch;
					}else{
						countSlash+=1;
						if(countSlash == 1){
							index = atof(ans.c_str());
							vIndexSets.push_back(index);
							ans = "";
						}
						if(countSlash == 2){
							index = atof(ans.c_str());
							vtIndexSets.push_back(index);
							ans = "";

							index = atof(x.substr(j+1).c_str());
							vnIndexSets.push_back(index);
						}
					}

				}
			}
			fVSets.push_back(vIndexSets);
			fVNSets.push_back(vnIndexSets);
			fVTSets.push_back(vtIndexSets);
			fIndex++;
		}else if (parameters[0] == "vn") {
			vector<GLfloat>vnValueSets;
			for (int i = 1; i < parameters.size(); i++) {
				GLfloat vnxyz = atof(parameters[i].c_str());
				vnValueSets.push_back(vnxyz);
				vnxyz*=scale;
				vnxyz*=scale;
			}
			vnSets.push_back(vnValueSets);
		}else if (parameters[0] == "vt") {
			vector<GLfloat>vtValueSets;
			for (int i = 1; i < parameters.size(); i++) {
				GLfloat vnxyz = atof(parameters[i].c_str());
				vtValueSets.push_back(vnxyz);
			}
			vtSets.push_back(vtValueSets);
		}else if (parameters[0] == "usemtl") {
			materialFMap.insert(make_pair(fIndex, parameters[1]));
		}
	}
	f.close();
	MtlLoader(shipName, relativeFilePath);
}

void ObjLoader::Draw(){

	//Concept. To be changed later, for debug purpose
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float tempKaArray[4];
	float tempKdArray[4];
	float tempKsArray[4];
	float tempKeArray[4];

	for (int i = 0; i < fVSets.size(); i++) {
		
		GLfloat VN[3];

		//three vertex represrnt a surface
		GLfloat SV1[3];
		GLfloat	SV2[3];
		GLfloat SV3[3];
		GLfloat SV4[3];

		GLfloat VT1[3];
		GLfloat	VT2[3];
		GLfloat VT3[3];
		GLfloat VT4[3];

		int useMaterial;
		int materialIndex;

		if(materialFMap[i].length() != 0){
			useMaterial = i;
			materialIndex = materialIndexMap[materialFMap[i]];
			for(int index = 0; index < 4; index++){
				tempKaArray[index] = ka[materialIndex][index];
				tempKdArray[index] = kd[materialIndex][index];
				tempKsArray[index] = ks[materialIndex][index];
				tempKeArray[index] = ke[materialIndex][index];
			}
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[materialIndex], height[materialIndex], 0, GL_RGB, GL_UNSIGNED_BYTE, imgData[materialIndex]);
			
		}

		glBegin(GL_TRIANGLES);
		
		if((fVSets[i]).size() == 3){

			GLint firstVertexIndex = fVSets[i][0]-1;
			GLint secondVertexIndex = fVSets[i][1]-1;
			GLint thirdVertexIndex = fVSets[i][2]-1;

			SV1[0] = vSets[firstVertexIndex][0];
			SV1[1] = vSets[firstVertexIndex][1];
			SV1[2] = vSets[firstVertexIndex][2];

			SV2[0] = vSets[secondVertexIndex][0];
			SV2[1] = vSets[secondVertexIndex][1];
			SV2[2] = vSets[secondVertexIndex][2];

			SV3[0] = vSets[thirdVertexIndex][0];
			SV3[1] = vSets[thirdVertexIndex][1];
			SV3[2] = vSets[thirdVertexIndex][2];


			GLint firstVTIndex = fVTSets[i][0]-1;
			GLint secondVTIndex = fVTSets[i][1]-1;
			GLint thirdVTIndex = fVTSets[i][2]-1;


			VT1[0] = vtSets[firstVTIndex][0];
			VT1[1] = vtSets[firstVTIndex][1];

			VT2[0] = vtSets[secondVTIndex][0];
			VT2[1] = vtSets[secondVTIndex][1];

			VT3[0] = vtSets[thirdVTIndex][0];
			VT3[1] = vtSets[thirdVTIndex][1];
			
			//if there is no vn in the objfile
			if (vnSets.size() == 0) {
				GLfloat vec1[3];
				GLfloat vec2[3];
				GLfloat vec3[3];

				//(x2-x1,y2-y1,z2-z1)
				vec1[0] = SV1[0] - SV2[0];
				vec1[1] = SV1[1] - SV2[1];
				vec1[2] = SV1[2] - SV2[2];

				//(x3-x2,y3-y2,z3-z2)
				vec2[0] = SV1[0] - SV3[0];
				vec2[1] = SV1[1] - SV3[1];
				vec2[2] = SV1[2] - SV3[2];

				//(x3-x1,y3-y1,z3-z1)
				vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
				vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
				vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

				GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));

				VN[0] = vec3[0] / D;
				VN[1] = vec3[1] / D;
				VN[2] = vec3[2] / D;

				glNormal3f(VN[0], VN[1], VN[2]);
			}else {
				if (i < fVNSets.size()) {
					GLint firstVNIndex = fVNSets[i][0]-1;
					GLint secondVNIndex = fVNSets[i][1]-1;
					GLint thirdVNIndex = fVNSets[i][2]-1;

					VN[0] = vnSets[firstVNIndex][0];
					VN[1] = vnSets[firstVNIndex][1];
					VN[2] = vnSets[firstVNIndex][2];
					glNormal3f(VN[0], VN[1], VN[2]);

					VN[0] = vnSets[secondVNIndex][0];
					VN[1] = vnSets[secondVNIndex][1];
					VN[2] = vnSets[secondVNIndex][2];
					glNormal3f(VN[0], VN[1], VN[2]);

					VN[0] = vnSets[thirdVNIndex][0];
					VN[1] = vnSets[thirdVNIndex][1];
					VN[2] = vnSets[thirdVNIndex][2];
					glNormal3f(VN[0], VN[1], VN[2]);
				}
			}
			
			glMaterialf(GL_FRONT, GL_SHININESS, ns[materialIndex]);
			glMaterialfv(GL_FRONT, GL_AMBIENT, tempKaArray);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, tempKdArray);
			//glMaterialfv(GL_FRONT, GL_SPECULAR, tempKsArray);
  			glMaterialfv(GL_FRONT, GL_EMISSION, tempKeArray);
			
			
			glColor3f(1,1,1);

			glTexCoord2f(VT1[0], VT1[1]);
			glVertex3f(SV1[0], SV1[1], SV1[2]);

			glTexCoord2f(VT2[0], VT2[1]);
			glVertex3f(SV2[0], SV2[1], SV2[2]);

			glTexCoord2f(VT3[0], VT3[1]);
			glVertex3f(SV3[0], SV3[1], SV3[2]);

		}else
			cout << "Warning: the fVSet_Size: "<< i << " is not correct, its behaviour is unpredictable: " << fVSets[i].size() << endl;
	
		glEnd();
	}
	
}

void ObjLoader::MtlLoader(string shipName, string relativeFilePath){

	string filename;
	if(relativeFilePath.length()==0){
		filename = "Models/";
		filename += shipName+"/"+shipName;
	}else{
		filename=relativeFilePath;
	}
	filename+=".mtl";

	cout << filename << std::endl;

	string line;
	fstream f;

	f.open(filename.c_str(), ios::in);

	if (!f.is_open()){
		cout << "Something Went Wrong When Opening Mtlfiles" << endl;
	}

	int blockCount = 0;

	string mtlName;

	while (!f.eof()){
		getline(f, line);

		vector<string> parameters;

		vector<GLfloat> tempVec;

		string tailMark = " ";

		string ans = "";

		line = line.append(tailMark);

		for(int i = 0; i < line.length(); i++) {

			char ch = line[i];
			if (ch != ' ') {
				ans+=ch;
			}
			else {
				parameters.push_back(ans);
				ans = "";
			}
		}


		if (parameters[0] == "newmtl") {
			mtlName = parameters[1];
			materialIndexMap.insert(make_pair(mtlName, blockCount));
		}else if (parameters[0] == "Ns") {
			ns.push_back(atof(parameters[0].c_str()));
		}else if (parameters[0] == "Ka") {
			tempVec.push_back(atof(parameters[0].c_str()));
			tempVec.push_back(atof(parameters[1].c_str()));
			tempVec.push_back(atof(parameters[2].c_str()));
			tempVec.push_back(1);
			ka.push_back(tempVec);
		}else if (parameters[0] == "Kd") {
			tempVec.push_back(atof(parameters[0].c_str()));
			tempVec.push_back(atof(parameters[1].c_str()));
			tempVec.push_back(atof(parameters[2].c_str()));
			tempVec.push_back(1);
			kd.push_back(tempVec);
		}else if (parameters[0] == "Ks") {
			tempVec.push_back(atof(parameters[0].c_str()));
			tempVec.push_back(atof(parameters[1].c_str()));
			tempVec.push_back(atof(parameters[2].c_str()));
			tempVec.push_back(1);
			ks.push_back(tempVec);
		}else if (parameters[0] == "Ke") {
			tempVec.push_back(atof(parameters[0].c_str()));
			tempVec.push_back(atof(parameters[1].c_str()));
			tempVec.push_back(atof(parameters[2].c_str()));
			tempVec.push_back(1);
			ke.push_back(tempVec);
		}else if (parameters[0] == "d"){
			ka[blockCount].push_back(atof(parameters[0].c_str()));
			kd[blockCount].push_back(atof(parameters[0].c_str()));
			ks[blockCount].push_back(atof(parameters[0].c_str()));
			ke[blockCount].push_back(atof(parameters[0].c_str()));
			blockCount+=1;
		}else if (parameters[0] == "map_Kd"){
			materialNameMap.insert(make_pair(mtlName, parameters[1]));
		}
	}

	f.close();

	width = new int[blockCount];
	height = new int[blockCount];
	myMax = new int[blockCount];

	map<string, string>::iterator it;
	int count = 0;
	filename = "Models/";
	filename += shipName+"/"+shipName;
	string baseFilepath = filename.substr(0, filename.size()-shipName.size());

	for ( it = materialNameMap.begin(); it != materialNameMap.end(); it++ ){
		string filepath = "";
		filepath=baseFilepath+"textures";
		filepath += it->second;
		char* filepathChar =  const_cast<char *>(filepath.c_str());

		GLubyte* temp;

		temp = loadPPM(filepathChar, &width[count], &height[count], &myMax[count]);

		imgData.push_back(temp);
			
		count++;
	}
}


GLubyte* ObjLoader::loadPPM(char* file, int* width, int* height, int* myMax){

    vector<vector<GLubyte> > imgVec;

    GLubyte* img;
    FILE *fd;
    int n, m;
    int  k, nm;
    char c;
    int i;
    char b[100];
    float s;
    int red, green, blue;
    
    fd = fopen(file, "r");
    fscanf(fd,"%[^\n] ",b);
    if(b[0]!='P'|| b[1] != '3')
    {
        printf("%s is not a PPM file!\n",file);
        exit(0);
    }
    //printf("%s is a PPM file\n", file);
    fscanf(fd, "%c",&c);
    while(c == '#')
    {
        fscanf(fd, "%[^\n] ", b);
        printf("%s\n",b);
        fscanf(fd, "%c",&c);
    }
    ungetc(c,fd);
    fscanf(fd, "%d %d %d", &n, &m, &k);
    
    //printf("%d rows  %d columns  max value= %d\n",n,m,k);
    
    nm = n*m;
    
    img = (GLubyte*)(malloc(3*sizeof(GLuint)*nm));
    
    s=255.0/k;
    
    
    for(i=0;i<nm;i++)
    {
        fscanf(fd,"%d %d %d",&red, &green, &blue );
        img[3*nm-3*i-3]=red*s;
        img[3*nm-3*i-2]=green*s;
        img[3*nm-3*i-1]=blue*s;
    }
    
    *width = n;
    *height = m;
    *myMax = k;

    return img;
}
