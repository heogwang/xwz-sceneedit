#include "PointDataManager.h"

PointDataManager::PointDataManager(string folderPath,POINTACTION actions[],int count)
{
	POINTS = 1000;
	//
	vector<string> folders;
	getFolders(folderPath,folders);
	for(int i=folders.size()-1;i>=0;i--)
	{
		vector<string> files;
		string thisFolder = folderPath + folders[i] + "\\";
		string thisTotal = thisFolder + folders[i];
		getFiles(thisFolder,files);
		//
		saveD2start = true;
		for(int j=files.size()-1;j>=0;j--)
		{
			if(files[j].substr(0,10) == "conference") continue;
			if(files[j] == "Wall.obj") continue;
			if(files[j].substr(files[j].size()-4,4) != ".obj") continue;
			string thisFile = thisFolder + files[j];
			//
			model = NULL;
			points = NULL;
			for(int k=0;k<count;k++)
			{
				switch(actions[k])
				{
				case CALCPOINTS:
					calcPoints(thisFile);break;
				case SAVEPOINTS:
					savePoints(thisFile);break;
				case LOADPOINTS:
					loadPoints(thisFile);break;
				case SAVED2:
					saveD2(thisTotal, files[j]);break;
				}
			}
			if(model != NULL)
			{
				delete model;
				model = NULL;
			}
			else if(points != NULL)
			{
				delete []points;
			}
			points = NULL;
		}
	}
}

PointDataManager::~PointDataManager()
{
	if(model != NULL)
	{
		delete model;
	}
}

void PointDataManager::getFiles(string path, vector<string>& files)
{
	//文件句柄 
    long hFile = 0;
    //文件信息 
    struct _finddata_t fileinfo; 
    string p;
    if((hFile = _findfirst(p.assign(path).append("/*").c_str(),&fileinfo)) != -1)
	{ 
        do
		{ 
            //如果是目录,迭代之
            //如果不是,加入列表
            if((fileinfo.attrib & _A_SUBDIR))
			{ 
                if(strcmp(fileinfo.name,".") != 0 && strcmp(fileinfo.name,"..") != 0)
				{
					//getFiles(p.assign(path).append("/").append(fileinfo.name), files); 
				}
            }
			else
			{ 
                //files.push_back(p.assign(path).append("/").append(fileinfo.name));
				files.push_back(fileinfo.name);
            }
        }while(_findnext(hFile,&fileinfo) == 0); 
        _findclose(hFile);
    }
}

void PointDataManager::getFolders(string path, vector<string>& folders)
{
	//文件句柄 
    long hFile = 0;
    //文件信息 
    struct _finddata_t fileinfo; 
    string p;
    if((hFile = _findfirst(p.assign(path).append("/*").c_str(),&fileinfo)) != -1)
	{ 
        do
		{ 
            //如果是目录,迭代之
            //如果不是,加入列表
            if((fileinfo.attrib & _A_SUBDIR))
			{ 
                if(strcmp(fileinfo.name,".") != 0 && strcmp(fileinfo.name,"..") != 0)
				{
					//getFiles(p.assign(path).append("/").append(fileinfo.name), files);
					folders.push_back(fileinfo.name);
				}
            }
			else
			{ 
                //files.push_back(p.assign(path).append("/").append(fileinfo.name));
            }
        }while(_findnext(hFile,&fileinfo) == 0); 
        _findclose(hFile);
    }
}

void PointDataManager::calcPoints(string filePath)
{
	model = new Model();
	model->ReadModel(filePath);
	model->PCAOperation();
	model->CalculateArea();
	model->GeneratePoints(POINTS);
	points = model->samplePoints;
}

void PointDataManager::savePoints(string filePath)
{
	if(model == NULL)
	{
		return;
	}
	ofstream fileout;
	fileout.open(filePath + ".txt");
	//for(int i=model->pointNumber-1; i>=0; i-- )
	//{
	//	fileout<<setprecision(16)<<model->samplePoints[i][0]<<' '
	//		<<setprecision(16)<<model->samplePoints[i][1]<<' '
	//		<<setprecision(16)<<model->samplePoints[i][2]<<std::endl;
	//}
	fileout.write((const char*)model->samplePoints,sizeof(point)*model->pointNumber);
	fileout.close();
}

void PointDataManager::loadPoints(string filePath)
{
	point* p = new point[POINTS];
	ifstream filein;
	filein.open(filePath + ".txt");
	filein.read((char*)p,sizeof(point)*POINTS);
	filein.close();
	points = p;
}

void PointDataManager::saveD2(string totalPath, string fileName)
{
	if(points == NULL)
	{
		return;
	}
	//
	float max = 0;
	float** result = new float*[POINTS];
	for(int i=0;i<POINTS;i++)
	{
		result[i] = new float[POINTS];
		for(int j=i+1;j<POINTS;j++)
		{
			float length = len(points[i] - points[j]);
			if(length > max)
			{
				max = length;
			}
			result[i][j] = length;
		}
	}
	//
	if(max > 2.0f)
	{
		return;
	}
	max = 2.0f;
	const int BINS = 10;
	int* bin = new int[BINS+1];
	memset(bin,0,(BINS+1) * sizeof(int));
	for(int i=0;i<POINTS;i++)
	{
		for(int j=i+1;j<POINTS;j++)
		{
			float length = result[i][j];
			length /= max;
			bin[(int)(length*BINS)]++;
		}
	}
	delete[] result;
	//bin[BINS-1]++;

	ofstream fileout;
	if(saveD2start)
	{
		fileout.open(totalPath + ".d2.txt");
		saveD2start = false;
	}
	else
	{
		fileout.open(totalPath + ".d2.txt",ofstream::ate|ofstream::app);
	}
	fileout<<"#"<<fileName.substr(0,fileName.length()-4)<<endl;
	for(int i=0; i<BINS; i++)
	{
		fileout<<bin[i]<<' ';
	}
	fileout<<endl;
	//fileout.write((const char*)model->samplePoints,sizeof(point)*model->pointNumber);
	fileout.close();

	delete bin;
}
