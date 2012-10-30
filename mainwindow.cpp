#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // 界面设定&&部件初始化
    ui->setupUi(this);
    sceneDisplayWidget=new QSceneDisplay;
	treeWidget=new QTreeWidget;
	treeWidget->setColumnCount(1);
	treeWidget->setHeaderLabel(tr("Scene Relationship"));


	rdocWidget=new QDockWidget;
	rdocWidget->setObjectName(tr("Scene Relationship"));
	rdocWidget->setWindowTitle(tr("Scene Relationship"));
	rdocWidget->setWidget(treeWidget);

	addDockWidget(Qt::RightDockWidgetArea,rdocWidget);

	// 界面内容层次结构
    hlayout=new QHBoxLayout;
    hlayout->addWidget(sceneDisplayWidget);

	// 界面主层次结构
    ui->centralWidget->setLayout(hlayout);

	// 属性设定
	scene=NULL;

	// 事件连接
	
    connect(ui->openFileAction,SIGNAL(triggered()),this,SLOT(OpenSceneFile()));
    connect(ui->saveFileAction,SIGNAL(triggered()),this,SLOT(SaveSceneFile()));

	 // sceneDisplayWidget
	connect(this,SIGNAL(SetDisScene(Scene*)),sceneDisplayWidget,SLOT(SetDisScene(Scene*)));
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
	QString fileName="F:\\QSceneEdit\\conference_room26\\conference_room26.obj";
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

