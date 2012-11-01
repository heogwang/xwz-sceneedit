#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QDockWidget>
#include <QList>
#include <QTreeWidgetItem>
#include <QFont>
#include <QBrush>
#include <QColor>
#include <QVariant>
#include <QAction>
#include <QMenu>
#include <QToolBar>

#include "QSceneDisplay.h"
#include "Scene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // 自定义属性
public:
	// 界面相关
    QSceneDisplay *sceneDisplayWidget;
	QHBoxLayout *hlayout;

	// 右边的停留空间
	QDockWidget *rdocWidget;  // right Dock Widget
	QTreeWidget* treeWidget;
	QList<QTreeWidgetItem *> rootList;

	// 菜单栏
	QMenu *fileMenu;
	QMenu *editMenu;



	// 工具栏
	QToolBar *fileToolBar;
	QToolBar *editToolBar;


	// 相关操作
	QAction *openSceneAction;
	QAction *saveSceneAction;
	QAction *chooseModelAction;


    Scene *scene;

    // 自定义方法
public:


signals:
	void SetDisScene(Scene* scene); // 将读入的场景指针传入显示界面

public slots:
    void OpenSceneFile();
    void SaveSceneFile();

private:
    Ui::MainWindow *ui;
	void CreateDockWidget();
	void CreateRelationItem(); // 添加QTreeWidget的内容
	void CreateActions();
	void CreateMenu();
	void CreateToolbar();
	void CreateCentralWidget();
};

#endif // MAINWINDOW_H
