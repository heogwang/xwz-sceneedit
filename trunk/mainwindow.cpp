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
