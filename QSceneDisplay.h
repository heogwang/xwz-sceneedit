#ifndef QSCENEDISPLAY_H
#define QSCENEDISPLAY_H

#include <QGLWidget>
#include <QPoint>
#include <QMouseEvent>
#include "gl/GL.h"
#include "gl/GLU.h"
#include "trimesh/Vec.h"
#include "Scene.h"
#include "arcball.h"

namespace Ui {
	class QSceneDisplay;
}

class QSceneDisplay : public QGLWidget
{
	Q_OBJECT

public:
	QSceneDisplay(QWidget *parent = 0);
	~QSceneDisplay();

	// 自定义属性
public:
	double xangle;
	double yangle;
	double scale;
    QPoint lbtnDown; // 左键按下时，屏幕点的位置
	float zNear;
	float zFar;
	GLfloat eye[4];
	GLfloat plane[4];
	//arcvec eye;
	//arcvec center;
	//arcvec up;

	Scene *scene;






	// 自定义方法
public:
	void DrawCoodinates();


public slots:
	void SetDisProperty(point center, float r);
	void SetCamera();
	void SetDisScene(Scene* scene);

signals:
	void DrawScene();
	//void SetCamera(GLfloat* eye,double scale,double aspect);

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width,int height);

	// 事件处理
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);



private:
};

#endif // QSCENEDISPLAY_H
