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

    // �Զ�������
public:
	// �������
    QSceneDisplay *sceneDisplayWidget;
	QHBoxLayout *hlayout;

	// �ұߵ�ͣ���ռ�
	QDockWidget *rdocWidget;  // right Dock Widget
	QTreeWidget* treeWidget;
	QList<QTreeWidgetItem *> rootList;


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
	void CreateRelationItem(); // ���QTreeWidget������
};

#endif // MAINWINDOW_H
