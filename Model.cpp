#include "Model.h"
//#include "gl/GL.h"
//#include "gl/GLU.h"

Model::Model(void)
{
}


Model::~Model(void)
{
}

void Model::DrawModel(Scene* scene)
{
	// loadÃû³Æ£¬·½±ãÊ°È¡
//	glLoadName(scene->ModelMap[this->name]);
	for (int i=faceStart;i<faceEnd;i++)
	{
		Face* face=scene->faces[i];
		glBegin(GL_TRIANGLES);
		glColor3b(255,0,0);
		for(int j=0;j<3;j++)
		{
			glNormal3f(scene->vnormals[face->vn[j]][0],scene->vnormals[face->vn[j]][1],scene->vnormals[face->vn[j]][2]);
			glVertex3f(scene->points[face->v[j]][0],scene->points[face->v[j]][1],scene->points[face->v[j]][2]);
		}
		glEnd();
	}
	glFlush();
}
