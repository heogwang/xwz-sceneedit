#include "QSceneDisplay.h"

QSceneDisplay::QSceneDisplay(QWidget *parent)
	: QGLWidget(parent)
{
	xangle=yangle=0.0;
	scale=1.0;
	eye=new GLfloat[4];
	eye[0]=eye[1]=eye[2]=0.0;
	eye[3]=1.0;
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
	GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };//��������ɫ
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };//���������ɫ
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };//���淴˼����ɫ
	GLfloat light_position[] = { 9999.0, 9999.0, 9999.0, 0.0 };//��Դλ��
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);//���û�����
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);//�����������
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);//���þ��淴���
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//���ù�Դλ��
	glEnable(GL_LIGHTING);//�򿪹���
	glEnable(GL_LIGHT0);//��0�Ź�Դ
	glEnable(GL_DEPTH_TEST);//����Ȳ���

	glDepthFunc(GL_LEQUAL);							// ������Ȳ��Ե�����
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// ������ϸ��͸������

	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	double aspect=(double)width()/height();

	emit SetCamera(eye,scale,aspect);
	DrawCoodinates();


	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//glTranslatef(0,0,-5);
	//glRotated(xangle,1.0,0.0,0.0);
	//glRotated(yangle,0.0,1.0,0.0);

	emit DrawScene();
}

void QSceneDisplay::resizeGL( int width,int height )
{
	if (height==0)
		height=1;
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat x=GLfloat(width)/height;
	/*glFrustum(-x,+x,-1.0,+1.0,4.0,100.0);*/
	//gluPerspective(45,x,1.0,10000.0);
	glOrtho(-x,x,-1.0,1.0,4.0,100);
	glMatrixMode(GL_MODELVIEW);
}

void QSceneDisplay::mouseMoveEvent(QMouseEvent *event)
{
 //   QPoint point=event->pos();
 //   double dx=double(lbtnDown.x()-point.x())/width();
 //   double dy=double(lbtnDown.y()-point.y())/height();
	//xangle -= 180 * dy;
	//yangle -= 180 * dx;
 //   this->updateGL();
 //   lbtnDown=point;
}

void QSceneDisplay::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
        lbtnDown=event->pos();
}

void QSceneDisplay::wheelEvent(QWheelEvent *event)
{

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

void QSceneDisplay::SetDisProperty( point center, float r )
{
	//zFar=zNear+2*r;
	//eye[2]=4*r;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat x=GLfloat(width())/height();
	gluPerspective(50.0*scale,x,1,2*r+1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0],eye[1],eye[2],center[0],center[1],center[2],0.0,1.0,0.0);
}
