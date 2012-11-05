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

    // �Զ�������
public:
	// �������
    QSceneDisplay *sceneDisplayWidget;
	QHBoxLayout *hlayout;

	// �ұߵ�ͣ���ռ�
	QDockWidget *rdocWidget;  // right Dock Widget
	QTreeWidget* treeWidget;
	QList<QTreeWidgetItem *> rootList;

	// �˵���
	QMenu *fileMenu;
	QMenu *modelEditMenu;
	QMenu *sceneEditMenu;


	// ������
	QToolBar *fileToolBar;
	QToolBar *editModelToolBar;
	QToolBar *editSceneToolBar;


	// ��ز���
	QAction *openSceneAction;
	QAction *saveSceneAction;

	QAction *chooseModelAction;
	QAction *transModelAction;
	QAction *rotateModelAction;
	QAction *scaleModelAction;

	QAction *transSceneAction;
	QAction *rotateSceneAction;
	QAction *scaleSceneAction;

    Scene *scene;

    // �Զ��巽��
public:


signals:
	void SetDisScene(Scene* scene); // ������ĳ���ָ�봫����ʾ����

public slots:
    void OpenSceneFile();
    void SaveSceneFile();

private:
    Ui::MainWindow *ui;
	void CreateDockWidget();
	void CreateRelationItem(); // ���QTreeWidget������
	void CreateActions();
	void CreateMenu();
	void CreateToolbar();
	void CreateCentralWidget();
};

#endif // MAINWINDOW_H
