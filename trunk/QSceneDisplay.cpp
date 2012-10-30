#include "QSceneDisplay.h"
#include <fstream>
const float PI=3.1415926;
const float speed=PI/180;

QSceneDisplay::QSceneDisplay(QWidget *parent)
	: QGLWidget(parent)
{
	xangle=yangle=0.0;
	scale=1.0;
	scene=NULL;
	up[0]=up[2]=0;
	up[1]=1;
}


QSceneDisplay::~QSceneDisplay()
{

}

void QSceneDisplay::initializeGL()
{
	glShadeModel(GL_SMOOTH);

	glClearColor(1.0,1.0,1.0,1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void QSceneDisplay::paintGL()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };//环境光颜色
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };//漫反射光颜色
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };//镜面反思光颜色
	GLfloat light_position[] = { 9999.0, 9999.0, 9999.0, 0.0 };//光源位置
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);//设置环境光
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);//设置漫反射光
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);//设置镜面反射光
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//设置光源位置
	glEnable(GL_LIGHTING);//打开光线
	glEnable(GL_LIGHT0);//打开0号光源
	glEnable(GL_DEPTH_TEST);//打开深度测试

	glDepthFunc(GL_LEQUAL);							// 所作深度测试的类型
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// 真正精细的透视修正

	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	if(scene==NULL)
		return;
	SetCamera();

	DrawScene();
}

void QSceneDisplay::resizeGL( int width,int height )
{
	if (height==0)
		height=1;
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(plane[0],plane[1],plane[2],plane[3],-10000,10000);

	glMatrixMode(GL_MODELVIEW);
}

void QSceneDisplay::mouseMoveEvent(QMouseEvent *event)
{
	QPoint point=event->pos();
	if (event->buttons() & Qt::LeftButton)
	{
		float dx=float(point.x()-btnDown.x())*(plane[1]-plane[0])/width(); // 转换到视景体移动的距离
		float dy=float(btnDown.y()-point.y())*(plane[3]-plane[2])/height();
		plane[0]-=dx;
		plane[1]-=dx;
		plane[2]-=dy;
		plane[3]-=dy;
		btnDown=point;
	}
	else if (event->buttons()&Qt::RightButton)
	{
		xangle+=float(point.x()-btnDown.x())*90/width();  // 绕y旋转的角度，角度值
		yangle+=float(btnDown.y()-point.y())*90/height(); // 绕x轴旋转角度，角度值

		if (xangle>360)
			xangle-=360;
		else if(xangle<0)
			xangle+=360;

		if (yangle>360)
		{
			yangle-=360;
			up[1]*=-1;
		}
		else if(yangle<0)
		{
			yangle+=360;
			up[1]*=-1;
		}

		float anglex=speed*xangle;
		float angley=speed*yangle;

		eye[0]=sin(angley)*cos(anglex);
		eye[1]=cos(angley);
		eye[2]=sin(angley)*sin(anglex);
		eye=normalize(eye);

		eye=scene->bsphere.center+radius*eye;

		btnDown=point;
	}

    this->updateGL();
}

void QSceneDisplay::mousePressEvent(QMouseEvent *event)
{
	setMouseTracking(true);
	btnDown=event->pos();
}

void QSceneDisplay::wheelEvent(QWheelEvent *event)
{
	double numDegrees = -event->delta() / 8.0;
	double numSteps = numDegrees / 15.0;
	scale = pow(1.125, numSteps);
	double width=(plane[1]-plane[0])*scale;
	double height=(plane[3]-plane[2])*scale;
	double centerx=(plane[0]+plane[1])/2;
	double centery=(plane[3]+plane[2])/2;

	plane[0]=centerx-width/2;
	plane[1]=centerx+width/2;
	plane[2]=centery-height/2;
	plane[3]=centery+height/2;
	this->updateGL();
}

void QSceneDisplay::DrawCoodinates()
{
	glTranslatef(0,0,-6);
	glBegin(GL_LINES);
	glColor3b(0,0,255);
	glVertex3f(0,-5000,0);
	glVertex3f(0,5000,0);
	glColor3b(0,255,0);
	glVertex3f(-5000,0,0);
	glVertex3f(5000,0,0);
	glColor3b(255,0,0);
	glVertex3f(0,0,-5000);
	glVertex3f(0,0,5000);
	glEnd();
}

void QSceneDisplay::SetDisScene( Scene* scene )
{
	this->scene=scene;
	radius=scene->bsphere.r;
	// 设置视点位置
	eye[0]=scene->bsphere.center[0];
	eye[1]=scene->bsphere.center[1];
	eye[2]=scene->bsphere.center[2]+radius;

	xangle=90;
	yangle=90;
	// 设置视景体的区域
	double diam=2*scene->bsphere.r;
	double tmp=abs(scene->bsphere.center[0]);
	plane[0]=-(tmp+diam);
	plane[1]=-plane[0];
	tmp=abs(scene->bsphere.center[1]);
	plane[2]=-(tmp+diam);
	plane[3]=-plane[2];
	
	double aspect=(double)width()/height();
	if (aspect<1)
	{
		plane[2]/=aspect;
		plane[3]/=aspect;
	}
	else
	{
		plane[0]*=aspect;
		plane[1]*=aspect;
	}
	updateGL();
}

void QSceneDisplay::SetCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(plane[0],plane[1],plane[2],plane[3],-10000,10000);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0],eye[1],eye[2],scene->bsphere.center[0],scene->bsphere.center[1],scene->bsphere.center[2],up[0],up[1],up[2]);

}

void QSceneDisplay::DrawScene()
{
	//scene->DrawSimpleScene();
	// 不绘制场景的墙壁
	int occurs = -1;
	map<string, int>::iterator it = scene->ModelMap.find("Wall");
	if(it != scene->ModelMap.end())
	{
		occurs = it ->second;
		scene->sceneModels[occurs]->visible=false;
	}
	scene->DrawScene();
}
