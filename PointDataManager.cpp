#include "PointDataManager.h"

PointDataManager::PointDataManager(string folderPath,POINTACTION actions[],int count)
{
	POINTS = 2000;
	binsD2 = 10;
	binsShell = 36;
	binsSector = 36;
	binsSSShell = 6;
	binsSSSector = 6;
	//
	model = NULL;
	points = NULL;
	//
	bool validate = false;
	if(actions[0] > VALISTART)
	{
		validate = true;
	}
	vector<string> folders;
	getFolders(folderPath,folders);
	//folders.push_back("conference_room27");
	for(int i=folders.size()-1;i>=0;i--)
	{
		vector<string> files;
		string thisFolder = folderPath + folders[i] + "\\";
		string thisTotal = thisFolder + folders[i];
		if(validate)
		{
			for (int j=0;j<count;j++)
			{
				switch(actions[j])
				{
				case VALID2:valiD2(thisTotal);break;
				}
			}
		}
		else
		{
			getFiles(thisFolder,files);
			//
			saveD2start = true;
			saveSectorstart = true;
			saveShellstart = true;
			saveSecShellstart = true;
			//
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
					case SAVESHELL:
						saveShell(thisTotal, files[j]);break;
					case SAVESECTOR:
						saveSector(thisTotal, files[j]);break;
					case SAVESECSHELL:
						saveSecShell(thisTotal, files[j]);break;
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
		//break;
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

float PointDataManager::getDistance(int *v1, int *v2, int size)
{
	float ret = 0;
	for(int i=size-1;i>=0;i--)
	{
		ret += abs(v1[i] - v2[i]);//*(v1[i] - v2[i]);
	}
	return ret;
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
	//if(OUTNUMBERS)
	{
		fileout.open(filePath + ".points.txt");
		for(int i=model->pointNumber-1; i>=0; i-- )
		{
			fileout<<setprecision(16)<<model->samplePoints[i][0]<<' ';
			fileout<<setprecision(16)<<model->samplePoints[i][1]<<' ';
			fileout<<setprecision(16)<<model->samplePoints[i][2]<<endl;
		}
		fileout.close();
	}
	//else
	{
		fileout.open(filePath + ".txt",ios::binary);
		fileout.write((const char*)model->samplePoints,sizeof(point)*model->pointNumber);
	}
	fileout.close();
}

void PointDataManager::loadPoints(string filePath)
{
	point* p = new point[POINTS];
	ifstream filein;
	filein.open(filePath + ".txt",ios::binary);
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
	float ti,tj;
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
				ti=i;tj=j;
			}
			result[i][j] = length;
		}
	}
	//
	assert(max <= 2.0f);
	max = 2.0f;
	const int BINS = binsD2;
	int* bin = new int[BINS+1];
	assert(bin != NULL);
	memset(bin,0,(BINS+1) * sizeof(int));
	for(int i=0;i<POINTS;i++)
	{
		for(int j=i+1;j<POINTS;j++)
		{
			float length = result[i][j];
			length /= max;
			bin[(int)(length*BINS)]++;
		}
		delete result[i];
	}
	delete []result;
	bin[BINS-1] += bin[BINS];

	const int denominater = (POINTS - 1) * (POINTS - 1) / 2;
	for(int i=0;i<BINS;i++)
	{
		bin[i] = bin[i] * 256 / denominater;
	}

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
	if(OUTNUMBERS || true)
	{
		for(int i=0; i<BINS; i++)
		{
			fileout<<bin[i]<<' ';
		}
	}
	else
	{
		fileout.write((const char*)bin,sizeof(int)*BINS);
	}
	fileout<<endl;
	fileout.close();

	delete []bin;
}

void PointDataManager::valiD2(string totalPath)
{
	ifstream filein;
	filein.open(totalPath + ".d2.txt");
	vector<string> names;
	vector<int*> values;
	while(true)
	{
		string temp;
		filein >> temp;
		if(temp.length() == 0)
		{
			break;
		}
		names.push_back(temp);
		int *tempValue = new int[binsD2];
		int value;
		for(int i=0;i<binsD2;i++)
		{
			filein >> value;
			tempValue[i] = value;
		}
		values.push_back(tempValue);
	}
	filein.close();
	//
	int width = 20;
	ofstream fileout;
	fileout.open(totalPath + ".v.d2.txt");
	fileout << setw(width) << "#";
	vector<string>::iterator it = names.begin(), end_it = names.end();
	for(;it!=end_it;++it)
	{
		fileout << setw(width) << *it;
	}
	fileout << endl;
	//
	it = names.begin(), end_it = names.end();
	vector<int*>::iterator it2 = values.begin(), end_it2 = values.end();
	for(;it!=end_it;++it,++it2)
	{
		fileout << setw(width) << *it;
		vector<int*>::iterator it3 = values.begin(), end_it3 = values.end();
		for(;it3!=end_it3;it3++)
		{
			float ret = getDistance(*it2,*it3,binsD2);
			fileout << setw(width) << setprecision(16) << ret;
		}
		fileout << endl;
	}
	fileout.close();
	//
	it2 = values.begin(), end_it2 = values.end();
	for(;it2!=end_it2;it2++)
	{
		//delete []*it2;
		//*it2 = NULL;
	}
}

