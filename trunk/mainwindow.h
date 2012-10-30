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
	void CreateRelationItem(); // 添加QTreeWidget的内容
};

#endif // MAINWINDOW_H
