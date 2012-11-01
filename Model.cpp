#include "Model.h"
//#include "gl/GL.h"
//#include "gl/GLU.h"

Model::Model(void)
{
	visible=true;
}


Model::~Model(void)
{
}

void Model::DrawModel()
{
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
	glFlush();
}

void Model::need_bbox()
{
	if(pointMap.size()==0) //没有面片
		return;
	//for (int i=faceStart;i<faceEnd;i++)
	//	for(int j=0;j<3;j++)
	//		bbox+=scene->points[scene->faces[i]->v[j]];
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
}
