#include "Model.h"

Model::Model(void)
{
	visible=true;
	ModelMatrix=new GLdouble[16];
	scale=1.0;
	xangle=yangle=0.0;
	faceAreaSum = NULL;
	samplePoints = NULL;
	pointNumber = 0;
	test = false;
}

Model::~Model(void)
{
	delete []ModelMatrix;
	if(faceAreaSum != NULL)
	{
		delete []faceAreaSum;
	}
	if(samplePoints != NULL)
	{
		delete []samplePoints;
	}
}

void Model::DrawModel()
{
	if(test) return;
	glPushMatrix();
	glTranslated(tx,ty,tz);
	glRotated(yangle,0.0,1.0,0.0);
	glRotated(xangle,1.0,0.0,0.0);
	glScalef(scale,scale,scale);
	glTranslatef(-bsphere.center[0],-bsphere.center[1],-bsphere.center[2]);

	glGetDoublev(GL_MODELVIEW_MATRIX,ModelMatrix);
	// load名称，方便拾取
	glLoadName(scene->ModelMap[this->name]);
	glColor3b(255,0,0);
	glBegin(GL_TRIANGLES);
	for (int i=faceStart;i<faceEnd;i++)
	{
		Face* face=scene->faces[i];
		for(int j=0;j<3;j++)
		{
			glNormal3f(scene->vnormals[face->vn[j]][0],scene->vnormals[face->vn[j]][1],scene->vnormals[face->vn[j]][2]);
			glVertex3f(scene->points[face->v[j]][0],scene->points[face->v[j]][1],scene->points[face->v[j]][2]);
		}
	}
	glEnd();
	glPopMatrix();
	glFlush();
}

void Model::need_bbox()
{
	if(pointMap.size()==0) //没有面片
		return;
	
	map<int,point>::iterator it;
	for(it=pointMap.begin();it!=pointMap.end();it++)
		bbox+=it->second;

	bbox.valid=true;
}

void Model::need_bsphere()
{
	if(pointMap.size()==0) //没有面片
		return;

	Miniball<3,float> mb;
	//for (int i=faceStart;i<faceEnd;i++)
	//	for(int j=0;j<3;j++)
	//		mb.check_in(scene->points[scene->faces[i]->v[j]]);
	map<int,point>::iterator it;
	for(it=pointMap.begin();it!=pointMap.end();it++)
		mb.check_in(it->second);
	mb.build();
	bsphere.center=mb.center();
	bsphere.r=sqrt(mb.squared_radius());
	bsphere.valid=true;
}

void Model::DrawBbox()
{
	//
	if(pointNumber == 0)
	{
		CalculateArea();
		GeneratePoints(1000);
	}
	DrawPoints();
	test = true;
	//
	glPushMatrix();

	glMultMatrixd(ModelMatrix);
	glColor3f(1.0,0,0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(bbox.min[0],bbox.max[1],bbox.min[2]);
	glVertex3f(bbox.min[0],bbox.min[1],bbox.min[2]);
	glVertex3f(bbox.max[0],bbox.min[1],bbox.min[2]);
	glVertex3f(bbox.max[0],bbox.max[1],bbox.min[2]);
	glVertex3f(bbox.min[0],bbox.max[1],bbox.min[2]);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(bbox.max[0],bbox.min[1],bbox.min[2]);
	glVertex3f(bbox.max[0],bbox.max[1],bbox.min[2]);
	glVertex3f(bbox.max[0],bbox.max[1],bbox.max[2]);
	glVertex3f(bbox.max[0],bbox.min[1],bbox.max[2]);
	glVertex3f(bbox.max[0],bbox.min[1],bbox.min[2]);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(bbox.max[0],bbox.max[1],bbox.max[2]);
	glVertex3f(bbox.max[0],bbox.min[1],bbox.max[2]);
	glVertex3f(bbox.min[0],bbox.min[1],bbox.max[2]);
	glVertex3f(bbox.min[0],bbox.max[1],bbox.max[2]);
	glVertex3f(bbox.max[0],bbox.max[1],bbox.max[2]);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(bbox.min[0],bbox.max[1],bbox.min[2]);
	glVertex3f(bbox.min[0],bbox.min[1],bbox.min[2]);
	glVertex3f(bbox.min[0],bbox.min[1],bbox.max[2]);
	glVertex3f(bbox.min[0],bbox.max[1],bbox.max[2]);
	glVertex3f(bbox.min[0],bbox.max[1],bbox.min[2]);
	glEnd();
	glPopMatrix();
}
void Model::CalculateArea()
{
	if(faceAreaSum != NULL)
	{
		delete []faceAreaSum;
	}
	int totalTriangle = faceEnd - faceStart;
	faceAreaSum = new GLfloat[totalTriangle];
	GLfloat currentSum = 0;
	for(int i = 0;i<totalTriangle;i++)
	{
		Face* face = scene->faces[i+faceStart];
		point a = scene->points[face->v[0]] - scene->points[face->v[1]];
		point b = scene->points[face->v[2]] - scene->points[face->v[1]];
		currentSum += len(point(a[1]*b[2] - a[2]*b[1], a[2]*b[0] - a[0]*b[2], a[0]*b[1] - a[1]*b[0]))/2;
		faceAreaSum[i] = currentSum;
	}
}

void Model::GeneratePoints(int total)
{
	if(samplePoints != NULL)
	{
		delete []samplePoints;
	}
	pointNumber = total;
	samplePoints = new point[total];
	seed((unsigned)time(NULL));
	int totalTriangle = faceEnd - faceStart;
	GLfloat totalArea = faceAreaSum[totalTriangle - 1];
	for(int i=0;i<pointNumber;i++)
	{
		GLfloat randArea = totalArea * randf_co();
		//
		int down = 0;
		int up = totalTriangle-1;
		int current = -1;
		while(true)
		{
			current = (up + down) / 2;
			GLfloat curArea = faceAreaSum[current];
			if(curArea == randArea)
			{
				current++;
				break;
			}
			else if(curArea > randArea)
			{
				up = current;
				if(up == down)
				{
					break;
				}
			}
			else
			{
				down = current + 1;
				if(down == up)
				{
					current++;
					break;
				}
			}
		}
		//
		float r1 = sqrt(randf_co());
		float r2 = randf_co();
		float A = 1 - r1;
		float B = r1 * (1 - r2);
		float C = r1 * r2;
		Face* face = scene->faces[current + faceStart];
		point pA = scene->points[face->v[0]];
		point pB = scene->points[face->v[1]];
		point pC = scene->points[face->v[2]];
		for(int j=0;j<3;j++)
		{
			samplePoints[i][j] = A*pA[j] + B*pB[j] + C*pC[j];
		}
	}
}
void Model::DrawPoints()
{
	glPushMatrix();
	glTranslated(tx,ty,tz);
	glRotated(yangle,0.0,1.0,0.0);
	glRotated(xangle,1.0,0.0,0.0);
	glScalef(scale,scale,scale);
	glTranslatef(-bsphere.center[0],-bsphere.center[1],-bsphere.center[2]);

	glGetDoublev(GL_MODELVIEW_MATRIX,ModelMatrix);
	// load名称，方便拾取
	glLoadName(scene->ModelMap[this->name]);
	glColor3b(255,0,0);
	glBegin(GL_POINTS);
	for (int i=0;i<pointNumber;i++)
	{
		glVertex3f(samplePoints[i][0],samplePoints[i][1],samplePoints[i][2]);
	}
	glEnd();
	glPopMatrix();
	glFlush();
}