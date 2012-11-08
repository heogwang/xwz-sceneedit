#include <QFileDialog>
#include <QMessageBox>
#include <QIcon>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	// 属性设定
	ui->setupUi(this);
	scene=NULL;

	CreateCentralWidget();
	CreateDockWidget();
	CreateActions();
	CreateMenu();
	CreateToolbar();

	// 事件连接

	// sceneDisplayWidget
	connect(this,SIGNAL(SetDisScene(Scene*)),sceneDisplayWidget,SLOT(SetDisScene(Scene*)));
	//connect(this,SIGNAL(SetChooseMode()),sceneDisplayWidget,SLOT(ChooseModelAction()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OpenSceneFile()
{
	if(scene!=NULL)
		delete scene;

	scene=new Scene;
    //QString fileName=QFileDialog::getOpenFileName(this,tr("Open Scene File"),".",tr("obj file(*.obj)"));
    QString fileName="E:\\QtProjects\\QSceneEdit\\conference_room26\\conference_room26.obj";
    bool flag=scene->readScene(fileName.toStdString().c_str());
    if(!flag)
        QMessageBox::warning(this,tr("ReadScene"),tr("Open Scene File Error"),QMessageBox::Yes);

	CreateRelationItem();
	emit SetDisScene(scene);
}

void MainWindow::SaveSceneFile()
{

}

void MainWindow::CreateRelationItem()
{
	rootList.clear();
	map<string,vector<int>>::iterator it;
	QString strQ;
	bool flag=false;  // 指示第一个物体是否是重要的
	QFont font;
	font.setBold(true);
	font.setItalic(true);

	for (it=scene->RelationMap.begin();it!=scene->RelationMap.end();it++)
	{
		QTreeWidgetItem *item=new QTreeWidgetItem;
		strQ = QString::fromLocal8Bit(it->first.c_str());
		item->setText(0,strQ);
		item->setFont(0,font);

		if (strQ.startsWith("group") || strQ.startsWith("support") || strQ.startsWith("enclousure"))
			flag=true;

		for(int i=0;i<it->second.size();i++)
		{
			QTreeWidgetItem *child=new QTreeWidgetItem(item);
			int mIndex=it->second[i];
			int occurs = -1;
			map<string, int>::iterator it = scene->ModelMap.find("Wall");
			if( it != scene->ModelMap.end() )
				occurs = it ->second;

			switch(mIndex)
			{
			case -1: 
				strQ=tr("Wall");
				mIndex=occurs;
				break;
			case -2: 
				strQ=tr("onWall");
				mIndex=occurs;
				break;
			case -3: 
				strQ=tr("hangWall");
				mIndex=occurs;
				break;
			default:
				strQ = QString::fromLocal8Bit(scene->sceneModels[mIndex]->name.c_str());
				break;
			}
			QVariant v(mIndex);
			child->setData(0,Qt::UserRole,v);
			child->setText(0,strQ);
			if(i==0 && flag)
			{
				child->setForeground(0,QBrush(QColor(Qt::red)));
				flag=false;
			}
			item->addChild(child);
		}
		rootList.append(item);
	}
	treeWidget->insertTopLevelItems(0,rootList);
	//treeWidget->expandAll(); //全部展开
}

void MainWindow::CreateActions()
{
    openSceneAction=new QAction(QIcon(":/image/open.png"),tr("打开场景"),this);
    connect(openSceneAction,SIGNAL(triggered()),this,SLOT(OpenSceneFile()));

    saveSceneAction=new QAction(QIcon(":/image/save.png"),tr("保存场景"),this);
    connect(saveSceneAction,SIGNAL(triggered()),this,SLOT(SaveSceneFile()));

	chooseModelAction=new QAction(QIcon(":/image/choose.png"),tr("选择物体"),this);
	connect(chooseModelAction,SIGNAL(triggered()),sceneDisplayWidget,SLOT(ChooseModelAction()));

	transModelAction=new QAction(QIcon(":/image/obj_trans.png"),tr("平移物体"),this);
	connect(transModelAction,SIGNAL(triggered()),sceneDisplayWidget,SLOT(TransModelAction()));
	
	rotateModelAction=new QAction(QIcon(":/image/obj_rotate.png"),tr("旋转物体"),this);
	connect(rotateModelAction,SIGNAL(triggered()),sceneDisplayWidget,SLOT(RotateModelAction()));

	transSceneAction=new QAction(QIcon(":/image/trans.png"),tr("平移场景"),this);
	connect(transSceneAction,SIGNAL(triggered()),sceneDisplayWidget,SLOT(ChooseModelAction()));

	rotateSceneAction=new QAction(QIcon(":/image/rotate.png"),tr("旋转场景"),this);
	connect(rotateSceneAction,SIGNAL(triggered()),sceneDisplayWidget,SLOT(ChooseModelAction()));
}

void MainWindow::CreateMenu()
{
	fileMenu=ui->menuBar->addMenu(tr("文件"));
	fileMenu->addAction(openSceneAction);
	fileMenu->addAction(saveSceneAction);

	modelEditMenu=ui->menuBar->addMenu(tr("模型编辑"));
	modelEditMenu->addAction(chooseModelAction);
	modelEditMenu->addAction(transModelAction);
	modelEditMenu->addAction(rotateModelAction);

	sceneEditMenu=ui->menuBar->addMenu(tr("场景编辑"));
	sceneEditMenu->addAction(transSceneAction);
	sceneEditMenu->addAction(rotateSceneAction);

}

void MainWindow::CreateToolbar()
{
	fileToolBar=addToolBar(tr("文件"));
	fileToolBar->addAction(openSceneAction);
	fileToolBar->addAction(saveSceneAction);

	editModelToolBar=addToolBar(tr("模型编辑"));
	editModelToolBar->addAction(chooseModelAction);
	editModelToolBar->addAction(transModelAction);
	editModelToolBar->addAction(rotateModelAction);

	editSceneToolBar=addToolBar(tr("场景编辑"));
	editSceneToolBar->addAction(transSceneAction);
	editSceneToolBar->addAction(rotateSceneAction);
}

void MainWindow::CreateDockWidget()
{
	// 树形结构
	treeWidget=new QTreeWidget;
	treeWidget->setColumnCount(1);
	treeWidget->setHeaderLabel(tr("Scene Relationship"));

	// 停靠栏
	rdocWidget=new QDockWidget;
	rdocWidget->setObjectName(tr("Scene Relationship"));
	rdocWidget->setWindowTitle(tr("Scene Relationship"));
	rdocWidget->setWidget(treeWidget);

	addDockWidget(Qt::RightDockWidgetArea,rdocWidget);
}

void MainWindow::CreateCentralWidget()
{
	// 界面设定&&部件初始化
	sceneDisplayWidget=new QSceneDisplay;

	// 界面内容层次结构
	hlayout=new QHBoxLayout;
	hlayout->addWidget(sceneDisplayWidget);

	// 界面主层次结构
	ui->centralWidget->setLayout(hlayout);
}


