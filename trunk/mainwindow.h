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

    // �Զ�������
public:
    QSceneDisplay *sceneDisplayWidget;
    QHBoxLayout *hlayout;
    Scene *scene;

    // �Զ��巽��
public:

signals:
	void FreshScreen();
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
