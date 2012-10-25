#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
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
    QSceneDisplay *sceneDisplayWidget;
    QHBoxLayout *hlayout;
    Scene *scene;

    // 自定义方法
public:

signals:
	void SetDisScene(Scene* scene); // 将读入的场景指针传入显示界面
	void DisplaySetting(point center,float r) ;


public slots:
    void DrawScene();
    void OpenSceneFile();
    void SaveSceneFile();
    void SetCamera(GLfloat* eye,double scale,double aspect);


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
