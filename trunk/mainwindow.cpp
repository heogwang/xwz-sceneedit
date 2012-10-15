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

    // 属性设定
    scene=new Scene;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DrawScene()
{

	scene->DrawSimpleScene();
	//scene->DrawScene();
	//scene->DrawTest();
	//glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	//static const GLfloat p1[3]={0.0,-1.0,2.0};
	//static const GLfloat p2[3]={1.73205001,-1.0,-1.0};
	//static const GLfloat p3[3]={-1.73205001,-1.0,-1.0};
	//static const GLfloat p4[3]={0.0,2.0,0.0};

	//static const GLfloat* const coords[4][3]={
	//	{p1,p2,p3},{p1,p3,p4},{p1,p4,p2},{p2,p4,p3}
	//};

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glTranslatef(0,0,-10);
	//glRotatef(rotationX,1,0,0);
	//glRotatef(rotationY,0,1,0);
	//glRotatef(rotationZ,0,0,1);
	//QColor faceColors[4];
	//faceColors[0]=Qt::red;
	//faceColors[1]=Qt::green;
	//faceColors[2]=Qt::blue;
	//faceColors[3]=Qt::yellow;

	//for (int i=0;i<4;i++)
	//{
	//	glLoadName(i);
	//	glBegin(GL_TRIANGLES);
	//	/*qglColor(faceColors[i]);*/
	//	glColor3b(255,0,0);
	//	for (int j=0;j<3;j++)
	//	{
	//		glVertex3f(coords[i][j][0],coords[i][j][1],coords[i][j][2]);
	//	}
	//	glEnd();
	//}
}

void MainWindow::OpenSceneFile()
{
    QString fileName=QFileDialog::getOpenFileName(this,tr("Open Scene File"),".",tr("obj file(*.obj)"));
	//QString fileName="J:\\code\\conference_room26\\conference_room26.obj";
    bool flag=scene->readScene(fileName.toStdString().c_str());
    if(!flag)
        QMessageBox::warning(this,tr("ReadScene"),tr("Open Scene File Error"),QMessageBox::Yes);
}

void MainWindow::SaveSceneFile()
{

}
