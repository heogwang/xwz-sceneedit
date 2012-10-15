#include "QSceneDisplay.h"

QSceneDisplay::QSceneDisplay(QWidget *parent)
	: QGLWidget(parent)
{
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
	//GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };//环境光颜色
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };//漫反射光颜色
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };//镜面反思光颜色
	GLfloat light_position[] = { 9999.0, 9999.0, 9999.0, 0.0 };//光源位置
	//glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);//设置环境光
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);//设置漫反射光
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);//设置镜面反射光
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//设置光源位置
	glEnable(GL_LIGHTING);//打开光线
	glEnable(GL_LIGHT0);//打开0号光源
	glEnable(GL_DEPTH_TEST);//打开深度测试

	glDepthFunc(GL_LEQUAL);							// 所作深度测试的类型
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// 真正精细的透视修正

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//gluLookAt(0,0,10,0,0,-1,0,1,0);
	//float scale=20;
	//glScalef(scale,scale,scale);
	////glScaled(0.01,0.01,0.01);
	////gluLookAt(5000,5000,-3100,3550,1280,-2200,0,1,0);
	////gluLookAt(0,0,50,0,0,-1,0,1,0);
	//glScaled(10,10,10);

	//GLfloat material[]={0.5,0.5,0.5,1.0};
	//glMaterialfv(GL_FLOAT,GL_AMBIENT_AND_DIFFUSE,material);

	//glClearColor(1,1,1,0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//glColor3f(1.0,0.0,0.0);

	//glPushMatrix();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0,0,0,1.0f);

	////glMatrixMode(GL_MODELVIEW);
	////glLoadIdentity();

	//glShadeModel(GL_FLAT);
	//glFrontFace(GL_CW);

	///*glColor3f(1.0f,1.0f,1.0f);
	//glutWireCube(100);*/

	//glBegin(GL_POLYGON);
	//glColor4f(1.0f,0.0f,0.0f,1.0f);
	//glVertex2f(100.0f,50.0f);
	//glColor4f(0.0f,1.0f,0.0f,1.0f);
	//glVertex2f(450.0f,400.0f);
	//glColor4f(0.0f,0.0f,1.0f,1.0f);
	//glVertex2f(450.0f,50.0f);
	//glEnd();

	//glFlush();

	emit DrawScene();
}

void QSceneDisplay::resizeGL( int width,int height )
{
	if (height==0)
		height=1;
	//glViewport(0,0,width,height);  // 左下角为0,0
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(45,(GLfloat)width/(GLfloat)height,0.1,100);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat x=GLfloat(width)/height;
	glFrustum(-x,+x,-1.0,+1.0,4.0,15.0);
	glMatrixMode(GL_MODELVIEW);
}


