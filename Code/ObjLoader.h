#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
#include <vector>
#include <string>
#include <map>
using namespace std;

class ObjLoader{

public:
	ObjLoader();
	ObjLoader(string filename, string relativeFilePath, double scale);

	void Draw();

private:
	vector<vector<GLfloat> >vSets;
	vector<vector<GLint> >fVSets;
	vector<vector<GLint> >fVNSets;
	vector<vector<GLint> >fVTSets;
	vector<vector<GLfloat> >vnSets;
	vector<vector<GLfloat> >vtSets;

	map<int, string> materialFMap;

	vector<GLfloat> ns;
	vector<vector<GLfloat> > ka;
	vector<vector<GLfloat> > kd;
	vector<vector<GLfloat> > ks;
	vector<vector<GLfloat> > ke;

	map<string, string> materialNameMap;
	map<string, int> materialIndexMap;

	void MtlLoader(string filename, string relativeFilePath);
	GLubyte* loadPPM(char* file, int* width, int* height, int* myMax);

	vector<GLubyte* >imgData;
	int* width;
	int* height;
	int* myMax;
};

