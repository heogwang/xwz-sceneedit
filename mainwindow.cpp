#include <QFileDialog>
#include <QMessageBox>
#include <QIcon>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	// �����趨
	ui->setupUi(this);
	scene=NULL;

	CreateCentralWidget();
	CreateDockWidget();
	CreateActions();
	CreateMenu();
	CreateToolbar();

	// �¼�����

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
    QString fileName=QFileDialog::getOpenFileName(this,tr("Open Scene File"),".",tr("obj file(*.obj)"));
    //QString fileName="D:\\geometry\\QSceneEdit\\conference_room26\\conference_room26.obj";
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
	bool flag=false;  // ָʾ��һ�������Ƿ�����Ҫ��
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
	//treeWidget->expandAll(); //ȫ��չ��
}

void MainWindow::CreateActions()
{
    openSceneAction=new QAction(QIcon(":/image/open.png"),tr("�򿪳���"),this);
    connect(openSceneAction,SIGNAL(triggered()),this,SLOT(OpenSceneFile()));

    saveSceneAction=new QAction(QIcon(":/image/save.png"),tr("���泡��"),this);
    connect(saveSceneAction,SIGNAL(triggered()),this,SLOT(SaveSceneFile()));

	chooseModelAction=new QAction(QIcon(":/image/choose.png"),tr("ѡ������"),this);
	connect(chooseModelAction,SIGNAL(triggered()),sceneDisplayWidget,SLOT(ChooseModelAction()));

	transModelAction=new QAction(QIcon(":/image/obj_trans.png"),tr("ƽ������"),this);
	connect(transModelAction,SIGNAL(triggered()),sceneDisplayWidget,SLOT(TransModelAction()));
	
	rotateModelAction=new QAction(QIcon(":/image/obj_rotate.png"),tr("��ת����"),this);
	connect(rotateModelAction,SIGNAL(triggered()),sceneDisplayWidget,SLOT(RotateModelAction()));

	transSceneAction=new QAction(QIcon(":/image/trans.png"),tr("ƽ�Ƴ���"),this);
	connect(transSceneAction,SIGNAL(triggered()),sceneDisplayWidget,SLOT(ChooseModelAction()));

	rotateSceneAction=new QAction(QIcon(":/image/rotate.png"),tr("��ת����"),this);
	connect(rotateSceneAction,SIGNAL(triggered()),sceneDisplayWidget,SLOT(ChooseModelAction()));
}

void MainWindow::CreateMenu()
{
	fileMenu=ui->menuBar->addMenu(tr("�ļ�"));
	fileMenu->addAction(openSceneAction);
	fileMenu->addAction(saveSceneAction);

	modelEditMenu=ui->menuBar->addMenu(tr("ģ�ͱ༭"));
	modelEditMenu->addAction(chooseModelAction);
	modelEditMenu->addAction(transModelAction);
	modelEditMenu->addAction(rotateModelAction);

	sceneEditMenu=ui->menuBar->addMenu(tr("�����༭"));
	sceneEditMenu->addAction(transSceneAction);
	sceneEditMenu->addAction(rotateSceneAction);

}

void MainWindow::CreateToolbar()
{
	fileToolBar=addToolBar(tr("�ļ�"));
	fileToolBar->addAction(openSceneAction);
	fileToolBar->addAction(saveSceneAction);

	editModelToolBar=addToolBar(tr("ģ�ͱ༭"));
	editModelToolBar->addAction(chooseModelAction);
	editModelToolBar->addAction(transModelAction);
	editModelToolBar->addAction(rotateModelAction);

	editSceneToolBar=addToolBar(tr("�����༭"));
	editSceneToolBar->addAction(transSceneAction);
	editSceneToolBar->addAction(rotateSceneAction);
}

void MainWindow::CreateDockWidget()
{
	// ���νṹ
	treeWidget=new QTreeWidget;
	treeWidget->setColumnCount(1);
	treeWidget->setHeaderLabel(tr("Scene Relationship"));

	// ͣ����
	rdocWidget=new QDockWidget;
	rdocWidget->setObjectName(tr("Scene Relationship"));
	rdocWidget->setWindowTitle(tr("Scene Relationship"));
	rdocWidget->setWidget(treeWidget);

	addDockWidget(Qt::RightDockWidgetArea,rdocWidget);
}

void MainWindow::CreateCentralWidget()
{
	// �����趨&&������ʼ��
	sceneDisplayWidget=new QSceneDisplay;

	// �������ݲ�νṹ
	hlayout=new QHBoxLayout;
	hlayout->addWidget(sceneDisplayWidget);

	// ��������νṹ
	ui->centralWidget->setLayout(hlayout);
}