void PointDataManager::saveShell(string totalPath, string fileName)
{
	if(points == NULL)
	{
		return;
	}
	//
	const int BINS = binsShell;
	int* bin = new int[BINS+1];
	assert(bin != NULL);
	memset(bin,0,(BINS+1) * sizeof(int));
	for(int i=0;i<POINTS;i++)
	{
		float length = len(points[i]);
		length /= 1.0f;
		int id = (int)(length*BINS);
		assert(id <= BINS);
		bin[id]++;
	}
	bin[BINS-1] += bin[BINS];
	//
	const int denominater = POINTS;
	for(int i=0;i<BINS;i++)
	{
		bin[i] = bin[i] * 256 / denominater;
	}
	//
	ofstream fileout;
	if(saveShellstart)
	{
		fileout.open(totalPath + ".shell.txt");
		saveShellstart = false;
	}
	else
	{
		fileout.open(totalPath + ".shell.txt",ofstream::ate|ofstream::app);
	}
	fileout<<"#"<<fileName.substr(0,fileName.length()-4)<<endl;
	if(OUTNUMBERS || true)
	{
		for(int i=0; i<BINS; i++)
		{
			fileout<<bin[i]<<' ';
		}
	}
	else
	{
		fileout.write((const char*)bin,sizeof(point)*BINS);
	}
	fileout<<endl;
	fileout.close();

	delete []bin;
}

void PointDataManager::saveSector(string totalPath, string fileName)
{
	if(points == NULL)
	{
		return;
	}
	//
	const int BINS = binsSector;
	int* bin = new int[BINS+1];
	assert(bin != NULL);
	memset(bin,0,(BINS+1) * sizeof(int));
	for(int i=0;i<POINTS;i++)
	{
		float angle = 0;
		angle += acos(points[i][0]);
		if(points[i][1] < 0)
		{
			angle = 2*M_PIf - angle;
		}
		angle /= (2*M_PIf);
		int id = (int)(angle*BINS);
		assert(id <= BINS);
		bin[id]++;
	}
	bin[BINS-1] += bin[BINS];
	//
	const int denominater = POINTS;
	for(int i=0;i<BINS;i++)
	{
		bin[i] = bin[i] * 256 / denominater;
	}
	//
	ofstream fileout;
	if(saveSectorstart)
	{
		fileout.open(totalPath + ".sector.txt");
		saveSectorstart = false;
	}
	else
	{
		fileout.open(totalPath + ".sector.txt",ofstream::ate|ofstream::app);
	}
	fileout<<"#"<<fileName.substr(0,fileName.length()-4)<<endl;
	if(OUTNUMBERS || true)
	{
		for(int i=0; i<BINS; i++)
		{
			fileout<<bin[i]<<' ';
		}
	}
	else
	{
		fileout.write((const char*)bin,sizeof(point)*BINS);
	}
	fileout<<endl;
	fileout.close();

	delete []bin;
}

void PointDataManager::saveSecShell(string totalPath, string fileName)
{
	if(points == NULL)
	{
		return;
	}
	//
	const int BINSECTOR = binsSSSector;
	const int BINSHELL = binsSSShell;
	const int BINS = BINSECTOR * BINSHELL;
	int* bin = new int[BINS+1];
	assert(bin != NULL);
	memset(bin,0,(BINS+1) * sizeof(int));
	for(int i=0;i<POINTS;i++)
	{
		float angle = 0;
		angle += acos(points[i][0]);
		if(points[i][1] < 0)
		{
			angle = 2*M_PIf - angle;
		}
		angle /= (2*M_PIf);
		int id1 = (int)(angle*BINSECTOR);
		//
		float length = len(points[i]);
		length /= 1.0f;
		int id2 = (int)(length*BINSHELL);
		//
		bin[id1 * BINSHELL + id2]++;
	}
	bin[BINS-1] += bin[BINS];
	//
	const int denominater = POINTS;
	for(int i=0;i<BINS;i++)
	{
		bin[i] = bin[i] * 256 / denominater;
	}
	//
	ofstream fileout;
	if(saveSecShellstart)
	{
		fileout.open(totalPath + ".secshell.txt");
		saveSecShellstart = false;
	}
	else
	{
		fileout.open(totalPath + ".secshell.txt",ofstream::ate|ofstream::app);
	}
	fileout<<"#"<<fileName.substr(0,fileName.length()-4)<<endl;
	if(OUTNUMBERS || true)
	{
		for(int i=0; i<BINS; i++)
		{
			fileout<<bin[i]<<' ';
		}
	}
	else
	{
		fileout.write((const char*)bin,sizeof(point)*BINS);
	}
	fileout<<endl;
	fileout.close();

	delete []bin;
}
