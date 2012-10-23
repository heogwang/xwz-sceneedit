#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // 界面设定
    ui->setupUi(this);
    sceneDisplayWidget=new QSceneDisplay;
    hlayout=new QHBoxLayout;
    hlayout->addWidget(sceneDisplayWidget);
    ui->centralWidget->setLayout(hlayout);

    connect(sceneDisplayWidget,SIGNAL(DrawScene()),this,SLOT(DrawScene()));
    connect(ui->openFileAction,SIGNAL(triggered()),this,SLOT(OpenSceneFile()));
    connect(ui->saveFileAction,SIGNAL(triggered()),this,SLOT(SaveSceneFile()));
	connect(this,SIGNAL(FreshScreen()),sceneDisplayWidget,SLOT(update()));
	connect(this,SIGNAL(DisplaySetting(point,float)),sceneDisplayWidget,SLOT(SetDisProperty(point,float)));
    connect(sceneDisplayWidget,SIGNAL(SetCamera(GLfloat*,double,double)),this,SLOT(SetCamera(GLfloat*,double,double)));
    // 属性设定
    scene=NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DrawScene()
{
	if(scene==NULL)
		return;
	//emit DisplaySetting(scene->bsphere.center,scene->bsphere.r);
	scene->DrawSimpleScene();
}

void MainWindow::OpenSceneFile()
{
	if(scene!=NULL)
		delete scene;

	scene=new Scene;
    QString fileName=QFileDialog::getOpenFileName(this,tr("Open Scene File"),".",tr("obj file(*.obj)"));
	//QString fileName="J:\\code\\conference_room26\\conference_room26.obj";
    bool flag=scene->readScene(fileName.toStdString().c_str());
    if(!flag)
        QMessageBox::warning(this,tr("ReadScene"),tr("Open Scene File Error"),QMessageBox::Yes);
	emit FreshScreen();
}

void MainWindow::SaveSceneFile()
{

}

void MainWindow::SetCamera( GLfloat* eye ,double scale,double aspect)
{
	if(scene==NULL)
		return;
	GLfloat diam=2*scene->bsphere.r;
	GLfloat zFar=1.0+2*diam;
	eye[2]=diam;
	GLfloat left=scene->bsphere.center[0]-diam;
	GLfloat right=scene->bsphere.center[0]+diam;
	GLfloat bottom=scene->bsphere.center[1]-diam;
	GLfloat top=scene->bsphere.center[1]+diam;
	if (aspect<1.0)
	{
		bottom/=aspect;
		top/=aspect;
	}
	else
	{
		left*=aspect;
		right*=aspect;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left,right,bottom,top,1.0,zFar);
	//glFrustum(left,right,bottom,top,1.0,zFar);
	//gluPerspective(50.0*scale,aspect,1.0,zFar);
	//gluLookAt(eye[0],eye[1],eye[2],scene->bsphere.center[0],scene->bsphere.center[1],scene->bsphere.center[2],0.0,1.0,0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glTranslatef(-scene->bsphere.center[0],-scene->bsphere.center[1],-scene->bsphere.center[2]);
}
