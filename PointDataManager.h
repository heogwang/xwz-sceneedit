#include <string>
#include <fstream>
#include <iostream>
#include <io.h>
#include <direct.h>
#include <vector>
#include <iomanip>
#include <ctime>
#include "Scene.h"

using namespace std;

enum POINTACTION{
	CALCPOINTS,LOADPOINTS,SAVEPOINTS,
	SAVED2
};

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
	//
	void calcPoints(string filePath);
	void savePoints(string filePath);
	void loadPoints(string filePath);
	//
	bool saveD2start;
	void saveD2(string totalPath, string fileName);
};