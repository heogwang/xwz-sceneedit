#include <string>
#include <fstream>
#include <iostream>
#include <io.h>
#include <direct.h>
#include <vector>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <assert.h>
#include "Scene.h"

using namespace std;

enum POINTACTION{
	CALCPOINTS,LOADPOINTS,SAVEPOINTS,
	SAVED2,
	SAVESHELL,SAVESECTOR,SAVESECSHELL,
	VALISTART,
	VALID2
};

#define OUTNUMBERS false

class PointDataManager
{
public:
	Model *model;
	point *points;
	int POINTS;
	//
	PointDataManager(string folderPath,POINTACTION actions[],int count);
	~PointDataManager();
	//
	void getFiles(string path, vector<string>& files);
	void getFolders(string path, vector<string>& folders);
	float getDistance(int *v1, int *v2, int size);
	//
	void calcPoints(string filePath);
	void savePoints(string filePath);
	void loadPoints(string filePath);
	//
	bool saveD2start;
	int binsD2;
	void saveD2(string totalPath, string fileName);
	void valiD2(string totalPath);
	//
	bool saveShellstart;
	void saveShell(string totalPath, string fileName);
	bool saveSectorstart;
	void saveSector(string totalPath, string fileName);
	bool saveSecShellstart;
	void saveSecShell(string totalPath, string fileName);
};