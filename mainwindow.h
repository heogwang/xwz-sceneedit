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
	void FreshScreen();

public slots:
    void DrawScene();
    void OpenSceneFile();
    void SaveSceneFile();
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
